#pragma once
#include <string>
#include <filesystem>

#include <Windows.h>

namespace WinWrap {
    class Module {
        HMODULE m_mod;
    public:
        Module() : m_mod(NULL) {}

        Module(HMODULE mod) : m_mod(mod) {}

        Module(const std::string& module_name) : m_mod(GetModuleHandleA(module_name.c_str())) {}

        Module(const std::wstring& module_name) : m_mod(GetModuleHandleW(module_name.c_str())) {}
        
        Module(const char* module_name) : m_mod(GetModuleHandleA(module_name)) {}

        Module(const wchar_t* module_name) : m_mod(GetModuleHandleW(module_name)) {}

        static Module getCallingProcessModule() {
            return Module(GetModuleHandleW(NULL));
        }

        constexpr bool is_valid() const {
            return m_mod != NULL;
        }

        [[nodiscard]]
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

        [[nodiscard]]
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

        [[nodiscard]]
        std::filesystem::path getFileName() const {
            return std::filesystem::path(getFileNameW());
        }

        [[nodiscard]]
        HMODULE getHandle() const { return m_mod; }

        [[nodiscard]]
        FARPROC getProcAddress(const std::string& proc_name) const {
            return GetProcAddress(m_mod, proc_name.c_str());
        }

        [[nodiscard]]
        FARPROC getProcAddress(const char* proc_name) const {
            return GetProcAddress(m_mod, proc_name);
        }
    };
} // namespace WinWrap
