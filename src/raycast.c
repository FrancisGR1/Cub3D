#include "cub3d.h"

t_ray *alloc_init_raycast(t_arena *game_memory)
{
	t_ray *r;

	LOG_DEBUG("Allocating and Initializing raycast");
	r = arena_alloc(&game_memory, sizeof(t_ray), 1);
	r->direction = make_vec2d(0, 0);
	r->camera_x = 0;
	r->map_position = make_vec2i(0, 0);
	r->side_dist = make_vec2d(0, 0);
	r->delta_dist = make_vec2d(0, 0);
	r->step = make_vec2i(0, 0);
	r->perp_wall_dist = 0;
	r->side = 0;
	r->hit_wall = false;
	LOG_TRACE("Success: raycast struct allocated/initialized");
	return (r);
}
