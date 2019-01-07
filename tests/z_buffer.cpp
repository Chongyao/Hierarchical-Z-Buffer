#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

#include <libigl/include/igl/readOBJ.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "model_obj.h"
#include "z_buffer_alg.h"
using namespace std;
using namespace Eigen;
using namespace marvel;



static float screen_range = 0.75;
static int window_width = 512, window_height = 512;
static float* pixels = new float[window_width * window_height * 3];


struct Color    // generic color class
{
	float r, g, b;  // red, green, blue
};

void loadData()
{
	// point cloud parser goes here
}

// Clear frame buffer
void clearBuffer()
{
	Color clearColor = { 255, 255.0, 0.0 };   // clear color: black
	// for (int i = 0; i<window_width*window_height; ++i)
	// {
	// 	pixels[i * 3] = clearColor.r;
	// 	pixels[i * 3 + 1] = clearColor.g;
	// 	pixels[i * 3 + 2] = clearColor.b;
	// }
}

// Draw a point into the frame buffer
void drawPoint(int x, int y, float r, float g, float b)
{
	int offset = y*window_width * 3 + x * 3;
	pixels[offset] = r;
	pixels[offset + 1] = g;
	pixels[offset + 2] = b;
}

void rasterize()
{
	// Put your main rasterization loop here
	// It should go over the point model and call drawPoint for every point in it
}

// Called whenever the window size changes
void resizeCallback(GLFWwindow* window, int width, int height)
{
	window_width = width;
	window_height = height;
	delete[] pixels;
	pixels = new float[window_width * window_height * 3];
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS /* TODO REMOVE FROM STARTER */ || action == GLFW_REPEAT)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
}

void displayCallback(GLFWwindow* window)
{
	clearBuffer();
	rasterize();

	// glDrawPixels writes a block of pixels to the framebuffer
	glDrawPixels(window_width, window_height, GL_RGB, GL_FLOAT, pixels);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void errorCallback(int error, const char* description)
{
	// Print error
	fputs(description, stderr);
}

int main(int argc, char** argv) {

  Eigen::initParallel();
  cout << "[INFO]>>>>>>>>>>>>>>>>>>>Eigen parallel<<<<<<<<<<<<<<<<<<" << endl;
  cout << "enable parallel in Eigen in " << nbThreads() << " threads" << endl;
  
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>load obj<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
  boost::property_tree::ptree pt;{
    const string jsonfile_path = argv[1];
    
    cout << jsonfile_path << endl;
    const size_t ext = jsonfile_path.rfind(".json");
    if (ext != std::string::npos){
      read_json(jsonfile_path, pt);
      cout << "read json successful" <<endl;
    }
    else{
      cout << "json file extension error" << endl;
      return 0;
    }
  }
  
  cout << "[INFO]>>>>>>>>>>>>>>>>>>>IMPORT MESH<<<<<<<<<<<<<<<<<<" << endl;
  const string mesh_name = pt.get<string>("surf");
  const string indir = pt.get<string>("indir");
  MatrixXi surf;
  MatrixXf nods;
  
  igl::readOBJ((indir+mesh_name+".obj").c_str(), nods, surf);
  cout << "surf: " << surf.rows() << " " << surf.cols() << endl << "nods: " << nods.rows() << " " << nods.cols() << endl;
  
  surf.transposeInPlace();
  nods.transposeInPlace();
  Vector3f color;
  color << 255.0,255.0,0.0;
  shared_ptr<model_obj> model_ptr(new model_obj(surf, nods, color));
  MatrixXf bdbox;
  //>>>>>>>>>>>scale and translate model<<<<<<<<<<
  bdbox = model_ptr->get_bdbox();
  cout << bdbox << endl;
  Vector3f model_center =  (bdbox.col(0) + bdbox.col(1))/2;
  Vector3f model_range = bdbox.col(1) - bdbox.col(0);
  // cout << model_range << endl << model_center << endl;
  float scale_factor = 1.0 / max(model_range(0) / window_width/screen_range, model_range(1)/window_height/screen_range);
  Vector3f screen_;screen_ << window_width, window_height ,0;
  Vector3f offset = scale_factor * (- bdbox.col(0)) + (1 - screen_range) * screen_ / 2  ;
  // offset(2) = scale_factor *(- bdbox(2, 0));
  model_ptr -> scale_and_translate(scale_factor, offset);
  cout << "after scale and translate" << endl;
  bdbox = model_ptr -> get_bdbox();
  cout << bdbox << endl;
  //>>>>>>>>>>>scale and translate model<<<<<<<<<<

#if 1

  model_ptr -> prepare_for_zbuffer();
  z_buffer_alg solver(model_ptr, window_height, window_width);
  solver.exec(pixels);

  size_t count = 0;
  for(size_t i = 0; i < window_width*window_height * 3; ++i){
    if (pixels[i] != 0)
      ++count;
  }
  cout << "pixels not zero num is : " << count << endl;;

  
#endif



  
  // Initialize GLFW
  if (!glfwInit())
  {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return 1;
  }

  // 4x antialiasing
  glfwWindowHint(GLFW_SAMPLES, 4);

  // Create the GLFW window
  GLFWwindow* window = glfwCreateWindow(window_height, window_height, "Rastizer", NULL, NULL);

  // Check if the window could not be created
  if (!window)
  {
    fprintf(stderr, "Failed to open GLFW window.\n");
    glfwTerminate();
    return 1;
  }

  // Make the context of the window
  glfwMakeContextCurrent(window);

  // Set swap interval to 1
  glfwSwapInterval(1);

  loadData();

  // Set the error callback
  glfwSetErrorCallback(errorCallback);
  // Set the key callback
  glfwSetKeyCallback(window, keyCallback);
  // Set the window resize callback
  glfwSetWindowSizeCallback(window, resizeCallback);

  // Loop while GLFW window should stay open
  while (!glfwWindowShouldClose(window))
  {
    // Main render display callback. Rendering of objects is done here.
    displayCallback(window);
  }

  // Destroy the window
  glfwDestroyWindow(window);
  // Terminate GLFW
  glfwTerminate();

  exit(EXIT_SUCCESS);
}


