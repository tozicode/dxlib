#include "sound.h"


namespace dxlib
{


sound_t::sound_t(const filepath_t &p)
    : m_handle(LoadSoundMem(p.c_str()))
{}


sound_t::sound_t(sound_handle_t h)
    : m_handle(h)
{}


sound_t::sound_t(sound_t &&s)
    : m_handle(s.release())
{}


void sound_t::play() const
{
    PlaySoundMem(handle(), DX_PLAYTYPE_BACK);
}


void sound_t::play(sound_volume_t v) const
{
    ChangeNextPlayVolumeSoundMem(v, handle());
    PlaySoundMem(handle(), DX_PLAYTYPE_BACK);
}


void sound_t::loop() const
{
    PlaySoundMem(handle(), DX_PLAYTYPE_LOOP);
}


void sound_t::stop() const
{
    StopSoundMem(handle());
}


sound_handle_t sound_t::release()
{
    sound_handle_t h = m_handle;
    m_handle = -1;
    return h;
}


std::unique_ptr<sound_library_t> sound_library_t::ms_instance;


sound_library_t& sound_library_t::instance()
{
    if (not ms_instance)
        ms_instance.reset(new sound_library_t);
    return *ms_instance;
}


void sound_library_t::update()
{
    for (auto it = m_fade_volume.begin(); it != m_fade_volume.end(); ++it)
    {
        bool do_fadein = (it->second > 0);
        it->second += do_fadein ? 10 : -10;

        sound_volume_t vol = do_fadein ? it->second : (255 + it->second);
        // TODO: 音量設定

        if (std::abs(it->second.get()) == 255)
            it = m_fade_volume.erase(it);
        else
            ++it;
    }
}


void sound_library_t::add(const filepath_t &p)
{
    push_back(std::make_unique<sound_t>(p));
}


void sound_library_t::fadein(sound_type_t key)
{
    m_fade_volume[key] = ranged_value_t<sound_volume_t>(1, 1, 255);
}


void sound_library_t::fadeout(sound_type_t key)
{
    m_fade_volume[key] = ranged_value_t<sound_volume_t>(-1, -255, -1);
}



sound_channel_t::sound_channel_t()
    : m_key(-1)
{}


void sound_channel_t::play(sound_type_t t)
{
    if (m_key >= 0)
        sounds().stop(m_key);

    m_key = t;
    sounds.play(m_key);
}


void sound_channel_t::loop(sound_type_t t, bool do_fade)
{
    if (m_key >= 0)
    {
        if (do_fade)
            sounds().fadeout(m_key);
        else
            sounds().stop(m_key);
    }

    m_key = t;
    
    if (do_fade)
        sounds.fadein(m_key);
    else
        sounds.loop(m_key);
}


void sound_channel_t::stop()
{
    if (m_key >= 0)
        sounds().stop(m_key);

    m_key = -1;
}


}
