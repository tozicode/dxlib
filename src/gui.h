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

/** 画面を構成する各種オブジェクトの基底クラス. */
class gui_t
{
public:
    gui_t(position_t p, width_t w, priority_t pr = 100);

    /** 毎フレーム行う更新処理。 */
    virtual void update();

    /** 座標 `p` を左上としてGUIを描画する。 */
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

    /** 自分自身および全ての子オブジェクトを更新する。 */
    void update_recursively();

    /** 位置 `p` を左上として自分自身および全ての子オブジェクトを描画する。 */
    void draw_recursively(const position_t &p = NPOS) const;

    inline const position_t& position() const { return m_position; }
    inline const width_t& width() const { return m_width; }
    inline age_t age() const { return m_age; }
    inline priority_t priority() const { return m_priority; }
    
    /** マウスポインタがこのオブジェクトの描画範囲内にあれば true を返す。 */
    inline bool is_pointed() const { return m_is_pointed; }

    /** マウスポインタが直接このオブジェクトをポイントしているなら true を返す。 */
    inline bool is_pointed_directly() const { return m_is_pointed_directly; }

    inline bool is_dragged_left() const { return m_is_dragged_left; }
    inline bool is_dragged_right() const { return m_is_dragged_right; }
    inline bool is_dragged_middle() const { return m_is_dragged_middle; }

    inline const std::unique_ptr<image_t>& image() const { return m_image; }
    
    /**
     * オブジェクト `p` を子として追加する。
     * オブジェクトは priority() が低い順に並ぶように追加される。
     * 同じ値の場合は後に追加された方が後ろに来る。
     */
    void add_child(gui_ptr_t p);

    /** 指定したオブジェクト `p` を子から削除する。 */
    void remove_child(gui_ptr_t p);

protected:
    /** GUIを描画した結果を画像として gui_t::m_image に生成する。 */
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


/** スクロールバーを表すクラス。 */
class gui_scroll_bar_t : public gui_t
{
public:
    gui_scroll_bar_t(position_t p, width_t w, priority_t pr, bool is_vertical);

    virtual int value() const = 0;
    virtual int value_min() const = 0;
    virtual int value_max() const = 0;

    /** 全体から見たスクロールの割合を [1, 0] の範囲で返す。 */
    double ratio() const;
    
    inline bool is_vertical() const { return m_is_vertical; }
    inline bool is_horizontal() const { return not is_vertical(); }
    
private:
    bool m_is_vertical;
    bool m_is_scrolled;
};


}
