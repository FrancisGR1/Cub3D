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

void	validate_and_replace(t_file_data *fdata)
{
	size_t	row;
	size_t	rows;
	size_t	col;
	size_t	cols;
	int incubation_arr[INCUBATION_ROWS][INCUBATION_COLS];

	if (fdata == NULL || fdata->rows == NULL || fdata->parser_error == true)
	{
		return ;
	}
	if (break_in_map_from_outside(fdata, incubation_arr))
	{
		fdata->parser_error = true;
		return ;
	}
	rows = get_map_size(fdata->rows);
	row = 0;
	while (row < rows)
	{
		col = 0;
		cols = get_map_row_size(fdata->rows, row);
		while (col < cols)
		{
			if (incubation_arr[row + 1][col + 1] == ' ')
				set_map_value(fdata->rows, 0, row, col);
			col++;
		}
		row++;
	}
	//agora o mapa é válido,
	//substituímos os espaços por 0s
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

//@TODO: isto é um crime de guerra! tem de ser removido o mais depressa possível!
void	substitute_spaces(t_file_data *fdata, t_string *line)
{
	size_t i;

	if (should_extract_textures(fdata) || should_extract_colors(fdata))
	{
		str_trim(line);
	}
	else
	{
		i = 0;
		while (line->data[i])
		{
			if (line->data[i] == '\n')
			{
				line->data[i] = '\0';
				line->size--;
			}
			i++;
		}
	}
}

//@REFACTOR: remover
bool	middle_line_valid(t_file_data *fdata, int current_row, int row_size)
{
	//int	first_col;
	//int	last_col;
	//int	start_idx;
	//int	neighbor_size;

	//first_col = get_map_value(fdata->rows, current_row, 0);
	//last_col = get_map_value(fdata->rows, current_row, row_size - 1);
	//if (first_col != 1 || last_col != 1)
	//	return (false);
	//neighbor_size = get_map_row_size(fdata->rows, current_row - 1);
	//start_idx = row_size;
	//if (neighbor_size < row_size)
	//	start_idx = neighbor_size;
	//neighbor_size = get_map_row_size(fdata->rows, current_row + 1);
	//if (neighbor_size < start_idx)
	//	start_idx = neighbor_size;
	//while (start_idx < row_size)
	//{
	//	if (get_map_value(fdata->rows, current_row, start_idx) != 1)
	//		return (false);
	//	start_idx++;
	//}
	return (true);
}

bool digit_or_space(char c)
{
	return (ft_isdigit(c) || c == ' ');
}
