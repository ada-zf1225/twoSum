#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "two_sum.h"

int main() {
    while (true) {
        std::cout << "请输入一行数字,空格分隔 (直接回车或输入 q 退出): ";
        std::string line;
        if (!std::getline(std::cin, line)) {
            break;  // 读到文件结束 (Ctrl+Z)
        }

        // 逐个抽取整数,数量自动识别
        std::istringstream iss(line);
        std::vector<int> nums;
        int x;
        while (iss >> x) {
            nums.push_back(x);
        }

        if (nums.empty()) {
            std::cout << "已退出。\n";
            break;  // 空行 或 q 等非数字
        }

        std::cout << "请输入目标值 target: ";
        int target;
        if (!(std::cin >> target)) {
            break;
        }
        // 清掉 target 后面残留的换行,方便下一轮 getline
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        const auto pairs = algo::two_sum(nums, target);
        if (pairs.empty()) {
            std::cout << "结果: 没有找到任何配对\n\n";
        } else {
            std::cout << "共找到 " << pairs.size() << " 组:\n";
            for (const auto& p : pairs) {
                std::cout << "  [" << p.first << "," << p.second << "]\n";
            }
            std::cout << "\n";
        }
    }

    return 0;
}
