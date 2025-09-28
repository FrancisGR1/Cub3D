/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_valid_p2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:27:15 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/28 11:58:37 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

bool	is_valid_input(int argc, char **argv)
{
	if (argc != 2)
	{
		return (false);
	}
	if (!is_valid_file_path(argv[1]))
	{
		return (false);
	}
	if (!is_valid_extension(argv[1], MAP_DATA_EXTENSION))
	{
		return (false);
	}
	return (true);
}

void	substitute_spaces(t_file_data *fdata, t_string *line)
{
	if (should_extract_textures(fdata) || should_extract_colors(fdata))
	{
		str_trim(line);
	}
	else
	{
		str_remove(line, "\n");
	}
}

bool	digit_or_space(char c)
{
	return (ft_isdigit(c) || c == ' ');
}

void	print_error(enum e_parser_errors e)
{
	if (e == MORE_THAN_TWO_LINES)
		ft_fprintf(STDERR, "Error: not 2 split strings\n");
	else if (e == INVALID_TEXTURE)
		ft_fprintf(STDERR, "Error: invalid texture\n");
	else if (e == INVALID_RGB)
		ft_fprintf(STDERR, "Error: invalid rgb\n");
	else if (e == EMPTY_LINE_IN_MAP)
		ft_fprintf(STDERR,
			"Error: found an empty line while parsing the map\n");
	else if (e == INVALID_MAP_CHARACTER)
		ft_fprintf(STDERR, "Error: unknown map character\n");
	else if (e == MISSING_PLAYER_POSITION)
		ft_fprintf(STDERR, "Error: no player position\n");
	else if (e == MAP_IS_OPEN)
		ft_fprintf(STDERR, "Error: map is not surrounded by walls\n");
	else
		ft_fprintf(STDERR, "Error: wrong data\n");
}
