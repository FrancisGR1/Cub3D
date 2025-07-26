#include "cub3d.h"

//@TEMP: delete loggers before evaluation
int main(int argc, char **argv)
{
	t_map *map;
	logger_initConsoleLogger(stderr);
	logger_setLevel(LOG_LEVEL);
	ft_fprintf(STDOUT, "Logging Level at %d\n", LOG_LEVEL);
	if (argc != 2)
	{
		ft_fprintf(STDERR, "Error\n");
		LOG_ERROR("Error: Need 1 arg: ./cub3d <path_to_map>\n");
		return (1);
	}
	logger_exitFileLogger();
	map = extract_map_data(argv[1]);
	if (map == NULL)
	{
		return (1);
	}
	cleanup_map(map);
	//initialization
	//render
	return 0;
}
