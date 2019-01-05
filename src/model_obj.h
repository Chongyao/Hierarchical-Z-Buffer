#ifndef MODEL_OBJ_H
#define MODEL_OBJ_H
#include <Eigen/Core>

namespace marvel{


class model_obj{
 public:
  model_obj(const Eigen::MatrixXi& tris, const Eigen::MatrixXf& nods):tris_(tris), nods_(nods), num_tris_(tris.cols()){}

  Eigen::MatrixXf get_bdbox() const;

  int scale_and_translate(const float factor, const Eigen::Vector3f& offset);
 // protected:
  size_t num_tris_;
  Eigen::MatrixXi tris_;
  Eigen::MatrixXf nods_;
};

}

#endif
