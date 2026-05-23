# cmake_minimum_required(VERSION 3.10)

# project(muduo_server)

# set(CMAKE_CXX_STANDARD 11)

# # 头文件路径
# include_directories(/usr/include)

# # 库路径
# link_directories(/usr/lib)


# # 设置可执行文件最终存储路径
# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)


# # 生成可执行文件
# add_executable(server muduo_server.cpp)

# # # 设置需要编译的源文件列表
# # set(SRC_LIST muduo_server.cpp)

# # # 把.指定路径下的所有源文件名字放入变量名SRC_LIST里面
# # # aux_source_directory(. SRC_LIST)

# # 链接 muduo 库 + 线程库
# target_link_libraries(server
#     muduo_net
#     muduo_base
#     pthread
# )

# bin:
# lib:
# include:
# src:
# build:
# example:
# thridparty:
# CMakeLists.txt
# autobuild.sh


cmake_minimum_required(VERSION 3.10)

project(muduo_server)

set(CMAKE_CXX_STANDARD 11)

# 头文件路径
include_directories(/usr/include)

# 库路径
link_directories(/usr/lib)

# ✅ 必须放在 add_executable 前面！
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 生成可执行文件
add_executable(server muduo_server.cpp)

# 链接 muduo 库
target_link_libraries(server
    muduo_net
    muduo_base
    pthread
)



