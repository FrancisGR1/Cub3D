/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 18:56:05 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/27 18:23:08 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	pixel_put(t_window *win, int x, int y, int color)
{
	char	*dest;

	if (!win || !win->addr)
		return ;
	if (x < 0 || x >= win->width || y < 0 || y >= win->height)
		return ;
	dest = win->addr + (y * win->line_length + x * (win->bits_per_pixel / 8));
	*(unsigned int *)dest = color;
}

void	render(t_game *game)
{
	t_window	*win;
	t_ray		*ray;
	t_player	*player;
	int			screen_x;

	win = game->win;
	ray = game->ray;
	player = game->player;
	screen_x = 0;
	while (screen_x < WINDOW_WIDTH)
	{
		set_ray_info(ray, player, screen_x);
		dda_loop(ray, game->map);
		fix_fish_eye_effect(ray, player);
		set_draw_info(game->draw_info, ray, player, game->textures);
		draw_vertical_line(game, game->draw_info, screen_x);
		screen_x++;
	}
	draw_minimap(game);
	mlx_put_image_to_window(win->mlx, win->mlx_win, win->img, 0, 0);
}
