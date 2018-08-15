#pragma once

#include <string>
#include <cassert>
#include <cmath>
#include <initializer_list>

#include <DxLib.h>


namespace dxlib
{

using age_t = size_t;
using time_t = int;

using tchar = TCHAR;

#ifdef UNICODE
using tstring = std::wstring;
#else
using tstring = std::string;
#endif

using filepath_t = tstring;


/** 二次元情報を扱うための構造体. */
template <typename T>
struct xy_t
{
    inline xy_t() {}
    inline xy_t(const T &_x, const T &_y) : x(_x), y(_y) {}

    inline bool operator ==(const xy_t<T> &n) const { return __cmp(n) == 0; }
    inline bool operator !=(const xy_t<T> &n) const { return __cmp(n) != 0; }
    inline bool operator <(const xy_t<T> &n) const { return __cmp(n) == 1; }
    inline bool operator >(const xy_t<T> &n) const { return __cmp(n) == -1; }

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

using position_t = xy_t<int>;
using width_t = xy_t<int>;


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


/** 現在のウィンドウサイズを取得する。 */
inline width_t get_window_size()
{
    width_t w;
    GetWindowSize(&w.x, &w.y);
    return w;
}

}