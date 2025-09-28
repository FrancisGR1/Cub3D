/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_extraction_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:07:02 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/28 13:13:33 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	map_value_from_char(char c)
{
	if (is_valid_map_num(c))
	{
		return (c - 48);
	}
	else if (is_valid_map_player_pos(c))
	{
		return (c);
	}
	else if (c == ' ')
	{
		return (c);
	}
	else
	{
		return (-1);
	}
}

bool	extract_texture(t_file_data *fdata, t_string *id,
		t_string *texture_path)
{
	size_t	idx;

	if (!is_valid_file_path(texture_path->data)
		|| !is_valid_extension(texture_path->data, TEXTURE_EXTENSION))
		return (false);
	if (ft_strncmp(id->data, "NO", id->size) == 0)
		idx = NORTH;
	else if (ft_strncmp(id->data, "SO", id->size) == 0)
		idx = SOUTH;
	else if (ft_strncmp(id->data, "WE", id->size) == 0)
		idx = WEST;
	else if (ft_strncmp(id->data, "EA", id->size) == 0)
		idx = EAST;
	else
		return (false);
	if (fdata->textures[idx] == NULL)
		fdata->textures[idx] = str_create(texture_path->data);
	else
		return (false);
	return (true);
}

bool	should_extract_colors(t_file_data *fdata)
{
	if (fdata->floor.r == -1)
	{
		return (true);
	}
	if (fdata->ceiling.r == -1)
	{
		return (true);
	}
	return (false);
}

bool	should_extract_textures(t_file_data *fdata)
{
	size_t	i;

	i = 0;
	while (i < MAX_TEXTURES)
	{
		if (fdata->textures[i] == NULL)
		{
			return (true);
		}
		i++;
	}
	return (false);
}

bool	should_extract_map_nums(t_file_data *fdata, t_string *line)
{
	return (!((fdata->player_position_is_set == true
				&& !str_is_only_this(line, digit_or_space))
			|| get_map_size(fdata->rows) == MAX_ROWS
			|| line->size > MAX_COLS - 1));
}
