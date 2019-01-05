#include "z_buffer_alg.h"
#include "geometry.h"
#include <iostream>
using namespace std;
using namespace Eigen;
namespace marvel{





int z_buffer_alg::construct_polygen_table(const int* tris, const float* nods, const size_t& num_v, const size_t num_f){

    Map<const MatrixXf> vertices(nods, 3, num_v);
    Map<const MatrixXi> triangles(tris, 3, num_f);
    for(size_t i = 0; i < num_f; ++i){
      float y_max, y_min;
      get_max_min(vertices(1, triangles(0,i)), vertices(1, triangles(1, i)), vertices(2, triangles(2, i)),
                  y_max, y_min);
      auto plane_ptr = cal_plane_coeff(vertices.col(triangles(0,i)).data(), vertices.col(triangles(1, i)).data(), vertices.col(triangles(2,i)).data());
      polygen_table_[static_cast<int>(ceil(y_max))].push_back({plane_ptr, i, static_cast<size_t>(ceil(y_max - y_min)), color_ptr});
    }

}
  
int z_buffer_alg::construct_edge_table(const int* tris, const float* nods, const size_t& num_v, const size_t num_f){
    Map<const MatrixXf> vertices(nods, 3, num_v);
    Map<const MatrixXi> triangles(tris, 3, num_f);
    for(size_t i = 0; i < num_f; ++i){
      for(size_t i = 0; i < 3; ++i){
        float y_max, y_min;
      }
    }
}


z_buffer_alg::z_buffer_alg(const int* tris, const float* nods, const size_t& num_v, const size_t& num_f, const size_t& range_y){
  color_ptr = make_shared<vector<float>>(3);
  *color_ptr = {0, 255, 255};
  
  polygen_table_ = vector<vector<polygen>>(range_y + 1);
  edge_table_ = vector<vector<edge>>(range_y + 1);
  construct_polygen_table(tris, nods, num_v, num_f);
  construct_edge_table(tris, nods, num_v, num_f);
}



}
