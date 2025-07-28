#include "cub3d.h"


t_map *init_map(void)
{
	t_map *map;

	map = ft_calloc(sizeof(t_map), 1);
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
	map->floor = (t_rgb){-1, -1, -1};  
	map->ceiling = (t_rgb){-1, -1, -1};
	return (map);
}

//@TEMP: delete loggers before evaluation
int main(int argc, char **argv)
{
	t_map *map;
	logger_initConsoleLogger(stderr);
	logger_setLevel(LOG_LEVEL);
	ft_fprintf(STDOUT, "Logging Level at %d\n", LOG_LEVEL);
	if (argc != 2)
	{
		ft_fprintf(STDERR, "Error\n");
		LOG_ERROR("Error: Need 1 arg: ./cub3d <path_to_map>\n");
		return (1);
	}
	map = init_map();
	if (map == NULL)
	{
		return (1);
	}
	extract_map_data(argv[1], map);
	cleanup_map(map);
	//initialization
	//render
	return 0;
}
