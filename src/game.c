#include "cub3d.h"

static void normalize_jagged_map(t_game *game, t_file_data *map);

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
	game->player = alloc_init_player(game->game_memory);
	game->render_ctx = alloc_init_render_ctx(game->game_memory, game->win, map);
	game->ray = alloc_init_raycast(game->game_memory);
	normalize_jagged_map(game, map);
	LOG_DEBUG("Success: initialized game struct");
	return (game);
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
	cleanup_extracted_data(game->extracted_data);
	arena_destroy(game->game_memory);
	LOG_DEBUG("Success: ended game - exiting");
	exit(EXIT_SUCCESS);
}

//@TODO: guardar posição do jogador e substituir por 0
static void normalize_jagged_map(t_game *game, t_file_data *map)
{
	int largest_row;
	int row_size;
	int rows;
	int row;
	int col;

	LOG_INFO("Normalizing jagged map");
	largest_row = 0;
	rows = get_map_size(map->rows);
	row = 0;
	LOG_DEBUG("Find largest row");
	while (row < rows)
	{
		row_size = get_map_row_size(map->rows, row);
		LOG_TRACE("Largest row: %d[%d]", largest_row, row);
		if (largest_row < row_size)
			largest_row = row_size;
		row++;
	}
	row = 0;
	LOG_TRACE("Largest row: %d", largest_row);
	while (row < rows)
	{
		col = 0;
		row_size = get_map_row_size(map->rows, row);
		LOG_TRACE("Filling line %d of size %d", row, row_size);
		while (col < row_size)
		{
			game->map[row][col] = get_map_value(map->rows, row, col); 
			if (game->map[row][col] != 0 && game->map[row][col] != 1)
			{
				//@ASSUMPTION se não for 1 ou 0, é a direção/posição do ray
				//@REFACTOR: isto não devia ser feito aqui 
				set_player_position(game->player, col, row);
				set_player_direction(game->player, game->map[row][col]);
			}
			col++;
		}
		if (col < row_size)
			LOG_TRACE("Filling rest with 1s");
		while (col < largest_row)
		{
			game->map[row][col] = 1; 
			col++;
		}
		if (col < MAX_COLS)
			game->map[row][col] = ROW_END;
		row++;
	}
	if (row < MAX_ROWS)
	{
		LOG_TRACE("Filling final row with -1s");
		col = 0;
		while (col < largest_row)
		{
			//@NOTE: ROW_END == não existem mais linhas
			game->map[row][col] = ROW_END;
			col++;
		}
	}
	LOG_DEBUG_MAP_NUMS(game->map);
	LOG_DEBUG("Success: Normalized jaggedd map");
}
