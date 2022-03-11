#pragma once
#include <stdexcept>
#include <sstream>

#include <Windows.h>

namespace WinWrap {
    class WinException : public std::runtime_error {
    public:
        WinException(DWORD errorCode) : std::runtime_error("winexception"), m_errorCode(errorCode) {}
        WinException(const char* message, DWORD errorCode) : std::runtime_error(message), m_errorCode(errorCode) {}

        DWORD code() const {
            return m_errorCode;
        }
        std::string codeStringA() const {
            DWORD size;
            std::string buf;
            buf.resize(256);
            while (!(size = FormatMessageA(
                FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, m_errorCode, 0, buf.data(), buf.size(), nullptr
            ))) {
                buf.resize(buf.size() * 2);
            }
            buf.resize(size);
            return buf;
        }
        std::wstring codeStringW() const {
            DWORD size;
            std::wstring buf;
            buf.resize(256);
            while (!(size = FormatMessageW(
                FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, m_errorCode, 0, buf.data(), buf.size(), nullptr
            ))) {
                buf.resize(buf.size() * 2);
            }
            buf.resize(size);
            return buf;
        }
    private:
        DWORD m_errorCode;
    };
}