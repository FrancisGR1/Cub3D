/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 19:48:01 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/14 21:28:30 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

t_vec2d	mult_vector(t_vec2d vector, double mult)
{
	return ((t_vec2d){.x = vector.x * mult, .y = vector.y * mult});
}

void	rotate_vector(t_vec2d *vector, double angle)
{
	t_vec2d	tmp;

	tmp = (t_vec2d){.x = vector->x, .y = vector->y};
	vector->x = tmp.x * cos(angle) - tmp.y * sin(angle);
	vector->y = tmp.x * sin(angle) + tmp.y * cos(angle);
}

t_vec2d	sum_vector(t_vec2d a, t_vec2d b)
{
	return ((t_vec2d){.x = a.x + b.x, .y = a.y + b.y});
}
