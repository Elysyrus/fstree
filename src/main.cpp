#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include "scanner.hpp"
#include "encoding_fix.hpp"

namespace fs = std::filesystem;

// 声明 human_size（定义在 scanner.cpp 里）
std::string human_size(uint64_t bytes);

int main(int argc, char* argv[]) {
    auto args = std::vector<std::string>(argv + 1, argv + argc);

    if (args.empty()) {
        std::cout << "Usage: fstree <path>" << std::endl;
        return 1;
    }

    fs::path target = path_from_narrow(args[0]);

    if (!fs::exists(target)) {
        std::cerr << "Error: path does not exist: " << target << std::endl;
        return 1;
    }
    if (!fs::is_directory(target)) {
        std::cerr << "Error: not a directory: " << target << std::endl;
        return 1;
    }

    // 打印根目录
    auto total = calc_size(target);
    std::cout << "📁 " << to_utf8(target.filename())
              << "  (" << human_size(total) << ")" << std::endl;

    // 收集根目录的子项
    std::vector<fs::path> children;
    // 把target目录下的所有项都放到children里
    for (const auto& entry : fs::directory_iterator(target)) {
        children.push_back(entry.path());
    }
    // 按照路径排序，保证输出稳定（不加这个，每次运行可能顺序都不一样）
    std::sort(children.begin(), children.end());

    // 打印树
    for (size_t i = 0; i < children.size(); ++i) {
        bool is_last = (i == children.size() - 1);
        try {
            print_tree(children[i], "", is_last);
        } catch (const fs::filesystem_error&) {}
    }

    return 0;
}