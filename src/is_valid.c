#include "cub3d.h"

bool is_valid_map_num(char c)
{
	return (c == '0' || c == '1');
}

bool is_valid_map_player_pos(char c)
{
	return (c == 'N' || c == 'E' || c == 'S' || c == 'W');
}

bool starts_with_texture_id(t_string *line)
{
	return ((str_starts_with(line, "NO ")
			|| str_starts_with(line, "SO ")
			|| str_starts_with(line, "WE ")
			|| str_starts_with(line, "EA ")));
}

//@TODO: substituir por bool
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

bool is_valid_extension(const char *path, const char *extension)
{
	bool has_one_dot;
	bool is_intended_extension;
	char *find_dot_from_beginning;
	char *find_dot_from_end;

	if (path == NULL)
		return (false);
	find_dot_from_beginning = ft_strchr(path, '.');
	find_dot_from_end = ft_strrchr(path, '.');
	has_one_dot = find_dot_from_beginning == find_dot_from_end;
	is_intended_extension = ft_strncmp(find_dot_from_end, extension, ft_strlen(extension)) == 0;
	return (is_valid_file_path(path) && has_one_dot && is_intended_extension);
}
