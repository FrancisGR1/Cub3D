/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 19:09:14 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/15 19:13:29 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	set_draw_info(t_draw *d, t_ray *ray, t_player *player,
		t_texture texture[MAX_TEXTURES])
{
	d->screen_y = 0;
	d->line_height = (int)WINDOW_HEIGHT / ray->perp_dist;
	d->draw_start = (WINDOW_HEIGHT / 2) - (d->line_height / 2);
	if (d->draw_start < 0)
		d->draw_start = 0;
	d->draw_end = (WINDOW_HEIGHT / 2) + (d->line_height / 2);
	if (d->draw_end >= WINDOW_HEIGHT)
		d->draw_end = WINDOW_HEIGHT - 1;
	d->tex = get_wall_texture(ray, texture);
	if (ray->hit_side == 0)
		d->wall_x = player->pos.y + ray->perp_dist * ray->dir.y;
	else
		d->wall_x = player->pos.x + ray->perp_dist * ray->dir.x;
	d->wall_x -= floor(d->wall_x);
	d->tex_x = (int)(d->wall_x * (float)d->tex->width);
	if ((ray->hit_side == 0 && ray->dir.x < 0) || (ray->hit_side == 1
			&& ray->dir.y > 0))
		d->tex_x = d->tex->width - d->tex_x - 1;
}

t_draw	*alloc_draw_info(t_arena *game_memory, t_file_data *extracted_data)
{
	t_draw	*d;

	d = arena_alloc(&game_memory, sizeof(t_draw), 1);
	d->line_height = 0;
	d->draw_start = 0;
	d->draw_end = 0;
	d->floor_color = extracted_data->floor;
	d->ceiling_color = extracted_data->ceiling;
	return (d);
}
