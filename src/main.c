#include "cub3d.h"


//@TEMP: delete loggers before evaluation
int main(int argc, char **argv)
{
	t_map *map;
	logger_initConsoleLogger(stderr);
	logger_setLevel(LogLevel_ERROR);
	ft_fprintf(STDOUT, "Logging Level at %d\n", LOG_LEVEL);
	if (argc != 2)
	{
		ft_fprintf(STDERR, "Error\n");
		LOG_ERROR("Error: ./cub3d <path_to_map>.cub\n");
		return (1);
	}
	map = init_map();
	if (map == NULL)
	{
		return (1);
	}
	int fd = open(argv[1], O_RDONLY);
	//@TODO: func para validar input
	if (fd <= 2)
	{
		LOG_ERROR("Error: invalid fd: %d", fd);
		close(fd);
		cleanup_map(map);
		return (1);
	}
	extract_map_data(fd, map);
	close(fd);
	cleanup_map(map);
	//window
	//render
	LOG_DEBUG("Success: exiting off main");
	return 0;
}
