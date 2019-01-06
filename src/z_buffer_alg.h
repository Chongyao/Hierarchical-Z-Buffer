#ifndef Z_BUFFER_ALG_H
#define Z_BUFFER_ALG_H

#include<list>
#include<vector>
#include<Eigen/Dense>
#include <memory>
#include "model_obj.h"

namespace marvel{


struct polygen{
  std::shared_ptr<std::vector<float>> plane_ptr;
  size_t id;
  size_t dy;
  std::shared_ptr<std::vector<float>> color_ptr;
};

struct edge{
  float x;
  float dx;
  size_t dy;
  size_t id;
  size_t v_id;//0~2 the top vertex id
};
struct active_edge{
  float xl;
  float dxl;
  size_t dy1;

  float xr;
  float dxr;
  size_t dyr;

  float zl;
  float dzl;
  float dzy;
  
  size_t id;
  
};
class z_buffer_alg{
 public:
  z_buffer_alg(const std::shared_ptr<model_obj> model_ptr, const size_t& range_y);
  int exec(std::vector<float>& frame_buffer, const size_t& num_frames);
  
 // private:
  int construct_polygen_table();
  int construct_edge_table();
  int construct_active_polygen_table(); 
  int construct_active_edge_table();
  
  int activate_polygens_and_edges(const size_t& line);
  int updata_active_polys();
  
  size_t range_y_;  
  std::vector<std::vector<polygen>> polygen_table_;
  std::vector<std::vector<edge>> edge_table_;
  std::list<polygen> active_polygen_table_;
  std::list<active_edge> active_edge_table_;
  //TODO:expand it to vector
  std::shared_ptr<model_obj> model_ptr_;
  std::shared_ptr<std::vector<float>> color_ptr;
};


}
#endif

