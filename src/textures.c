/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 19:09:35 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/15 19:09:35 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

bool	load_textures(t_game *game)
{
	t_texture	tex;
	int			idx;

	idx = 0;
	while (idx < MAX_TEXTURES)
	{
		tex.img = mlx_xpm_file_to_image(game->win->mlx,
				game->extracted_data->textures[idx]->data, &tex.width,
				&tex.height);
		if (tex.img == NULL)
		{
			return (false);
		}
		tex.addr = mlx_get_data_addr(tex.img, &tex.bits_per_pixel,
				&tex.line_length, &tex.endian);
		game->textures[idx] = tex;
		idx++;
	}
	return (true);
}

void	cleanup_textures(t_window *win, t_texture textures[MAX_TEXTURES])
{
	size_t	idx;

	idx = 0;
	while (idx < MAX_TEXTURES)
	{
		if (textures[idx].img != NULL)
			mlx_destroy_image(win->mlx, textures[idx].img);
		idx++;
	}
}

int	get_pixel_from_texture(t_texture *tex, int x, int y)
{
	char	*pixel_addr;

	if (x < 0 || x >= tex->width || y < 0 || y >= tex->height)
		return (0);
	pixel_addr = tex->addr + (y * tex->line_length + x * (tex->bits_per_pixel
				/ 8));
	return (*(unsigned int *)pixel_addr);
}

t_texture	*get_wall_texture(t_ray *ray, t_texture texture[MAX_TEXTURES])
{
	if (ray->hit_side == 0)
	{
		if (ray->dir.x > 0)
			return (&texture[EAST]);
		else
			return (&texture[WEST]);
	}
	else
	{
		if (ray->dir.y > 0)
			return (&texture[SOUTH]);
		else
			return (&texture[NORTH]);
	}
}
