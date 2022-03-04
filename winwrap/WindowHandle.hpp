#include <string>
#include <type_traits>
#include <Windows.h>

namespace WinWrap {
    class WindowHandle {
        HWND m_hWnd;
    public:
        WindowHandle(HWND hWnd) : m_hWnd(hWnd) {}

        HWND getHWND() const { return m_hWnd; }

        std::string getWindowTextA() const {
            std::string buf;
            buf.resize(128);
            int size;
            while(!(size = GetWindowTextA(m_hWnd, buf.data(), buf.size()))) {
                buf.resize(buf.size() * 2);
            };
            buf.resize(size);
            return buf;
        }
        std::wstring getWindowTextW() const {
            std::wstring buf;
            buf.resize(128);
            int size;
            while(!(size = GetWindowTextW(m_hWnd, buf.data(), buf.size()))) {
                buf.resize(buf.size() * 2);
            };
            buf.resize(size);
            return buf;
        }

        bool setWindowTextA(std::string_view str) const {
            return SetWindowTextA(m_hWnd, str.data());
        }
        bool setWindowTextW(std::wstring_view str) const {
            return SetWindowTextW(m_hWnd, str.data());
        }

        enum class GetWindowLongMode {
            WndProc = GWL_WNDPROC,
            HInstance = GWL_HINSTANCE,
            HWndParent = GWL_HWNDPARENT,
            Style = GWL_STYLE,
            ExStyle = GWL_EXSTYLE,
            Userdata = GWL_USERDATA,
            Id = GWL_ID
        };

        template<GetWindowLongMode nIndex>
        struct GetWindowLongRet {
            using type = std::enable_if_t<false>;
        };
        
        template<>
        struct GetWindowLongRet<GetWindowLongMode::WndProc> {
            using type = WNDPROC;
        };

        template<GetWindowLongMode nIndex>
        using GetWindowLongRet_t = GetWindowLongRet<nIndex>::type;

        template<GetWindowLongMode nIndex, class R = GetWindowLongRet_t<nIndex>> R getWindowLong() const {
            static_assert(!std::is_same_v<int, int>, "invalid nIndex");
        }

        WNDPROC getWindowLongWndProc() const {
            return reinterpret_cast<WNDPROC>(GetWindowLongA(m_hWnd, GWL_WNDPROC));
        }
        HINSTANCE getWindowLongHInst() const {
            return reinterpret_cast<HINSTANCE>(GetWindowLongA(m_hWnd, GWL_HINSTANCE));
        }

    };
} // namespace WinWrap
