#include "cub3d.h"

//@TODO: alocação
t_game *alloc_init_game(t_file_data *map)
{
	t_game *game;
	t_arena *game_memory;

	LOG_DEBUG("Allocating and Initializing game");
	game_memory = arena_init(GAME_DATA_BUFFER);
	game = arena_alloc(&game_memory, sizeof(t_game), 1);
	game->game_memory = game_memory;
	game->extracted_data = map;
	game->win = alloc_init_window(game->game_memory);
	//@TODO: initialize player here
	//@TODO: initialize draw here
	//@TODO: initialize ray here
	LOG_DEBUG("Success: initialized game struct");
	return (game);
}

//@TODO: inicialização

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
	cleanup_extracted_data(game->extracted_data);
	//@TODO: fazer cleanup_game()
	arena_destroy(game->game_memory);
	LOG_DEBUG("Success: ended game - exiting");
	exit(EXIT_SUCCESS);
}
