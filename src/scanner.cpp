#include "scanner.hpp"
#include <iostream>

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