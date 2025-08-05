#include "cub3d.h"


//@TEMP: delete loggers before evaluation
int main(int argc, char **argv)
{
	t_map *map;

	logger_initConsoleLogger(stderr);
	logger_setLevel(LOG_LEVEL);
	if (LOG_LEVEL < 7)
		ft_fprintf(STDOUT, "Logging Level at %d\n", LOG_LEVEL);
	if (argc != 2)
	{
		ft_fprintf(STDERR, "Error\n");
		LOG_ERROR("Error: ./cub3d <path_to_map>.cub\n");
		return (1);
	}
	map = init_map();
	if (map == NULL)
		return (1);
	//@REFACTOR: isto pode ir possivelmente
	//para dentro de extract_map_data
	int fd = open(argv[1], O_RDONLY);
	//@REFACTOR: isto poder ir para o início 
	//qd fizer uma função para validar o input
	if (fd <= 2)
	{
		LOG_ERROR("Error: invalid fd: %d", fd);
		close(fd);
		cleanup_map(map);
		return (1);
	}
	extract_map_data(fd, map);
	check_if_map_nums_are_valid(map);
	close(fd);
	if (map->parse_error)
	{
		ft_fprintf(STDERR, "Error\n");
		cleanup_map(map);
		LOG_ERROR("Error: exiting off main");
		return (1);
	}
	else
	{
		//@TODO: fazer cenas aqui
		//inicializar jogo
		//game loop:
		//renderizar
		//esperar por input
	}
	//@REFACTOR: isto depois pode ir para dentro da
	//limpeza geral, "cleanup_game()"
	cleanup_map(map);
	LOG_DEBUG("Success: exiting off main");
	return 0;
}
