#pragma once

#include "DxLib.h"

#include "util.h"
#include "image.h"


namespace dxlib
{

using softimage_handle_t = int;


/** Wrapper class of soft-image in DxLib. */
class softimage_t
{
public:
    softimage_t() : m_handle(-1) {}
    softimage_t(const filepath_t &p);
    softimage_t(softimage_handle_t);

    ~softimage_t();

    void draw(position_t p) const;

    width_t width() const;

    int get_palette_code_at(position_t p) const;
    color_t get_color_at(position_t p) const;

    inline bool empty() const { return m_handle == -1; }
    bool is_256color() const;
   
    image_handle_t create_graph() const;

    /** scale2xアルゴリズムを用いて2倍に拡大したソフトイメージを返す. */
    softimage_handle_t scale2x() const;

    /**
    * 256パレット画像を二つのソフトイメージに分割する.
    * より具体的には, `key` より小さい色コードで構成された画像と
    * `key` 以上の色コードで構成された画像とに分割される.
    * @param bound 画像分割における境界のパレット番号.
    * @param bg    透明色として解釈されるパレット番号.
    */
    std::pair<softimage_handle_t, softimage_handle_t>
        split(int bound = 240, int bg = 255) const;

private:
    softimage_handle_t m_handle;
};


}