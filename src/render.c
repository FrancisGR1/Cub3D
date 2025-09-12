/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 18:56:05 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/09 19:03:13 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


void	pixel_put(t_window *win, int x, int y, int color)
{
	char	*dest;

	if (!win || !win->addr)
		return ;
	if (x < 0 || x >= win->width || y < 0 || y >= win->height)
		return;
	/* size_line sendo o valor de uma linha (numa janela 800x600 cada linha ocupa 3200 bytes)
	entao y * size_line eu acho a linha que quero modificar.
	normamente bits_per_pixel = 32 bits (4 bytes por pixel RGBA) entao divido por 8 para saber quantos bytes 
	e faco * x  para achar a posicao na linha. Somando entao os dois calculos e depois somando com addr
	eu acho o pixel que quero modificar */
	dest = win->addr + (y * win->line_length + x * (win->bits_per_pixel / 8));
	/* dest agora aponta para o pixel de 'addr' que desejo modificar */

	*(unsigned int *)dest = color; /* substitui os bytes daquele pixel por 'color' */
	/* entao quando eu imprimir a 'img' esses bytes de 'addr' estarao 'color' e ira pararecer 'color' na janela */
}

bool	is_walkable(t_game *game, double x, double y)
{
	int	mx;
	int	my;

	mx = (int)floor(x);
	my = (int)floor(y);
	if (mx < 0 || my < 0)
		return (false);
	if (my >= MAX_ROWS || mx >= MAX_COLS)
		return (false);
	return (game->map[my][mx] == 0);
}

void	update(t_game *game)
{
	t_player *p;
	t_vec2d	desloc = {.x = 0, .y = 0};
	t_vec2d	step = {.x = 0, .y = 0};
	t_vec2d	test = {.x = 0, .y = 0};
	t_vec2i	sign = {.x = 0, .y = 0};

	if (!game || !game->player)
		return;

	p = game->player;

	/* movimento frente/tras (W / S) */
	if (p->move_up)
	{
		step = mult_vector(&p->dir, p->move_speed);
		sum_vector(&desloc, &step);
	}
	if (p->move_down)
	{
		step = mult_vector(&p->dir, -p->move_speed);
		sum_vector(&desloc, &step);
	}
	/* movimento esquerda/direita (A / D) */
	if (p->move_left)
	{
		step = mult_vector(&p->dir, p->move_speed);
		rotate_vector(&step, -M_PI / 2);
		sum_vector(&desloc, &step);
	}
	if (p->move_right)
	{
		step = mult_vector(&p->dir, p->move_speed);
		rotate_vector(&step, M_PI / 2);
		sum_vector(&desloc, &step);
	}
	if (p->rot_left)
	{
		rotate_vector(&p->dir, -p->rot_speed);
		rotate_vector(&p->plane, -p->rot_speed);
	}
	if (p->rot_right)
	{
		rotate_vector(&p->dir, p->rot_speed);
		rotate_vector(&p->plane, p->rot_speed);
	}

	/* achar o sentido em X e Y */
	if (desloc.x > 0.0)
		sign.x = 1;
	if (desloc.x < 0.0)
		sign.x = -1;

	if (desloc.y > 0.0)
		sign.y = 1;
	if (desloc.y < 0.0)
		sign.y = -1;
	/* tento mover no eixo X (mantendo Y atual) */
	test.x = p->pos.x + desloc.x;
	if (sign.x != 0)
		test.x += sign.x * p->collision_buffer; /* aplica buffer na direcao do movimento */

	if (is_walkable(game, test.x, p->pos.y))
		p->pos.x += desloc.x;
	/* se nao andar em X, nao atualiza X (colidiu) */

	/* tento mover no eixo Y (com X possivelmente ja atualizado) */
	test.y = p->pos.y + desloc.y;
	if (sign.y != 0)
		test.y += sign.y * p->collision_buffer;

	if (is_walkable(game, p->pos.x, test.y))
		p->pos.y += desloc.y;
}


