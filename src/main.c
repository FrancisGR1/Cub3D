/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 19:09:56 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/27 18:20:49 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	main(int argc, char **argv)
{
	t_file_data	*extracted_data;
	t_game		*game;

	if (!is_valid_input(argc, argv))
	{
		ft_fprintf(STDERR, "Error: Invalid Input\n");
		return (1);
	}
	extracted_data = alloc_init_extracted_data();
	extract_file_data(argv[1], extracted_data);
	//printf("before:\n");
	//LOG_DEBUG_JAGGED_MAP_NUMS(extracted_data);
	if (extracted_data->parser_error == false)
	{
		//@TODO: esta função tem de mudar de nome
		//não faz o que está a dizer
		validate_and_replace(extracted_data);
	}
	if (extracted_data->parser_error)
	{
		ft_fprintf(STDERR, "Error\n");
		cleanup_extracted_data(extracted_data);
		return (1);
	}
	game = alloc_init_game(extracted_data);
	setup_hooks(game->win, game);
	mlx_loop(game->win->mlx);
	return (0);
}
