#include "cub3d.h"

bool is_valid_map_num(char c)
{
	return (c == '0' || c == '1');
}

bool is_valid_map_player_pos(char c)
{
	return (c == 'N' || c == 'E' || c == 'S' || c == 'W');
}

bool is_valid_texture_path(t_string *line)
{
	return (str_starts_with(line, "NO ")
			|| str_starts_with(line, "SO ")
			|| str_starts_with(line, "WE ")
			|| str_starts_with(line, "EA "));
}
