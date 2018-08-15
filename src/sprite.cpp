#include "sprite.h"


namespace dxlib
{


std::unique_ptr<sprite_library_t> sprite_library_t::ms_inst;


sprite_library_t& sprite_library_t::instance()
{
    if (not ms_inst)
        ms_inst.reset(new sprite_library_t());
    return *ms_inst;
}


sprite_library_t::sprite_library_t()
{}


const sprite_library_t::sprite_t& sprite_library_t::get(const filepath_t &s)
{
    if (m_sprites.count(s) == 0)
        load(s);

    return m_sprites.at(s);
}


void sprite_library_t::load(const filepath_t &s, bool forced)
{
    if (not forced and m_sprites.count(s) > 0) return;

    softimage_t si(s);
    softimage_t si8x(
        softimage_t(softimage_t(si.scale2x()).scale2x()).scale2x());

    auto &spr = m_sprites[s];

    if (si8x.is_256color())
    {
        auto splitted = si8x.split();
        spr.body.reset(
            new image_t(softimage_t(splitted.first).create_graph()));
        spr.edge.reset(
            new image_t(softimage_t(splitted.second).create_graph()));
    }
    else
    {
        spr.body.reset(new image_t(si8x.create_graph()));
        spr.edge.reset();
    }
}


void sprite_library_t::load_all()
{
    std::list<filepath_t> keys;

    for (const auto &p : m_sprites)
        keys.push_back(p.first);

    for (const auto &s : keys)
        load(s, true);
}


sprite_t::sprite_t(const filepath_t &s)
    : filepath(s), position(0, 0), scale(1.0, 1.0), angle(0.0)
{
    sprites().load(filepath);
}


const sprite_library_t::sprite_t& sprite_t::image() const
{
    return sprites().get(filepath);
}

}