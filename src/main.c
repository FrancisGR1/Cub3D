#include "cub3d.h"

int main(int argc, char **argv)
{
	t_file_data *extracted_data;
	t_game *game;

	logger_initConsoleLogger(stderr);
	logger_setLevel(7);
	if (LOG_LEVEL < 7)
		ft_fprintf(STDOUT, "Logging Level at %d\n", LOG_LEVEL);
	if (!is_valid_input(argc, argv))
	{
		ft_fprintf(STDERR, "Error\n");
		return (1);
	}
	extracted_data = init_extracted_data();
	if (extracted_data == NULL)
		return (1);
	extract_file_data(argv[1], extracted_data);
	check_if_map_nums_are_valid(extracted_data);
	if (extracted_data->parser_error)
	{
		ft_fprintf(STDERR, "Error\n");
		cleanup_extracted_data(extracted_data);
		LOG_ERROR("Error: exiting off main");
		return (1);
	}
	game = alloc_init_game(extracted_data);
	put_pixel(game->win, 300, 100, 16777215);
	put_pixel(game->win, 301, 100, 16777215);
	put_pixel(game->win, 302, 100, 16777215);
	put_pixel(game->win, 303, 100, 16777215);
	put_pixel(game->win, 304, 100, 16777215);
	put_pixel(game->win, 305, 100, 16777215);
	put_pixel(game->win, 306, 100, 16777215);
	put_pixel(game->win, 307, 100, 16777215);
	put_pixel(game->win, 308, 100, 16777215);
	event_loop(game);
	LOG_DEBUG("Success: exiting off main");
	return 0;
}
