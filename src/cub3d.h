#ifndef CUB3D_H
# define CUB3D_H

#include "libft.h"
#include "logger.h"
#include "mlx.h"
#include "vector.h"
#include <X11/keysym.h>
#include <math.h>

#ifndef LOG_LEVEL
# define LOG_LEVEL 7
#endif

#ifndef BONUS
# define BONUS 0
# endif

//@QUESTION: provavelmente seria melhor definir tamanhos iniciais como máximos? 
//de modo a evitar realocações, embora os maps que levem a isso sejam raros
# define MAP_INITIAL_ROWS 30
# define MAP_INITIAL_COLS 30

//@QUESTION: é possível fazer isto?
//eles não dizem no subject o tamanho máximo
# define MAX_ROWS 100
# define MAX_COLS 100

//@NOTE: o -1 sinaliza que chegámos ao fim da linha
//e mapa (se for map[i][0] == -1)
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
	int r;
	int g;
	int b;
} t_rgb;

enum e_textures
{
	NORTH = 0,
	SOUTH,
	WEST,
	EAST,
	MAX_TEXTURES
};

enum e_extraction_phase
{
	TEXTURES_AND_COLORS = 0,
	MAP_VALUES,
	EXTRACTION_FINISHED,
};


typedef struct s_file_data
{
	t_string *textures[MAX_TEXTURES];
	t_rgb floor;
	t_rgb ceiling;
	t_dynamic_array *rows;
	enum e_extraction_phase extraction_phase;
	bool player_position_is_set;
	bool parser_error;
} t_file_data;

typedef struct s_window
{
	void *mlx;
	void *mlx_win;
	void *img;
	char *addr;
	int bits_per_pixel;
	int line_length;
	int endian;
	int height;
	int width;
} t_window;

typedef struct	s_texture
{
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	int			width;
	int			height;
}				t_texture;

typedef struct s_player
{
	t_vec2d		pos; /* posicao no jogador no mapa (5, 5)*/
	t_vec2d		dir; /* direcao onde o jogador esta olhando (-1, 0) */
	t_vec2d		plane; /* vetor perpendicular a direction: define o FOV */
	double		move_speed; /* unidades por frame */
	double		rot_speed; /* radianos por frame para rotacionar */
	double		collision_buffer; /* distancia para nao encostar na parede */
	double		fov;
	bool 		move_up;
	bool 		move_down;
	bool 		move_left;
	bool 		move_right;
	bool 		rot_left;
	bool 		rot_right;

} t_player;

typedef struct s_ray
{
	t_vec2d		dir; /* direcao do raio */
	double		camera_pixel;  /* -1 .. +1 para cada coluna */
	t_vec2i		map_pos; /* celula atual do mapa (x, y) */
	t_vec2d		side_dist;  /* distancia ate a proxima borda (x, y) */
	t_vec2d		delta_dist; /* distancia para atravessar 1 celula (x, y) */
	t_vec2i		step; /* +1 ou -1 para X e Y */
	double		perp_dist; /* distancia perpendicular (corrige fish-eye) */
	int			hit_side; /* 0 = vertical (X), 1 = horizontal (Y) */
} t_ray;

typedef struct s_draw
{
	int			line_height;
	int			draw_start;
	int			draw_end;
	t_rgb			floor_color;
	t_rgb			ceiling_color;
	int			tex_x;
	float			wall_x;
	t_texture		*tex;
	//@TODO: colocar aqui o conteúdo de cada .xpm
} t_draw;

typedef struct	s_minimap
{
	int			x;
	int			y;
	int			size;
	int			color;
}				t_minimap;

typedef struct s_game
{
	t_arena *game_memory;
	t_file_data *extracted_data;
	t_window *win;
	t_draw *draw_info;
	t_dynamic_array *jagged_map;
	int map[MAX_ROWS][MAX_COLS];
	t_texture textures[MAX_TEXTURES];
	t_player *player;
	t_ray *ray;
} t_game;



// ===============
// Data Extraction
// ===============
//
// data extraction
t_file_data *extract_file_data(const char *map_data_file_path, t_file_data *map);
//
// utils
bool extract_texture(t_file_data *map, t_string *id, t_string *texture_path);
int map_value_from_char(char c);
bool should_extract_textures(t_file_data *map);
bool should_extract_colors(t_file_data *map);
//
// validate map data
bool is_valid_map_num(char c);
bool is_valid_map_player_pos(char c);
bool starts_with_texture_id(t_string *line);
bool check_if_map_nums_are_valid(t_file_data *map);
// validate file
bool is_valid_file_path(const char *path);
bool is_valid_extension(const char *path, const char *extension);
// validate program args
bool is_valid_input(int argc, char **argv);
//
// rgb
bool is_rgb_id(t_string *line);
bool rgb_str_is_valid(t_string **colors, int colors_num);
bool extract_rgb(t_file_data *map, t_string *id, t_string *colors);
int rgb_to_int(t_rgb color);
//
// initialization
t_file_data *init_extracted_data(void);
//
// free
//@TODO: mudar nome cleanup_extracted_map_data
void cleanup_extracted_data(t_file_data *map);


// ===========
// Game Struct
// ===========
t_game *alloc_init_game(t_file_data *map);
int	end_game(t_game *game, int exit_code);
void	update(t_game *game);


// ======
// Window
// ======
t_window *alloc_init_window(t_arena *game_memory);



// ======
// Render
// ======
void render(t_game *game);
void paint_background(t_window *win, t_draw *draw_info);
t_draw *alloc_draw_info(t_arena *game_memory, t_file_data *extracted_data);
void set_draw_info(t_draw *d, t_ray *ray, t_player *player, t_texture texture[MAX_TEXTURES]);
void draw_vertical_line(t_game *game, t_draw *d, int screen_x);



// ======
// Player
// ======
t_player *alloc_init_player(t_arena *game_memory);
void set_player_position(t_player *player, double x, double y);
void set_player_direction(t_player *player, size_t direction);



// =======
// Raycast
// =======
t_ray *alloc_ray(t_arena *game_memory);
void	raycasting(t_game *game, int screen_x);



// ==========
// Event Loop
// ==========
void setup_event_listeners(t_window *win, t_game *game);
int event_loop(t_game *game);
// keys
int	key_press(int keycode, t_game *game);
int	key_release(int keycode, t_game *game);
// free



// =====
// Utils
// =====
//
// accessors
int get_col_value(t_dynamic_array *row, int col);
int get_map_value(t_dynamic_array *map, int row, int col);
void set_map_value(t_dynamic_array *map, int value, int row, int col);
int get_map_size(t_dynamic_array *map);
int get_map_row_size(t_dynamic_array *map, int row);
//
// debug
void LOG_DEBUG_JAGGED_MAP_NUMS(t_file_data *map);
void LOG_DEBUG_MAP_NUMS(int map[MAX_ROWS][MAX_COLS]);

#endif /*CUB3D_H*/
