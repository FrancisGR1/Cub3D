/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 19:12:58 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/28 10:44:02 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_map_tile(t_window *win, int start_x, int start_y, int color)
{
	int	x;
	int	y;
	int	size;

	if (color == -1)
		return ;
	y = start_y;
	size = MINIMAP_SCALE;
	while (y < start_y + size)
	{
		x = start_x;
		while (x < start_x + size)
		{
			pixel_put(win, x, y, color);
			x++;
		}
		y++;
	}
}

// stackoverflow.com/questions/1201200/fast-algorithm-for-drawing-filled-circles
void	draw_circle(t_window *win, int center_x, int center_y, int color)
{
	int	y;
	int	x;
	int	radius;

	radius = MINIMAP_PLAYER_SCALE;
	y = -radius;
	while (y <= radius)
	{
		x = -radius;
		while (x <= radius)
		{
			if (x * x + y * y <= radius * radius)
				pixel_put(win, center_x + x, center_y + y, color);
			x++;
		}
		y++;
	}
}

void	draw_minimap(t_game *game)
{
	int				y;
	int				x;
	unsigned int	color;

	if (!BONUS && !game->show_minimap)
		return ;
	y = -1;
	while (game->map[++y][0] != -1 && y < MAX_ROWS)
	{
		x = -1;
		while (game->map[y][++x] != -1 && x < MAX_COLS)
		{
			if (game->map[y][x] == 1)
				color = MINIMAP_WALL_TILE_COLOR;
			else if (game->map[y][x] == 0)
				color = MINIMAP_EMPTY_TILE_COLOR;
			else
				color = -1;
			draw_map_tile(game->win, x * MINIMAP_SCALE, y * MINIMAP_SCALE,
				color);
		}
	}
	x = (int)(game->player->pos.x * MINIMAP_SCALE);
	y = (int)(game->player->pos.y * MINIMAP_SCALE);
	draw_circle(game->win, x, y, MINIMAP_PLAYER_COLOR);
}
