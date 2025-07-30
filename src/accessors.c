#include "cub3d.h"

int get_map_value(t_dynamic_array *map, int row, int col)
{
	//@WARNING: proteger contra out of bound idx!
    return ((int *)(((t_dynamic_array**)map->data)[row]->data))[col];
}

void set_map_value(t_dynamic_array *map, int value, int row, int col)
{
	//@WARNING: proteger contra out of bound idx!
   ((int *)(((t_dynamic_array**)map->data)[row]->data))[col] = value;
}

//@TODO
int get_map_size(t_dynamic_array *map)
{
	//@WARNING: proteger contra out of bound idx!
	return (map->len);
}

int get_map_row_size(t_dynamic_array *map, int row)
{
	//@WARNING: proteger contra out of bound idx!
	return (((t_dynamic_array**)map->data)[row]->len);
}
