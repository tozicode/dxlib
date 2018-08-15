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

    operator image_handle_t() const;

    void draw_rotated(
        position_t p, double scale, double radius,
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

image_t::operator image_handle_t() const
{
    return handle();
}

void image_t::draw_rotated(
    position_t p, double scale, double radius,
    bool reverse_x = false, bool reverse_y = false) const
{
    DrawRotaGraph(
        p.x, p.y, scale, radius, m_handle, TRUE,
        reverse_x ? TRUE : FALSE, reverse_y ? TRUE : FALSE);
}

inline image_handle_t image_t::release()
{
    auto h = handle();
    m_handle = -1;
    return h;
}

inline image_handle_t image_t::handle() const
{
    return m_handle;
}


}