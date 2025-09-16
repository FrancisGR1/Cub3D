/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 23:28:49 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/14 22:43:07 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	game_loop(t_game *game)
{
	update_player_dir(game->player);
	update_player_pos(game->map, game->player);
	render(game);
	return (0);
}

void	setup_hooks(t_window *win, t_game *game)
{
	mlx_hook(win->mlx_win, 2, 1L << 0, key_press, game);
	mlx_hook(win->mlx_win, 3, 1L << 1, key_release, game);
	mlx_hook(win->mlx_win, 17, 0L, end_game, game);
	mlx_loop_hook(win->mlx, game_loop, game);
}
