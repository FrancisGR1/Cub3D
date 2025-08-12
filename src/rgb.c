#include "cub3d.h"

static int count_comas(t_string *colors);

bool is_rgb_id(t_string *line)
{
	return (str_starts_with(line, "F ")
			|| str_starts_with(line, "C "));
}


bool rgb_str_is_valid(t_string **colors, int colors_num)
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

bool extract_rgb(t_file_data *map, t_string *id, t_string *colors)
{
	t_rgb *rgb_ptr;
	t_string **split_colors;
	int split_nums;
	bool extraction_success;

	LOG_DEBUG("Extracting RGBs: %s %s", id->data, colors->data);
	if (ft_strncmp(id->data, "C", id->size) == 0 && map->ceiling.r == -1)
	{
		rgb_ptr = &map->ceiling;
		LOG_DEBUG("Loading: Ceiling");
	}
	else if (ft_strncmp(id->data, "F", id->size) == 0 && map->floor.r == -1)
	{
		rgb_ptr = &map->floor;
		LOG_DEBUG("Loading: Floor");
	}
	else
	{
		LOG_WARN("Error: Don't know what %s is?", id->data);
		return (false);
	}
	split_colors = str_split_using_char_as_delim(colors, ",", &split_nums);

	extraction_success = false;
	if (split_colors == NULL)
	{
		LOG_FATAL("Split colors is NULL!");
	}
	else if (split_nums != 3)
	{
		LOG_ERROR("Error: expected 3 values, got %d instead", split_nums);
	}
	else if (count_comas(colors) != 2)
	{
		LOG_ERROR("Error: invalid number of comas");
	}
	else if (!rgb_str_is_valid(split_colors, split_nums))
	{
		LOG_ERROR("Error: rgb not valid");
	}
	else
	{
		rgb_ptr->r = str_to_int(split_colors[0]);
		rgb_ptr->g = str_to_int(split_colors[1]);
		rgb_ptr->b = str_to_int(split_colors[2]);
		extraction_success = true;
	}
	str_array_deallocate(split_colors, split_nums);
	extraction_success ? LOG_DEBUG("Success: RGBs extracted: %d %d %d", rgb_ptr->r, rgb_ptr->g, rgb_ptr->b) : "";
	return (extraction_success);
}

//@TODO: conversão() RGB -> int

//@NOTE extract_rgb() version for norminette
//bool extract_rgb_new(t_map *map, t_string *id, t_string *colors)
//{
//	t_rgb *rgb_ptr;
//	t_string **split_colors;
//	int split_nums;
//	bool extraction_success;
//
//	if (ft_strncmp(id->data, "C", id->size) == 0 && map->ceiling.r == -1)
//		rgb_ptr = &map->ceiling;
//	else if (ft_strncmp(id->data, "F", id->size) == 0 && map->floor.r == -1)
//		rgb_ptr = &map->floor;
//	else
//		return  (false);
//	split_colors = str_split_using_char_as_delim(colors, ",", &split_nums);
//	if (split_colors == NULL || split_nums != 3 || count_comas(colors) != 2
//			|| !rgb_str_is_valid(split_colors, split_nums))
//		extraction_success = false;
//	else
//	{
//		rgb_ptr->r = str_to_int(split_colors[0]);
//		rgb_ptr->g = str_to_int(split_colors[1]);
//		rgb_ptr->b = str_to_int(split_colors[2]);
//		extraction_success = true;
//	}
//	str_array_deallocate(split_colors, split_nums);
//	return (extraction_success);
//}

static int count_comas(t_string *colors)
{
	size_t comas;
	int last_pos;
	int new_pos;

	LOG_TRACE("Counting comas of RGB");
	comas = 0;
	last_pos = 0;
	new_pos = 0;
	while (new_pos != -1)
	{
		new_pos = str_find(colors, ",", last_pos);
		if (new_pos != -1)
			comas++;
		last_pos = new_pos + 1;
	}
	if (comas != 2)
	{
		LOG_ERROR("Error: wrong number of comas: %zu instead of 2", comas);
	}
	return (comas);
}
