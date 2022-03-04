#pragma once
#include <type_traits>

namespace WinWrap {
    namespace detail::flag {
        template<class T1, class T2=T1>struct ops_def:std::false_type{};

        template<class T, bool is_enum> struct safe_underlying_type_impl { using type = T; };
        template<class T> struct safe_underlying_type_impl<T, true> { using type = std::underlying_type_t<T>; };
        template<class T> using safe_underlying_type_t = typename safe_underlying_type_impl<T, std::is_enum_v<T>>::type;

        template<class T1, class T2>
        constexpr inline bool is_enable = (ops_def<T1, T2>::value || ops_def<T2, T1>::value) && std::is_same_v<safe_underlying_type_t<T1>, safe_underlying_type_t<T2>>;

        template<class T1, class T2, class T3>
        constexpr inline bool is_enable_tri = is_enable<T1, T2> && is_enable<T2, T3> && is_enable<T3, T1>;

        template<class T1, class T2>
        using result_t = std::enable_if_t<is_enable<T1, T2>, std::conditional_t<std::is_enum_v<T1>, T1, T2>>;

        template<class T1, class T2>
        using result_lhs_t = std::enable_if_t<is_enable<T1, T2>, T1>;

        template<class T1, class T2>
        using result_bool_t = std::enable_if_t<is_enable<T1, T2>, bool>;

        template<class T> constexpr safe_underlying_type_t<T> to_underlying(T flags) { return static_cast<safe_underlying_type_t<T>>(flags); }

        template<class T> constexpr result_t<T, T> operator~(T x) { return static_cast<T>(~to_underlying(x)); }
        template<class T1, class T2> constexpr result_t<T1, T2> operator&(T1 x, T2 y) { return static_cast<result_t<T1, T2>>(to_underlying(x) & to_underlying(y)); }
        template<class T1, class T2> constexpr result_t<T1, T2> operator|(T1 x, T2 y) { return static_cast<result_t<T1, T2>>(to_underlying(x) | to_underlying(y)); }
        template<class T1, class T2> constexpr result_t<T1, T2> operator^(T1 x, T2 y) { return static_cast<result_t<T1, T2>>(to_underlying(x) ^ to_underlying(y)); }
        template<class T1, class T2> constexpr result_lhs_t<T1, T2>& operator&=(T1& x, T2 y) { return x = static_cast<T1>(to_underlying(x) & to_underlying(y)); }
        template<class T1, class T2> constexpr result_lhs_t<T1, T2>& operator|=(T1& x, T2 y) { return x = static_cast<T1>(to_underlying(x) | to_underlying(y)); }
        template<class T1, class T2> constexpr result_lhs_t<T1, T2>& operator^=(T1& x, T2 y) { return x = static_cast<T1>(to_underlying(x) ^ to_underlying(y)); }
        template<class T1, class T2> constexpr result_bool_t<T1, T2> includes(T1 x, T2 y) { return (to_underlying(x) & to_underlying(y)) != to_underlying(0); }
        template<class T1, class T2> constexpr result_bool_t<T1, T2> includes_all(T1 x, T2 y) { return (to_underlying(x) & to_underlying(y)) == to_underlying(y); }
        template<class T1, class T2, class T3> constexpr std::enable_if_t<is_enable_tri<T1, T2, T3>, bool> masked_eq(T1 x, T2 y, T3 mask) { return (to_underlying(x) ^ to_underlying(y)) & to_underlying(mask); }
    }
}
using WinWrap::detail::flag::operator~;
using WinWrap::detail::flag::operator&;
using WinWrap::detail::flag::operator|;
using WinWrap::detail::flag::operator^;
using WinWrap::detail::flag::operator&=;
using WinWrap::detail::flag::operator|=;
using WinWrap::detail::flag::operator^=;
using WinWrap::detail::flag::includes;
using WinWrap::detail::flag::includes_all;
