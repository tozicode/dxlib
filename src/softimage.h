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

    /** scale2x�A���S���Y����p����2�{�Ɋg�債���\�t�g�C���[�W��Ԃ�. */
    softimage_handle_t scale2x() const;

    /**
    * 256�p���b�g�摜���̃\�t�g�C���[�W�ɕ�������.
    * ����̓I�ɂ�, `key` ��菬�����F�R�[�h�ō\�����ꂽ�摜��
    * `key` �ȏ�̐F�R�[�h�ō\�����ꂽ�摜�Ƃɕ��������.
    * @param bound �摜�����ɂ����鋫�E�̃p���b�g�ԍ�.
    * @param bg    �����F�Ƃ��ĉ��߂����p���b�g�ԍ�.
    */
    std::pair<softimage_handle_t, softimage_handle_t>
        split(int bound = 240, int bg = 255) const;

private:
    softimage_handle_t m_handle;
};


}