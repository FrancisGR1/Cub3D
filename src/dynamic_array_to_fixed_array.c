/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_map.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 23:19:09 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/13 23:21:47 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	get_largest_row(t_dynamic_array *jmap);
static void	dynamic_array_to_fixed_array_inner_loop(t_game *game, int *row,\
		int *col, int rows);

void	dynamic_array_to_fixed_array(t_game *game, t_file_data *map)
{
	int	largest_row;
	int	rows;
	int	row;
	int	col;

	largest_row = 0;
	rows = get_map_size(map->rows);
	row = 0;
	largest_row = get_largest_row(map->rows);
	dynamic_array_to_fixed_array_inner_loop(game, &row, &col, rows);
	if (row < MAX_ROWS)
	{
		col = 0;
		while (col < largest_row)
		{
			game->map[row][col] = ROW_END;
			col++;
		}
	}
}

static int	get_largest_row(t_dynamic_array *jmap_rows)
{
	int	rows;
	int	row;
	int	row_size;
	int	largest_row;

	largest_row = 0;
	rows = get_map_size(jmap_rows);
	row = 0;
	while (row < rows)
	{
		row_size = get_map_row_size(jmap_rows, row);
		if (largest_row < row_size)
			largest_row = row_size;
		row++;
	}
	return (largest_row);
}

static void	dynamic_array_to_fixed_array_inner_loop(t_game *game, int *row,\
		int *col, int rows)
{
	int	row_size;

	while (*row < rows)
	{
		*col = 0;
		row_size = get_map_row_size(game->extracted_data->rows, *row);
		while (*col < row_size)
		{
			game->map[*row][*col] = get_map_value(game->extracted_data->rows,
					*row, *col);
			if (game->map[*row][*col] != 0 && game->map[*row][*col] != 1
					&& game->map[*row][*col] != 32)
			{
				set_player_position(game->player, *col, *row);
				set_player_direction(game->player, game->map[*row][*col]);
				game->map[*row][*col] = 0;
			}
			(*col)++;
		}
		if (*col < MAX_COLS)
			game->map[*row][*col] = ROW_END;
		(*row)++;
	}
}
