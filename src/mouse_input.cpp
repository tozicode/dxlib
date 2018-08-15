#include "./mouse_input.h"
#include "./kernel.h"


namespace dxlib
{

std::unique_ptr<mouse_input_t> mouse_input_t::ms_inst;


mouse_input_t& mouse_input_t::instance()
{
    if (not ms_inst)
        ms_inst.reset(new mouse_input_t());

    return (*ms_inst);
}


mouse_input_t::mouse_input_t()
    : m_position(0, 0), m_delta(0, 0),
    m_wheel(0)
{}


void mouse_input_t::update()
{
    int state = GetMouseInput();

    auto prev = m_position;
    if (GetMousePoint(&m_position.x, &m_position.y) != -1)
    {
        m_delta.x = m_position.x - prev.x;
        m_delta.y = m_position.y - prev.y;
    }

    m_wheel = GetMouseWheelRotVol();

    auto update_button = [&state](int key, button_t *button)
    {
        if (state & key)
            button->push();
        else
            button->release();
    };

    update_button(MOUSE_INPUT_LEFT, &m_left);
    update_button(MOUSE_INPUT_RIGHT, &m_right);
    update_button(MOUSE_INPUT_MIDDLE, &m_middle);
}


mouse_input_t::button_t::button_t()
    : m_count(0)
{}


void mouse_input_t::button_t::push()
{
    if (m_count < 0)
    {
        m_count = 1;

        m_log_pushed.push_front(kernel().age());
        if (m_log_pushed.size() > 3)
            m_log_pushed.pop_back();
    }
    else
        ++m_count;
}


void mouse_input_t::button_t::release()
{
    if (m_count > 0)
    {
        m_count = -1;

        m_log_released.push_front(kernel().age());
        if (m_log_released.size() > 3)
            m_log_released.pop_back();
    }
    else
        --m_count;
}


bool mouse_input_t::button_t::is_clicked() const
{
    if (m_count != -1) return false;
    if (m_log_pushed.empty()) return false;

    return (kernel().age() - m_log_pushed.front()) <= 5;
}


bool mouse_input_t::button_t::is_double_clicked() const
{
    if (m_count != -1) return false;
    if (m_log_pushed.size() < 2) return false;

    return
        (kernel().age() - m_log_pushed.at(0)) <= 5 and
        (kernel().age() - m_log_pushed.at(1)) <= 10;
}


bool mouse_input_t::button_t::is_dragged() const
{
    return m_count > 5;
}

}