#include "cub3d.h"

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

bool extract_rgb(t_map *map, t_string *id, t_string *colors)
{
	t_rgb *rgb_ptr;
	t_string **split_colors;
	int split_nums;

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
	LOG_DEBUG("Success: RGBs extracted: %d %d %d", rgb_ptr->r, rgb_ptr->g, rgb_ptr->b);
	return (true);
}
