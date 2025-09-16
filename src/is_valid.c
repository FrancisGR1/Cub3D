/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_valid.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:21:22 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/15 22:05:03 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

bool	is_valid_map_num(char c)
{
	return (c == '0' || c == '1');
}

bool	is_valid_map_player_pos(char c)
{
	return (c == 'N' || c == 'E' || c == 'S' || c == 'W');
}

bool	starts_with_texture_id(t_string *line)
{
	return ((str_starts_with(line, "NO ") || str_starts_with(line, "SO ")
			|| str_starts_with(line, "WE ") || str_starts_with(line, "EA ")));
}

bool	is_valid_file_path(const char *path)
{
	int	fd;

	if (!path)
	{
		return (false);
	}
	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		return (false);
	}
	close(fd);
	return (true);
}

bool	is_valid_extension(const char *path, const char *extension)
{
	bool	is_intended_extension;
	char	*find_dot;

	if (path == NULL)
		return (false);
	find_dot = ft_strrchr(path, '.');
	is_intended_extension = ft_strncmp(find_dot, extension,
			ft_strlen(extension)) == 0;
	return (is_intended_extension);
}
