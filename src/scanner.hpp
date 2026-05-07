#pragma once   // 防止头文件被重复包含，必须写，记住即可

#include <filesystem>
#include <cstdint>   // 提供 uint64_t 类型

namespace fs = std::filesystem;

// 代表一个目录节点的数据结构
struct DirEntry {
    fs::path path;          // 这个条目的路径
    uint64_t size = 0;      // 总大小（字节）
    bool is_dir = false;    // 是否是目录
};

// 计算一个路径（文件或目录）的总大小
// 如果是目录，会递归统计所有子项
uint64_t calc_size(const fs::path& path);

// 新增：打印树形结构
// prefix 是当前行前面的「缩进前缀」，递归时传递下去
// is_last 表示当前项是否是父目录的最后一项
void print_tree(const fs::path& path,
                const std::string& prefix,
                bool is_last);