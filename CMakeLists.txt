cmake_minimum_required(VERSION 3.0.0)

project(my-sm2 VERSION 0.1.0)

include_directories(${PROJECT_SOURCE_DIR}/include)# -I
link_directories(${PROJECT_SOURCE_DIR}/lib)# -L

file(GLOB SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp 
    ${PROJECT_SOURCE_DIR}/src/*.c)#搜索src中所有可编译文件

add_executable(my-sm2-gektest gektest.cpp ${SRC_FILES}) #密钥对生成测试
add_executable(my-sm2-enctest enctest.cpp ${SRC_FILES}) #加解密测试
#add_executable(my-sm2-sigtest sigtest.cpp ${SRC_FILES}) #签名测试

#add_executable(my-sm2-test sm2test.cpp ${SRC_FILES}) #所有测试

target_link_libraries(my-sm2-gektest miracl)
target_link_libraries(my-sm2-enctest miracl)
#target_link_libraries(my-sm2-sigtest miracl)
#target_link_libraries(my-sm2-test miracl)

add_subdirectory(use_sample)
