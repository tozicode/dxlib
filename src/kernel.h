#pragma once

#include <memory>
#include <deque>

#include <DxLib.h>

#include "util.h"


namespace dxlib
{


class scene_t;

struct initializer_t
{
    initializer_t();

    int fps;
    tstring title;
    width_t window_size;
    bool is_full_screen;
    bool do_run_always;
    bool can_change_window_size;
    bool can_drag_file_into_window;
};


class kernel_t
{
public:
    static void initialize(const initializer_t&);
    static kernel_t& instance() { return (*ms_inst); }

    void run();
    void update();

    inline void quit() { m_do_quit = true; }
    inline bool do_quit() const { return m_do_quit; }

    int fps() const;
    inline int fps_base() const { return m_fps; }
    inline int millisec_per_frame() const { return 1000 / m_fps; }
    inline age_t age() const { return m_age; }

    struct scene_manager_t
    {
        void reserve(std::unique_ptr<scene_t>);
        void update();

        std::unique_ptr<scene_t> current;
        std::unique_ptr<scene_t> next;
    } scene;

private:
    kernel_t(const initializer_t&);

    static std::unique_ptr<kernel_t> ms_inst;

    int m_fps;
    age_t m_age;
    std::deque<time_t> m_counts;

    bool m_do_quit;
};


inline kernel_t& kernel() { return kernel_t::instance(); }

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


}