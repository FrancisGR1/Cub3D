/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 23:19:09 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/27 17:29:18 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_game	*alloc_init_game(t_file_data *map)
{
	t_game	*game;
	t_arena	*game_memory;

	game_memory = arena_init(GAME_DATA_BUFFER);
	game = arena_alloc(&game_memory, sizeof(t_game), 1);
	game->game_memory = game_memory;
	game->extracted_data = map;
	game->win = alloc_init_window(game->game_memory);
	game->player = alloc_init_player(game->game_memory);
	game->draw_info = alloc_draw_info(game->game_memory, map);
	game->show_minimap = false;
	game->ray = alloc_ray(game->game_memory);
	dynamic_array_to_fixed_array(game, map);
	if (!load_textures(game))
	{
		ft_fprintf(STDERR, "Error: Faulty texture\n");
		end_game(game);
	}
	return (game);
}

int	end_game(t_game *game)
{
	cleanup_textures(game->win, game->textures);
	mlx_destroy_image(game->win->mlx, game->win->img);
	mlx_destroy_window(game->win->mlx, game->win->mlx_win);
	mlx_destroy_display(game->win->mlx);
	free(game->win->mlx);
	cleanup_extracted_data(game->extracted_data);
	arena_destroy(game->game_memory);
	exit(0);
}
