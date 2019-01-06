#include "z_buffer_alg.h"
#include "geometry.h"
#include <iostream>
using namespace std;
using namespace Eigen;
namespace marvel{





int z_buffer_alg::construct_polygen_table(){
  for(size_t i = 0; i < model_ptr_ -> get_num_tris(); ++i){
      size_t y_max, y_min;
      model_ptr_ -> get_ymax_and_ymin(i, y_max, y_min);
      
      polygen_table_[y_max].push_back({std::move(model_ptr_ -> get_plane(i)), i, y_max - y_min, color_ptr});
    }

}
  
int z_buffer_alg::construct_edge_table(){

    for(size_t i = 0; i < model_ptr_ -> get_num_tris(); ++i){
      for(size_t j = 0; j < 3; ++j){
        float x, dx;
        size_t dy, y_max, v_id;
        model_ptr_ -> get_edge_info(i, j, x, dx, dy, y_max, v_id);
        edge_table_[y_max].push_back({x, dx, dy, i, v_id});
      }
    }
}


z_buffer_alg::z_buffer_alg(const shared_ptr<model_obj> model_ptr, const size_t& range_y):range_y_(range_y), model_ptr_(model_ptr){
  
  color_ptr = make_shared<vector<float>>(3);
  *color_ptr = {0, 255, 255};
  polygen_table_ = vector<vector<polygen>>(range_y_);
  edge_table_ = vector<vector<edge>>(range_y_);
  active_polygen_table_ = list<polygen>(0);
  active_edge_table_ = list<active_edge>(0);


  construct_polygen_table();

  construct_edge_table();
}

int z_buffer_alg::exec(vector<float>& frame_buffer, const size_t& num_frames){
  Map<MatrixXf> map_frame_buffer(frame_buffer.data(), 3, num_frames);
  //map_frame_buffer = MatrixXf::Zero(3, num_frames);

  VectorXf z_buffer = VectorXf::Zero(num_frames);
  
  for(size_t line = range_y_ -1 ; line >= 0; --line){
    if( !polygen_table_[line].empty() ){
      activate_polygens_and_edges(line);
      

    }
    updata_active_polys();
  }
  
}


int z_buffer_alg::updata_active_polys(){
  for(auto iter = active_polygen_table_.begin(); iter != active_polygen_table_.end();){
    if( --( (*iter).dy ) == 0)
      active_polygen_table_.erase(iter);
    else
      ++iter;
  }
}

int z_buffer_alg::activate_polygens_and_edges(const size_t& line){
  size_t iter_edge = 0;
  
  for(const auto& poly : polygen_table_[line]){
    active_polygen_table_.push_back(poly);

    
    for(size_t i = iter_edge; i < edge_table_[line].size(); ++i){
      if(edge_table_[line][iter_edge].id == poly.id){
        assert(edge_table_[line][iter_edge].id == poly.id);
        active_edge_table_.push_back({
            edge_table_[line][iter_edge].x, edge_table_[line][iter_edge].dx, edge_table_[line][iter_edge].dy,
                edge_table_[line][iter_edge + 1].x, edge_table_[line][iter_edge + 1].dx,edge_table_[line][iter_edge + 1].dy,
                model_ptr_ -> get_depth(edge_table_[line][iter_edge].v_id), model_ptr_ -> get_dzx(poly.id), model_ptr_ -> get_dzy(poly.id)});
        iter_edge = i +2;
        break;
      }
      else
        continue;
        
    }
  }
  return 0;
}

}//namespace
