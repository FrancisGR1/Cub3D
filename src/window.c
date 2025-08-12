#include "cub3d.h"

void init_window(t_window *win)
{
	LOG_DEBUG("Initializing window");
	win->mlx = mlx_init();
	win->mlx_win = mlx_new_window(win->mlx, 640, 360, "cub3d");
	win->img = mlx_new_image(win->mlx, 640, 360);
	win->addr = mlx_get_data_addr(win->img, &win->bits_per_pixel,
			&win->line_length, &win->endian);
	if (win->addr == NULL)
	LOG_TRACE("Success: initialized mlx window");
}
