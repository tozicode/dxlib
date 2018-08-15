#pragma once

#include <memory>
#include <deque>

#include "./util.h"


namespace dxlib
{


class gui_t;

using gui_ptr_t = std::shared_ptr<gui_t>;


class gui_t
{
public:
    gui_t(position_t p, width_t w);

    virtual void update();
    virtual void draw() const {}

    virtual void on_point_start() {}
    virtual void on_point_end() {}

    virtual void on_click_left() {}
    virtual void on_click_right() {}
    virtual void on_click_middle() {}

    virtual void on_double_click_left() {}
    virtual void on_double_click_right() {}
    virtual void on_double_click_middle() {}

    virtual void on_drag_left_start() {}
    virtual void on_drag_left_end() {}
    virtual void on_drag_right_start() {}
    virtual void on_drag_right_end() {}
    virtual void on_drag_middle_start() {}
    virtual void on_drag_middle_end() {}

    void update_recursively();
    void draw_recursively() const;

    void add_child(gui_ptr_t);

    bool is_pointed() const;

protected:
    std::deque<gui_ptr_t> m_children;

    position_t m_position;
    width_t m_width;
    age_t m_age;

    bool m_is_pointed;
    bool m_is_dragged_left;
    bool m_is_dragged_right;
    bool m_is_dragged_middle;
};


}