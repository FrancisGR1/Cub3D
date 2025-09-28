/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luiz-dos <luiz-dos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 19:09:26 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/28 16:31:02 by luiz-dos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_player	*alloc_init_player(t_arena *game_memory)
{
	t_player	*p;

	p = arena_alloc(&game_memory, sizeof(t_player), 1);
	p->pos = make_vec2d(0, 0);
	p->dir = make_vec2d(0, 0);
	p->plane = make_vec2d(0, 0);
	p->move_speed = 0.020;
	p->rot_speed = 0.012;
	p->collision_buffer = 0.1;
	p->move_up = false;
	p->move_down = false;
	p->move_left = false;
	p->move_right = false;
	p->rot_left = false;
	p->rot_right = false;
	return (p);
}

void	set_player_position(t_player *player, double x, double y)
{
	player->pos = make_vec2d(x + 0.5, y + 0.5);
}

void	set_player_direction(t_player *player, size_t direction)
{
	if (direction == 'N')
	{
		player->dir = make_vec2d(0.0, -1.0);
		player->plane = make_vec2d(FOV, 0.0);
	}
	else if (direction == 'S')
	{
		player->dir = make_vec2d(0.0, 1.0);
		player->plane = make_vec2d(-FOV, 0.0);
	}
	else if (direction == 'E')
	{
		player->dir = make_vec2d(1.0, 0.0);
		player->plane = make_vec2d(0.0, FOV);
	}
	else if (direction == 'W')
	{
		player->dir = make_vec2d(-1.0, 0.0);
		player->plane = make_vec2d(0.0, -FOV);
	}
}
