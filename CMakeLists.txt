cmake_minimum_required(VERSION 3.14)
project(twoSum LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# MSVC 下把源码/执行字符集都当作 UTF-8,否则中文注释会被 GBK 误解析
if(MSVC)
  add_compile_options(/utf-8)
endif()

# ---------- 算法库(纯逻辑,可复用、可测试)----------
add_library(two_sum src/two_sum.cpp)
target_include_directories(two_sum PUBLIC include)

# ---------- 主程序(只负责输入输出)----------
add_executable(twoSum src/main.cpp)
target_link_libraries(twoSum PRIVATE two_sum)

# ---------- 可视化 GUI(Qt Widgets,可选)----------
# 优先 Qt6(Mac/Linux 上 Homebrew/包管理器默认),回退 Qt5(如 Windows Anaconda)。
# 两者都找不到就自动跳过 GUI,不影响命令行程序与测试。
option(BUILD_GUI "构建 Qt 可视化界面" ON)

set(QT_GUI_FOUND FALSE)
if(BUILD_GUI)
  find_package(Qt6 COMPONENTS Widgets QUIET)
  if(Qt6_FOUND)
    set(QT_VER 6)
    set(QT_GUI_FOUND TRUE)
  else()
    find_package(Qt5 COMPONENTS Widgets QUIET)
    if(Qt5_FOUND)
      set(QT_VER 5)
      set(QT_GUI_FOUND TRUE)
    endif()
  endif()
endif()

if(QT_GUI_FOUND)
  set(CMAKE_AUTOMOC ON)
  add_executable(twoSumGui
    MACOSX_BUNDLE            # 在 macOS 上产出 .app;其他平台忽略此关键字
    gui/gui_main.cpp
    gui/mainwindow.cpp
    gui/visualizer.cpp
    gui/mainwindow.h
    gui/visualizer.h
  )
  target_link_libraries(twoSumGui PRIVATE two_sum Qt${QT_VER}::Widgets)
  if(APPLE)
    set_target_properties(twoSumGui PROPERTIES
      MACOSX_BUNDLE_BUNDLE_NAME "TwoSum"
      MACOSX_BUNDLE_GUI_IDENTIFIER "com.ada-zf1225.twosum"
      MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
      MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
    )
  endif()
  message(STATUS "找到 Qt${QT_VER} -> 将构建 GUI 目标 twoSumGui")
else()
  message(STATUS "未找到 Qt(或 BUILD_GUI=OFF)-> 跳过 GUI,仅构建 CLI + 测试")
endif()

# ---------- 单元测试 ----------
enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
)
# Windows 上避免 gtest 覆盖父项目的 CRT 设置
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(two_sum_tests tests/test_two_sum.cpp)
target_link_libraries(two_sum_tests PRIVATE two_sum GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(two_sum_tests)
