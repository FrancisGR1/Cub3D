/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_extraction.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:18:09 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/28 11:14:26 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	extract_file_data_first_phase(t_file_data *fdata, t_string *line);
static bool	extract_file_data_nums(t_file_data *fdata, t_string *line);

t_file_data	*extract_file_data(const char *game_data_path, t_file_data *fdata)
{
	int			fd;
	t_string	*line;

	fd = open(game_data_path, O_RDONLY);
	line = get_next_line_to_str(fd);
	while (line != NULL)
	{
		substitute_spaces(fdata, line);
		if (line->size == 0)
		{
			if (fdata->rows->len != 0)
				fdata->parser_error = EMPTY_LINE_IN_MAP;
		}
		else if (should_extract_textures(fdata) || should_extract_colors(fdata))
			extract_file_data_first_phase(fdata, line);
		else if (!extract_file_data_nums(fdata, line))
			fdata->parser_error = INVALID_MAP_CHARACTER;
		str_deallocate(line);
		if (fdata->parser_error)
			break ;
		line = get_next_line_to_str(fd);
	}
	if (fdata->player_position_is_set == false && fdata->parser_error == false)
		fdata->parser_error = MISSING_PLAYER_POSITION;
	return (close(fd), (fdata));
}

static void	extract_file_data_first_phase(t_file_data *fdata, t_string *line)
{
	int			split_count;
	t_string	**split_strs;

	split_strs = str_split_using_char_as_delim(line, STRING_WHITESPACE,
			&split_count);
	if (split_count != 2)
		fdata->parser_error = MORE_THAN_TWO_LINES;
	else if (starts_with_texture_id(line))
	{
		if (!extract_texture(fdata, split_strs[0], split_strs[1]))
			fdata->parser_error = INVALID_TEXTURE;
	}
	else if (is_rgb_id(line))
	{
		if (!extract_rgb(fdata, split_strs[0], split_strs[1]))
			fdata->parser_error = INVALID_RGB;
	}
	else
		fdata->parser_error = UNKOWN_LINE;
	str_array_deallocate(split_strs, split_count);
}

//@TODO: a posição/direção do jogador devia ser guardada aqui
static bool	extract_file_data_nums(t_file_data *fdata, t_string *line)
{
	t_dynamic_array	*cols;
	char			c;
	int				value;
	int				i;

	if (!should_extract_map_nums(fdata, line))
		return (false);
	cols = darr_init(sizeof(int), MAP_INITIAL_COLS);
	i = -1;
	while (++i < (int)line->size)
	{
		c = str_at(line, i);
		if (is_valid_map_num(c) || (is_valid_map_player_pos(c)
				&& fdata->player_position_is_set == false) || c == ' ')
		{
			value = map_value_from_char(c);
			if (value != 0 && value != 1 && value != ' ')
				fdata->player_position_is_set = true;
			darr_append(cols, &value);
		}
		else
			return (darr_free(cols), (false));
	}
	return (darr_append(fdata->rows, &cols), (true));
}

t_file_data	*alloc_init_extracted_data(void)
{
	t_file_data	*fdata_buf;
	size_t		i;

	fdata_buf = ft_calloc(sizeof(t_file_data), 1);
	if (fdata_buf == NULL)
		return (NULL);
	fdata_buf->rows = darr_init(sizeof(t_dynamic_array *), MAP_INITIAL_ROWS);
	if (fdata_buf->rows == NULL)
		return (NULL);
	fdata_buf->player_position_is_set = false;
	fdata_buf->parser_error = 0;
	fdata_buf->floor = (t_rgb){-1, -1, -1};
	fdata_buf->ceiling = (t_rgb){-1, -1, -1};
	i = 0;
	while (i < MAX_TEXTURES)
	{
		fdata_buf->textures[i] = NULL;
		i++;
	}
	return (fdata_buf);
}

void	cleanup_extracted_data(t_file_data *fdata)
{
	size_t			i;
	t_dynamic_array	*row;

	if (fdata == NULL)
		return ;
	i = 0;
	while (i < MAX_TEXTURES)
	{
		if (fdata->textures[i] != NULL && fdata->textures[i]->data != NULL)
			str_deallocate(fdata->textures[i]);
		i++;
	}
	i = 0;
	while (i < fdata->rows->len)
	{
		row = ((t_dynamic_array **)fdata->rows->data)[i];
		if (row != NULL)
			darr_free(row);
		i++;
	}
	darr_free(fdata->rows);
	free(fdata);
}
