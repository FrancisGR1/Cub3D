#include "cub3d.h"

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

int	end_game(void *data)
{
	t_game *game = (t_game *)data;
	mlx_destroy_image(game->win->mlx, game->win->img);
	mlx_destroy_window(game->win->mlx, game->win->mlx_win);
	mlx_destroy_display(game->win->mlx);
	free(game->win->mlx);
	cleanup_map(game->cub_file_data);
	//@TODO: limpar game aqui
	exit(EXIT_SUCCESS);
}
