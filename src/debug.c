#include "cub3d.h"

//@TODO: transformar isto num macro!
void LOG_DEBUG_MAP_NUMS(t_map *map)
{
	for (size_t i = 0; i < map->rows->len; i++)
	{
		//@TODO: substituir por get_map_valuedynamic
		t_dynamic_array *cols = ((t_dynamic_array**)map->rows->data)[i];
		for (size_t j = 0; j < cols->len; j++)
		{
			int num = ((int *)cols->data)[j];
			LOG_TRACE(" %d", num);
		}
		LOG_TRACE("---");
	}
	(void) map;
}
