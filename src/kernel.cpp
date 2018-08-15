#include "kernel.h"
#include "key_input.h"
#include "mouse_input.h"
#include "scene.h"


namespace dxlib
{


initializer_t::initializer_t()
    : fps(30), window_size(600, 400), is_full_screen(false),
    do_run_always(false), can_change_window_size(false),
    can_drag_file_into_window(false)
{}


std::unique_ptr<kernel_t> kernel_t::ms_inst;


void kernel_t::initialize(const initializer_t &init)
{
    ms_inst.reset(new kernel_t(init));
}


kernel_t::kernel_t(const initializer_t &init)
    : m_fps(init.fps), m_age(0), m_do_quit(false)
{
    ChangeWindowMode(init.is_full_screen ? FALSE : TRUE);
    SetWindowSizeChangeEnableFlag(init.can_change_window_size ? TRUE : FALSE);
    SetGraphMode(init.window_size.x, init.window_size.y, 32);
    SetMainWindowText(init.title.c_str());

    if (DxLib_Init() == -1)
        quit();
    else if (SetDrawScreen(DX_SCREEN_BACK) != 0)
        quit();
    else
    {
        SetDragFileValidFlag(init.can_drag_file_into_window ? TRUE : FALSE);
        SetAlwaysRunFlag(init.do_run_always ? TRUE : FALSE);
        SetHookWinProc(WndProc);
    }
}


void kernel_t::run()
{
    while (not do_quit())
    {
        scene.update();

        if (scene.current)
        {
            scene.current->update_recursively();
            scene.current->draw_recursively();
        }

        update();
    }
}


void kernel_t::update()
{
    scene.update();
    ScreenFlip();

    ++m_age;
    m_counts.push_front(GetNowCount());

    if (m_counts.size() > 30)
        m_counts.pop_back();

    if (m_counts.size() > 1)
    {
        time_t d = m_counts.at(0) - m_counts.at(1);
        if (d < millisec_per_frame())
            Sleep(millisec_per_frame() - d);
    }

    key_input().update();
    mouse_input().update();

    if (ProcessMessage() != 0 or ClearDrawScreen() != 0)
        quit();
}


int kernel_t::fps() const
{
    time_t d = 0;

    if (m_counts.size() > 1)
    {
        for (int i = 0; i < (int)m_counts.size() - 1; ++i)
            d += m_counts.at(i) - m_counts.at(i + 1);

        d /= m_counts.size() - 1;
        return (d > 0) ? (1000 / d) : 0;
    }
    else
        return 0;
}


void kernel_t::scene_manager_t::reserve(std::unique_ptr<scene_t> s)
{
    next.reset(s.release());
}


void kernel_t::scene_manager_t::update()
{
    if (next)
        current.reset(next.release());
}


LRESULT CALLBACK WndProc(
    HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg)
    {
    case WM_CLOSE:
        kernel().quit();
        break;
    }
    return 0;
}


}