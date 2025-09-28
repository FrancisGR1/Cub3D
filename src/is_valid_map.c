/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_valid_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 10:44:12 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/28 10:44:12 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	break_in_map_from_outside(t_file_data *fdata,
				int incubation_arr[INCUBATION_ROWS][INCUBATION_COLS]);
static int	flood_fill(int arr[INCUBATION_ROWS][INCUBATION_COLS], int x, int y,
				int *spaces);

void	validate_and_replace(t_file_data *fdata)
{
	size_t	rows;
	size_t	cols;
	int		col;
	int		row;
	int		incubation_arr[INCUBATION_ROWS][INCUBATION_COLS];

	if (fdata == NULL || fdata->rows == NULL || fdata->parser_error)
		return ;
	if (break_in_map_from_outside(fdata, incubation_arr))
	{
		fdata->parser_error = MAP_IS_OPEN;
		return ;
	}
	rows = get_map_size(fdata->rows);
	row = -1;
	while (++row < (int)rows)
	{
		col = -1;
		cols = get_map_row_size(fdata->rows, row);
		while (++col < (int)cols)
		{
			if (incubation_arr[row + 1][col + 1] == ' ')
				set_map_value(fdata->rows, 0, row, col);
		}
	}
}

static int	break_in_map_from_outside(t_file_data *fdata,
		int incubation_arr[INCUBATION_ROWS][INCUBATION_COLS])
{
	int	i;
	int	j;
	int	expected_marked_cells;
	int	marked_cells;
	int	spaces;

	i = -1;
	while (++i < INCUBATION_ROWS)
	{
		j = -1;
		while (++j < INCUBATION_COLS)
			incubation_arr[i][j] = -1;
	}
	i = 0;
	while (++i < get_map_size(fdata->rows) + 1)
	{
		j = 0;
		while (++j < get_map_row_size(fdata->rows, i - 1) + 1)
			incubation_arr[i][j] = get_map_value(fdata->rows, i - 1, j - 1);
	}
	spaces = 0;
	expected_marked_cells = INCUBATION_ROWS * INCUBATION_COLS
		- get_map_total_cells(fdata->rows);
	marked_cells = flood_fill(incubation_arr, 0, 0, &spaces) - spaces;
	return (marked_cells != expected_marked_cells);
}

static int	flood_fill(int arr[INCUBATION_ROWS][INCUBATION_COLS], int x, int y,
		int *spaces)
{
	int	marked_cells;

	if (x < 0 || x >= INCUBATION_COLS || y < 0 || y >= INCUBATION_ROWS)
		return (0);
	if (arr[y][x] == 1 || arr[y][x] == 2)
		return (0);
	if (arr[y][x] == ' ')
		(*spaces)++;
	arr[y][x] = 2;
	marked_cells = 1;
	marked_cells += flood_fill(arr, x + 1, y, spaces);
	marked_cells += flood_fill(arr, x - 1, y, spaces);
	marked_cells += flood_fill(arr, x, y + 1, spaces);
	marked_cells += flood_fill(arr, x, y - 1, spaces);
	return (marked_cells);
}
