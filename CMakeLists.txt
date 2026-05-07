cmake_minimum_required(VERSION 3.16)
project(fstree VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(fstree
    src/main.cpp
    src/scanner.cpp    # 新增这行
)

# Windows: 文件系统和控制台默认用 GBK（中文系统），项目源码是 UTF-8
# encoding_fix.hpp 在启动时做两件事：
#   1. 把控制台设为 UTF-8（SetConsoleOutputCP）
#   2. 提供 to_utf8() / path_from_narrow() 处理路径和文件名的编码转换
if(MSVC)
    # MSVC 需要 /utf-8 才知道源码是 UTF-8
    target_compile_options(fstree PRIVATE /utf-8)
    # 嵌入 manifest：把进程活动代码页设为 UTF-8，这样 fs::path 的
    # 窄字符串构造和 .string() 都走 UTF-8，不再依赖 encoding_fix.hpp 的 fallback
    enable_language(RC)
    target_link_options(fstree PRIVATE /MANIFEST:NO)
    target_sources(fstree PRIVATE src/fstree.rc)
endif()