int get_pixel_from_texture(t_texture *tex, int x, int y)
{
    char *pixel_addr;

    if (x < 0 || x >= tex->width || y < 0 || y >= tex->height)
        return 0;

    pixel_addr = tex->addr + (y * tex->line_length + x * (tex->bits_per_pixel / 8));
    return *(unsigned int *)pixel_addr;
}

t_texture *get_wall_texture(t_ray *ray, t_texture texture[MAX_TEXTURES])
{
    if (ray->hit_side == 0) // vertical
    {
        if (ray->dir.x > 0)
            return &texture[EAST];
        else
            return &texture[WEST];
    }
    else // horizontal 
    {
        if (ray->dir.y > 0)
            return &texture[SOUTH];
        else
            return &texture[NORTH];
    }
}

void set_draw_info(t_draw *d, t_ray *ray, t_player *player, t_texture texture[MAX_TEXTURES])
{
	LOG_DEBUG("Set Draw Info");
	/* calcular altura da linha (inversamente proporcional ao tamanho do raio) */
	d->line_height = (int) WINDOW_HEIGHT / ray->perp_dist;

	/* calcular onde comeca e termina a coluna na tela */
	d->draw_start = (WINDOW_HEIGHT / 2) - (d->line_height / 2);
	if (d->draw_start < 0) /* para nao comecar onde nao tem janela */
		d->draw_start = 0;
	d->draw_end = (WINDOW_HEIGHT / 2) + (d->line_height / 2);
	if (d->draw_end >= WINDOW_HEIGHT)
		d->draw_end = WINDOW_HEIGHT - 1;

	d->tex = get_wall_texture(ray, texture);

	/* calcular o ponto em que o ray atinge a parede */
	if (ray->hit_side == 0)
		d->wall_x = player->pos.y + (ray->map_pos.x - player->pos.x + (1 - ray->step.x) / 2) / ray->dir.x * ray->dir.y;
	else
		d->wall_x = player->pos.x + (ray->map_pos.y - player->pos.y + (1 - ray->step.y) / 2) / ray->dir.y * ray->dir.x;
	d->wall_x -= floor(d->wall_x);

	/* transformar o ponto atingido para a escala da textura */
	d->tex_x = (int)(d->wall_x * (float)d->tex->width);

	/* corrigir o efeito de reflexão */
	if ((ray->hit_side == 0 && ray->dir.x < 0) || (ray->hit_side == 1 && ray->dir.y > 0))
		d->tex_x = d->tex->width - d->tex_x - 1;
}

// @TODO: deve desenhar o céu/chão
void draw_vertical_line(t_game *game, t_draw *d, int screen_x)
{
	LOG_DEBUG("Draw Vertical Line");
	int screen_y;

	if (screen_x < 0 || screen_x >= WINDOW_WIDTH)
		return;
	if (d->draw_start < 0)
		d->draw_start = 0;
	if (d->draw_end >= WINDOW_HEIGHT)
		d->draw_end = WINDOW_HEIGHT - 1;

	//@TODO: mudar para textura escolhoia
	LOG_TRACE("Getting wall texture");
	t_texture *tex = get_wall_texture(game->ray, game->textures);

	if (d->line_height <= 0 || tex->height <= 0)
	{
		LOG_ERROR("Error: Line height: %ld; Texture height: %ld\n", d->line_height, tex->height);
		return;
	}
	/* Use double to avoid integer division truncation */
	LOG_TRACE("Calc tex step");
	float tex_step = (float)tex->height / (float)d->line_height;

	/* Start texture Y so texture is vertically centered and handles clipping:
	 * If the slice was partially off-screen we start inside the texture accordingly.
	 */
	LOG_TRACE("Calc tex pos");
	float tex_pos = (d->draw_start - (WINDOW_HEIGHT / 2.0) + (d->line_height / 2.0)) * tex_step;

	LOG_TRACE("Paint vertical tex");
	for (screen_y = 0; screen_y < d->draw_start; screen_y++)
	{
		unsigned int color = rgb_to_int(d->ceiling_color);
		pixel_put(game->win, screen_x, screen_y, color);
	}
	for (screen_y = d->draw_start; screen_y <= d->draw_end; screen_y++)
	{
		int tex_y = (int)tex_pos;
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= tex->height)
			tex_y = tex->height - 1;

		unsigned int color = get_pixel_from_texture(tex, d->tex_x, tex_y);
		pixel_put(game->win, screen_x, screen_y, color);

		tex_pos += tex_step;
	}
	for (; screen_y < WINDOW_HEIGHT; screen_y++)
	{
		unsigned int color = rgb_to_int(d->floor_color);
		pixel_put(game->win, screen_x, screen_y, color);
	}
}

