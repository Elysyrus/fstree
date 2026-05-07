#include "scanner.hpp"
#include "encoding_fix.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

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

uint64_t calc_size(const fs::path& path) {
    // 错误处理：路径不存在直接返回 0
    if (!fs::exists(path)) {
        return 0;
    }

    // 如果是普通文件，直接返回文件大小
    if (fs::is_regular_file(path)) {
        return fs::file_size(path);
    }

    // 如果是目录，遍历所有子项，累加大小
    uint64_t total = 0;
    if (fs::is_directory(path)) {
        for (const auto& entry : fs::directory_iterator(path)) {
            // 用 try-catch 跳过没有权限读取的文件
            // 不加这个，遇到系统目录会直接崩溃
            try {
                // entry 是目录里的一项，.path() 取出它的路径
                total += calc_size(entry.path());   // 递归调用自己
            } catch (const fs::filesystem_error&) {
                // 权限不足，跳过这个文件
            }
        }
    }
    return total;
}

// 规律：
// 当我们往下递归一层时，子项的前缀 = 父项的前缀 + 一段新字符：

// 如果父项不是最后一项（用了 ├──）：新加 "│   "（要保留竖线，下面还有兄弟）
// 如果父项是最后一项（用了 └──）：新加 "    "（四个空格，下面没有兄弟了）
void print_tree(const fs::path& path,
                const std::string& prefix,
                bool is_last) {

    // 根据是否是最后一项，选择不同的符号
    std::string connector = is_last ? "└── " : "├── ";
    auto size = calc_size(path);
    auto name = to_utf8(path.filename());
    
    // 就是拼接输出：前缀 + 连接符 + 文件名 + 大小。比如：
    //     "│   " + "├── " + "main.cpp" + "  1.2 KB"
    // → │   ├── main.cpp  1.2 KB
    std::cout << prefix << connector
              << name << "  " << human_size(size) << std::endl;

    // 如果是目录，递归打印子项
    if (fs::is_directory(path)) {
        // 先收集所有子项（排序让输出稳定）
        std::vector<fs::path> children;
        for (const auto& entry : fs::directory_iterator(path)) {
            children.push_back(entry.path());
        }
        std::sort(children.begin(), children.end());

        // 计算下一层的前缀
        // 如果当前项是最后一项，下面不需要竖线；否则要加 │
        std::string child_prefix = prefix + (is_last ? "    " : "│   ");

        for (size_t i = 0; i < children.size(); ++i) {
            bool child_is_last = (i == children.size() - 1);
            try {
                print_tree(children[i], child_prefix, child_is_last);
            } catch (const fs::filesystem_error&) {}
        }
    }
}