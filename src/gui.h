#pragma once

#include <memory>
#include <deque>

#include "./util.h"
#include "./image.h"


namespace dxlib
{


class gui_t;

using gui_ptr_t = std::shared_ptr<gui_t>;
using priority_t = int;

/** ��ʂ��\������e��I�u�W�F�N�g�̊��N���X. */
class gui_t
{
public:
    gui_t(position_t p, width_t w, priority_t pr = 100);

    /** ���t���[���s���X�V�����B */
    virtual void update();

    /** ���W `p` ������Ƃ���GUI��`�悷��B */
    virtual void draw(const position_t &p) const {}

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

    virtual void on_turn_wheel() {}

    /** �������g����ёS�Ă̎q�I�u�W�F�N�g���X�V����B */
    void update_recursively();

    /** �ʒu `p` ������Ƃ��Ď������g����ёS�Ă̎q�I�u�W�F�N�g��`�悷��B */
    void draw_recursively(const position_t &p = NPOS) const;

    inline const position_t& position() const { return m_position; }
    inline const width_t& width() const { return m_width; }
    inline age_t age() const { return m_age; }
    inline priority_t priority() const { return m_priority; }
    
    /** �}�E�X�|�C���^�����̃I�u�W�F�N�g�̕`��͈͓��ɂ���� true ��Ԃ��B */
    inline bool is_pointed() const { return m_is_pointed; }

    /** �}�E�X�|�C���^�����ڂ��̃I�u�W�F�N�g���|�C���g���Ă���Ȃ� true ��Ԃ��B */
    inline bool is_pointed_directly() const { return m_is_pointed_directly; }

    inline bool is_dragged_left() const { return m_is_dragged_left; }
    inline bool is_dragged_right() const { return m_is_dragged_right; }
    inline bool is_dragged_middle() const { return m_is_dragged_middle; }

    inline const std::unique_ptr<image_t>& image() const { return m_image; }
    
    /**
     * �I�u�W�F�N�g `p` ���q�Ƃ��Ēǉ�����B
     * �I�u�W�F�N�g�� priority() ���Ⴂ���ɕ��Ԃ悤�ɒǉ������B
     * �����l�̏ꍇ�͌�ɒǉ����ꂽ�������ɗ���B
     */
    void add_child(gui_ptr_t p);

    /** �w�肵���I�u�W�F�N�g `p` ���q����폜����B */
    void remove_child(gui_ptr_t p);

protected:
    /** GUI��`�悵�����ʂ��摜�Ƃ��� gui_t::m_image �ɐ�������B */
    void make_image();

    std::deque<gui_ptr_t> m_children;

    position_t m_position;
    width_t m_width;
    age_t m_age;
    priority_t m_priority;

    bool m_is_pointed;
    bool m_is_pointed_directly;
    bool m_is_dragged_left;
    bool m_is_dragged_right;
    bool m_is_dragged_middle;

    std::unique_ptr<image_t> m_image;
};


/** �X�N���[���o�[��\���N���X�B */
class gui_scroll_bar_t : public gui_t
{
public:
    gui_scroll_bar_t(position_t p, width_t w, priority_t pr, bool is_vertical);

    virtual int value() const = 0;
    virtual int value_min() const = 0;
    virtual int value_max() const = 0;

    /** �S�̂��猩���X�N���[���̊����� [1, 0] �͈̔͂ŕԂ��B */
    double ratio() const;
    
    inline bool is_vertical() const { return m_is_vertical; }
    inline bool is_horizontal() const { return not is_vertical(); }
    
private:
    bool m_is_vertical;
    bool m_is_scrolled;
};


}
