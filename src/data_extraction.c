#include "cub3d.h"

static bool should_extract_textures(t_map *map);
static bool should_extract_colors(t_map *map);
static bool extract_map_nums(t_map *map, t_string *line);
//static void extract_map_data_texture_or_color(t_map *map, t_string *line);

t_map *extract_map_data(int fd, t_map *map)
{
	int debug_idx = 0;
	t_string *line;

	LOG_INFO("Extracting map info");
	line = get_next_line_to_str(fd);
	while (line != NULL)
	{
		LOG_DEBUG(BLU "Extracting from line: %d: %s" RESET, debug_idx++, line->data);
		str_trim(line);
		if (line->size == 0)
		{
			if (map->extraction_phase != TEXTURES_AND_COLORS)
			{
				LOG_ERROR("Error: found an empty line in MAP_VALUES phase");
				map->parser_error = true;
			}
			else
			{
				//@NOTE: se o str for vazio na fase de
				//texturas/cores, não fazemos nada
			}
		}
		else if (should_extract_textures(map) || should_extract_colors(map))
		{

			int split_count;
			t_string **split_strs = str_split_using_char_as_delim(line, STRING_WHITESPACE, &split_count);
			if (split_count != 2)
			{
				LOG_ERROR("Error: Wrong format: %zu strings instead of 2", split_count);
				map->parser_error = true;

			}
			else if (starts_with_texture_id(line))
			{
				if (!extract_texture(map, split_strs[0], split_strs[1]))
				{
					LOG_ERROR("Error: texture extraction failed. Line: %s", line->data);
					map->parser_error = true;
				}
			}
			else if (is_rgb_id(line))
			{
				if (!extract_rgb(map, split_strs[0], split_strs[1]))
				{
					LOG_ERROR("Error: color extraction failed. Line: %s", line->data);
					map->parser_error = true;
				}
			}
			else
			{
				LOG_ERROR("Error: what is this?: %s", line->data);
				map->parser_error = true;
			}
			str_array_deallocate(split_strs, split_count);

		}
		else
		{
			map->extraction_phase = MAP_VALUES;
			if (!extract_map_nums(map, line))
			{
				LOG_ERROR("Error: something went wrong with map numbers extraction");
				map->parser_error = true;
			}
		}
		str_deallocate(line);
		if (map->parser_error)
			break ;
		line = get_next_line_to_str(fd);
	}
	map->extraction_phase = EXTRACTION_FINISHED;
	if (map->player_position_is_set == false && map->parser_error == false)
	{
		LOG_ERROR("Error: Player position not set");
		map->parser_error = true;
	}
	if (map->parser_error == false)
	{
		LOG_INFO("Success: Map info extracted");
		LOG_DEBUG("Textures:\n\t%s\n\t%s\n\t%s\n\t%s", map->textures[0]->data, map->textures[1]->data, map->textures[2]->data, map->textures[3]->data);
		LOG_DEBUG("RGB: Ceiling: %d; Floor: %d", map->ceiling, map->floor);
		LOG_DEBUG_MAP_NUMS(map);
	}
	return (map);
}

//@NOTE: extract_map_data() version for norminette
//t_map *extract_map_data(int fd, t_map *map)
//{
//	t_string *line;
//
//	while (!map->parser_error)
//	{
//		line = get_next_line_to_str(fd);
//		if (line == NULL)
//			break ;
//		str_trim(line);
//		if (line->size == 0)
//		{
//			if (map->rows->len != 0)
//				map->parser_error = true;
//		}
//		else if (should_extract_textures(map) || should_extract_colors(map))
//			extract_map_data_texture_or_color(map, line);
//		else
//			if (!extract_map_nums(map, line))
//				map->parser_error = true;
//		str_deallocate(line);
//	}
//	if (map->player_position_is_set == false)
//		map->parser_error = true;
//	return (map);
//}
//
//static void extract_map_data_texture_or_color(t_map *map, t_string *line)
//{
//	t_string **split_strs;
//	int split_count;
//
//	split_strs = str_split_using_char_as_delim(line, STRING_WHITESPACE, &split_count);
//	if (split_count != 2)
//	{
//		LOG_ERROR("Error: Wrong format: %zu strings instead of 2", split_count);
//		map->parser_error = true;
//
//	}
//	else if (starts_with_texture_id(line))
//	{
//		if (!extract_texture(map, split_strs[0], split_strs[1]))
//		{
//			LOG_ERROR("Error: texture extraction failed. Line: %s", line->data);
//			map->parser_error = true;
//		}
//	}
//	else if (is_rgb_id(line))
//	{
//		if (!extract_rgb(map, split_strs[0], split_strs[1]))
//		{
//			LOG_ERROR("Error: color extraction failed. Line: %s", line->data);
//			map->parser_error = true;
//		}
//	}
//	else
//	{
//		LOG_ERROR("Error: what is this?: %s", line->data);
//		map->parser_error = true;
//	}
//	str_array_deallocate(split_strs, split_count);
//}

static bool should_extract_colors(t_map *map)
{
	if (map->floor.r == -1)
	{
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

static bool extract_map_nums(t_map *map, t_string *line)
{
	t_dynamic_array *cols;
	char c;
	int value;
	size_t i;

	LOG_DEBUG("Extracting map numbers from line: %s", line->data);
	str_erase_if(line, ft_isspace, 0);
	LOG_TRACE("After erasing whitespaces:        %s", line->data);
	if (map->player_position_is_set == true && !str_is_digit(line))
	{
		LOG_ERROR("Error: string is not a digit");
		return (false);
	}
	if (get_map_size(map->rows) == MAX_ROWS)
	{
		LOG_ERROR("Error: map rows at max rows: %zu", MAX_ROWS);
		return (false);
	}
	//@ASSUMPTION: os espaços à esq., dir. e meio foram eliminados
	if (line->size > MAX_COLS)
	{
		LOG_ERROR("Error: line size larger than max cols: %zu > %zu", line->size, MAX_COLS);
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
	LOG_DEBUG("Success: extracted map numbers from line into array at %zu size", map->rows->len);
	return (true);
}
