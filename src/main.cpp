#include <iostream>
#include <string>
#include <vector>

// 打印使用说明
void print_usage() {
    std::cout << "fstree - file system tree analyzer" << std::endl;
    std::cout << "Usage: fstree <path> [options]" << std::endl;
    std::cout << "  <path>   Directory to analyze" << std::endl;
}

int main(int argc, char* argv[]) {
    // 把命令行参数转成 vector，更方便处理
    // argv 是 C 风格的数组，这里把它转成现代 C++ 的 vector
    auto args = std::vector<std::string>(argv + 1, argv + argc);

    if (args.empty()) {
        print_usage();
        return 1;
    }

    // 用 range-based for 遍历所有参数，先打印出来看看
    std::cout << "Arguments received:" << std::endl;
    for (const auto& arg : args) {
        std::cout << "  " << arg << std::endl;
    }

    auto target_path = args[0];
    std::cout << "\nTarget path: " << target_path << std::endl;

    return 0;
}