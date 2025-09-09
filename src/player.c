#include "cub3d.h"

t_player *alloc_init_player(t_arena *game_memory)
{
	t_player *p;

	LOG_DEBUG("Allocating and Initializing player");
	p = arena_alloc(&game_memory, sizeof(t_player), 1);
	//@TODO: mudar para posições dadas pela data_extraction
	p->pos = make_vec2d(0, 0);
	p->dir = make_vec2d(0, 0);
	p->plane = make_vec2d(0, 0);
	p->fov = DEFAULT_FOV;

	p->move_speed = 0.3;
	p->rot_speed = 0.1;
	p->collision_buffer = 0.1;
	p->move_up = false;
	p->move_down = false;
	p->move_left = false;
	p->move_right = false;
	p->rot_left = false;
	p->rot_right = false;
	LOG_TRACE("Success: player struct allocated/initialized");
	return (p);
}

void set_player_position(t_player *player, double x, double y)
{
	LOG_DEBUG("Setting player position");
	player->pos = make_vec2d(x, y);
	LOG_TRACE("Success: player position set: %f %f", x, y);
}

void set_player_direction(t_player *player, size_t direction)
{

	LOG_DEBUG("Setting player direction");
	if (direction == 'N')
	{
		player->dir = make_vec2d(0.0, -1.0);
		player->plane = make_vec2d(0.66, 0.0);
	}
	else if (direction == 'S')
	{
		player->dir = make_vec2d(0.0, 1.0);
		player->plane = make_vec2d(-0.66, 0.0);
	}
	else if (direction == 'E')
	{
		player->dir = make_vec2d(1.0, 0.0);
		player->plane = make_vec2d(0.0, 0.66);
	}
	else if (direction == 'W')
	{
		player->dir = make_vec2d(-1.0, 0.0);
		player->plane = make_vec2d(0.0, -0.66);
	}
	else
	{
		LOG_ERROR("Don't know what this direction is: %zu", direction);
	}
	LOG_TRACE("Success: player direction set: %c", direction);
}
