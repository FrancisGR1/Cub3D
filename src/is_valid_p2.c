/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_valid_p2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:27:15 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/23 09:21:12 by frmiguel         ###   ########.fr       */
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
		else if (!middle_line_valid(fdata, row, row_size))
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

void	substitute_spaces(t_file_data *fdata, t_string *line)
{
	int	i;

	i = 0;
	if (should_extract_textures(fdata) || should_extract_colors(fdata))
		str_trim(line);
	else
	{
		while (line->data[i])
		{
			if (line->data[i] == ' ')
			{
				line->data[i] = '1';
			}
			else if (line->data[i] == '\n')
			{
				line->data[i] = '\0';
				line->size--;
			}
			i++;
		}
	}
}

bool	middle_line_valid(t_file_data *fdata, int current_row, int row_size)
{
	int	first_col;
	int	last_col;
	int	start_idx;
	int	neighbor_size;

	first_col = get_map_value(fdata->rows, current_row, 0);
	last_col = get_map_value(fdata->rows, current_row, row_size - 1);
	if (first_col != 1 || last_col != 1)
		return (false);
	neighbor_size = get_map_row_size(fdata->rows, current_row - 1);
	start_idx = row_size;
	if (neighbor_size < row_size)
		start_idx = neighbor_size;
	neighbor_size = get_map_row_size(fdata->rows, current_row + 1);
	if (neighbor_size < start_idx)
		start_idx = neighbor_size;
	while (start_idx < row_size)
	{
		if (get_map_value(fdata->rows, current_row, start_idx) != 1)
			return (false);
		start_idx++;
	}
	return (true);
}
