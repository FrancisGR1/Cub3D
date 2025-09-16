/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_vertical.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 19:09:09 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/15 19:09:09 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	draw_ceiling(t_window *win, t_draw *d, int screen_x);
static void	draw_wall(t_game *game, t_draw *d, int screen_x);
static void	draw_floor(t_window *win, t_draw *d, int screen_x);

void	draw_vertical_line(t_game *game, t_draw *d, int screen_x)
{
	if (screen_x < 0 || screen_x >= WINDOW_WIDTH)
		return ;
	if (d->draw_start < 0)
		d->draw_start = 0;
	if (d->draw_end >= WINDOW_HEIGHT)
		d->draw_end = WINDOW_HEIGHT - 1;
	if (d->line_height <= 0 || d->tex->height <= 0)
		return ;
	draw_ceiling(game->win, game->draw_info, screen_x);
	draw_wall(game, game->draw_info, screen_x);
	draw_floor(game->win, game->draw_info, screen_x);
}

static void	draw_ceiling(t_window *win, t_draw *d, int screen_x)
{
	const unsigned int	color = rgb_to_int(d->ceiling_color);

	while (d->screen_y < d->draw_start)
	{
		pixel_put(win, screen_x, d->screen_y, color);
		d->screen_y++;
	}
}

static void	draw_wall(t_game *game, t_draw *d, int screen_x)
{
	unsigned int	color;
	float			tex_step;
	float			tex_pos;
	int				tex_y;

	tex_step = (float)d->tex->height / (float)d->line_height;
	tex_pos = (d->draw_start - (WINDOW_HEIGHT / 2.0) + (d->line_height / 2.0))
		* tex_step;
	while (d->screen_y < d->draw_end)
	{
		tex_y = (int)tex_pos;
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= d->tex->height)
			tex_y = d->tex->height - 1;
		color = get_pixel_from_texture(d->tex, d->tex_x, tex_y);
		pixel_put(game->win, screen_x, d->screen_y, color);
		tex_pos += tex_step;
		d->screen_y++;
	}
}

static void	draw_floor(t_window *win, t_draw *d, int screen_x)
{
	const unsigned int	color = rgb_to_int(d->floor_color);

	while (d->screen_y < WINDOW_HEIGHT)
	{
		pixel_put(win, screen_x, d->screen_y, color);
		d->screen_y++;
	}
}
