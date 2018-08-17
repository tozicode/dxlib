#pragma once

#include <memory>
#include <deque>

#include "./util.h"


namespace dxlib
{


class gui_t;

using gui_ptr_t = std::shared_ptr<gui_t>;
using priority_t = int;

/** ��ʂ��\������e��I�u�W�F�N�g�̊��N���X. */
class gui_t
{
public:
    gui_t(position_t p, width_t w, priority_t = 100);

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

    /** �������g����ёS�Ă̎q�I�u�W�F�N�g���X�V����B */
    void update_recursively();

    /** �������g����ёS�Ă̎q�I�u�W�F�N�g��`�悷��B */
    void draw_recursively() const;

    inline const position_t& position() const { return m_position; }
    inline const width_t& width() const { return m_width; }
    inline age_t age() const { return m_age; }
    inline priority_t priority() const { return m_priority; }

    /**
     * gui_t �I�u�W�F�N�g���q�Ƃ��Ēǉ�����B
     * �I�u�W�F�N�g�� priority() ���Ⴂ���ɕ��Ԃ悤�ɒǉ������B
     * �����l�̏ꍇ�͌�ɒǉ����ꂽ�������ɗ���B
     */
    void add_child(gui_ptr_t);

    /** �}�E�X�|�C���^����ɂ���� true ��Ԃ��B */
    bool is_pointed() const;

protected:
    std::deque<gui_ptr_t> m_children;

    position_t m_position;
    width_t m_width;
    age_t m_age;
    priority_t m_priority;

    bool m_is_pointed;
    bool m_is_dragged_left;
    bool m_is_dragged_right;
    bool m_is_dragged_middle;
};


}