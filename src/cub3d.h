/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 19:10:05 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/15 22:08:36 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"
# include "logger.h"
# include "mlx.h"
# include "vector.h"
# include <X11/keysym.h>
# include <math.h>

# ifndef LOG_LEVEL
#  define LOG_LEVEL 7
# endif

# ifndef BONUS
#  define BONUS 0
# endif

//@QUESTION: provavelmente seria melhor definir tamanhos iniciais como máximos?
// de modo a evitar realocações, embora os maps que levem a isso sejam raros
# define MAP_INITIAL_ROWS 30
# define MAP_INITIAL_COLS 30

//@QUESTION: é possível fazer isto?
// eles não dizem no subject o tamanho máximo
# define MAX_ROWS 100
# define MAX_COLS 100

//@NOTE: o -1 sinaliza que chegámos ao fim da linha
// e mapa (se for map[i][0] == -1)
//@TODO: substituir todos os -1 por ROW_END
# define ROW_END -1

# define WINDOW_WIDTH 1920
# define WINDOW_HEIGHT 1080

# define MINIMAP_SCALE 10

//@NOTE: this is where all the game data goes
//(excluding extracted data from .cub)
# define GAME_DATA_BUFFER 50000

# define TEXTURE_EXTENSION ".xpm"
# define MAP_DATA_EXTENSION ".cub"

//@NOTE: 60º degrees in radians
# define DEFAULT_FOV 1.04719755

typedef struct s_rgb
{
	int				r;
	int				g;
	int				b;
}					t_rgb;

enum				e_textures
{
	NORTH = 0,
	SOUTH,
	WEST,
	EAST,
	MAX_TEXTURES
};

typedef struct s_file_data
{
	t_string		*textures[MAX_TEXTURES];
	t_rgb			floor;
	t_rgb			ceiling;
	t_dynamic_array	*rows;
	bool			player_position_is_set;
	bool			parser_error;
}					t_file_data;

typedef struct s_window
{
	void			*mlx;
	void			*mlx_win;
	void			*img;
	char			*addr;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
	int				height;
	int				width;
}					t_window;

typedef struct s_texture
{
	void			*img;
	char			*addr;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
	int				width;
	int				height;
}					t_texture;

typedef struct s_player
{
	t_vec2d			pos;
	t_vec2d			dir;
	t_vec2d			plane;
	double			move_speed;
	double			rot_speed;
	double			collision_buffer;
	double			fov;
	bool			move_up;
	bool			move_down;
	bool			move_left;
	bool			move_right;
	bool			rot_left;
	bool			rot_right;

}					t_player;

typedef struct s_ray
{
	t_vec2d			dir;
	double			camera_pixel;
	t_vec2i			map_pos;
	t_vec2d			side_dist;
	t_vec2d			delta_dist;
	t_vec2i			step;
	double			perp_dist;
	int				hit_side;
}					t_ray;

typedef struct s_draw
{
	int				line_height;
	int				draw_start;
	int				draw_end;
	t_rgb			floor_color;
	t_rgb			ceiling_color;
	int				tex_x;
	float			wall_x;
	t_texture		*tex;
	int				screen_y;
}					t_draw;

typedef struct s_minimap
{
	int				x;
	int				y;
	int				size;
	int				color;
}					t_minimap;

typedef struct s_game
{
	t_arena			*game_memory;
	t_file_data		*extracted_data;
	t_window		*win;
	t_draw			*draw_info;
	t_dynamic_array	*jagged_map;
	int				map[MAX_ROWS][MAX_COLS];
	t_texture		textures[MAX_TEXTURES];
	t_player		*player;
	t_ray			*ray;
}					t_game;

// ===============
// Data Extraction
// ===============
//
// data extraction
t_file_data			*extract_file_data(const char *game_data_path,
						t_file_data *fdata);
//
// utils
bool				extract_texture(t_file_data *fdata, t_string *id,
						t_string *texture_path);
int					map_value_from_char(char c);
bool				should_extract_textures(t_file_data *fdata);
bool				should_extract_colors(t_file_data *fdata);
//
// validate map data
bool				is_valid_map_num(char c);
bool				is_valid_map_player_pos(char c);
bool				starts_with_texture_id(t_string *line);
void				check_if_map_nums_are_valid(t_file_data *fdata);
// validate file
bool				is_valid_file_path(const char *path);
bool				is_valid_extension(const char *path, const char *extension);
// validate program args
bool				is_valid_input(int argc, char **argv);
//
// rgb
bool				is_rgb_id(t_string *line);
bool				rgb_str_is_valid(t_string **colors, int colors_num);
bool				extract_rgb(t_file_data *fdata, t_string *id,
						t_string *colors);
int					rgb_to_int(t_rgb color);
//
// initialization
t_file_data			*alloc_init_extracted_data(void);
//
// free
void				cleanup_extracted_data(t_file_data *fdata);
//@TODO: descrição
void				normalize_jagged_map(t_game *game, t_file_data *map);

// ===========
// Game Struct
// ===========
t_game				*alloc_init_game(t_file_data *fdata);
int					end_game(t_game *game, int exit_code);
void				update_player_pos(int map[MAX_ROWS][MAX_COLS], t_player *p);
void				update_player_dir(t_player *p);

// ======
// Window
// ======
t_window			*alloc_init_window(t_arena *game_memory);

// ======
// Render
// ======
// draw
void				render(t_game *game);
void				pixel_put(t_window *win, int x, int y, int color);
t_draw				*alloc_draw_info(t_arena *game_memory,
						t_file_data *extracted_data);
void				set_draw_info(t_draw *d, t_ray *ray, t_player *player,
						t_texture texture[MAX_TEXTURES]);
void				draw_vertical_line(t_game *game, t_draw *d, int screen_x);
//
// textures
bool				load_textures(t_game *game);
void				cleanup_textures(t_window *win,
						t_texture textures[MAX_TEXTURES]);
int					get_pixel_from_texture(t_texture *tex, int x, int y);
t_texture			*get_wall_texture(t_ray *ray,
						t_texture texture[MAX_TEXTURES]);

// ======
// Player
// ======
t_player			*alloc_init_player(t_arena *game_memory);
void				set_player_position(t_player *player, double x, double y);
void				set_player_direction(t_player *player, size_t direction);

// =======
// Raycast
// =======
t_ray				*alloc_ray(t_arena *game_memory);
void				set_ray_info(t_ray *r, t_player *player, int screen_x);
void				dda_loop(t_ray *r, int map[MAX_ROWS][MAX_COLS]);
void				fix_fish_eye_effect(t_ray *ray, t_player *player);

// ==========
// Game Loop
// ==========
void				setup_hooks(t_window *win, t_game *game);
int					game_loop(t_game *game);
// keys
int					key_press(int keycode, t_game *game);
int					key_release(int keycode, t_game *game);
// free

// ==========
// Bonus
// ==========
//
// minimap
void				draw_square(t_window *win, int start_x, int start_y,
						int color);
void				draw_minimap(t_game *game);

// =====
// Utils
// =====
//
// accessors
int					get_col_value(t_dynamic_array *row, int col);
int					get_map_value(t_dynamic_array *map, int row, int col);
void				set_map_value(t_dynamic_array *map, int value, int row,
						int col);
int					get_map_size(t_dynamic_array *map);
int					get_map_row_size(t_dynamic_array *map, int row);
//
// debug
void				LOG_DEBUG_JAGGED_MAP_NUMS(t_file_data *map);
void				LOG_DEBUG_MAP_NUMS(int map[MAX_ROWS][MAX_COLS]);

#endif /*CUB3D_H*/
