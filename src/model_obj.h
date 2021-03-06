#ifndef MODEL_OBJ_H
#define MODEL_OBJ_H
#include <Eigen/Dense>
#include <memory>
#include <vector>
namespace marvel{


class model_obj{
 public:
  model_obj(const Eigen::MatrixXi& tris, const Eigen::MatrixXf& nods, const Eigen::Vector3f& color):tris_(tris), nods_(nods), num_tris_(tris.cols()), if_prepared_(false){
    color_ = color;
  }

  Eigen::MatrixXf get_bdbox() const;
  int scale_and_translate(const float factor, const Eigen::Vector3f& offset);

  int prepare_for_zbuffer();
  size_t get_num_tris()const ;
  float get_dzx(const size_t& poly_id) const;
  float get_dzy(const size_t& poly_id) const;
  void get_ymax_and_ymin(const size_t& poly_id, size_t& y_max, size_t& y_min) const;
  void get_edge_info(const size_t& poly_id, const size_t& edge_id, float& top_x_coor, float& dx, int& dy, size_t& y_max, size_t& v_id) const;
  std::shared_ptr<std::vector<float>> get_plane(const size_t poly_id) const;
  float get_depth(const size_t& vertex_id) const;
  Eigen::Vector3f get_depth_shader_value(const size_t& poly_id) const;
  Eigen::Vector3f get_color() const;
 protected:
  size_t num_tris_;
  Eigen::MatrixXi tris_;
  Eigen::MatrixXf nods_;
  Eigen::Vector3f color_;
  
  std::vector<float> dzx_;
  std::vector<float> dzy_;
  // std::vector<float> shader_;
  bool if_prepared_;

  Eigen::MatrixXf shader_;


  void set_verts_to_pixels();
  
};
}

#endif
