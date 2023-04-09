cmake_minimum_required(VERSION 3.15.0)
project(http_app VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

option(CMAKE_BUILD_TYPE "build type, Debug/Release" Debug)

## 查找依赖
find_package(Boost REQUIRED COMPONENTS headers thread)
message(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARY_DIRS = ${Boost_LIBRARY_DIRS}")
message(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")
message(STATUS "Boost_LIB_VERSION = ${Boost_LIB_VERSION}")

include(FetchContent)
FetchContent_Declare(
  libhv
  GIT_REPOSITORY https://gitee.com/libhv/libhv.git
  GIT_TAG c9eda2fc88a3caea7ae73f89e4d2a6be0d1a5bbe
)
FetchContent_MakeAvailable(libhv)           # hv_static

# 本项目依赖的头文件目录
set(PROJECT_INCLUDE_DIR "")
list(
	APPEND PROJECT_INCLUDE_DIR 
	  ${PROJECT_SOURCE_DIR}
)
# 本项目依赖的库
set(PROJECT_LINK_LIBRARIES "")
list(
	APPEND PROJECT_LINK_LIBRARIES
    ${Boost_LIBRARIES}
    pthread
    Boost::headers
    hv_static
)

FILE (GLOB PROJECT_SOURCES "*.cpp" "http-app/**/*.cpp" )
message(STATUS "${PROJECT_SOURCES}")

add_library(
    ${PROJECT_NAME} STATIC
    ${PROJECT_SOURCES}
)

target_include_directories(
	${PROJECT_NAME} PUBLIC
	${PROJECT_INCLUDE_DIR}
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC 
    ${PROJECT_LINK_LIBRARIES}
)

target_compile_options(${PROJECT_NAME} PUBLIC -std=c++20)

# 区分调试模式和发布模式的编译参数
message(STATUS "${CMAKE_BUILD_TYPE} 模式")
if(CMAKE_BUILD_TYPE MATCHES Release)
  target_compile_options(
    ${PROJECT_NAME} PRIVATE
		-O3
		-DNDEBUG # 定义宏 NDEBUG，关闭 assert() 函数
	)

else()
  target_compile_options(
    ${PROJECT_NAME} PRIVATE
    -g
    -ggdb
    -O0
    -DDEBUG	# 定义宏 DEBUG
  )
endif()

# 编译例子程序
add_subdirectory(examples)