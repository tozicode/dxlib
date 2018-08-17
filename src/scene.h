#pragma once

#include "gui.h"

namespace dxlib
{

/**
 * �Q�[���̑J�ڏ�Ԃ�\�����߂̃N���X�B
 * kernel_t �ɓn�����Ƃł��̏�ԂɑJ�ڂ���B
 */
class scene_t : public gui_t
{
public:
    /** ���t���[���Ăяo����鏈���B */
    inline void routine();

    /**
     * ���̃V�[���̏�ɕʂ̃V�[����ǉ�����B
     * �ǉ����ꂽ�V�[�����I������܂ŁA���̃V�[���͕`��͂���邪�X�V����Ȃ��B
     */
    inline void push_scene(std::unique_ptr<scene_t>);

    /**
     * push_scene() �Œǉ����ꂽ�V�[�����폜����B
     * �폜�����^�C�~���O�͂��̃t���[���̍Ō�B
     * �V�[�����ǉ�����Ă��Ȃ���ԂŌĂяo�����ꍇ�͉������Ȃ��B
     */
    inline void pop_scene();

protected:
    std::unique_ptr<scene_t> m_scene;
    bool m_do_pop_scene;
};



inline void scene_t::routine()
{
    if (m_scene)
    {
        m_scene->update_recursively();
        draw_recursively();
        m_scene->draw_recursively();

        if (m_do_pop_scene)
            m_scene.reset();
    }
    else
    {
        update_recursively();
        draw_recursively();
    }
}


inline void scene_t::push_scene(std::unique_ptr<scene_t> s)
{
    m_scene.reset(s.release());
    m_do_pop_scene = false;
}


inline void scene_t::pop_scene()
{
    m_do_pop_scene = true;
}

}