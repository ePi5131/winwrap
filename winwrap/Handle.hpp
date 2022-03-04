#pragma once

#include <Windows.h>

namespace WinWrap {
    class Handle;

    class Handle {
    public:
        HANDLE m_handle;

        constexpr Handle() : m_handle(NULL) {}

        constexpr Handle(HANDLE handle) : m_handle(handle) {}
        
    };
    inline constexpr Handle NullHandle{};
    inline constexpr Handle InvalidHandle{INVALID_HANDLE_VALUE};
} // namespace WinWrap
