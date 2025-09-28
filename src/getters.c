/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accessors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:33:54 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/28 10:44:05 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_col_value(t_dynamic_array *row, int col)
{
	return (((int *)row->data)[col]);
}

int	get_map_value(t_dynamic_array *map, int row, int col)
{
	return (((int *)(((t_dynamic_array **)map->data)[row]->data))[col]);
}

int	get_map_size(t_dynamic_array *map)
{
	return (map->len);
}

int	get_map_row_size(t_dynamic_array *map, int row)
{
	return (((t_dynamic_array **)map->data)[row]->len);
}

int	get_map_total_cells(t_dynamic_array *map)
{
	int	i;
	int	j;
	int	rows;
	int	cols;
	int	total_cells;

	i = 0;
	rows = get_map_size(map);
	total_cells = 0;
	while (i < rows)
	{
		j = 0;
		cols = get_map_row_size(map, i);
		while (j < cols)
		{
			total_cells++;
			j++;
		}
		i++;
	}
	return (total_cells);
}
