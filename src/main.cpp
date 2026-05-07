#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;
// 打印使用说明
void print_usage() {
    std::cout << "fstree - file system tree analyzer" << std::endl;
    std::cout << "Usage: fstree <path> [options]" << std::endl;
    std::cout << "  <path>   Directory to analyze" << std::endl;
}

// main函数里面的参数是命令行工具的入口
// argc 是命令行参数的数量，argv 是一个数组，包含了所有的命令行参数
int main(int argc, char* argv[]) {
    // 把命令行参数转成 vector，更方便处理
    // argv 是 C 风格的数组，这里把它转成现代 C++ 的 vector
    // auto是类型推断，让编译器自行推断这是什么类型
    auto args = std::vector<std::string>(argv + 1, argv + argc);

    if (args.empty()) {
        print_usage();
        return 1;
    }
    // 把字符串变成 fs::path 对象
    // fs::path 不只是字符串，它理解路径的结构（斜杠、文件名、扩展名等）
    fs::path target(args[0]);
    // 用 fs 的函数检查路径
    std::cout << "Path: "       << target                    << std::endl;
    std::cout << "Exists: "     << fs::exists(target)        << std::endl;
    std::cout << "Is dir: "     << fs::is_directory(target)  << std::endl;
    std::cout << "Is file: "    << fs::is_regular_file(target) << std::endl;

    // 如果是普通文件，打印大小
    if (fs::is_regular_file(target)) {
        std::cout << "Size: " << fs::file_size(target) << " bytes" << std::endl;
    }

    return 0;
}