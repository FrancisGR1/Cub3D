#include "cub3d.h"

static bool should_extract_textures(t_map *map);
static bool should_extract_colors(t_map *map);
static bool extract_texture(t_map *map, t_string *id, t_string *texture_path);
static bool extract_map_nums(t_map *map, t_string *line);

//@TODO: o bloco if else precisa de um refactor
t_map *extract_map_data(int fd, t_map *map)
{
	LOG_INFO("Extracting map info");
	while (true)
	{
		//@TODO: fazer uma gnl que devolva t_string
		char *l = get_next_line(fd);
		LOG_DEBUG(BLU "Extracting from line: %s" RESET, l);
		if (l == NULL)
		{
			LOG_DEBUG("Line is null");
			break ;
		}
		t_string *line = str_create(l);
		free(l);
		str_trim(line);
		if (line->size == 0)
		{
			//@FIXME: se estivers a processar o mapa,
			//isto deve dar erro
			str_deallocate(line);
			continue;
		}
		else if (should_extract_textures(map) || should_extract_colors(map))
		{

			int split_count;
			t_string **split_strs = str_split_using_char_as_delim(line, STRING_WHITESPACE, &split_count);
			//@TODO: se fizer o todo abaixo não preciso disto
			if (split_count != 2)
			{
				//@BUG: com este input: 
				//
				LOG_ERROR("Error: Wrong format: %zu strings instead of 2", split_count);
				str_array_deallocate(split_strs, split_count);
				str_deallocate(line);
				map->parse_error = true;
				return (map);

			}
			if (is_valid_texture_path(line))
			{
				//@TODO: passar split_count em vez de strs[0] e [1]
				if (!extract_texture(map, split_strs[0], split_strs[1]))
				{
					LOG_ERROR("Error: texture extraction failed. Line: %s", line->data);
					//@IDEA: em vez de repetir estas 3 linhas, posso usar uma variável booleana "error" e retornar no fim
					str_array_deallocate(split_strs, split_count);
					str_deallocate(line);
					map->parse_error = true;
					return (map);
				}
			}
			else if (is_rgb_id(line))
			{
				if (!extract_rgb(map, split_strs[0], split_strs[1]))
				{
					LOG_ERROR("Error: color extraction failed. Line: %s", line->data);
					str_array_deallocate(split_strs, split_count);
					str_deallocate(line);
					map->parse_error = true;
					return (map);
				}
			}
			else
			{
				LOG_ERROR("Error: what is this?: %s", line->data);
				str_array_deallocate(split_strs, split_count);
				str_deallocate(line);
				map->parse_error = true;
				return (map);
			}
			str_array_deallocate(split_strs, split_count);

		}
		else
		{
			if (!extract_map_nums(map, line))
			{
				LOG_ERROR("Error: something went wrong with map numbers extraction");
				str_deallocate(line);
				map->parse_error = true;
				return (map);
			}
		}
		str_deallocate(line);
	}
	//@TODO: os retornos anteriores também têm de fechar o ficheiro
	LOG_INFO("Success: Map info extracted");
	LOG_DEBUG("Textures:\n\t%s\n\t%s\n\t%s\n\t%s", map->textures[0]->data, map->textures[1]->data, map->textures[2]->data, map->textures[3]->data);
	LOG_DEBUG("RGB: Ceiling: %d; Floor: %d", map->ceiling, map->floor);
	LOG_DEBUG_MAP_NUMS(map);
	return (map);
}

static bool should_extract_colors(t_map *map)
{
	LOG_TRACE("Checking if we need to extract colors");
	if (map->floor.r == -1)
	{
		LOG_DEBUG("Floor color not set");
		return (true);
	}
	if (map->ceiling.r == -1)
	{
		LOG_DEBUG("Ceiling color not set");
		return (true);
	}
	LOG_TRACE("Don't need to extract colors");
	return (false);

}

static bool should_extract_textures(t_map *map)
{
	size_t i;

	LOG_TRACE("Checking if we need to extract textures");
	i = 0;
	while (i < MAX_TEXTURES)
	{
		LOG_TRACE("Checking if texture %zu exists", i);
		if (map->textures[i] == NULL)
		{
			LOG_DEBUG("Texture %zu has not been extracted", i);
			return (true);
		}
		i++;
	}
	LOG_TRACE("Don't need to extract textures");
	return (false);
}

static bool extract_texture(t_map *map, t_string *id, t_string *texture_path)
{
	size_t idx;

	LOG_DEBUG("Extracting texture path: %s %s", id->data, texture_path->data);
	if (is_valid_file_path(texture_path->data) != 1)
	{
		LOG_FATAL("Error: invalid texutre path");
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
	LOG_INFO("Success: Texture paths extracted");
	return (true);
}

static bool extract_map_nums(t_map *map, t_string *line)
{
	t_dynamic_array *cols;
	char c;
	int value;
	size_t i;

	LOG_DEBUG("Extracting map numbers from line: %s", line->data);
	str_replace_all(line," ", "");
	LOG_TRACE("After trimming whitespaces:       %s", line->data);
	if (map->player_position_is_set == true && !str_is_digit(line))
	{
		LOG_ERROR("Error: string is not a digit");
		return (false);
	}
	cols = darr_init(sizeof(int), MAP_INITIAL_COLS); 
	i = 0;
	while (i < line->size)
	{
		c = str_at(line, i);
		if (is_valid_map_num(c) || is_valid_map_player_pos(c))
		{
			LOG_TRACE("Appending: %c of col %zu", c, i); 
			value = map_value_from_char(c);
			if (value != 0 && value != 1)
			{
				map->player_position_is_set = true;
				LOG_DEBUG("Player position is set: %c", c);
			}
			darr_append(cols, &value);
		}
		else
		{
			LOG_ERROR("Error: \"%c\" is not a valid map element", c); 
			darr_free(cols);
			return (false);
		}
		i++;
	}
	darr_append(map->rows, &cols);
	LOG_DEBUG("Success: extracted map numbers from line into array");
	return (true);
}
