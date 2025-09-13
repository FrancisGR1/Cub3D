/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_valid_p2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:27:15 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/13 19:32:10 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	check_if_map_nums_are_valid(t_file_data *fdata)
{
	size_t	row;
	size_t	row_size;
	size_t	col;

	if (fdata == NULL || fdata->rows == NULL || fdata->parser_error == true)
		return ;
	row = 0;
	while (!fdata->parser_error && row < fdata->rows->len)
	{
		row_size = get_map_row_size(fdata->rows, row);
		if (row == 0 || row == fdata->rows->len - 1)
		{
			col = 0;
			while (!fdata->parser_error && col < row_size)
			{
				if (get_map_value(fdata->rows, row, col) != 1)
					fdata->parser_error = true;
				col++;
			}
		}
		else if (get_map_value(fdata->rows, row, row_size - 1) != 1
			|| get_map_value(fdata->rows, row, 0) != 1)
			fdata->parser_error = true;
		row++;
	}
}

bool	is_valid_input(int argc, char **argv)
{
	if (argc != 2)
	{
		return (false);
	}
	if (!is_valid_file_path(argv[1]))
	{
		return (false);
	}
	if (!is_valid_extension(argv[1], MAP_DATA_EXTENSION))
	{
		return (false);
	}
	return (true);
}
