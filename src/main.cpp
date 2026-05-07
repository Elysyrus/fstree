#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "scanner.hpp"   // 引入我们自己写的模块

namespace fs = std::filesystem;

// 把字节数转成人类可读的格式：1024 → "1.0 KB"
// 这个函数先让 AI 生成，重点在于理解它被调用的地方
std::string human_size(uint64_t bytes) {
    const double KB = 1024.0;
    const double MB = 1024.0 * 1024.0;
    const double GB = 1024.0 * 1024.0 * 1024.0;

    char buf[32];
    if (bytes >= GB)
        snprintf(buf, sizeof(buf), "%.1f GB", bytes / GB);
    else if (bytes >= MB)
        snprintf(buf, sizeof(buf), "%.1f MB", bytes / MB);
    else if (bytes >= KB)
        snprintf(buf, sizeof(buf), "%.1f KB", bytes / KB);
    else
        snprintf(buf, sizeof(buf), "%llu B", (unsigned long long)bytes);
    return std::string(buf);
}

int main(int argc, char* argv[]) {
    // 含义是：把命令行参数（argv 数组）转成一个 vector<string>，跳过第 0 个（程序名本身）。
    auto args = std::vector<std::string>(argv + 1, argv + argc);

    if (args.empty()) {
        std::cout << "Usage: fstree <path>" << std::endl;
        return 1;
    }

    fs::path target(args[0]);

    if (!fs::exists(target)) {
        std::cerr << "Error: path does not exist: " << target << std::endl;
        return 1;
    }

    if (!fs::is_directory(target)) {
        std::cerr << "Error: not a directory: " << target << std::endl;
        return 1;
    }

    // 计算并打印总大小
    auto total = calc_size(target);
    std::cout << "📁 " << target.filename().string()
              << "  (" << human_size(total) << ")" << std::endl;

    // 打印第一层子项（还没有树形，先用简单格式）
    for (const auto& entry : fs::directory_iterator(target)) {
        auto size = calc_size(entry.path());
        auto name = entry.path().filename().string();
        std::cout << "  " << name << "  " << human_size(size) << std::endl;
    }

    return 0;
}