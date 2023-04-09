#
# 拉取存放 proto 文件的仓库，并编译成 C++ 库
# @param OUTPUT_NAME 指定编译结果的自定义名称
# @param PROTO_DIR 指定需要编译的 proto 目录
#
# 宏 fetch_proto 执行后，会定义两个变量，用于后续链接库文件和引入头文件，
# 输出的变量是 <OUTPUT_NAME>_INCLUDES <OUTPUT_NAME>_LIBRARIES
# 用法示例:
# fetch_proto(my_proto proto)
# target_include_directories(main PUBLIC ${my_proto_INCLUDES})
# target_link_libraries(main PUBLIC ${my_proto_LIBRARIES})
#
macro(fetch_proto OUTPUT_NAME GIT_URL)
    set(PROTO_SRC_DIR "${PROJECT_BINARY_DIR}/_proto/${OUTPUT_NAME}-src")
    set(PROTO_OUTPUT_DIR "${PROJECT_BINARY_DIR}/_proto/${OUTPUT_NAME}")
    set(PROTO_SRCS "")
    set(PROTO_HDRS "")

    if(EXISTS ${PROTO_SRC_DIR})
		if (CMAKE_HOST_SYSTEM_NAME MATCHES Linux)
			execute_process(COMMAND rm -rf ${PROTO_SRC_DIR})
			execute_process(COMMAND rm -rf ${PROTO_OUTPUT_DIR})
		elseif (CMAKE_HOST_SYSTEM_NAME MATCHES Windows)
			execute_process(COMMAND ${CMAKE_COMMAND} -E rm -rf ${PROTO_SRC_DIR})
			execute_process(COMMAND ${CMAKE_COMMAND} -E rm -rf ${PROTO_OUTPUT_DIR})
		endif()
    endif()
    message(STATUS "开始拉取 proto 仓库 ${GIT_URL}")
    execute_process(COMMAND git clone --depth=1 ${GIT_URL} ${PROTO_SRC_DIR})
    
    include(${PROJECT_SOURCE_DIR}/cmake-utility/compile_proto.cmake)
    compile_proto(${OUTPUT_NAME} ${PROTO_SRC_DIR})
endmacro()
