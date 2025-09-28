/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 10:56:26 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/28 10:56:51 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	set_map_value(t_dynamic_array *map, int value, int row, int col)
{
	((int *)(((t_dynamic_array **)map->data)[row]->data))[col] = value;
}
