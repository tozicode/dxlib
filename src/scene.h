#pragma once

#include "gui.h"

namespace dxlib
{

/**
 * ゲームの遷移状態を表すためのクラス。
 * kernel_t に渡すことでその状態に遷移する。
 */
class scene_t : public gui_t
{
public:
    /** 毎フレーム呼び出される処理。 */
    inline void routine();

    /**
     * このシーンの上に別のシーンを追加する。
     * 追加されたシーンが終了するまで、このシーンは描画はされるが更新されない。
     */
    inline void push_scene(std::unique_ptr<scene_t>);

    /**
     * push_scene() で追加されたシーンを削除する。
     * 削除されるタイミングはそのフレームの最後。
     * シーンが追加されていない状態で呼び出した場合は何もしない。
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