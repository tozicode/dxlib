#pragma once

#include <unordered_map>

#include "DxLib.h"

#include "util.h"
#include "image.h"
#include "softimage.h"


namespace dxlib
{


/**
 * スプライト画像を統括管理するクラス.
 * シングルトンクラスとして実装される.
 */
class sprite_library_t
{
public:
    struct sprite_t
    {
        std::unique_ptr<image_t> body;
        std::unique_ptr<image_t> edge;
    };

    static sprite_library_t& instance();

    /** 指定したパスに対応する画像オブジェクトを返す. */
    const sprite_t& get(const filepath_t&);

    /**
     * 指定した画像を読み込む.
     * forced が false の場合, 既に読み込み済みの画像はスキップする.
     */
    void load(const filepath_t&, bool forced = false);

    /** 全ての画像を読み込みし直す. */
    void load_all();

private:
    sprite_library_t();

    static std::unique_ptr<sprite_library_t> ms_inst;

    std::unordered_map<tstring, sprite_t> m_sprites;
};

inline sprite_library_t sprites() { return sprite_library_t::instance(); }


using depth_t = int;


/** モーション中の各スプライトオブジェクトを表すクラス. */
struct sprite_t
{
    sprite_t(const filepath_t&);

    /** このスプライトに割り当てられた画像を返す. */
    const sprite_library_t::sprite_t& image() const;

    /** このスプライトに割り当てられた画像のパス. */
    filepath_t filepath;

    /** モーションの中心からこのスプライトの中心までの相対座標. */
    position_t position;

    /** このスプライトのXY方向それぞれの拡大率. */
    xy_t<double> scale;

    /** このスプライトの回転角. [rad] */
    double angle;

    /**
     * モーション描画時の, このスプライトの描画優先順位.
     * 値が低いものほど先に (= 後ろに) 描画される.
     */
    depth_t depth;
};

}