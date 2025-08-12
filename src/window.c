#include "cub3d.h"

void init_window(t_window *win)
{
	LOG_DEBUG("Initializing window");
	win->mlx = mlx_init();
	if (win->mlx == NULL)
	{
		LOG_ERROR("Error: couldn't initialize mlx");
		return ;
	}
	win->mlx_win = mlx_new_window(win->mlx, 640, 360, "cub3d");
	if (win->mlx_win == NULL)
	{
		LOG_ERROR("Error: couldn't make new window");
		return ;
	}
	win->img = mlx_new_image(win->mlx, 640, 360);
	if (win->img == NULL)
	{
		LOG_ERROR("Error: couldn't make new image");
		return ;
	}
	win->addr = mlx_get_data_addr(win->img, &win->bits_per_pixel,
			&win->line_length, &win->endian);
	if (win->addr == NULL)
	{
		LOG_ERROR("Error: couldn't get data address");
		return ;
	}
	LOG_TRACE("Success: initialized mlx window");
}
