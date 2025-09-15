#include "vector.h"

t_vec2f make_vec2f(float x, float y)
{
	t_vec2f vec;

	vec.x = x;
	vec.y = y;
	return (vec);
}

t_vec2i make_vec2i(int x, int y)
{
	t_vec2i vec;

	vec.x = x;
	vec.y = y;
	return (vec);
}

t_vec2d make_vec2d(double x, double y)
{
	t_vec2d vec;

	vec.x = x;
	vec.y = y;
	return (vec);
}

t_vec2d	mult_vector(t_vec2d *vector, double mult)
{
	return ((t_vec2d){.x = vector->x * mult, .y = vector->y * mult});
}

void	rotate_vector(t_vec2d *vector, double angle)
{
	t_vec2d tmp;

	tmp = (t_vec2d){.x = vector->x, .y = vector->y};
	vector->x = tmp.x * cos(angle) - tmp.y * sin(angle);
	vector->y = tmp.x * sin(angle) + tmp.y * cos(angle);
}

void	sum_vector(t_vec2d *dst, t_vec2d *src)
{
	dst->x += src->x;
	dst->y += src->y;
}
