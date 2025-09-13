/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:31:55 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/13 19:32:32 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	count_comas(t_string *colors);

bool	is_rgb_id(t_string *line)
{
	return (str_starts_with(line, "F ") || str_starts_with(line, "C "));
}

bool	rgb_str_is_valid(t_string **colors, int colors_num)
{
	int	idx;
	int	rgb_value;

	idx = 0;
	while (idx < colors_num)
	{
		if (!str_is_digit(colors[idx]))
		{
			return (false);
		}
		rgb_value = str_to_int(colors[idx]);
		if (!(rgb_value >= 0 && rgb_value <= 255))
		{
			return (false);
		}
		idx++;
	}
	return (true);
}

bool	extract_rgb(t_file_data *fdata, t_string *id, t_string *colors)
{
	t_rgb		*rgb_ptr;
	t_string	**split_colors;
	int			split_nums;
	bool		extraction_success;

	if (ft_strncmp(id->data, "C", id->size) == 0 && fdata->ceiling.r == -1)
		rgb_ptr = &fdata->ceiling;
	else if (ft_strncmp(id->data, "F", id->size) == 0 && fdata->floor.r == -1)
		rgb_ptr = &fdata->floor;
	else
		return (false);
	split_colors = str_split_using_char_as_delim(colors, ",", &split_nums);
	if (split_colors == NULL || split_nums != 3 || count_comas(colors) != 2
		|| !rgb_str_is_valid(split_colors, split_nums))
		extraction_success = false;
	else
	{
		rgb_ptr->r = str_to_int(split_colors[0]);
		rgb_ptr->g = str_to_int(split_colors[1]);
		rgb_ptr->b = str_to_int(split_colors[2]);
		extraction_success = true;
	}
	str_array_deallocate(split_colors, split_nums);
	return (extraction_success);
}

static int	count_comas(t_string *colors)
{
	size_t	comas;
	int		last_pos;
	int		new_pos;

	comas = 0;
	last_pos = 0;
	new_pos = 0;
	while (new_pos != -1)
	{
		new_pos = str_find(colors, ",", last_pos);
		if (new_pos != -1)
			comas++;
		last_pos = new_pos + 1;
	}
	return (comas);
}

int	rgb_to_int(t_rgb color)
{
	return ((color.r << 16) | (color.g << 8) | color.b);
}
