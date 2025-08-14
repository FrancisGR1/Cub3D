#include "cub3d.h"

void render(t_game *game)
{
	t_window *win;

	LOG_INFO("Render");
	win = game->win;
	paint_background(win);
	LOG_DEBUG("Put image to window");
	mlx_put_image_to_window(win->mlx, win->mlx_win, win->img, 0, 0);
}

//@TODO: mudar as vars abaixo para t_render_context
void paint_background(t_window *win)
{
	unsigned int *pixels;
	int total_pixels = win->width * win->height;
	int ceiling_color = 0x87CEEB; // light blue
	int floor_color   = 0x000000; // black
	int half = total_pixels / 2;

	LOG_DEBUG("Painting background");
	pixels = (unsigned int *)win->addr;

	LOG_TRACE("Paiting ceiling");
	for (int i = 0; i < half; i++)
		pixels[i] = ceiling_color;

	LOG_TRACE("Paiting floor");
	for (int i = half; i < total_pixels; i++)
		pixels[i] = floor_color;
}

t_render_context *alloc_init_render_ctx(t_arena *game_memory, t_window *win, t_file_data *extracted_data)
{
	t_render_context *render_ctx;

	LOG_DEBUG("Allocating and Initializing render");
	render_ctx = arena_alloc(&game_memory, sizeof(t_render_context), 1);
	render_ctx->line_height = 0;
	render_ctx->draw_start = 0;
	render_ctx->draw_end = 0;
	render_ctx->total_pixels = win->width * win->height;
	render_ctx->floor_color = rgb_to_int(extracted_data->floor);
	render_ctx->ceiling_color = rgb_to_int(extracted_data->ceiling);
	LOG_TRACE("Success: render ctx struct allocated/initialized");
	return (render_ctx);
}

//@TODO: draw textures
