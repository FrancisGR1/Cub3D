#ifndef CUB3D_H
# define CUB3D_H

#include "libft.h"
#include "logger.h"
#include "mlx.h"


//@QUESTION: provavelmente seria melhor definir tamanhos iniciais como máximos? 
//de modo a evitar realocações, embora os maps que levem a isso sejam raros
# define MAP_INITIAL_ROWS 30
# define MAP_INITIAL_COLS 30

//@QUESTION: é possível fazer isto?
//eles não dizem no subject o tamanho máximo
# define MAX_ROWS 100
# define MAX_COLS 100


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

typedef struct s_map
{
	t_string *textures[MAX_TEXTURES];
	t_rgb floor;
	t_rgb ceiling;
	t_dynamic_array *rows;
	enum e_extraction_phase extraction_phase;
	bool player_position_is_set;
	bool parse_error;
} t_map;

typedef struct s_game
{
	t_map *map;
} t_game;

//@TEMP:
#ifndef LOG_LEVEL
# define LOG_LEVEL 5
#endif

// ===============
// Data Extraction
// ===============

// initialization
t_map *init_map(void);

// data extraction
t_map *extract_map_data(int fd, t_map *map);
t_map *extract_map_data_new(int fd, t_map *map);
//-utils
bool extract_texture(t_map *map, t_string *id, t_string *texture_path);
int map_value_from_char(char c);

// validate
//-map data
bool is_valid_map_num(char c);
bool is_valid_map_player_pos(char c);
bool starts_with_texture_id(t_string *line);
bool check_if_map_nums_are_valid(t_map *map);
//-file
int is_valid_file_path(const char *path);
bool is_valid_extension(const char *path, const char *extension);
//-program args
bool is_valid_input(int argc, char **argv);

// rgb
bool is_rgb_id(t_string *line);
bool rgb_str_is_valid(t_string **colors, int colors_num);
bool extract_rgb(t_map *map, t_string *id, t_string *colors);

// free
void cleanup_map(t_map *map);

// ======
// Window
// ======

// =======
// Raycast
// =======

// ======
// Render
// ======

// ==========
// User Input
// ==========

// =====
// Utils
// =====

//accessors
int get_col_value(t_dynamic_array *row, int col);
int get_map_value(t_dynamic_array *map, int row, int col);
void set_map_value(t_dynamic_array *map, int value, int row, int col);
int get_map_size(t_dynamic_array *map);
int get_map_row_size(t_dynamic_array *map, int row);

//-debug
void LOG_DEBUG_MAP_NUMS(t_map *map);

#endif /*CUB3D_H*/
