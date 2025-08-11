#include "cub3d.h"


int main(int argc, char **argv)
{
	t_map *map;

	logger_initConsoleLogger(stderr);
	logger_setLevel(LOG_LEVEL);
	if (LOG_LEVEL < 7)
		ft_fprintf(STDOUT, "Logging Level at %d\n", LOG_LEVEL);
	if (!is_valid_input(argc, argv))
	{
		ft_fprintf(STDERR, "Error\n");
		return (1);
	}
	map = init_map();
	if (map == NULL)
		return (1);
	//@REFACTOR: isto pode ir possivelmente
	//para dentro de extract_map_data
	int fd = open(argv[1], O_RDONLY);
	extract_map_data(fd, map);
	close(fd);
	check_if_map_nums_are_valid(map);
	if (map->parser_error)
	{
		ft_fprintf(STDERR, "Error\n");
		cleanup_map(map);
		LOG_ERROR("Error: exiting off main");
		return (1);
	}
	//@TODO: posso libertar a informação 
	//do mapa se copiar o necessário para
	//a struct principal
	//inicializar janela
	t_window *win = ft_calloc(1, sizeof(t_window));
	init_window(win);
	//@TODO: criar estrutura de jogo
	setup_mlx_hooks(win, NULL);
	mlx_loop(win->mlx);
	free(win);
	//inicializar jogo
	//game loop:
	//renderizar
	//esperar por input
	cleanup_map(map);
	LOG_DEBUG("Success: exiting off main");
	return 0;
}
