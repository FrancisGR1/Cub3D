#include "cub3d.h"


int main(int argc, char **argv)
{
	t_map *extracted_data;
	t_game *game;

	logger_initConsoleLogger(stderr);
	logger_setLevel(LOG_LEVEL);
	if (LOG_LEVEL < 7)
		ft_fprintf(STDOUT, "Logging Level at %d\n", LOG_LEVEL);
	if (!is_valid_input(argc, argv))
	{
		ft_fprintf(STDERR, "Error\n");
		return (1);
	}
	extracted_data = init_map();
	if (extracted_data == NULL)
		return (1);
	//@REFACTOR: isto pode ir possivelmente
	//para dentro de extract_map_data
	int fd = open(argv[1], O_RDONLY);
	extract_map_data(fd, extracted_data);
	close(fd);
	check_if_map_nums_are_valid(extracted_data);
	if (extracted_data->parser_error)
	{
		ft_fprintf(STDERR, "Error\n");
		cleanup_map(extracted_data);
		LOG_ERROR("Error: exiting off main");
		return (1);
	}
	//@TODO: posso libertar a informação 
	//do mapa se copiar o necessário para
	//a struct principal
	//inicializar janela
	game = ft_calloc(1, sizeof(t_game));
	game->win = ft_calloc(1, sizeof(t_window));
	game->extracted_data = extracted_data;
	init_window(game->win);
	event_loop(game);
	//inicializar jogo
	//game loop:
	//renderizar
	//esperar por input
	LOG_DEBUG("Success: exiting off main");
	return 0;
}
