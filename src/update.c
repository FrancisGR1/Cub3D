/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 21:11:09 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/14 23:37:09 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	update_player_pos_p2(t_vec2d desloc, int map[MAX_ROWS][MAX_COLS],
				t_player *p);
static bool	is_walkable(int map[MAX_ROWS][MAX_COLS], double x, double y);

void	update_player_dir(t_player *p)
{
	if (p->rot_left)
	{
		rotate_vector(&p->dir, -p->rot_speed);
		rotate_vector(&p->plane, -p->rot_speed);
	}
	if (p->rot_right)
	{
		rotate_vector(&p->dir, p->rot_speed);
		rotate_vector(&p->plane, p->rot_speed);
	}
}

void	update_player_pos(int map[MAX_ROWS][MAX_COLS], t_player *p)
{
	t_vec2d	desloc;
	t_vec2d	step;

	desloc.x = 0.0;
	desloc.y = 0.0;
	if (p->move_up)
		desloc = sum_vector(desloc, mult_vector(p->dir, p->move_speed));
	if (p->move_down)
		desloc = sum_vector(desloc, mult_vector(p->dir, -p->move_speed));
	if (p->move_left)
	{
		step = mult_vector(p->dir, p->move_speed);
		rotate_vector(&step, -PI / 2);
		desloc = sum_vector(desloc, step);
	}
	if (p->move_right)
	{
		step = mult_vector(p->dir, p->move_speed);
		rotate_vector(&step, PI / 2);
		desloc = sum_vector(desloc, step);
	}
	update_player_pos_p2(desloc, map, p);
}

static void	update_player_pos_p2(t_vec2d desloc, int map[MAX_ROWS][MAX_COLS],
		t_player *p)
{
	float	test;

	if (desloc.x != 0)
	{
		test = p->pos.x + desloc.x;
		if (desloc.x > 0)
			test += p->collision_buffer;
		else
			test -= p->collision_buffer;
		if (is_walkable(map, test, p->pos.y))
			p->pos.x += desloc.x;
	}
	if (desloc.y != 0)
	{
		test = p->pos.y + desloc.y;
		if (desloc.y > 0)
			test += p->collision_buffer;
		else
			test -= p->collision_buffer;
		if (is_walkable(map, p->pos.x, test))
			p->pos.y += desloc.y;
	}
}

static bool	is_walkable(int map[MAX_ROWS][MAX_COLS], double x, double y)
{
	int	mx;
	int	my;

	mx = (int)floor(x);
	my = (int)floor(y);
	if (mx < 0 || my < 0)
		return (false);
	if (my >= MAX_ROWS || mx >= MAX_COLS)
		return (false);
	return (map[my][mx] == 0);
}