t_draw *alloc_draw_info(t_arena *game_memory, t_file_data *extracted_data)
{
	t_draw *d;

	LOG_DEBUG("Allocating and Initializing render");
	d = arena_alloc(&game_memory, sizeof(t_draw), 1);
	d->line_height = 0;
	d->draw_start = 0;
	d->draw_end = 0;
	d->floor_color = extracted_data->floor;
	d->ceiling_color = extracted_data->ceiling;
	LOG_TRACE("Success: render ctx struct allocated/initialized");
	return (d);
}

void	draw_square(t_game *game, int start_x, int start_y, int size, int color)
{
	int	x;
	int	y;

	y = start_y;
	while (y < start_y + size)
	{
		x = start_x;
		while (x < start_x + size)
		{
			pixel_put(game->win, x, y, color);
			x++;
		}
		y++;
	}
}

void	draw_minimap(t_game *game)
{
	t_minimap m;

	if (!BONUS)
		return ;
	m.y = 0;
	/* percorro o mapa e desenho um quadrado com a cor baseado na celula */
	while (game->map[m.y][0] != -1)
	{
		m.x = 0;
		while (game->map[m.y][m.x] != -1)
		{
			if (game->map[m.y][m.x] == 1)
				m.color = 0x000000;
			else if (game->map[m.y][m.x] == 0)
				m.color = 0xFFFFFF;
			else /* caso a celula esteja vazia, nao pintar */
			{
				m.x++;
				continue ;
			}
			draw_square(game, m.x * MINIMAP_SCALE, m.y * MINIMAP_SCALE, MINIMAP_SCALE, m.color);
			m.x++;
		}
		m.y++;
	}
	/* calcular a posicao do personagem no minimapa */
	m.x = (int)(game->player->pos.x * MINIMAP_SCALE);
	m.y = (int)(game->player->pos.y * MINIMAP_SCALE);
	m.size = MINIMAP_SCALE / 2; /* tamanho do personagem no minimapa */
	/* desenhar o personagem no minimapa (um quadradinho vermelho) */
	//@TODO: substituir por círculo
	//@TODO: desenhar rays
	draw_square(game, m.x - m.size / 2, m.y - m.size / 2, m.size, 0xFF0000);
}


void render(t_game *game)
{
	t_window *win;
	int screen_x;

	LOG_INFO("Render");
	win = game->win;
	//@TODO: passar isto para draw_vertical_line
	//paint_background(win, game->draw_info);
	screen_x = 0;
	LOG_DEBUG("Raycasting loop");
	while (screen_x < WINDOW_WIDTH)
	{
		//@TODO: expandir esta parte ao reduzir raycasting()
		raycasting(game, screen_x);
		screen_x++;	
	}
	LOG_DEBUG("Draw minimap");
	draw_minimap(game);
	LOG_DEBUG("Put image to window");
	mlx_put_image_to_window(win->mlx, win->mlx_win, win->img, 0, 0);
}
