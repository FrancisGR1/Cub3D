/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 19:09:29 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/15 22:07:17 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	set_ray_info(t_ray *r, t_player *player, int screen_x)
{
	r->camera_pixel = (2 * (screen_x / (double)WINDOW_WIDTH)) - 1;
	r->dir.x = player->dir.x + player->plane.x * r->camera_pixel;
	r->dir.y = player->dir.y + player->plane.y * r->camera_pixel;
	r->map_pos = make_vec2i((int)player->pos.x, (int)player->pos.y);
	r->delta_dist = make_vec2d(fabs(1 / r->dir.x), fabs(1 / r->dir.y));
	if (r->dir.x < 0)
	{
		r->side_dist.x = (player->pos.x - r->map_pos.x) * r->delta_dist.x;
		r->step.x = -1;
	}
	else
	{
		r->side_dist.x = (r->map_pos.x + 1.0 - player->pos.x) * r->delta_dist.x;
		r->step.x = 1;
	}
	if (r->dir.y < 0)
	{
		r->side_dist.y = (player->pos.y - r->map_pos.y) * r->delta_dist.y;
		r->step.y = -1;
	}
	else
	{
		r->side_dist.y = (r->map_pos.y + 1.0 - player->pos.y) * r->delta_dist.y;
		r->step.y = 1;
	}
}

void	fix_fish_eye_effect(t_ray *ray, t_player *player)
{
	if (ray->hit_side == 0)
		ray->perp_dist = (ray->map_pos.x - player->pos.x + (1 - ray->step.x)
				/ 2) / ray->dir.x;
	else
		ray->perp_dist = (ray->map_pos.y - player->pos.y + (1 - ray->step.y)
				/ 2) / ray->dir.y;
}

void	dda_loop(t_ray *r, int map[MAX_ROWS][MAX_COLS])
{
	int	hit;

	hit = 0;
	while (hit == 0)
	{
		if (r->side_dist.x < r->side_dist.y)
		{
			r->side_dist.x += r->delta_dist.x;
			r->map_pos.x += r->step.x;
			r->hit_side = 0;
		}
		else
		{
			r->side_dist.y += r->delta_dist.y;
			r->map_pos.y += r->step.y;
			r->hit_side = 1;
		}
		if (map[r->map_pos.y][r->map_pos.x] == 1)
			hit = 1;
	}
}

t_ray	*alloc_ray(t_arena *game_memory)
{
	t_ray	*r;

	r = arena_alloc(&game_memory, sizeof(t_ray), 1);
	return (r);
}
