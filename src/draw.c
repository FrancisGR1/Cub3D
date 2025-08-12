#include "cub3d.h"

void put_pixel(t_window *win, int x, int y, int color)
{
	char *dst;
	
	dst = win->addr + (y * win->line_length + x * (win->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

//@TODO: paint ceiling (metade superior)
//@TODO: paint floor (metade inferior)
//@TODO: draw textures
