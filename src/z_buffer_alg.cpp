#include "z_buffer_alg.h"
#include "geometry.h"
#include <iostream>
#include <cmath>
#include <limits>
#include <set>
using namespace std;
using namespace Eigen;

namespace marvel{
static const float max_float = numeric_limits<float>::max();




int z_buffer_alg::construct_polygen_table(){
  for(size_t i = 0; i < model_ptr_ -> get_num_tris(); ++i){
    //exculde the plane vertical to z=0
    if(model_ptr_ -> get_dzx(i) == max_float)
      continue;
    size_t y_max, y_min;
    model_ptr_ -> get_ymax_and_ymin(i, y_max, y_min);
    
    polygen_table_[y_max].push_back({i, static_cast<int>(y_max - y_min) });
  }

}
  
int z_buffer_alg::construct_edge_table(){
  for(size_t i = 0; i < model_ptr_ -> get_num_tris(); ++i){
  if(model_ptr_ -> get_dzx(i) == max_float)
      continue;
    for(size_t j = 0; j < 3; ++j){
      float x, dx;
      size_t y_max, v_id;
      int dy;
      model_ptr_ -> get_edge_info(i, j, x, dx, dy, y_max, v_id);
      //exclude the line parallel to the x axis
      if(dx == max_float)
        continue;
      
      assert(x >= 0);
      assert(dy <= range_y_);
      assert(y_max <= range_y_);
      edge_table_[y_max].push_back({x, dx, dy, i, v_id});
    }
  }
}


z_buffer_alg::z_buffer_alg(const shared_ptr<model_obj> model_ptr, const size_t& range_y, const size_t& range_x):range_y_(range_y), range_x_(range_x), model_ptr_(model_ptr){
  

  polygen_table_ = vector<vector<polygen>>(range_y_);
  edge_table_ = vector<vector<edge>>(range_y_);
  active_polygen_table_ = list<polygen>(0);
  active_edge_table_ = list<active_edge>(0);


  construct_polygen_table();

  construct_edge_table();
}

int z_buffer_alg::exec(float* frame_buffer, bool if_section){

  
  vector<float> z_buffer(range_x_);
  for(int line = range_y_ -1 ; line >= 0; --line){
    fill(z_buffer.data(), z_buffer.data() + z_buffer.size(), 0);
    
    if( !polygen_table_[line].empty() ){
      activate_polygens_and_edges(line);
    }
    if(if_section)
      section_update_buffers(z_buffer, frame_buffer, line);
    else
      update_buffers(z_buffer, frame_buffer, line);
    update_active_edges(line);
    update_active_polys();
  }
  
}


int z_buffer_alg::update_active_polys(){
  for(auto iter = active_polygen_table_.begin(); iter != active_polygen_table_.end();){
    if( -- iter->dy  < 0)
      iter = active_polygen_table_.erase(iter);
    else
      ++iter;
  }
}

int z_buffer_alg::activate_polygens_and_edges(const size_t& line){
  size_t iter_edge = 0;
  
  for(const auto& poly : polygen_table_[line]){

    active_polygen_table_.push_back(poly);

    
    for(size_t i = iter_edge; i < edge_table_[line].size(); ++i){
      if(edge_table_[line][i].id == poly.id){
        assert(edge_table_[line][i + 1].id == poly.id);
        size_t left, right;{
          //begin with same point
          if(edge_table_[line][i].x == edge_table_[line][i + 1].x){
            if(edge_table_[line][i].dx < edge_table_[line][i + 1].dx){
              left = i; right = i + 1;            
            }
            else{
              left = i + 1; right = i;            
            }            
          }
          //end to same point
          else{
            if(edge_table_[line][i].x < edge_table_[line][i + 1].x){
              left = i; right = i + 1;            
            }
            else{
              left = i + 1; right = i;            
            }
          }
        }
        
        active_edge_table_.push_back({
            edge_table_[line][left].x, edge_table_[line][left].dx, edge_table_[line][left].dy,
                edge_table_[line][right].x, edge_table_[line][right].dx,edge_table_[line][right].dy,
                model_ptr_ -> get_depth(edge_table_[line][left].v_id), model_ptr_ -> get_dzx(poly.id), model_ptr_ -> get_dzy(poly.id), poly.id});
        iter_edge = i +2;
        break;
      }
      else
        continue;
        
    }
  }
  return 0;
}

int z_buffer_alg::update_active_edges(const size_t& line){
  auto find_another_edge = [this](const size_t& poly_id, const size_t& line ) -> float{
    for(auto& edge : edge_table_[line])
      if(edge.id == poly_id)
        return edge.dx;

  };
  for(auto edge_it = active_edge_table_.begin(); edge_it != active_edge_table_.end();){
    bool if_continue = false;

    switch(edge_it->dyl == edge_it->dyr){
      case true:
        if(--(edge_it->dyl) < 0 || --(edge_it->dyr) < 0){
          edge_it = active_edge_table_.erase(edge_it);
          if_continue = true;        
        }
        break;
      case false:
        if(--(edge_it->dyl) < 0){
          edge_it->dxl = find_another_edge(edge_it->id, line);
          edge_it->dyl = edge_it->dyr - 1;
        } 
        if(--(edge_it->dyr) < 0){
          edge_it->dxr = find_another_edge(edge_it->id, line);
          edge_it->dyr = edge_it->dyl;
        }
        break;
    }
    if(if_continue)
      continue;
  
    edge_it->xl += edge_it->dxl;
    edge_it->xr += edge_it->dxr;

    edge_it->zl = edge_it->zl + edge_it->dzx * edge_it->dxl + edge_it->dzy;
    ++edge_it;
  }
}

int z_buffer_alg::update_buffers(vector<float>& z_buffer, float* frame_buffer, const size_t& line){
  Map<MatrixXf> map_frame_buffer(frame_buffer, 3, range_y_ * range_x_);

  for(auto& edge_pair : active_edge_table_){
    auto begin = static_cast<size_t>(round(edge_pair.xl));
    auto end = static_cast<size_t>(round(edge_pair.xr));
    assert(end >= begin);
    for(size_t i = begin; i <= end; ++i){
      auto new_z_value = edge_pair.zl + (i - begin) * edge_pair.dzx;
      // assert(new_z_value >= 0);
      if(new_z_value > z_buffer[i]){
        z_buffer[i] = new_z_value;
        map_frame_buffer.col(line * range_x_  + i) = model_ptr_ -> get_depth_shader_value(edge_pair.id);

      }
    }
  }
  
  return 0;
}
int z_buffer_alg::section_update_buffers(vector<float>& z_buffer, float* frame_buffer, const size_t& line){
  Map<MatrixXf> map_frame_buffer(frame_buffer, 3, range_y_ * range_x_);


  multiset<break_point> section;
  set<size_t> in_pairs;
  
  size_t edge_id = 0;
  for(auto& edge_pair : active_edge_table_){
    auto xl = static_cast<size_t>(round(edge_pair.xl));
    auto xr = static_cast<size_t>(round(edge_pair.xr));
    if( xl!= xr){
      section.insert({xl, true, edge_id});
      section.insert({xr, false, edge_id});
    }
    ++edge_id;      
  }
  
  for(auto l_bound = section.begin(); l_bound != section.end(); ++ l_bound){

    auto r_bound = l_bound;
    bool at_end = false;
    while(r_bound->x == l_bound->x){
      l_bound = r_bound;
      r_bound = next(l_bound, 1);
      if(r_bound == section.end()){
        at_end = true;
        break;
      }
        
      if(l_bound->if_left)
        in_pairs.insert(l_bound->edge_id);
      else
        in_pairs.erase(l_bound->edge_id);
    }
    if(at_end)
      break;

    size_t left_pix = static_cast<size_t>(l_bound->x);
    size_t right_pix = static_cast<size_t>(r_bound->x);
    assert(right_pix >= left_pix);
    
    //compare to get domain poly
    int max_z = 0;
    int domain_poly_id = -1;
    for(auto pair_iter = in_pairs.begin(); pair_iter != in_pairs.end(); ++pair_iter){
      
      auto edge_pair = active_edge_table_.begin();
      advance(edge_pair, *pair_iter);
      float new_z = edge_pair->zl + (left_pix + 1 - static_cast<int>(edge_pair->xl)) * edge_pair->dzx;
      if(new_z > max_z){
        max_z = new_z;
        domain_poly_id = edge_pair->id;
      }
    }
    if(domain_poly_id != -1){
      Vector3f shader = model_ptr_->get_depth_shader_value(domain_poly_id);
      for(size_t i = 0; i < right_pix - left_pix + 1; ++i){
        size_t offset = (line * range_x_  + left_pix + i) * 3;
        frame_buffer[offset] = shader(0);
        frame_buffer[offset + 1] = shader(1);
        frame_buffer[offset + 1] = shader(2);
      }
    }




    
  }
  return 0;
}
  



}//namespace


