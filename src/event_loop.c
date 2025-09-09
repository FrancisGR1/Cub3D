#include "cub3d.h"

int event_loop(t_game *game)
{
	LOG_INFO("Event Loop");
	LOG_DEBUG("Mlx: entering mlx loop");
	LOG_DEBUG("Updating data");
	update(game);
	LOG_DEBUG("Rendering");
	render(game);
	return (0);
}

void setup_event_listeners(t_window *win, t_game *game)
{
	LOG_DEBUG("Setup even listeners");

	mlx_hook(win->mlx_win, 2, 1L<<0, key_press, game);
	mlx_hook(win->mlx_win, 3, 1L<<1, key_release, game);
	mlx_hook(win->mlx_win, 17, 0L, end_game, game);
	mlx_loop_hook(win->mlx, event_loop, game);
}
