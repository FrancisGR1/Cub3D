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

int key_press(int keycode, t_player *player)
{
	LOG_TRACE("Pressing key: %d\n", keycode);
	if (keycode == XK_w)
		player->move_up = true;
	else if (keycode == XK_a)
		player->move_left = true;
	else if (keycode == XK_s)
		player->move_down = true;
	else if (keycode == XK_d)
		player->move_right = true;
	else if (keycode == XK_Left)
		player->rotate_left = true;
	else if (keycode == XK_Right)
		player->rotate_right = true;
	return 0;
}

int key_release(int keycode, t_player *player)
{
	LOG_TRACE("Released key: %d\n", keycode);
	if (keycode == XK_w)
		player->move_up = false;
	else if (keycode == XK_a)
		player->move_left = false;
	else if (keycode == XK_s)
		player->move_down = false;
	else if (keycode == XK_d)
		player->move_right = false;
	else if (keycode == XK_Left)
		player->rotate_left = false;
	else if (keycode == XK_Right)
		player->rotate_right = false;
	return 0;
}
