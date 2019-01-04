#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <GLFW/glfw3.h>
#include <Eigen/Core>
#include <vector>

#include <iostream>
class frame_buffer{
 public:
  frame_buffer():height_(0), width_(0){}
  frame_buffer(const int height, const int width):height_(height),width_(width),m_buffer_(std::vector<GLubyte>(height * width * 3)){}
  
  int init_mbuffer(const int value){
      fill(m_buffer_.begin(), m_buffer_.end(), value);


      return 0;
  }
  int write_pixel(const int x, const int y, const Eigen::Matrix<GLubyte, 3, 1> color);
  
  GLubyte* get_buffer(){
    return &m_buffer_[0];
  }
  int get_height(){
    return height_;
  }
  int get_width(){
    return width_;
  }


  void test_buffer(){
    for(size_t i = 0; i < height_ * width_; ++i){
      m_buffer_[i * 3 + 0] = 0;
      m_buffer_[i * 3 + 1] = 255;
      m_buffer_[i * 3 + 2] = 255;
    }

  }
 private:
  int height_, width_;
  std::vector<GLubyte> m_buffer_;
  // GLubyte m_buffer_[]
  
};




#endif

