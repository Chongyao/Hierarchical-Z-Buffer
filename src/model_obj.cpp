#include "model_obj.h"

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

}
