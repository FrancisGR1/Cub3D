#include "cub3d.h"

//@TODO: transformar estas funcs em macros!
void LOG_DEBUG_JAGGED_MAP_NUMS(t_file_data *map)
{
	for (size_t i = 0; i < map->rows->len; i++)
	{
		//@TODO: substituir por get_map_valuedynamic
		t_dynamic_array *cols = ((t_dynamic_array**)map->rows->data)[i];
		for (size_t j = 0; j < cols->len; j++)
		{
			int num = ((int *)cols->data)[j];
			LOG_TRACE("[%d][%d] %d", i, j, num);
		}
		LOG_TRACE("---");
	}
}

void LOG_DEBUG_MAP_NUMS(int map[MAX_ROWS][MAX_COLS])
{
	if (LOG_LEVEL > LogLevel_DEBUG)
		return ;
	//@ASSUMPTION: -1 = fim da coluna
	size_t i;
	for (i = 0; i < MAX_ROWS && map[i][0] != ROW_END; i++)
	{
		printf("%2zu: ", i);
		size_t j;
		for (j = 0; j < MAX_COLS && map[i][j] != ROW_END; j++)
		{
			printf("%d", map[i][j]);
		}
		if (j < MAX_COLS)
		{
			printf("%d", map[i][j]);
		}
		printf("\n");
	}
	if (i < MAX_ROWS)
		printf("%zu: %d\n", i, map[i][0]);
}
