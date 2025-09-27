/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 19:09:56 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/15 22:04:14 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	main(int argc, char **argv)
{
	t_file_data	*extracted_data;
	t_game		*game;

	if (!is_valid_input(argc, argv))
	{
		ft_fprintf(STDERR, "Error\n");
		return (1);
	}
	extracted_data = alloc_init_extracted_data();
	if (extracted_data == NULL)
		return (1);
	extract_file_data(argv[1], extracted_data);
		printf("Parser error at 1st phase: %s", extracted_data->parser_error ? "YES\n" : "no\n");
	if (extracted_data->parser_error == false)
	{
		//@TODO: esta função tem de mudar de nome
		//não faz o que está a dizer
		validate_and_replace(extracted_data);
		printf("Parser error at 2nd phase: %s", extracted_data->parser_error ? "YES\n" : "no\n");
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
