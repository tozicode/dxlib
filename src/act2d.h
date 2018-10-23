#pragma once

#include <memory>
#include <unordered_map>

#include "util.h"
#include "image_t"


namespace dxlib
{

namespace act2d
{


class playarea_t;
class mapchip_library_t;
class object_t;

using priority_t = int;
using weight_t = int;
using mapchip_id_t = int;


/** レイヤーの位置を指定するための列挙子。 */
enum layer_key_e
{
    LAYER_PLUS_0,  // オブジェクトのすぐ手前に表示される
    LAYER_PLUS_1,  // 手前に表示される
    LAYER_MINUS_0, // オブジェクトのすぐ奥に表示される
    LAYER_MINUS_1, // 奥に表示される
};


/** プレイ領域上に存在する全てのオブジェクトの基底クラス。 */
class object_t
{
public:
    object_t() : m_pos(0, 0), m_col(0, 0), m_weight(-1);
    object_t(const position_t &p, const width_t &s, weight_t w = -1)
        : m_pos(p), m_col(s), m_weight(w) {}

    /** 更新処理を実行する。 */
    virtual void update() = 0;
    
    /** 画面上の座標 `p` を中心として拡大率 `s` で描画する。 */
    virtual void draw(const position_t &p, scale_t s) const = 0;

    const position_t& position() const { return m_pos; }
    const width_t& collision() const { return m_col; }

    /**
     * オブジェクトの重さを返す。
     * 接触判定の解決に用いられ、値が大きいほど動きづらい。
     * 値が -1 の場合は完全に固定されていて動かない。
     */
    weight_t weight() const { return m_weight; }

    /** 描画時の優先度を返す。この値が高いほど手前に描画される。 */
    priority_t priority() const { return m_priority; }

protected:
    position_t m_pos;
    width_t    m_col;
    weight_t   m_weight;
    priority_t m_priority;
};

using object_ptr_t = std::shared_ptr<object_t>;


//マップチップの情報を保持する構造体。
struct mapchip_t
{
    mapchip_t(const filepath_t &p, bool s = false)
        : image(p), solid(s) {}
    
    image_t image; // マップチップ画像
    bool solid;    // 接触判定を持つかどうか
    priority_t priority; // 描画時の優先度
};


/** マップチップの情報を一括管理するためシングルトンクラス。 */
class mapchip_library_t : public hash_map<index_t, std::unique_ptr<mapchip_t>>
{
public:
    static void initialize(const filepath_t &p);
    static const mapchip_library_t& instance();

    /** マップチップの大きさを返す。 */
    const xy_t<int>& width() const { return m_width; }

private:
    mapchip_library_t(const filepath_t &p);

    static std::unique_ptr<mapchip_library_t> ms_instance;
        
    xy_t<int> m_width;
};

inline const mapchip_library_t& mapchips() { return mapchip_library_t::instance(); }


/** 2Dアクションのプレイ領域を表すクラス。 */
class playarea_t
{
public:
    playarea_t(const filepath_t &p);

    /** 毎フレーム実行する処理。 */
    void update();

    /** 位置 `p` にプレイ領域内の領域 `a` を描画する。 */
    void draw(const position_t &p, const box_t &a) const;

protected:
    /** 各背景レイヤーの画像を保持する連想配列。 */
    hash_map<layer_key_e, std::unique_ptr<image_t>> m_layers;

    std::list<object_ptr_t> m_objs;
    position_t m_camera;
};



}

}
