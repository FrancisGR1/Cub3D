/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 19:09:56 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/28 11:47:27 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	main(int argc, char **argv)
{
	t_file_data	*extracted_data;
	t_game		*game;

	if (!is_valid_input(argc, argv))
	{
		ft_fprintf(STDERR, "Error: Invalid Input: ./Cub3D <file>.cub\n");
		return (1);
	}
	extracted_data = alloc_init_extracted_data();
	extract_file_data(argv[1], extracted_data);
	validate_and_replace(extracted_data);
	if (extracted_data->parser_error)
	{
		print_error(extracted_data->parser_error);
		cleanup_extracted_data(extracted_data);
		return (1);
	}
	game = alloc_init_game(extracted_data);
	setup_hooks(game->win, game);
	mlx_loop(game->win->mlx);
	return (0);
}
