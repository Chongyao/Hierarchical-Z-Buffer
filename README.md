# 区间扫描线Z Buffer算法　说明文档

## 开发环境
### 系统信息
系统 | 信息
------------ | ------------- 
操作系统 | Linux Mint 19 Cinnamon 
处理器 | Intel© Core™ i7-4790 CPU @ 3.60GHz × 4
内存　|　32GB
显卡　| GeForce GTX 750
编译器　| gcc 7.3
编译工具 |　Cmake 3.10.2
### 项目依赖库
图形库：Opengl、GLFW、GLEW

矩阵库：Eigen3

并行库：Openmp(verison:4.5)

常用库:Boost(version:1.65.1)

具体查看根目录下的CMakeLists.txt
## 编译运行
在满足依赖的情况下，进入根目录，开启终端运行：
```
$ mkdir build & cd build & cmake .. & make -k
```
### 输入模型
将模型（.obj格式）放入根目录下的models/中，并修改根目录下的input.json文件。

*input.json*
```javascript
{
  "indir":"../../models/",
  "surf":"bunny"
}
```
将surf中的bunny改为模型名称。可以直接使用bunny作为demo测试。
### 运行
开启终端进入build/bin/目录，并运行./z_buffer
```bash
$ cd build/bin/
$ ./z_buffer
```
## 数据结构
### model_obj类
主要用来存储一个obj模型，包括每一个vertex的位置和拓扑关系，以及每一个面片的颜色（模型的颜色乘该面片的法向与屏幕坐标系ｚ轴的夹角的余弦值）。同时提供了一系列获取该模型的一些信息的函数为后续扫描算法提供数据。

### z_buffer_alg类
依照ppt中，建立了多边形表，边表，活化边表和活化多边形表。用于执行算法。

其中更新帧缓冲器的函数z-buffer_alg::update_buffer为使用了扫描线z_buffer，z_buffer_alg::section_update_buffer为使用区间扫面线。







