#pragma once
#include <string>

#include <Windows.h>

namespace WinWrap {
    class Module {
        HMODULE m_mod;
    public:
        Module() : m_mod(GetModuleHandleW(NULL)) {}

        Module(HMODULE mod) : m_mod(mod) {}

        Module(std::string_view module_name) : m_mod(GetModuleHandleA(module_name.data())) {}

        Module(std::wstring_view module_name) : m_mod(GetModuleHandleW(module_name.data())) {}

        std::string getFileNameA() const {
            std::string ret(260, '\0');
            auto size = GetModuleFileNameA(m_mod, ret.data(), ret.size() + 1);
            if (size == 0) return {};
            while (size >= ret.size() + 1) {
                if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) return {};
                ret.resize(ret.size() * 2);
                size = GetModuleFileNameA(m_mod, ret.data(), ret.size() + 1);
                if (size == 0) return {};
            }
            ret.resize(size);
            return ret;
        }
        std::wstring getFileNameW() const {
            std::wstring ret(260, '\0');
            auto size = GetModuleFileNameW(m_mod, ret.data(), ret.size() + 1);
            if (size == 0) return {};
            while (size >= ret.size() + 1) {
                if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) return {};
                ret.resize(ret.size() * 2);
                size = GetModuleFileNameW(m_mod, ret.data(), ret.size() + 1);
                if (size == 0) return {};
            }
            ret.resize(size);
            return ret;
        }

        HMODULE getHandle() const { return m_mod; }

        FARPROC getProcAddress(std::string_view proc_name) const {
            return GetProcAddress(m_mod, proc_name.data());
        }
    };
} // namespace WinWrap
