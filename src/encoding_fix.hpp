#pragma once
#include <windows.h>
#include <filesystem>
#include <string>

// RAII: program startup sets console to UTF-8 before main()
namespace {
struct _ConsoleUtf8 {
    _ConsoleUtf8() {
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    }
};
static _ConsoleUtf8 _console_utf8_init;
} // namespace

// path::string() on Windows returns the system locale encoding (GBK on
// Chinese Windows), but we output UTF-8. Convert from the wide (UTF-16)
// representation so filenames survive the trip to cout intact.
inline std::string to_utf8(const std::filesystem::path& p) {
    auto w = p.wstring();
    if (w.empty()) return {};
    int len = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(),
                                  nullptr, 0, nullptr, nullptr);
    std::string result(len, '\0');
    WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(),
                        &result[0], len, nullptr, nullptr);
    return result;
}

// Narrow → path: on Windows the narrow bytes may be GBK or UTF-8
// depending on terminal code page. Try ACP first, fall back to UTF-8.
inline std::filesystem::path path_from_narrow(const std::string& s) {
#ifdef _WIN32
    if (s.empty()) return {};
    // Try active code page first (GBK on Chinese Win)
    int wlen = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS,
                                   s.c_str(), -1, nullptr, 0);
    if (wlen > 0) {
        std::wstring wstr(wlen - 1, L'\0');
        MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, &wstr[0], wlen);
        return std::filesystem::path(wstr);
    }
    // ACP failed — probably UTF-8 bytes (e.g. VS Code / Windows Terminal)
    wlen = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
    if (wlen > 0) {
        std::wstring wstr(wlen - 1, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &wstr[0], wlen);
        return std::filesystem::path(wstr);
    }
#endif
    return std::filesystem::path(s);
}
