#ifndef CUB3D_H
# define CUB3D_H

#include "libft.h"
#include "logger.h"
#include "mlx.h"

# define MAP_INITIAL_ROWS 30
# define MAP_INITIAL_COLS 30

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

typedef struct s_map
{
	t_string *textures[MAX_TEXTURES];
	t_rgb floor;
	t_rgb ceiling;
	t_dynamic_array *rows;
	bool player_position_is_set;
} t_map;

typedef struct s_game
{
	t_map *map;
} t_game;

//@TEMP:
#ifndef LOG_LEVEL
# define LOG_LEVEL 5
#endif

// parse
t_map *extract_map_data(char *file_path);
void cleanup_map(t_map *map);


//debug
void LOG_DEBUG_MAP_NUMS(t_map *map);

#endif /*CUB3D_H*/
