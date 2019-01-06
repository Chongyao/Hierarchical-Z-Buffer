#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <iostream>
#include <Eigen/Core>
#include <math.h>
#include <vector>
#include <memory>
#include <algorithm>
// using namespace Eigen;
namespace marvel{

double clo_surf_vol(const Eigen::MatrixXd &nods, const Eigen::MatrixXi &surf);
int build_bdbox(const Eigen::MatrixXd &nods, Eigen::MatrixXd & bdbox);

template < typename T>
int project_triangle(const Eigen::Matrix<T, -1, -1> &vertices, const Eigen::Matrix<T, 4, 1>& plane, Eigen::Matrix<T, -1, -1>&  projected_triangle){
  #pragma parallel omp for
  for(size_t i = 0; i < vertices.cols(); ++i){
    //only loop x and y to save depth!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
    for(size_t j = 0; j < 3; ++j){
      size_t dim_a = j, dim_b = (j + 1) % 3, dim_c = ( j + 2) % 3;
      projected_triangle(j, i) = (pow(plane(dim_b), 2) + pow(plane(dim_c), 2)) * vertices(dim_a, i)
          -plane(dim_a) * (plane(dim_b) * vertices(dim_b, i) + plane(dim_c) * vertices(dim_c, i) + plane(3));
    }
  }
  projected_triangle /= plane(0) * plane(0) + plane(1) * plane(1) + plane(2) * plane(2);

  return 0;
}


template <typename T>
bool cross_points(const int scan_line_y, const Eigen::Matrix<T, 3, 3>& nods_of_tri,  std::vector<int>& x_values ){

  x_values.resize(0);
  x_values.reserve(2);
  int x1, x2, y1, y2;
  for(size_t i = 0; i < 3; ++i){
    
    if (x_values.size() == 2)
      break;
    
    x1 = nods_of_tri(0, i%3);
    x2 = nods_of_tri(0, (i + 1) %3);
    y1 = nods_of_tri(1, i%3);
    y1 = nods_of_tri(1, (i + 1)%3);
    if( (scan_line_y < y1 && scan_line_y > y2) || (scan_line_y < y2 && scan_line_y > y1) ){
      if(fabs(y1 - y2 ) > 1e-5){
        x_values.push_back( (x1 - x2)/(y1 - y2) * (scan_line_y - y1) + x1 );
      }
    }
  }

  if(x_values.size() > 0)
    return true;
  else
    return false;


}

template<typename T>
std::shared_ptr<std::vector<float>>  cal_plane_coeff(const T* v1_ptr, const T* v2_ptr, const T* v3_ptr){
  Eigen::Map<const Eigen::Matrix<T, 3, 1>> v1(v1_ptr, 3, 1);
  Eigen::Map<const Eigen::Matrix<T, 3, 1>> v2(v2_ptr, 3, 1);
  Eigen::Map<const Eigen::Matrix<T, 3, 1>> v3(v3_ptr, 3, 1);
  
  Eigen::Matrix<T, 3, 1> product = (v2 - v1).cross(v3 - v1);
  std::shared_ptr<std::vector<float>> plane_ptr(new std::vector<float>(4));
  std::copy(product.data(), product.data() + 3, plane_ptr->data());
  
  (*plane_ptr)[3] = - (*plane_ptr)[0] * v1(0) - (*plane_ptr)[1] * v1(1) - (*plane_ptr)[2] * v1(2);
  return plane_ptr;
}

template<typename T>
inline void get_max_min(const T& a, const T& b, const T& c, T& max, T& min){

  auto compare = [&max, &min](const T& me, const T& you, const T& him)-> bool{
    if(me < you || me < him )
      return false;
    else{
      if(you > him)
        min = him;
      else
        min = you;
      max = me;
      return true;
    }
  };
  if ( !compare(a, b, c) ){
    if ( !compare(b, c, a))
      compare(c, a, b);
  }
    
}


}//namespcae : marvel
#endif
