/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 19:10:35 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/27 17:29:10 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
	if (keycode == XK_m)
	{
		if (game->show_minimap == false)
			game->show_minimap = true;
		else
			game->show_minimap = false;
	}
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
