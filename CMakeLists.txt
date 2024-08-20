cmake_minimum_required(VERSION 3.10)
project(myTinyWebServer)


# 生成compile_commands.json文件，让ccls能正常识别项目
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# .h文件的目录从哪里开始找
include_directories(.)

# 如果要创建测试程序，则将这个BUILD_TEST设置为ON打开
option(BUILD_TEST "ON for compile test" ON)

# 添加日志模块的源文件
set(LIB_SRC
    tinyWebserver/log/log.cpp
)

# 创建日志模块的库, 这里要创建的是动态库，由中间的参数SHARED来制定，创建静态库为STATIC
add_library(TinyWebServer SHARED ${LIB_SRC}) #动态库

# 将用到的动态库打包到LIBS变量中
set(LIBS
    TinyWebServer)

# 添加头文件搜索路径
##target_include_directories(PUBLIC ${CMAKE_SOURCE_DIR})

#定义函数生成多个测试文件
function(tinywebserver_add_executable targetname srcs depends libs)
    add_executable(${targetname} ${srcs})
    add_dependencies(${targetname} ${depends})
    target_link_libraries(${targetname} ${libs})
endfunction()

# 添加测试可执行文
if(BUILD_TEST)
    tinywebserver_add_executable(test_log "test/testlog.cpp" TinyWebServer "${LIBS}")
endif()


#设置库文件的输出路径为./lib文件夹下
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib) #动态库
#设置二进制可执行文件的输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
