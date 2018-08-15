#include <kernel.h>

using namespace dxlib;


int WINAPI WinMain(
    HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    initializer_t init;
    {
        init.fps = 30;
        init.title = _T("game title");
        init.window_size.x = 800;
        init.window_size.y = 600;
        init.is_full_screen = false;
        init.do_run_always = true;
        init.can_change_window_size = true;
        init.can_drag_file_into_window = true;
    }

    kernel_t::initialize(init);
    kernel().run();

    return 0; 
}


