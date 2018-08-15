#include <algorithm>

#include "./softimage.h"


namespace dxlib
{


softimage_t::softimage_t(const filepath_t &p)
    : m_handle(LoadSoftImage(p.c_str()))
{}


softimage_t::softimage_t(softimage_handle_t h)
    : m_handle(h)
{}


softimage_t::~softimage_t()
{
    DeleteSoftImage(m_handle);
}


void softimage_t::draw(position_t p) const
{
    DrawSoftImage(p.x, p.y, m_handle);
}


width_t softimage_t::width() const
{
    width_t w;
    GetSoftImageSize(m_handle, &w.x, &w.y);
    return w;
}


int softimage_t::get_palette_code_at(position_t p) const
{
    return GetPixelPalCodeSoftImage(m_handle, p.x, p.y);
}


color_t softimage_t::get_color_at(position_t p) const
{
    color_t c;
    GetPixelSoftImage(m_handle, p.x, p.y, &c.r, &c.g, &c.b, &c.a);
    if (is_256color())
        c.a = 255;
    return c;
}


bool softimage_t::is_256color() const
{
    if (not empty())
        return (GetPixelPalCodeSoftImage(m_handle, 0, 0) >= 0);
    return false;
}


image_handle_t softimage_t::create_graph() const
{
    return CreateGraphFromSoftImage(m_handle);
}


softimage_handle_t softimage_t::scale2x() const
{
    width_t w = width();
    bool _is_256color = is_256color();
    softimage_handle_t si = _is_256color ?
        MakePAL8ColorSoftImage(w.x * 2, w.y * 2) :
        MakeARGB8ColorSoftImage(w.x * 2, w.y * 2);

    // パレット情報をコピー
    if (_is_256color)
    {
        for (int i = 0; i < 256; ++i)
        {
            int r, g, b, a;
            GetPaletteSoftImage(m_handle, i, &r, &g, &b, &a);
            SetPaletteSoftImage(si, i, r, g, b, a);
        }
    }

    /* Reference: http://www.scale2x.it/algorithm.html */

    auto is_equal = [&](position_t p1, position_t p2)
    {
        assert(p1.is_in_box({ 0, 0 }, w));
        assert(p2.is_in_box({ 0, 0 }, w));

        if (_is_256color)
            return (get_palette_code_at(p1) == get_palette_code_at(p2));
        else
            return (get_color_at(p1) == get_color_at(p2));
    };

    auto draw_pixel = [&](position_t p1, position_t p2)
    {
        if (_is_256color)
        {
            int c = get_palette_code_at(p1);
            DrawPixelPalCodeSoftImage(si, p2.x, p2.y, c);
        }
        else
        {
            auto c = get_color_at(p1);
            DrawPixelSoftImage(si, p2.x, p2.y, c.r, c.g, c.b, c.a);
        }
    };

    for (int x = 0; x < w.x; ++x)
    {
        for (int y = 0; y < w.y; ++y)
        {
            position_t p0(x, y), p1(x, y), p2(x, y), p3(x, y);
            bool eq_bd(false), eq_bf(false), eq_dh(false), eq_fh(false);

            if (x > 0 and y > 0)
                eq_bd = is_equal({ x - 1, y }, { x, y - 1 });
            if (x < w.x - 1 and y > 0)
                eq_bf = is_equal({ x, y - 1 }, { x + 1, y });
            if (x > 0 and y < w.y - 1)
                eq_dh = is_equal({ x - 1, y }, { x, y + 1 });
            if (x < w.x - 1 and y < w.y - 1)
                eq_fh = is_equal({ x, y + 1 }, { x + 1, y });

            if (eq_bd and !eq_bf and !eq_dh) --p0.x;
            if (eq_bf and !eq_bd and !eq_fh) ++p1.x;
            if (eq_dh and !eq_bd and !eq_fh) --p2.x;
            if (eq_fh and !eq_dh and !eq_bf) ++p3.x;

            draw_pixel(p0, { 2 * x,     2 * y });
            draw_pixel(p1, { 2 * x + 1, 2 * y });
            draw_pixel(p2, { 2 * x,     2 * y + 1 });
            draw_pixel(p3, { 2 * x + 1, 2 * y + 1 });
        }
    }

    return si;
}


std::pair<softimage_handle_t, softimage_handle_t>
softimage_t::split(int bound, int bg) const
{
    assert(is_256color());

    width_t w = width();
    std::pair<softimage_handle_t, softimage_handle_t> si(
        MakeARGB8ColorSoftImage(w.x, w.y),
        MakeARGB8ColorSoftImage(w.x, w.y));

    for (int x = 0; x < w.x; ++x)
    {
        for (int y = 0; y < w.y; ++y)
        {
            int i = get_palette_code_at({ x, y });
            color_t c = get_color_at({ x, y });

            if (i == bg)
            {
                DrawPixelSoftImage(si.first, x, y, 0, 0, 0, 0);
                DrawPixelSoftImage(si.second, x, y, 0, 0, 0, 0);
            }
            else if (i > bound)
            {
                DrawPixelSoftImage(si.first, x, y, 0, 0, 0, 0);
                DrawPixelSoftImage(si.second, x, y, c.r, c.g, c.b, 255);
            }
            else
            {
                DrawPixelSoftImage(si.first, x, y, c.r, c.g, c.b, 255);
                DrawPixelSoftImage(si.second, x, y, 0, 0, 0, 0);
            }
        }
    }

    return si;
}


}