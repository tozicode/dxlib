#include "./gui.h"
#include "./mouse_input.h"


namespace dxlib
{

gui_t::gui_t(position_t p, width_t w, priority_t pr)
    : m_position(p), m_width(w), m_age(0), m_priority(pr),
    m_is_pointed(false),
    m_is_pointed_directly(false),
    m_is_dragged_left(false),
    m_is_dragged_right(false),
    m_is_dragged_middle(false)
{}


void gui_t::update()
{
    ++m_age;
    
    bool pointed = mouse_input().position().is_in_box(m_position, m_width);

    if (pointed)
    {
        m_is_pointed_directly = true;
        for (const auto &child : m_children)
            if (child->is_pointed())
                m_is_pointed_directly = false; // 子の上にポインタが乗っている
        
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

        // ホイールを回した時
        if (mouse_input().wheel() != 0)
            on_turn_wheel();
    }
    else
    {
        if (m_is_pointed)
            on_point_end();
        m_is_pointed = m_is_pointed_directly = false;
    }

    // ドラッグを終えた時
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


void gui_t::draw_recursively(const position_t &p) const
{
    const position_t &_p = (p == NPOS) ? position() : p;

    if (m_image)
        m_image->draw(_p); // 生成済みの描画結果を使用
    else
        draw(_p); // オンラインで描画
    
    xy_t<int> d = _p - position();
    for (const auto &child : m_children)
        child->draw_recursively(child->position() + d);
}


void gui_t::add_child(gui_ptr_t p)
{
    for (auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        if ((*it)->priority() > p->priority())
        {
            m_children.insert(it, std::move(p));
            return;
        }
    }

    m_children.push_back(std::move(p));
    m_image.reset();
}


void gui_t::remove_child(gui_ptr_t p)
{
    auto it = std::find(m_children.begin(), m_children.end(), p);
    if (it != m_children.end())
    {
        m_children.erase(it);
        m_image.reset();
    }
}


void gui_t::make_image()
{
    m_image.reset();
    
    image_handle_t h = MakeScreen(m_width.x, m_width.y);
    {
        SetDrawScreen(h);
        draw_recursively({0, 0});
        SetDrawScreen(DX_SCREEN_BACK);
    }

    m_image.reset(new image_t(h));
}


}
