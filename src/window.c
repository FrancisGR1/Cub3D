#include "cub3d.h"

//@TODO: alloc window

void init_window(t_window *win)
{
	win->mlx = mlx_init();
	win->mlx_win = mlx_new_window(win->mlx, 640, 360, "hello");
}

