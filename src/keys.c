#include "cub3d.h"

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

int	key_press(int keycode, t_game *game)
{
	if (keycode == XK_Escape)
		end_game(game);
	if (keycode == XK_w)
		game->player->move_up = true;
	if (keycode == XK_s)
		game->player->move_down = true;
	if (keycode == XK_a)
		game->player->move_left = true;
	if (keycode == XK_d)
		game->player->move_right = true;
	if (keycode == XK_Left)
		game->player->rot_left = true;
	if (keycode == XK_Right)
		game->player->rot_right = true;
	return (0);
}

int	key_release(int keycode, t_game *game)
{
	if (keycode == XK_w)
		game->player->move_up = false;
	if (keycode == XK_s)
		game->player->move_down = false;
	if (keycode == XK_a)
		game->player->move_left = false;
	if (keycode == XK_d)
		game->player->move_right = false;
	if (keycode == XK_Left)
		game->player->rot_left = false;
	if (keycode == XK_Right)
		game->player->rot_right = false;

	return (0);
}
