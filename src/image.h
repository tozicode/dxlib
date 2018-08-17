#pragma once

#include "DxLib.h"
#include "util.h"


namespace dxlib
{

using image_handle_t = int;


class image_t
{
public:
    inline image_t();
    inline image_t(image_handle_t h);
    inline image_t(const tstring &path);
    inline ~image_t();

    inline operator image_handle_t() const;

    inline width_t width() const;

    inline void draw_rotated(
        position_t p, double scale, double radius,
        bool reverse_x = false, bool reverse_y = false) const;
    inline void draw_rotated_3(
        position_t p, xy_t<int> center, xy_t<double> scale, double radius,
        bool reverse_x = false, bool reverse_y = false) const;

    inline image_handle_t release();
    inline image_handle_t handle() const { return m_handle; }

private:
    image_handle_t m_handle;
};


// ---- inline methods


inline image_t::image_t()
    : m_handle(-1)
{}

inline image_t::image_t(image_handle_t h)
    : m_handle(h)
{}

inline image_t::image_t(const tstring &path)
    : m_handle(LoadGraph(path.c_str()))
{}

inline image_t::~image_t()
{
    DeleteGraph(m_handle);
}

inline image_t::operator image_handle_t() const
{
    return handle();
}

inline width_t image_t::width() const
{
    width_t w;
    GetGraphSize(m_handle, &w.x, &w.y);
    return w;
}

inline void image_t::draw_rotated(
    position_t p, double scale, double radius,
    bool reverse_x, bool reverse_y) const
{
    DrawRotaGraph(
        p.x, p.y, scale, radius, m_handle, TRUE,
        reverse_x ? TRUE : FALSE, reverse_y ? TRUE : FALSE);
}

inline void image_t::draw_rotated_3(
    position_t p, xy_t<int> c, xy_t<double> scale, double radius,
    bool reverse_x, bool reverse_y) const
{
    auto w = width();
    DrawRotaGraph3(
        p.x, p.y, w.x / 2 + c.x, w.y / 2 + c.y,
        scale.x, scale.y, radius, m_handle, true,
        reverse_x ? TRUE : FALSE, reverse_y ? TRUE : FALSE);
}

inline image_handle_t image_t::release()
{
    auto h = handle();
    m_handle = -1;
    return h;
}

}