#include <string>

#include <Windows.h>

namespace WinWrap {
    class Module {
        HMODULE m_mod;
    public:
        Module() : m_mod(NULL) {}

        Module(std::string_view module_name) : m_mod(GetModuleHandleA(module_name.data())) {}

        Module(std::wstring_view module_name) : m_mod(GetModuleHandleW(module_name.data())) {}

        std::string GetFileNameA() const {
            DWORD size;
            std::string buf;
            buf.resize(MAX_PATH);
            while (!(size = GetModuleFileNameA(m_mod, buf.data(), buf.size()))) {
                buf.resize(buf.size() * 2);
            }
            buf.resize(size);
            return buf;
        }
        std::wstring GetFileNameW() const {
            DWORD size;
            std::wstring buf;
            buf.resize(MAX_PATH);
            while (!(size = GetModuleFileNameW(m_mod, buf.data(), buf.size()))) {
                buf.resize(buf.size() * 2);
            }
            buf.resize(size);
            return buf;
        }

        HMODULE GetHandle() const { return m_mod; }
    };
} // namespace WinWrap
