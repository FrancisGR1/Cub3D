/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_valid_p2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:27:15 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/27 19:20:52 by frmiguel         ###   ########.fr       */
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

	if (fdata == NULL || fdata->rows == NULL || fdata->parser_error)
	{
		return ;
	}
	if (break_in_map_from_outside(fdata, incubation_arr))
	{
		fdata->parser_error = MAP_IS_OPEN;
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

void print_error(enum e_parser_errors e)
{
	if (e == MORE_THAN_TWO_LINES)
		ft_fprintf(STDERR, "Error: more than 2 split strings\n");
	else if (e == INVALID_TEXTURE)
		ft_fprintf(STDERR, "Error: invalid texture\n");
	else if (e == INVALID_RGB)
		ft_fprintf(STDERR, "Error: invalid rgb\n");
	else if (e == EMPTY_LINE_IN_MAP)
		ft_fprintf(STDERR, "Error: found an empty line while parsing the map\n");
	else if (e == INVALID_MAP_CHARACTER)
		ft_fprintf(STDERR, "Error: unknown map character\n");
	else if (e == MISSING_PLAYER_POSITION)
		ft_fprintf(STDERR, "Error: no player position\n");
	else if (e == MAP_IS_OPEN)
		ft_fprintf(STDERR, "Error: map is not surrounded by walls\n");
	else
		ft_fprintf(STDERR, "Error: wrong data\n");
}
