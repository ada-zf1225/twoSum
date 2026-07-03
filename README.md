# twoSum · 两数之和(可视化)

[![CI](https://github.com/ada-zf1225/twoSum/actions/workflows/ci.yml/badge.svg)](https://github.com/ada-zf1225/twoSum/actions/workflows/ci.yml)

LeetCode「两数之和」的工程化实现:纯算法库 + 命令行程序 + **Qt 可视化界面**,
带单元测试,一套 CMake 代码在 **macOS / Linux / Windows** 都能构建运行。

## 功能

- **算法库** `two_sum` —— 返回**所有**满足 `nums[i] + nums[j] == target` 的下标对(`i < j`)。
- **命令行** `twoSum` —— 交互式:一行输入任意个数字(自动识别个数),回车/`q` 退出。
- **可视化 GUI** `twoSumGui` —— 数组画成方块,可**单步 / 播放**观察哈希表扫描过程,
  当前扫描位、命中的搭档、已确认的配对连线都有颜色区分。
- **单元测试** —— GoogleTest,`ctest` 一键运行;CI 在三大系统上自动验证。

## 目录结构

```
twoSum/
├── CMakeLists.txt              # 构建脚本(Qt 可选,找不到就只构建 CLI + 测试)
├── include/two_sum.h           # 算法接口
├── src/
│   ├── two_sum.cpp             # 算法实现(纯逻辑,零 IO)
│   └── main.cpp                # 命令行交互
├── gui/                        # Qt Widgets 界面(复用算法库)
│   ├── gui_main.cpp
│   ├── mainwindow.{h,cpp}
│   └── visualizer.{h,cpp}
├── tests/test_two_sum.cpp      # GoogleTest 单元测试
└── .github/workflows/ci.yml    # 三平台 CI
```

## 快速开始

前置:CMake ≥ 3.14、支持 C++17 的编译器、Qt(5 或 6,仅 GUI 需要)。
首次构建会自动联网下载 GoogleTest。

### macOS

```bash
brew install cmake qt                       # 装 CMake 和 Qt6
git clone https://github.com/ada-zf1225/twoSum.git
cd twoSum
cmake -S . -B build -DCMAKE_PREFIX_PATH="$(brew --prefix qt)"
cmake --build build

./build/twoSumGui                           # 打开可视化界面
./build/twoSum                              # 或运行命令行版
ctest --test-dir build --output-on-failure  # 跑测试
```

> 如果只想要命令行、不装 Qt:`cmake -S . -B build -DBUILD_GUI=OFF`。

### Linux (Debian/Ubuntu)

```bash
sudo apt update && sudo apt install -y cmake g++ qt6-base-dev
git clone https://github.com/ada-zf1225/twoSum.git && cd twoSum
cmake -S . -B build
cmake --build build
./build/twoSumGui
ctest --test-dir build --output-on-failure
```

### Windows

装好 Visual Studio(含 C++ 组件)和 Qt。用 `文件 > 打开 > CMake...` 选中
`CMakeLists.txt` 即可;或命令行:

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH="<Qt 安装路径，如 C:/Qt/6.7.2/msvc2019_64>"
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
# GUI 首次运行前部署 Qt 运行库:
#   <Qt>/bin/windeployqt.exe build/Release/twoSumGui.exe
.\build\Release\twoSumGui.exe
```

## 算法说明

用哈希表一次遍历。对每个 `nums[i]`,查表里是否已存在搭档 `target - nums[i]`;
存在就配对,然后把当前元素记入表。返回的下标对**小的在前**。

- 时间复杂度 **O(n + k)**,空间 **O(n + k)**(`k` = 配对数量,输出敏感)。
- GUI 用 `two_sum_trace` 记录每步扫描过程,供动画回放,算法本体仍保持零 IO、可测试。

## 许可证

[MIT](LICENSE)
