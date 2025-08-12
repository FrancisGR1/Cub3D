#include "cub3d.h"

static void setup_event_listeners(t_window *win, t_game *game);

void event_loop(t_game *game)
{
	LOG_INFO("Event Loop");
	setup_event_listeners(game->win, game);
	LOG_DEBUG("Mlx: entering mlx loop");
	mlx_loop(game->win->mlx);
}

int	handle_key(int keycode, t_game *game)
{
	LOG_DEBUG("Handling key");
	//@TODO:
	//<- -> arrow keys
	//WASD
	//ESC
	(void) keycode, (void) game;
	return (EXIT_SUCCESS);
}

static void setup_event_listeners(t_window *win, t_game *game)
{
	LOG_DEBUG("Setup even listeners");
	mlx_hook(win->mlx_win, 17, 0L, end_game, game);
	mlx_hook(win->mlx_win, 2, 1L << 0, handle_key, game);
}
