#include "cub3d.h"

//@TODO: init_game_struct
//

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
