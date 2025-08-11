#include "cub3d.h"

static void setup_event_listeners(t_window *win, void *main_struct);

void event_loop(t_game *g)
{
	setup_event_listeners(g->win, (void *)g);
	mlx_loop(g->win->mlx);
}

static void setup_event_listeners(t_window *win, void *main_struct)
{
	//@TODO: fazer cast type à struct_to_pass
	mlx_hook(win->mlx_win, 17, 0L, close_window, &main_struct);
	mlx_hook(win->mlx_win, 2, 1L << 0, handle_key, &main_struct);
}
