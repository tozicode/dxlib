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

        /** ボタンが押されている間, 毎フレーム呼び出す処理. */
        void push();

        /** ボタンが離されている間, 毎フレーム呼び出す処理. */
        void release();

        /**
        * このボタンに関するカウント値を返す.
        * 返り値が正なら何フレーム押し続けているかを表す.
        * 返り値が負なら何フレーム押されていないかを表す.
        */
        int count() const { m_count; }

        /**
        * このボタンがクリックされた瞬間のみ true を返す.
        * より具体的には, ボタンが押されて5フレーム以内に離された瞬間.
        */
        bool is_clicked() const;

        /**
        * このボタンがダブルクリックされた瞬間のみ true を返す.
        * より具体的には, 10フレーム以内に2回ボタンが押されて離された瞬間.
        */
        bool is_double_clicked() const;

        /**
        * このボタンがドラッグされている間 true を返す.
        * より具体的には, ボタンが5フレーム以上押されている状態.
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
