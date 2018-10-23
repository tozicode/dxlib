#pragma once

#include "util.h"


namespace dxlib
{

using sound_handle_t = int;
using sound_volume_t = int;
using sound_type_t = int;


class sound_t
{
public:
    sound_t() : m_handle(-1) {}
    sound_t(const filepath_t &p);
    sound_t(sound_handle_t h);
    sound_t(sound_t&&);

    sound_t(const sound_t&) = delete;
    
    ~sound_t();

    void play() const;
    void play(sound_volume_t v) const;
    void loop() const;
    void stop() const;

    inline sound_handle_t handle() const { return m_handle; }
    sound_handle_t release();

private:
    sound_handle_t m_handle;
};


class sound_library_t : public std::deque<std::unique_ptr<sound_t>>
{
public:
    static sound_library_t& instance();

    void update();

    void add(const filepath_t &p);
    
    void play(sound_type_t key) const { at(key).play(); }
    void loop(sound_type_t key) const { at(key).loop(); }
    void stop(sound_type_t key) const { at(key).stop(); }
    void fadein(sound_type_t key);
    void fadeout(sound_type_t key);

private:
    sound_library_t();
    
    static std::unique_ptr<sound_library_t> ms_instance;

    std::unordered_map<sound_type_t, ranged_value_t<sound_volume_t>> m_fade_volume;
};

inline sound_library_t& sounds() { return sound_library_t::instance(); }


class sound_channel_t
{
public:
    sound_channel_t();

    void play(sound_type_t t);
    void loop(sound_type_t t, bool do_fade);
    void stop();

private:
    sound_type_t m_key;
};


}
