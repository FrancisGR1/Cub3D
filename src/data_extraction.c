#include "cub3d.h"

static bool extract_file_data_nums(t_file_data *map, t_string *line);
//static void extract_map_data_texture_or_color(t_file_data *map, t_string *line);

t_file_data *extract_file_data(const char *map_data_file_path, t_file_data *map)
{
	int debug_idx = 0;
	int fd;
	t_string *line;

	LOG_INFO("Extracting map info");
	fd = open(map_data_file_path, O_RDONLY);
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
			if (!extract_file_data_nums(map, line))
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
		LOG_DEBUG_JAGGED_MAP_NUMS(map);
	}
	close(fd);
	return (map);
}

t_file_data *init_extracted_data(void)
{
	t_file_data *map;
	size_t i;

	LOG_TRACE("Allocing map");
	map = ft_calloc(sizeof(t_file_data), 1);
	if (map == NULL)
	{
		LOG_FATAL("Error: failed allocation for map");
		return (NULL);

	}
	map->rows = darr_init(sizeof(t_dynamic_array*), MAP_INITIAL_ROWS); 
	if (map->rows == NULL)
	{
		LOG_FATAL("Error: failed allocation for map rows");
		return (NULL);

	}
	map->player_position_is_set = false;
	map->parser_error = false;
	map->extraction_phase = TEXTURES_AND_COLORS;
	map->floor = (t_rgb){-1, -1, -1};  
	map->ceiling = (t_rgb){-1, -1, -1};
	i = 0;
	while (i < MAX_TEXTURES)
	{
		map->textures[i] = NULL;
		i++;
	}
	LOG_DEBUG("Success: Map initialized");
	return (map);
}

//@NOTE: extract_file_data_data() version for norminette
//@TODO: atualizar func: 
//recebe string do caminho do ficheiro .cub
//gerir ficheiro, open() e close()
//t_file_data *extract_file_data_data(int fd, t_file_data *map)
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
//			extract_file_data_data_texture_or_color(map, line);
//		else
//			if (!extract_file_data_nums(map, line))
//				map->parser_error = true;
//		str_deallocate(line);
//	}
//	if (map->player_position_is_set == false)
//		map->parser_error = true;
//	return (map);
//}
//
//static void extract_file_data_data_texture_or_color(t_file_data *map, t_string *line)
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

//@REFACTOR too long for norminette
static bool extract_file_data_nums(t_file_data *map, t_string *line)
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
	if (line->size > MAX_COLS - 1)
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

void cleanup_extracted_data(t_file_data *map)
{
	size_t i;
	t_dynamic_array *row;

	LOG_DEBUG("Cleaning up map");
	if (map == NULL)
	{
		LOG_ERROR("Map is null");
		return ;
	}
	i = 0;
	while (i < MAX_TEXTURES)
	{
		LOG_TRACE("Looking to clean texture at: %zu\n", i);
		if (map->textures[i] != NULL && map->textures[i]->data != NULL)
		{
			LOG_TRACE("Cleaned texture: %zu: %s", i, map->textures[i]->data);
			str_deallocate(map->textures[i]);
		}
		else
		{
			LOG_TRACE("Skipping null ptr at: %zu", i);
		}
		i++;
	}
	i = 0;
	while (i < map->rows->len)
	{
		LOG_TRACE("Cleaning map row: %zu", i);
		row = ((t_dynamic_array**)map->rows->data)[i];
		if (row != NULL)
			darr_free(row);
		else
			LOG_ERROR("Row %zu is NULL", i);
		i++;
	}
	darr_free(map->rows);
	free(map);
	LOG_INFO("Success: Cleaned up map");
}
