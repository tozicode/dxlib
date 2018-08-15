#pragma once

#include <unordered_map>

#include "DxLib.h"

#include "util.h"
#include "image.h"
#include "softimage.h"


namespace dxlib
{


/**
 * �X�v���C�g�摜�𓝊��Ǘ�����N���X.
 * �V���O���g���N���X�Ƃ��Ď��������.
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

    /** �w�肵���p�X�ɑΉ�����摜�I�u�W�F�N�g��Ԃ�. */
    const sprite_t& get(const filepath_t&);

    /**
     * �w�肵���摜��ǂݍ���.
     * forced �� false �̏ꍇ, ���ɓǂݍ��ݍς݂̉摜�̓X�L�b�v����.
     */
    void load(const filepath_t&, bool forced = false);

    /** �S�Ẳ摜��ǂݍ��݂�����. */
    void load_all();

private:
    sprite_library_t();

    static std::unique_ptr<sprite_library_t> ms_inst;

    std::unordered_map<tstring, sprite_t> m_sprites;
};

inline sprite_library_t sprites() { return sprite_library_t::instance(); }


using depth_t = int;


/** ���[�V�������̊e�X�v���C�g�I�u�W�F�N�g��\���N���X. */
struct sprite_t
{
    sprite_t(const filepath_t&);

    /** ���̃X�v���C�g�Ɋ��蓖�Ă�ꂽ�摜��Ԃ�. */
    const sprite_library_t::sprite_t& image() const;

    /** ���̃X�v���C�g�Ɋ��蓖�Ă�ꂽ�摜�̃p�X. */
    filepath_t filepath;

    /** ���[�V�����̒��S���炱�̃X�v���C�g�̒��S�܂ł̑��΍��W. */
    position_t position;

    /** ���̃X�v���C�g��XY�������ꂼ��̊g�嗦. */
    xy_t<double> scale;

    /** ���̃X�v���C�g�̉�]�p. [rad] */
    double angle;

    /**
     * ���[�V�����`�掞��, ���̃X�v���C�g�̕`��D�揇��.
     * �l���Ⴂ���̂قǐ�� (= ����) �`�悳���.
     */
    depth_t depth;
};

}