#include "cub3d.h"

static bool should_extract_textures(t_map *map);
static bool should_extract_colors(t_map *map);
static bool is_rgb(t_string *line);
static bool is_texture_path(t_string *line);
static bool extract_texture(t_map *map, t_string *id, t_string *texture_path);
static bool rgb_str_is_valid(t_string **colors, int colors_num);
static bool extract_rgb(t_map *map, t_string *id, t_string *colors);
static bool is_valid_map_num(char c);
static bool is_valid_map_player_pos(char c);
static int map_value_from_char(char c);
static bool extract_map_nums(t_map *map, t_string *line);

//@TODO: mudar para libft
int is_valid_file_path(const char *path);
int check_file_path(const char *path);



t_map *extract_map_data(char *file_path)
{
	t_map *map;
	int fd;

	LOG_INFO("Extracting map info");
	if (file_path == NULL)
	{
		LOG_FATAL("Error: file path is NULL");
		return (NULL);
	}
	fd = open(file_path, O_RDONLY);
	if (fd <= 2)
	{
		LOG_ERROR("Error: invalid fd: %d", fd);
		return (NULL);
	}
	//@TODO: mudar a alocação e inicialização para fora, esta função deve receber
	//não alocar o mapa
	map = ft_calloc(sizeof(t_map), 1);
	map->rows = darr_init(sizeof(t_dynamic_array*), MAP_INITIAL_ROWS); 
	map->floor.r = -1;
	map->ceiling.r = -1;
	if (map == NULL)
	{
		LOG_FATAL("Error: failed allocation for map");
		return (map);

	}
	while (true)
	{
		//@TODO: fazer uma gnl que devolva t_string
		char *l = get_next_line(fd);
		if (l == NULL)
			break ;
		t_string *line = str_create(l);
		free(l);
		str_trim(line);
		if (should_extract_textures(map) || should_extract_colors(map))
		{

			int split_count;
			t_string **split_strs = str_split_using_char_as_delim(line, STRING_WHITESPACE, &split_count);
			//@TODO: se fizer o todo abaixo não preciso disto
			if (split_count != 2)
			{
				LOG_ERROR("Error: Wrong format: %zu strings instead of 2", split_strs);
				str_array_deallocate(split_strs, split_count);
				str_deallocate(line);
				return (map);

			}
			if (is_texture_path(line))
			{
				//@TODO: passar split_count em vez de strs[0] e [1]
				if (!extract_texture(map, split_strs[0], split_strs[1]))
				{
					LOG_ERROR("Error: texture extraction failed. Line: %s", line->data);
					//@IDEA: em vez de repetir estas 3 linhas, posso usar uma variável booleana "error" e retornar no fim
					str_array_deallocate(split_strs, split_count);
					str_deallocate(line);
					return (map);
				}
			}
			else if (is_rgb(line))
			{
				if (!extract_rgb(map, split_strs[0], split_strs[1]))
				{
					LOG_ERROR("Error: color extraction failed. Line: %s", line);
					str_array_deallocate(split_strs, split_count);
					str_deallocate(line);
					return (map);
				}
			}
			else
			{
				LOG_ERROR("Error: is it a texture or color? Can't recognize this: %s", line);
				str_array_deallocate(split_strs, split_count);
				str_deallocate(line);
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
				return (map);
			}
		}
		str_deallocate(line);
	}
	close(fd);
	LOG_INFO("Success: Map info extracted");
	LOG_DEBUG("Textures:\n\t%s\n\t%s\n\t%s\n\t%s", map->textures[0]->data, map->textures[1]->data, map->textures[2]->data, map->textures[3]->data);
	LOG_DEBUG("RGB: Ceiling: %d; Floor: %d", map->ceiling, map->floor);
	LOG_DEBUG_MAP_NUMS(map);
	return (map);
}

void cleanup_map(t_map *map)
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
		LOG_TRACE("Cleaning texture: %s", map->textures[i]->data);
		if (map->textures[i] != NULL)
			str_deallocate(map->textures[i]);
		else
			LOG_WARN("Tried to clean up null ptr");
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

