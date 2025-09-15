/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 23:37:37 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/14 23:37:37 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_window	*alloc_init_window(t_arena *game_memory)
{
	t_window	*win;

	win = arena_alloc(&game_memory, sizeof(t_window), 1);
	win->mlx = mlx_init();
	win->width = WINDOW_WIDTH;
	win->height = WINDOW_HEIGHT;
	win->mlx_win = mlx_new_window(win->mlx, win->width, win->height, "cub3d");
	win->img = mlx_new_image(win->mlx, win->width, win->height);
	win->addr = mlx_get_data_addr(win->img, &win->bits_per_pixel,
			&win->line_length, &win->endian);
	return (win);
}
