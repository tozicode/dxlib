#include "./gui.h"
#include "./mouse_input.h"


namespace dxlib
{

gui_t::gui_t(position_t p, width_t w)
    : m_position(p), m_width(w), m_age(0),
    m_is_pointed(false),
    m_is_dragged_left(false),
    m_is_dragged_right(false),
    m_is_dragged_middle(false)
{}


void gui_t::update()
{
    ++m_age;

    bool pointed = is_pointed();

    if (pointed)
    {
        for (const auto &child : m_children)
            if (child->is_pointed())
                pointed = false; // 子の上にポインタが乗っている
    }

    if (pointed)
    {
        if (not m_is_pointed)
            on_point_start();
        m_is_pointed = true;

        // クリック時
        if (mouse_input().left().is_clicked())
            on_click_left();
        if (mouse_input().right().is_clicked())
            on_click_right();
        if (mouse_input().middle().is_clicked())
            on_click_middle();

        // ダブルクリック時
        if (mouse_input().left().is_double_clicked())
            on_double_click_left();
        if (mouse_input().right().is_double_clicked())
            on_double_click_right();
        if (mouse_input().middle().is_double_clicked())
            on_double_click_middle();

        // ドラッグ開始時
        if (mouse_input().left().is_dragged() and not m_is_dragged_left)
        {
            on_drag_left_start();
            m_is_dragged_left = true;
        }
        if (mouse_input().right().is_dragged() and not m_is_dragged_right)
        {
            on_drag_right_start();
            m_is_dragged_right = true;
        }
        if (mouse_input().middle().is_dragged() and not m_is_dragged_middle)
        {
            on_drag_middle_start();
            m_is_dragged_middle = true;
        }
    }
    else
    {
        if (m_is_pointed)
            on_point_end();
        m_is_pointed = false;
    }

    if (not mouse_input().left().is_dragged() and m_is_dragged_left)
    {
        on_drag_left_end();
        m_is_dragged_left = false;
    }
    if (not mouse_input().right().is_dragged() and m_is_dragged_right)
    {
        on_drag_right_end();
        m_is_dragged_right = false;
    }
    if (not mouse_input().middle().is_dragged() and m_is_dragged_middle)
    {
        on_drag_middle_end();
        m_is_dragged_middle = false;
    }
}


void gui_t::update_recursively()
{
    update();
    for (auto &child : m_children)
        child->update_recursively();
}


void gui_t::draw_recursively() const
{
    draw();
    for (const auto &child : m_children)
        child->draw_recursively();
}


void gui_t::add_child(gui_ptr_t p)
{
    m_children.push_back(std::move(p));
}


bool gui_t::is_pointed() const
{
    return mouse_input().position().is_in_box(m_position, m_width);
}

}