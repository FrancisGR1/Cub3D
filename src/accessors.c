/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accessors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:33:54 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/13 17:33:54 by frmiguel         ###   ########.fr       */
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

void	set_map_value(t_dynamic_array *map, int value, int row, int col)
{
	((int *)(((t_dynamic_array **)map->data)[row]->data))[col] = value;
}

int	get_map_size(t_dynamic_array *map)
{
	return (map->len);
}

int	get_map_row_size(t_dynamic_array *map, int row)
{
	return (((t_dynamic_array **)map->data)[row]->len);
}
