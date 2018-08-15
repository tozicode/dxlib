#include "./key_input.h"

namespace dxlib
{

std::unique_ptr<key_input_t> key_input_t::ms_inst;


key_input_t& key_input_t::instance()
{
    if (not ms_inst)
        ms_inst.reset(new key_input_t());

    return (*ms_inst);
}


key_input_t::key_input_t()
{
    m_keys.fill(0);
};


void key_input_t::update()
{
    char buf[256];
    GetHitKeyStateAll(buf);

    for (int i = 0; i < 256; i++)
    {
        int& key = m_keys[i];
        if (buf[i] == 1) key = std::max(+1, key + 1);
        else             key = std::min(-1, key - 1);
    }
}


int key_input_t::get_shift()
{
    int rs(get(KEY_INPUT_RSHIFT));
    int ls(get(KEY_INPUT_LSHIFT));
    return std::max(rs, ls);
}

}