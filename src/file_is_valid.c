#include "cub3d.h"

int is_valid_file_path(const char *path)
{
	int fd;

	LOG_TRACE("Checking if %s is valid file path", path);
	if (!path)
	{
		LOG_ERROR("Path to file is null");
		return (0);
	}
	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		LOG_ERROR("Couldn't open: %s", path);
		return (0);
	}
	LOG_DEBUG("Success: valid: %s", path);
	close(fd);
	return (1);
}
