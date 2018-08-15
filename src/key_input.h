#pragma once

#include <array>
#include <memory>

#include <DxLib.h>

#include "util.h"


namespace dxlib
{

class key_input_t
{
public:
    static key_input_t& instance();

    void update();

    inline int get(int i) { return m_keys.at(i); }

    inline int get_up()    { return get(KEY_INPUT_UP); }
    inline int get_down()  { return get(KEY_INPUT_DOWN); }
    inline int get_right() { return get(KEY_INPUT_RIGHT); }
    inline int get_left()  { return get(KEY_INPUT_LEFT); }

    int get_shift();

private:
    key_input_t();

    static std::unique_ptr<key_input_t> ms_inst;

    std::array<int, 256> m_keys;
};


inline key_input_t& key_input() { return key_input_t::instance(); }

}