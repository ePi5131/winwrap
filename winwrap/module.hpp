#include <string>

#include <Windows.h>

namespace WinWrap {
    class Module {
        HMODULE m_mod;
    public:
        Module() : m_mod(NULL) {}

        Module(HMODULE mod) : m_mod(mod) {}

        Module(std::string_view module_name) : m_mod(GetModuleHandleA(module_name.data())) {}

        Module(std::wstring_view module_name) : m_mod(GetModuleHandleW(module_name.data())) {}

        std::string getFileNameA() const {
            DWORD size;
            std::string buf;
            buf.resize(MAX_PATH);
            while (!(size = GetModuleFileNameA(m_mod, buf.data(), buf.size()))) {
                buf.resize(buf.size() * 2);
            }
            buf.resize(size);
            return buf;
        }
        std::wstring getFileNameW() const {
            DWORD size;
            std::wstring buf;
            buf.resize(MAX_PATH);
            while (!(size = GetModuleFileNameW(m_mod, buf.data(), buf.size()))) {
                buf.resize(buf.size() * 2);
            }
            buf.resize(size);
            return buf;
        }

        HMODULE getHandle() const { return m_mod; }

        FARPROC getProcAddress(std::string_view proc_name) const {
            return GetProcAddress(m_mod, proc_name.data());
        }
    };
} // namespace WinWrap
