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
