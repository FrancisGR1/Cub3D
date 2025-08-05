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

bool extract_texture(t_map *map, t_string *id, t_string *texture_path)
{
	size_t idx;

	LOG_DEBUG("Extracting texture path: %s %s", id->data, texture_path->data);
	if (!is_valid_file_path(texture_path->data) || !is_valid_extension(texture_path->data, TEXTURE_EXTENSION))
	{
		LOG_FATAL("Error: invalid texture file: %s", texture_path->data);
		return (false);
	}
	if      (ft_strncmp(id->data, "NO", id->size) == 0)
		idx = NORTH;
	else if (ft_strncmp(id->data, "SO", id->size) == 0)
		idx = SOUTH;
	else if (ft_strncmp(id->data, "WE", id->size) == 0)
		idx = WEST;
	else if (ft_strncmp(id->data, "EA", id->size) == 0)
		idx = EAST;
	else
	{
		//@QUESTION: isto pode acontecer?
		LOG_WARN("Error: invalid texture identification: %s", id->data);
		return (false);
	}
	if (map->textures[idx] == NULL)
	{
		map->textures[idx] = str_create(texture_path->data);
		LOG_DEBUG("Success: Loaded: %d %s %s", idx, id->data, texture_path->data);
	}
	else
	{
		LOG_WARN("Error: texture already loaded at index: %zu", idx);
		return (false);
	}
	LOG_DEBUG("Success: Texture paths extracted");
	return (true);
}
