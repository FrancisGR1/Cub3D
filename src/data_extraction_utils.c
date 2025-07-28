#include "cub3d.h"

int map_value_from_char(char c)
{
	LOG_TRACE("Converting map char to int");
	if (is_valid_map_num(c))
	{
		LOG_TRACE("Num: %c -> %zu", c, c - 48);
		return (c - 48);
	}
	else if (is_valid_map_player_pos(c))
	{
		LOG_TRACE("Player position: %c -> %zu", c, c);
		return (c);
	}
	else //@NOTE: em princípio nunca chega aquie
	{
		LOG_ERROR("This is not valid:", c);
		return (-1);
	}
}
