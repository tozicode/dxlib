#pragma once

#include <string>
#include <vector>
#include <list>
#include <functional>
#include <cassert>
#include <cmath>
#include <cstdarg>
#include <algorithm>
#include <initializer_list>

#include <DxLib.h>


namespace dxlib
{

using age_t = size_t;
using time_t = int;
using index_t = int;

using tchar = TCHAR;

#ifdef UNICODE
using tstring = std::wstring;
typedef std::wifstream tifstream;
typedef std::wofstream tofstream;
#else
using tstring = std::string;
typedef std::ifstream tifstream;
typedef std::ofstream tofstream;
#endif


/** tstring のラッパークラス。 */
class string_t : public tstring
{
public:
    string_t() = default;
    string_t(const string_t&) = default;
    string_t(const tstring &s) : tstring(s) {}
    string_t(const tchar *s) : tstring(s) {}

    /**
     * 区切り文字 sep によって文字列を分割して返す。
     * @param MAX_NUM 分割数の最大値
     */
    std::vector<string_t> split(const tchar *sep, const int MAX_NUM = -1) const;

    /** 文字列内にある全ての from を to に置き換えた結果を返す。 */
    string_t replace(const tstring &from, const tstring &to) const;
};


/** ファイルパスを扱うためのクラス。 */
class filepath_t : public string_t
{
public:
    static filepath_t module_file_path();
    static filepath_t module_file_dir();

    filepath_t() = default;
    filepath_t(const filepath_t&) = default;
    filepath_t(const tstring &s) : string_t(s) {}
    filepath_t(const tchar *s) : string_t(s) {}

    /**
     * パスのディレクトリ部分を切り出して返す。
     * 返り値の末尾の "\\" は常に削除される。
     */
    filepath_t dir() const;

    /** ディレクトリ d から見た相対パスに変換した結果を返す。 */
    filepath_t relative_path_from(const filepath_t &d) const;

    /** ディレクトリ d から見た相対パスと仮定して絶対パスに変換した結果を返す。 */
    filepath_t absolute_path_with(const filepath_t &d) const;    
};


/** 二次元情報を扱うための構造体. */
template <typename T>
struct xy_t
{
    inline xy_t() = default;
    inline xy_t(const xy_t<T>&) = default;
    inline xy_t(const T &_x, const T &_y) : x(_x), y(_y) {}

    inline bool operator ==(const xy_t<T> &n) const { return __cmp(n) == 0; }
    inline bool operator !=(const xy_t<T> &n) const { return __cmp(n) != 0; }
    inline bool operator <(const xy_t<T> &n) const { return __cmp(n) == 1; }
    inline bool operator >(const xy_t<T> &n) const { return __cmp(n) == -1; }

    inline xy_t<T>& operator += (const xy_t<T> &n) { x += n.x; y += n.y; return (*this); }
    inline xy_t<T>& operator -= (const xy_t<T> &n) { x -= n.x; y -= n.y; return (*this); }

    inline xy_t<T> operator + () { return xy_t<T>(+x, +y); }
    inline xy_t<T> operator - () { return xy_t<T>(-x, -y); }

    inline int __cmp(const xy_t<T> &n) const
    {
        if (x != n.x) return (x < n.x) ? 1 : -1;
        if (y != n.y) return (y < n.y) ? 1 : -1;
        return 0;
    }

    bool is_in_box(xy_t<int> position, xy_t<int> width) const
    {
        if (x < position.x) return false;
        if (y < position.y) return false;
        if (x >= position.x + width.x) return false;
        if (y >= position.y + width.y) return false;
        return true;
    }

    T x, y;
};

template <class T> xy_t<T> operator+(const xy_t<T> &n1, const xy_t<T> &n2)
{
    return xy_t<T>(n1.x + n2.x, n1.y + n2.y);
}

template <class T> xy_t<T> operator-(const xy_t<T> &n1, const xy_t<T> &n2)
{
    return xy_t<T>(n1.x - n2.x, n1.y - n2.y);
}

using position_t = xy_t<int>;
using width_t = xy_t<int>;


/** 値域を持つ変数を表すためのクラス。 */
template <typename T>
class ranged_value_t
{
public:
    ranged_value_t(T min, T max, T init)
        : m_min(min), m_max(max), m_val(init) {}
    ranged_value_t(const ranged_value_t<T> &x)
        : m_min(x.min()), m_max(x.max()), m_val(x.get()) {}

