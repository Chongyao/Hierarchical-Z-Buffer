#include <frame_buffer.h>

using namespace std;
using namespace Eigen;


// int frame_buffer::init_mbuffer(const int value){
//   fill(m_buffer_.begin(), m_buffer_.end(), value);
//   return 0;
// }


// int frame_buffer::get_height(){
//   return height_;
// }

// int frame_buffer::get_width(){
//   return width_;
//}
int frame_buffer::write_pixel(const int x, const int y, const Eigen::Matrix<GLubyte, 3, 1> color){
  if (x < 0 || y < 0 || x > width_ || y > height_) return 1;

  copy(color.data(), color.data() + 3, &m_buffer_[(width_ * y + x) * 3]);
  return 0;
}
