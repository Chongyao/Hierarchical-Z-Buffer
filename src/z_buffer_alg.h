#ifndef Z_BUFFER_ALG_H
#define Z_BUFFER_ALG_H

#include<list>
#include<vector>
#include<Eigen/Dense>
#include <memory>
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
};
struct active_edge{
  float x1;
  float dx1;
  size_t dy1;

  float xr;
  float dxr;
  size_t dyr;

  size_t id;
  
};






class z_buffer_alg{
public:
  z_buffer_alg(const int* tris, const float* nods, const size_t& num_v, const size_t& num_f, const size_t& range_y);
  int exec(std::vector<float>& frame_buffer);
      
// private:
  int construct_polygen_table(const int* tris, const float* nods, const size_t& num_v, const size_t num_f);
  int construct_edge_table(const int* tris, const float* nods, const size_t& num_v, const size_t num_f);
  int construct_active_polygen_table(); 
  int construct_active_edge_table();
  
  std::vector<std::vector<polygen>> polygen_table_;
  std::vector<std::vector<edge>> edge_table_;
  std::list<std::list<polygen>> active_polygen_table_;
  std::list<std::list<active_edge>> active_edge_table_;

  size_t range_y;
  std::shared_ptr<std::vector<float>> color_ptr;
};


}
#endif

