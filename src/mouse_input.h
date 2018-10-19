#pragma once

#include <deque>
#include <memory>

#include <DxLib.h>

#include "util.h"


namespace dxlib
{


class mouse_input_t
{
public:
    class button_t
    {
    public:
        button_t();

        /** �{�^����������Ă����, ���t���[���Ăяo������. */
        void push();

        /** �{�^����������Ă����, ���t���[���Ăяo������. */
        void release();

        /**
        * ���̃{�^���Ɋւ���J�E���g�l��Ԃ�.
        * �Ԃ�l�����Ȃ牽�t���[�����������Ă��邩��\��.
        * �Ԃ�l�����Ȃ牽�t���[��������Ă��Ȃ�����\��.
        */
        int count() const { m_count; }

        /**
        * ���̃{�^�����N���b�N���ꂽ�u�Ԃ̂� true ��Ԃ�.
        * ����̓I�ɂ�, �{�^�����������5�t���[���ȓ��ɗ����ꂽ�u��.
        */
        bool is_clicked() const;

        /**
        * ���̃{�^�����_�u���N���b�N���ꂽ�u�Ԃ̂� true ��Ԃ�.
        * ����̓I�ɂ�, 10�t���[���ȓ���2��{�^����������ė����ꂽ�u��.
        */
        bool is_double_clicked() const;

        /**
        * ���̃{�^�����h���b�O����Ă���� true ��Ԃ�.
        * ����̓I�ɂ�, �{�^����5�t���[���ȏ㉟����Ă�����.
        */
        bool is_dragged() const;

    private:
        int m_count;
        std::deque<age_t> m_log_pushed;
        std::deque<age_t> m_log_released;
    };

    static mouse_input_t& instance();

    void update();

    inline const xy_t<int>& position() const { return m_position; }
    inline const xy_t<int>& delta() const { return m_delta; }

    inline const button_t& left() const { return m_left; }
    inline const button_t& right() const { return m_right; }
    inline const button_t& middle() const { return m_middle; }

    inline int wheel() const { return m_wheel; }

private:
    mouse_input_t();

    static std::unique_ptr<mouse_input_t> ms_inst;

    xy_t<int> m_position;
    xy_t<int> m_delta;

    button_t m_left;
    button_t m_right;
    button_t m_middle;

    int m_wheel;
};

inline mouse_input_t& mouse_input() { return mouse_input_t::instance(); }

}
