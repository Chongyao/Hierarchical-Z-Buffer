#include "model_obj.h"
#include "geometry.h"
using namespace std;
using namespace Eigen;
namespace marvel{


MatrixXf model_obj::get_bdbox() const{
  MatrixXf bdbox = nods_.col(0) * MatrixXf::Ones(1, 2);
  for(size_t i = 0; i < nods_.cols(); ++i){
    for(size_t j = 0; j < nods_.rows(); ++j){
      if(bdbox(j, 0) > nods_(j, i))
        bdbox(j, 0) = nods_(j ,i);
      if(bdbox(j, 1) < nods_(j, i))
        bdbox(j, 1) = nods_(j ,i);      
    }
  }
  return bdbox;
}

int model_obj::scale_and_translate(const float factor, const Eigen::Vector3f& offset){
    Matrix3f scale_mat = Matrix3f::Identity() * factor;
    nods_ = scale_mat * nods_;
    nods_ = nods_ + offset * MatrixXf::Ones(1, nods_.cols()); 
    return 0;
}

float model_obj::get_dzx(const size_t& poly_id) const{
  assert(if_prepared_);
  return dzx_[poly_id];
}

float model_obj::get_dzy(const size_t& poly_id) const{
  assert(if_prepared_);
  return dzy_[poly_id];
}

size_t model_obj::get_num_tris() const{
  return num_tris_;
}
int model_obj::prepare_for_zbuffer(){
  proj_verts_.resize(3, nods_.cols());
  Matrix<float, 4, 1> z_plane =  Matrix<float, 4, 1>::Zero();z_plane(2) = 1;
  project_triangle(nods_, z_plane, proj_verts_);
  dzx_.resize(num_tris_);
  dzy_.resize(num_tris_);
  for(size_t i = 0; i < num_tris_; ++i){
    auto plane = get_plane(i);
    dzx_[i] = -(*plane)[0] / (*plane)[2];
    dzy_[i] =  (*plane)[1] / (*plane)[2];
  }
  if_prepared_ = true;

}


void model_obj::get_ymax_and_ymin(const size_t& poly_id, size_t& y_max, size_t& y_min) const{
  float max, min;
  get_max_min(proj_verts_(1, tris_(0, poly_id)), proj_verts_(1, tris_(1, poly_id)), proj_verts_(1, tris_(2, poly_id)), max, min);
  y_max = static_cast<int>(ceil(max));
  y_min = static_cast<int>(ceil(min));  
}

shared_ptr<vector<float>> model_obj::get_plane(const size_t poly_id) const{
  auto plane = cal_plane_coeff(nods_.col(tris_(0, poly_id)).data(), nods_.col(tris_(1, poly_id)).data(), nods_.col(tris_(2, poly_id)).data());
  return plane;
}

void model_obj::get_edge_info(const size_t& poly_id, const size_t& edge_id, float& top_x_coor, float& dx, size_t& dy, size_t& y_max, size_t& v_id) const{
  size_t v1 = tris_(edge_id % 3, poly_id);
  size_t v2 = tris_(( edge_id  + 1 ) % 3, poly_id);
  //set v1 max and v2 min
  if (proj_verts_(1, v1) < proj_verts_(1, v2)){
    auto temp = v1;
    v1 = v2;
    v2 = temp;
  }
  
  top_x_coor = proj_verts_(0, v1);
  dx = -(proj_verts_(0, v1) - (proj_verts_(0, v2))) / (proj_verts_(1, v1) - (proj_verts_(1, v2)));
  y_max = static_cast<size_t>(ceil(proj_verts_(1, v1)));
  dy = y_max  - static_cast<size_t>(ceil(proj_verts_(1, v2)));
  v_id = v1;
}
float model_obj::get_depth(const size_t& vertex_id) const{
  return nods_(2, vertex_id);
}

}//namespace