    inline operator T() const { return get(); }

    inline ranged_value_t& operator = (T x) { set(x); return (*this); }
    inline ranged_value_t& operator += (T x) { add(+x); return (*this); }
    inline ranged_value_t& operator -= (T x) { add(-x); return (*this); }

    inline bool operator == (const ranged_value_t<T> &x) const { return get() == x.get(); }

    inline void set(T v) { m_val = v; normalize(); }
    inline void add(T d) { m_val += d; normalize(); }

    inline T min() const { return m_min; }
    inline T max() const { return m_max; }
    inline T get() const { return m_val; }

private:
    inline void normalize() { m_val = std::max<T>(std::min<T>(m_val, m_max), m_min); }

    const T m_min;
    const T m_max;
    T m_val;
};


/** 透明度付きの色情報を扱うための構造体. */
struct color_t
{
    color_t() : r(0), g(0), b(0), a(0) {}
    color_t(int _r, int _g, int _b, int _a = 255)
        : r(_r), g(_g), b(_b), a(_a) {}

    operator int() const { return GetColor(r, g, b); }

    bool operator==(const color_t &c) const { return __cmp(c) == 0; }
    bool operator!=(const color_t &c) const { return __cmp(c) != 0; }
    bool operator<(const color_t &c) const { return __cmp(c) == 1; }
    bool operator>(const color_t &c) const { return __cmp(c) == -1; }

    int __cmp(const color_t &c) const
    {
        if (r != c.r) return (r < c.r) ? 1 : -1;
        if (g != c.g) return (g < c.g) ? 1 : -1;
        if (b != c.b) return (b < c.b) ? 1 : -1;
        if (a != c.a) return (a < c.a) ? 1 : -1;
        return 0;
    }

    int r, g, b, a;
};


/** 指定した角度でのベクトル回転を繰り返し行うための関数クラス. */
class rotater_t
{
public:
    rotater_t(double radius)
        : m_radius(radius), m_sin(std::sin(radius)), m_cos(std::cos(radius)) {}

    position_t operator()(position_t p)
    {
        return position_t(
            (int)(((double)p.x * m_cos) - ((double)p.y * m_sin)),
            (int)(((double)p.x * m_sin) + ((double)p.y * m_cos)));
    }
    double radius() const { return m_radius; }

private:
    double m_radius;
    double m_sin, m_cos;
};


inline int stoi(const tstring& str)
{
    int i(-1);
    _stscanf_s(str.c_str(), _T("%d"), &i);
    return i;
}


inline double stod(const tstring& str)
{
    double d;
    _stscanf_s(str.c_str(), _T("%lf"), &d);
    return d;
}


inline tstring format(const tchar* fmt, ...)
{
    static tchar buf[4096];

    va_list list;
    va_start(list, fmt);
    _vstprintf_s(buf, fmt, list);
    va_end(list);

    return tstring(buf);
}


inline std::string string(const tstring &str)
{
#ifdef UNICODE
    size_t size;
    char buf[4096];
    wcstombs_s(&size, buf, 4096, str.c_str(), _TRUNCATE);
    return std::string(buf);
#else
    return str;
#endif
}


inline std::wstring wstring(const tstring &str)
{
#ifdef UNICODE
    return str;
#else
    size_t size;
    wchar_t buf[4096];
    mbstowcs_s(&size, buf, str.c_str(), 4095);
    return std::wstring(buf);
#endif
}


/** 現在のウィンドウサイズを取得する。 */
inline width_t get_window_size()
{
    width_t w;
    GetWindowSize(&w.x, &w.y);
    return w;
}


/** ドラッグ&ドロップされたファイルの絶対パスのリストを返す。 */
inline std::list<filepath_t> get_dragged_files()
{
    std::list<filepath_t> out;
    tchar path[4096];
    size_t num = GetDragFileNum();

    for (size_t i = 0; i < num; ++i)
    {
        GetDragFilePath(path);
        out.push_back(filepath_t(path));
    }

    return out;
}

}


namespace std
{

template <> struct hash<dxlib::filepath_t> : public hash<dxlib::tstring>
{
    size_t operator()(const dxlib::filepath_t &x) const
    {
        return hash<dxlib::tstring>::operator()(x);
    }
};

}