static bool is_texture_path(t_string *line)
{
	return (str_starts_with(line, "NO ")
			|| str_starts_with(line, "SO ")
			|| str_starts_with(line, "WE ")
			|| str_starts_with(line, "EA "));
}

static bool is_rgb(t_string *line)
{
	return (str_starts_with(line, "F ")
			|| str_starts_with(line, "C "));
}


static bool extract_texture(t_map *map, t_string *id, t_string *texture_path)
{
	size_t idx;

	LOG_DEBUG("Extracting texture path: %s %s", id->data, texture_path->data);
	if (check_file_path(texture_path->data) != 1)
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
		LOG_DEBUG("Success: Loaded: %s %s", id->data, texture_path->data);
	}
	else
	{
		LOG_WARN("Error: texture already loaded at index: %zu", idx);
		return (false);
	}
	LOG_INFO("Success: Texture paths extracted");
	return (true);
}

static bool rgb_str_is_valid(t_string **colors, int colors_num)
{
	int idx;
	int rgb_value;

	idx = 0;
	while (idx < colors_num)
	{

		if (!str_is_digit(colors[idx]))
		{
			LOG_ERROR("Error: RGB value is not a digit: %s", colors[idx]->data);
			return (false);
		}
		rgb_value = str_to_int(colors[idx]);
		if (!(rgb_value >= 0 && rgb_value <= 255))
		{
			LOG_ERROR("Error: RGB value is out of bounds: %d", rgb_value);
			return (false);
		}
		idx++;
	}
	return (true);
}

static bool extract_rgb(t_map *map, t_string *id, t_string *colors)
{
	t_rgb *rgb_ptr;
	t_string **split_colors;
	int split_nums;

	LOG_DEBUG("Extracting RGBs: %s %s", id->data, colors->data);
	if (ft_strncmp(id->data, "C", id->size) == 0)
	{
		rgb_ptr = &map->ceiling;
		LOG_DEBUG("Loading: Ceiling");
	}
	else if (ft_strncmp(id->data, "F", id->size) == 0)
	{
		rgb_ptr = &map->floor;
		LOG_DEBUG("Loading: Floor");
	}
	else
	{
		LOG_WARN("Error: Don't know what %s is?", id->data);
		return  (false);
	}
	//dividr colors
	split_colors = str_split_using_char_as_delim(colors, ",", &split_nums);
	if (split_colors == NULL)
	{
		LOG_FATAL("Split colors is NULL!");
		str_array_deallocate(split_colors, split_nums);
		return (false);
	}
	if (split_nums != 3)
	{
		LOG_ERROR("Error: expected 3 values, got %d instead", split_nums);
		str_array_deallocate(split_colors, split_nums);
		return (false);
	}
	if (!rgb_str_is_valid(split_colors, split_nums))
	{
		LOG_ERROR("Error: rgb not valid");
		str_array_deallocate(split_colors, split_nums);
		return (false);
	}
	rgb_ptr->r = str_to_int(split_colors[0]);
	rgb_ptr->g = str_to_int(split_colors[1]);
	rgb_ptr->b = str_to_int(split_colors[2]);
	str_array_deallocate(split_colors, split_nums);
	LOG_INFO("Success: RGBs extracted: %d %d %d", rgb_ptr->r, rgb_ptr->g, rgb_ptr->b);
	return (true);
}

static bool is_valid_map_num(char c)
{
	return (c == '0' || c == '1');
}

static bool is_valid_map_player_pos(char c)
{
	return (c == 'N' || c == 'E' || c == 'S' || c == 'W');
}

static int map_value_from_char(char c)
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

int is_valid_file_path(const char *path)
{
	int fd;

	if (!path)
		return (0);
	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		return (0);
	}
	close(fd);
	return (1);
}

// has additional error logging
int check_file_path(const char *path)
{
	int fd;

	if (!path)
	{
		ft_fprintf(STDERR, "Error: NULL path provided\n");
		return (0);
	}

	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		perror("File open failed");

		ft_fprintf(STDERR, "Error opening file '%s': %s\n", path, strerror(errno));
		return (0);
	}

	close(fd);
	ft_fprintf(STDERR, "File '%s' is valid and readable\n", path);
	return (1);
}
