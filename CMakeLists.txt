# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)
# 设置项目名称
project(socket_server)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)

# 指定头文件搜索路径
include_directories(./)

# 设置源文件
set(SOURCES ini_file.cpp)

# 添加可执行文件
add_executable(main ${SOURCES} main.cpp)
