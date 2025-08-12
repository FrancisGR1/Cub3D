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

int	end_game(t_game *game)
{
	LOG_INFO("Ending game");
	if (game == NULL)
		LOG_FATAL("GAME NULL!");
	LOG_TRACE("Mlx: destroying image");
	mlx_destroy_image(game->win->mlx, game->win->img);
	LOG_TRACE("Mlx: destroying window");
	mlx_destroy_window(game->win->mlx, game->win->mlx_win);
	LOG_TRACE("Mlx: destroying display");
	mlx_destroy_display(game->win->mlx);
	LOG_TRACE("Mlx: free mlx ptr");
	free(game->win->mlx);
	cleanup_map(game->extracted_data);
	//@TODO: fazer cleanup_game()
	free(game->win);
	free(game);
	LOG_DEBUG("Success: ended game - exiting");
	exit(EXIT_SUCCESS);
}
