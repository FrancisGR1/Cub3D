#include "cub3d.h"

bool check_if_map_nums_are_valid(t_map *map)
{
	size_t row;
	size_t row_size;
	size_t col;
	size_t last_col;
	size_t val;

	LOG_DEBUG("Checking if map nums are valid");
	if (map == NULL)
	{
		LOG_FATAL("Map ptr is null");
		map->parse_error = true;
		return (false);
	}
	if (map->rows == NULL)
	{
		LOG_FATAL("Map rows ptr is null");
		map->parse_error = true;
		return (false);
	}
	if (map->parse_error == true)
	{
		LOG_ERROR("Error: there was a parse error before");
		return (false);
	}
	row = 0;
	while (row < map->rows->len)
	{
		row_size = get_map_row_size(map->rows, row);
		LOG_TRACE("Checking [%zu] row of size %zu", row, row_size);
		if (row == 0 || row == map->rows->len - 1)
		{
			LOG_TRACE("Checking %s row", row == 0 ? "first" : "last");
			col = 0;
			while (col < row_size)
			{
				val = get_map_value(map->rows, row, col);
				LOG_TRACE("Checking val of:%zu == [%zu][%zu] == 1", val, row, col);
				if (val != 1)
				{
					LOG_ERROR("Value (%zu) at [%zu][%zu] should be 1", val, row, col);
					map->parse_error = true;
					return (false);
				}
				col++;
			}
			LOG_DEBUG("%s is full 1s", row == 0 ? "First row" : "Last row");
		}
		else
		{
			last_col = row_size - 1;
			val = get_map_value(map->rows, row, last_col);
			if (val != 1)
			{
				LOG_ERROR("Value (%zu) at [%zu][%zu] should be 1", val, row, last_col);
				map->parse_error = true;
				return (false);
			}
			val = get_map_value(map->rows, row, 0);
			if (val != 1)
			{
				LOG_ERROR("Value (%zu) at [%zu][%zu] should be 1", val, row, 0);
				map->parse_error = true;
				return (false);
			}
		}
		row++;
	}
	LOG_INFO("Success: map nums are valid");
	return (true);
}

bool is_valid_input(int argc, char **argv)
{
	LOG_DEBUG("Check: program input is valid?");
	if (argc != 2)
	{
		LOG_ERROR("Error: ./cub3d <path_to_map>.cub\n");
		return (false);
	}
	if (!is_valid_file_path(argv[1]))
	{
		return (false);
	}
	if (!is_valid_extension(argv[1], MAP_DATA_EXTENSION))
	{
		LOG_ERROR("Invalid extension: have \"%s\" and need \"%s\"", ft_strrchr(argv[1], '.'), MAP_DATA_EXTENSION);
		return (false);
	}
	LOG_TRACE("Success: program input is valid");
	return (true);
}
