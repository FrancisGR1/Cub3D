#include "cub3d.h"

t_player *alloc_init_player(t_arena *game_memory)
{
	t_player *player;

	LOG_DEBUG("Allocating and Initializing player");
	player = arena_alloc(&game_memory, sizeof(t_player), 1);
	player->position = make_vec2d(0, 0);
	player->direction = make_vec2d(0, 0);
	player->plane = make_vec2d(0, 0);
	player->fov = DEFAULT_FOV;
	LOG_TRACE("Success: player struct allocated/initialized");
	return (player);
}

void set_player_position(t_player *player, double x, double y)
{
	LOG_DEBUG("Setting player position");
	player->position = make_vec2d(x, y);
	LOG_TRACE("Success: player position set: %f %f", x, y);
}

void set_player_direction(t_player *player, size_t direction)
{

	LOG_DEBUG("Setting player direction");
	if (direction == 'N')
	{
		player->direction = make_vec2d(0.0, -1.0);
		player->plane = make_vec2d(0.66, 0.0);
	}
	else if (direction == 'S')
	{
		player->direction = make_vec2d(0.0, 1.0);
		player->plane = make_vec2d(-0.66, 0.0);
	}
	else if (direction == 'E')
	{
		player->direction = make_vec2d(1.0, 0.0);
		player->plane = make_vec2d(0.0, 0.66);
	}
	else if (direction == 'W')
	{
		player->direction = make_vec2d(-1.0, 0.0);
		player->plane = make_vec2d(0.0, -0.66);
	}
	else
	{
		LOG_ERROR("Don't know what this direction is: %zu", direction);
	}
	LOG_TRACE("Success: player direction set: %c", direction);
}
