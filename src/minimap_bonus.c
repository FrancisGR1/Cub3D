/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 19:12:58 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/15 22:05:20 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_square(t_window *win, int start_x, int start_y, int color)
{
	int	x;
	int	y;
	int	size;

	y = start_y;
	size = MINIMAP_SCALE / 2;
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

void	draw_minimap(t_game *game)
{
	t_minimap	m;

	if (!BONUS)
		return ;
	m.y = 0;
	while (game->map[m.y][0] != -1)
	{
		m.x = 0;
		while (game->map[m.y][m.x] != -1)
		{
			if (game->map[m.y][m.x] == 1)
				m.color = 0x000000;
			else
				m.color = 0xFFFFFF;
			draw_square(game->win, m.x * MINIMAP_SCALE, m.y * MINIMAP_SCALE,
				m.color);
			m.x++;
		}
		m.y++;
	}
	m.x = (int)(game->player->pos.x * MINIMAP_SCALE);
	m.y = (int)(game->player->pos.y * MINIMAP_SCALE);
	draw_square(game->win, m.x - m.size / 2, m.y - m.size / 2, 0xFF0000);
}
