#include "cub3d.h"

void	raycasting(t_game *game, int screen_x)
{
	//@TODO: game->ray e game->draw deviam ser mallocados
	t_ray		*ray = game->ray;
	t_draw		*draw = game->draw_info;
	t_player	*p= game->player;
	int			hit = 0;

	LOG_TRACE("Camera_pixel");
	/* calcular a posição X da camera para esta coluna 
		-1.0 (extrema esquerda da tela) ate +1.0 (extrema direita) sendo 0.0 o centro */
	ray->camera_pixel = (2 * (screen_x /(double)WINDOW_WIDTH)) - 1;
	
	LOG_TRACE("Ray dir");
	/* direcao do raio: direcao do jogador + plano da camera escalado */
	ray->dir.x = p->dir.x + (p->plane.x * ray->camera_pixel);
	ray->dir.y = p->dir.y + (p->plane.y * ray->camera_pixel);

	LOG_TRACE("Ray map pos");
	/* posicao atual do jogador no mapa 
	(arrendando o numero para baixo para saber o bloco exato) */ 
	ray->map_pos.x = (int)p->pos.x;
	ray->map_pos.y = (int)p->pos.y;

	/* distancia para atravessar uma celula (usando 'fabs' para ter o valor absoluto) */
	/* divide por 1 para saber a distancia para atrvessar 1 celula */
	/* ray->delta_dist.x = fabs(sqrt(ray->dir.x * ray->dir.x
									+ ray->dir.y * ray->dir.y) / ray->dir.x)
	entao eu teria que calcular a magnitude de 'ray->dir' mas como tudo e proporcional,
	eu posso simplesmente dividir por 1 */
	LOG_TRACE("Ray delta dist");
	ray->delta_dist.x = fabs(1 / ray->dir.x);
	ray->delta_dist.y = fabs(1 / ray->dir.y);

	LOG_TRACE("Ray side dist and step");
	/* determinar direcao do passo e distancia inicial ate a primeira borda (side_dist) */
	/* considerando o bloco como tamanho 1 'side_dist' sempre sera menor que 1 */
	if (ray->dir.x < 0) /* x sendo negativo = raio indo para a esquerda */
	{
		ray->side_dist.x = (p->pos.x - ray->map_pos.x) * ray->delta_dist.x;
		ray->step.x = -1; /* raio para esquerda */
	}
	else /* contrario, x sendo positivo raio para a direita */
	{
		ray->side_dist.x = (ray->map_pos.x + 1.0 - p->pos.x) * ray->delta_dist.x;
		ray->step.x = 1; /* raio para direita */
	}

	if (ray->dir.y < 0)
	{
		ray->side_dist.y = (p->pos.y - ray->map_pos.y) * ray->delta_dist.y;
		ray->step.y = -1; /* raio para cima */
	}
	else
	{
		ray->side_dist.y = (ray->map_pos.y + 1.0 - p->pos.y) * ray->delta_dist.y;
		ray->step.y = 1; /* raio para baixo */
	}

	LOG_DEBUG("DDA loop");
	/* DDA loop */
	while (hit == 0)
	{
		if (ray->side_dist.x < ray->side_dist.y)
		{
			ray->side_dist.x += ray->delta_dist.x;
			ray->map_pos.x += ray->step.x;
			ray->hit_side = 0;
		}
		else
		{
			ray->side_dist.y += ray->delta_dist.y;
			ray->map_pos.y += ray->step.y;
			ray->hit_side = 1;
		}
		/* se a posicao no mapa onde o raio estiver for uma parede sinalizo que acertei a parede */
		if (game->map[ray->map_pos.y][ray->map_pos.x] == 1)
			hit = 1;
	}

	LOG_TRACE("Fish eye");
	/* calcular distancia perpendicular o raio ate a parede (perp_dist) para evitar efeito "fish-eye" */
	if (ray->hit_side == 0) /* acertou o raio no eixo X */
		ray->perp_dist = (ray->map_pos.x - p->pos.x + (1 - ray->step.x) / 2) / ray->dir.x;
	else
		ray->perp_dist = (ray->map_pos.y - p->pos.y + (1 - ray->step.y) / 2) / ray->dir.y;

	set_draw_info(draw, ray, p, game->textures);

	/* desenhar coluna */
	draw_vertical_line(game, draw, screen_x);
}

t_ray *alloc_ray(t_arena *game_memory)
{
	t_ray *r;

	LOG_DEBUG("Allocating and Initializing raycast");
	r = arena_alloc(&game_memory, sizeof(t_ray), 1);
	LOG_TRACE("Success: raycast struct allocated");
	return (r);
}
