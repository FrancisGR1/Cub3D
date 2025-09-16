/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 19:44:58 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/14 21:38:24 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H

# include <math.h>

typedef struct s_vec2f
{
	float	x;
	float	y;
}			t_vec2f;

typedef struct s_vec2i
{
	int		x;
	int		y;
}			t_vec2i;

typedef struct s_vec2d
{
	double	x;
	double	y;
}			t_vec2d;

// creation
t_vec2f		make_vec2f(float x, float y);
t_vec2i		make_vec2i(int x, int y);
t_vec2d		make_vec2d(double x, double y);

// operations
t_vec2d		mult_vector(t_vec2d vector, double mult);
t_vec2d		sum_vector(t_vec2d a, t_vec2d b);
void		rotate_vector(t_vec2d *vector, double angle);

#endif /* VECTOR_H */
