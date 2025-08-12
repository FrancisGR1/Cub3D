#ifndef CUB3D_H
# define CUB3D_H

#include "libft.h"
#include "logger.h"
#include "mlx.h"
#include "vector.h"


//@QUESTION: provavelmente seria melhor definir tamanhos iniciais como máximos? 
//de modo a evitar realocações, embora os maps que levem a isso sejam raros
# define MAP_INITIAL_ROWS 30
# define MAP_INITIAL_COLS 30

//@QUESTION: é possível fazer isto?
//eles não dizem no subject o tamanho máximo
# define MAX_ROWS 100
# define MAX_COLS 100


# define SCREENWIDTH 1280
# define SCREENHEIGHT 1024

# define TEXTURE_EXTENSION ".xpm"
# define MAP_DATA_EXTENSION ".cub"

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

//@REFACTOR: mudar isto para parsed_data
typedef struct s_map
{
	t_string *textures[MAX_TEXTURES];
	t_rgb floor;
	t_rgb ceiling;
	t_dynamic_array *rows;
	enum e_extraction_phase extraction_phase;
	bool player_position_is_set;
	bool parser_error;
} t_map;

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

typedef struct s_player
{
	t_vec2d position;
	t_vec2d direction;
	t_vec2d plane;
	float fov;

} t_player;

typedef struct s_ray
{
	t_vec2d direction;
	int camera_x;
	t_vec2i map_position;
	t_vec2d side_dist;
	t_vec2d delta_dist;
	t_vec2i step;
	int perp_wall_dis; //@NOTE: corrected distance to avoid fish eye effect
	int side;
	bool hit_wall;
} t_ray;

typedef struct s_draw_info
{
	int line_height;
	int draw_start;
	int draw_end;
} t_draw_info;

typedef struct s_game
{
	t_map *extracted_data;
	t_window *win;
	t_draw_info *draw;
	t_dynamic_array *map;
	t_player *player;
	t_ray *r;
} t_game;



// ===============
// Data Extraction
// ===============
//
// initialization
t_map *init_map(void);
//
// data extraction
t_map *extract_map_data(int fd, t_map *map);
t_map *extract_map_data_new(int fd, t_map *map);
//
// utils
bool extract_texture(t_map *map, t_string *id, t_string *texture_path);
int map_value_from_char(char c);
bool should_extract_textures(t_map *map);
bool should_extract_colors(t_map *map);
//
// validate map data
bool is_valid_map_num(char c);
bool is_valid_map_player_pos(char c);
bool starts_with_texture_id(t_string *line);
bool check_if_map_nums_are_valid(t_map *map);
// validate file
bool is_valid_file_path(const char *path);
bool is_valid_extension(const char *path, const char *extension);
// validate program args
bool is_valid_input(int argc, char **argv);
//
// rgb
bool is_rgb_id(t_string *line);
bool rgb_str_is_valid(t_string **colors, int colors_num);
bool extract_rgb(t_map *map, t_string *id, t_string *colors);
//
// free
//@TODO: mudar nome
void cleanup_map(t_map *map);



// ======
// Window
// ======
void init_window(t_window *win);



// =======
// Raycast
// =======



// ======
// Render
// ======



// ==========
// Event Loop
// ==========
void event_loop(t_game *game);
// keys
int	handle_key(int keycode, t_game *game);
// free
int	end_game(t_game *game);



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
void LOG_DEBUG_MAP_NUMS(t_map *map);

#endif /*CUB3D_H*/
