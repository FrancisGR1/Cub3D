/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_extraction.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 11:23:28 by frmiguel          #+#    #+#             */
/*   Updated: 2025/07/30 11:26:48 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_extraction.h"
#include "cub3d.h"
#include "minunit.h"

#define MAX_ROWS_TEST MAX_ROWS + 1
#define MAX_COLS_TEST MAX_COLS + 1

#define TEST_ASSETS_DIR "test_assets/"

// ===============
// EXPECTED VALUES
// ===============

// Texture paths initialization
const char *g_expected_textures[MAX_TEXTURES] = {
    "/home/francisco/Documents/42_School/04/Cub3D/assets/textures/bricks.xpm", // NO
    "/home/francisco/Documents/42_School/04/Cub3D/assets/textures/bricks.xpm", // SO
    "/home/francisco/Documents/42_School/04/Cub3D/assets/textures/bricks.xpm", // WE
    "/home/francisco/Documents/42_School/04/Cub3D/assets/textures/bricks.xpm"  // EA
};

// Floor and ceiling colors
t_rgb g_expected_floor = {220, 100, 0};
t_rgb g_expected_ceiling = {225, 30, 0};
t_rgb g_expected_nothing = {-1, -1, -1};

// Map layout
int g_expected_map[][MAX_COLS_TEST] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1},
    {1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,-1},
    {1,0,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,-1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,-1},
    {1,1,1,1,1,1,1,1,1,0,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,-1},
    {1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,-1},
    {1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,0,0,0,0,0,1,0,0,0,1,-1},
    {1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,0,0,1,0,0,0,1,-1},
    {1,1,0,0,0,0,0,0,1,1,0,1,0,1,0,1,1,1,0,0,0,0,0,0,1,0,0,0,1,-1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,1,-1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,0,0,1,0,0,0,1,-1},
    {1,1,0,0,0,0,0,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,1,0,78,0,1,1,1,-1},
    {1,1,1,1,0,1,1,1,1,1,1,0,1,0,1,1,0,1,1,1,1,0,1,0,0,0,1,-1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1},
    {-1}
};

#define EXPECTED_ROWS (sizeof(g_expected_map) / sizeof(g_expected_map[0]))


// Utils
void test_extracted_textures(const char **expected_textures, t_string *map_textures[]);
void test_extracted_rgb(t_rgb expected_rgb, t_rgb map_rgb);
void test_extracted_map(const int expected_map[][MAX_COLS_TEST], t_dynamic_array *map);

// TEXTURAS
MU_TEST(test_extract_map_data_textures_2_equal_ids)
{
	LOG_INFO(BHBLU "textures_2_equal_ids" RESET);
	const char *test_file = TEST_ASSETS_DIR "textures_error_2_equal_ids.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	//@TODO este check mais os dois últimos antes de cleanup_map() podem
	//ser só uma função
	mu_check(map->parse_error == true);
	mu_check(map->textures[NORTH] != NULL);
	mu_check(map->textures[SOUTH] != NULL);
	mu_check(map->textures[WEST] == NULL); //dá erro a partir do west
	mu_check(map->textures[EAST] == NULL);
	test_extracted_rgb(g_expected_nothing, map->floor);
	test_extracted_rgb(g_expected_nothing, map->ceiling);
	mu_check(map->rows->len == 0);
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_textures_wrong_id)
{
	LOG_INFO(BHBLU "textures_wrong_id" RESET);
	// testar 2 ids das iguais das texturas
	const char *test_file = TEST_ASSETS_DIR "textures_error_wrong_id.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	mu_check(map->textures[NORTH] != NULL);
	mu_check(map->textures[SOUTH] != NULL);
	mu_check(map->textures[WEST] == NULL); //dá erro a partir do west
	mu_check(map->textures[EAST] == NULL);
	test_extracted_rgb(g_expected_nothing, map->floor);
	test_extracted_rgb(g_expected_nothing, map->ceiling);
	mu_check(map->rows->len == 0);
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_textures_wrong_path)
{
	LOG_INFO(BHBLU "textures_wrong_path" RESET);
	// testar 2 ids das iguais das texturas
	const char *test_file = TEST_ASSETS_DIR "textures_error_wrong_path.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	mu_check(map->textures[NORTH] != NULL);
	mu_check(map->textures[SOUTH] != NULL);
	mu_check(map->textures[WEST] != NULL);
	mu_check(map->textures[EAST] == NULL); //dá erro a partir do east
	test_extracted_rgb(g_expected_nothing, map->floor);
	test_extracted_rgb(g_expected_nothing, map->ceiling);
	mu_check(map->rows->len == 0);
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}
//@TODO:
//MU_TEST(test_extract_map_data_textures_wrong_extension);

MU_TEST(test_extract_map_data_textures_more_than_2_strings)
{
	LOG_INFO(BHBLU "textures_more_than_2_strings" RESET);
	// testar 2 ids das iguais das texturas
	const char *test_file = TEST_ASSETS_DIR "textures_error_more_than_2_strings.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	mu_check(map->textures[NORTH] != NULL);
	mu_check(map->textures[SOUTH] == NULL); //dá erro a partir de south
	mu_check(map->textures[WEST] == NULL);
	mu_check(map->textures[EAST] == NULL);
	test_extracted_rgb(g_expected_nothing, map->floor);
	test_extracted_rgb(g_expected_nothing, map->ceiling);
	mu_check(map->rows->len == 0);
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_textures_1_string)
{
	LOG_INFO(BHBLU "textures_1_string" RESET);
	// testar 2 ids das iguais das texturas
	const char *test_file = TEST_ASSETS_DIR "textures_error_1_string.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	mu_check(map->textures[NORTH] != NULL);
	mu_check(map->textures[SOUTH] != NULL); 
	mu_check(map->textures[WEST] == NULL); //dá erro a partir de south
	mu_check(map->textures[EAST] == NULL);
	test_extracted_rgb(g_expected_nothing, map->floor);
	test_extracted_rgb(g_expected_nothing, map->ceiling);
	mu_check(map->rows->len == 0);
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST_SUITE(test_suite_error_extract_textures)
{
	// testar texturas em formato errado
	MU_RUN_TEST(test_extract_map_data_textures_2_equal_ids);
	MU_RUN_TEST(test_extract_map_data_textures_wrong_id);
	MU_RUN_TEST(test_extract_map_data_textures_wrong_path);
	//@TODO: tenho de implementar o código para passar este teste:
	//MU_RUN_TEST(test_extract_map_data_textures_wrong_extension);
	MU_RUN_TEST(test_extract_map_data_textures_more_than_2_strings);
	MU_RUN_TEST(test_extract_map_data_textures_1_string);
}

// CORES
MU_TEST(test_extract_map_data_colors_more_than_3_values)
{
	LOG_INFO(BHBLU "Test: colors_more_than_3_values" RESET);
	const char *test_file = TEST_ASSETS_DIR "colors_more_than_3_values.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_nothing, map->ceiling); // o erro acontece aqui
	mu_check(map->rows->len == 0);
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_colors_less_than_3_values)
{
	LOG_INFO(BHBLU "Test: colors_less_than_3_values" RESET);
	const char *test_file = TEST_ASSETS_DIR "colors_less_than_3_values.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_nothing, map->floor); // o erro acontece aqui
	test_extracted_rgb(g_expected_nothing, map->ceiling);
	mu_check(map->rows->len == 0);
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_colors_wrong_id)
{
	LOG_INFO(BHBLU "Test: colors_wrong_id" RESET);
	const char *test_file = TEST_ASSETS_DIR "colors_wrong_id.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_nothing, map->ceiling); // o erro acontece aqui
	mu_check(map->rows->len == 0);
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_colors_equal_ids)
{
	LOG_INFO(BHBLU "Test: colors_equal_ids" RESET);
	const char *test_file = TEST_ASSETS_DIR "colors_equal_ids.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_nothing, map->ceiling); // o erro acontece aqui
	mu_check(map->rows->len == 0);
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_colors_more_than_2_strings)
{
	LOG_INFO(BHBLU "Test: colors_more_than_2_strings" RESET);
	const char *test_file = TEST_ASSETS_DIR "colors_more_than_2_strings.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_nothing, map->ceiling); // o erro acontece aqui
	mu_check(map->rows->len == 0);
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_colors_1_string)
{
	LOG_INFO(BHBLU "Test: colors_1_string" RESET);
	const char *test_file = TEST_ASSETS_DIR "colors_1_string.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_nothing, map->ceiling); // o erro acontece aqui
	mu_check(map->rows->len == 0);
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_colors_above_255)
{
	LOG_INFO(BHBLU "Test: colors_above_255" RESET);
	const char *test_file = TEST_ASSETS_DIR "colors_above_255.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	t_rgb expected_floor = {220, 100, 255};
	test_extracted_rgb(expected_floor, map->floor);
	test_extracted_rgb(g_expected_nothing, map->ceiling); // o erro acontece aqui
	mu_check(map->rows->len == 0);
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);

}

MU_TEST(test_extract_map_data_colors_below_0)
{
	LOG_INFO(BHBLU "Test: colors_below_0" RESET);
	const char *test_file = TEST_ASSETS_DIR "colors_below_0.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	t_rgb expected_floor = {220, 100, 255};
	test_extracted_rgb(expected_floor, map->floor);
	test_extracted_rgb(g_expected_nothing, map->ceiling); // o erro acontece aqui
	mu_check(map->rows->len == 0);
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST_SUITE(test_suite_error_extract_colors)
{
	// testar cores em formato errado
	MU_RUN_TEST(test_extract_map_data_colors_more_than_3_values);
	MU_RUN_TEST(test_extract_map_data_colors_less_than_3_values);
	MU_RUN_TEST(test_extract_map_data_colors_wrong_id);
	MU_RUN_TEST(test_extract_map_data_colors_equal_ids);
	MU_RUN_TEST(test_extract_map_data_colors_more_than_2_strings);
	MU_RUN_TEST(test_extract_map_data_colors_1_string);
	MU_RUN_TEST(test_extract_map_data_colors_above_255);
	MU_RUN_TEST(test_extract_map_data_colors_below_0);
}


// test mixed lines of colors and textures
MU_TEST(test_extract_map_data_mixed_colors_first)
{
	LOG_INFO(BHBLU "Test: mixed_colors_first" RESET);
	const char *test_file = TEST_ASSETS_DIR "mixed_colors_first.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == false);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == EXPECTED_ROWS);
	mu_check(map->player_position_is_set == true);
	test_extracted_map(g_expected_map, map->rows);
	cleanup_map(map);

}

MU_TEST(test_extract_map_data_mixed_colors_in_between_textures)
{
	LOG_INFO(BHBLU "Test: mixed_colors_in_between_textures" RESET);
	const char *test_file = TEST_ASSETS_DIR "mixed_colors_first.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == false);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == EXPECTED_ROWS);
	mu_check(map->player_position_is_set == true);
	test_extracted_map(g_expected_map, map->rows);
	cleanup_map(map);
}

MU_TEST_SUITE(test_suite_textures_and_colors_mixed)
{
	// testar linhas de cores e texturas sem ordem lógica
	MU_RUN_TEST(test_extract_map_data_mixed_colors_first);
	MU_RUN_TEST(test_extract_map_data_mixed_colors_in_between_textures);
}


// MAPA
//file position
MU_TEST(test_extract_map_data_map_at_beginning_of_file)
{
	LOG_INFO(BHBLU "Test: map_at_beginning_of_file" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_at_beginning_of_file.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	mu_check(map->textures[NORTH] == NULL);
	mu_check(map->textures[SOUTH] == NULL);
	mu_check(map->textures[WEST] == NULL);
	mu_check(map->textures[EAST] == NULL);
	test_extracted_rgb(g_expected_nothing, map->floor);
	test_extracted_rgb(g_expected_nothing, map->ceiling);
	mu_check(map->rows->len == 0); //@ASSUMPTION: fails at first line
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_map_at_middle_of_file)
{
	LOG_INFO(BHBLU "Test: map_at_middle_of_file" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_at_middle_of_file.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_nothing, map->floor);
	test_extracted_rgb(g_expected_nothing, map->ceiling);
	mu_check(map->rows->len == 0);
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_map_with_empty_lines)
{
	LOG_INFO(BHBLU "Test: map_with_empty_lines" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_with_empty_lines.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == 1);
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_map_divided_in_multiple_parts_throughout_file)
{
	LOG_INFO(BHBLU "Test: map_divided_in_multiple_parts_throughout_file" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_divided_in_multiple_parts_throughout_file.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	mu_check(map->textures[NORTH] != NULL);
	mu_check(map->textures[NORTH]->data != NULL);
	mu_assert_string_eq(g_expected_textures[NORTH], map->textures[NORTH]->data);
	mu_check(map->textures[SOUTH] == NULL);
	mu_check(map->textures[WEST] == NULL);
	mu_check(map->textures[EAST] == NULL);
	test_extracted_rgb(g_expected_nothing, map->floor);
	test_extracted_rgb(g_expected_nothing, map->ceiling);
	mu_check(map->rows->len == 0); //ASSUMPTION: doesnt allocate space to any row
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}


MU_TEST(test_extract_map_data_map_with_multiple_player_positions)
{
	LOG_INFO(BHBLU "Test: map_divided_in_multiple_player_positions" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_with_multiple_player_positions.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == 8); //A 2ª posição está na 9a linha
	mu_check(map->player_position_is_set == true);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_map_player_position_case_sensitivity)
{
	LOG_INFO(BHBLU "Test: map_player_position_case_sensitivity" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_player_position_case_sensitivity.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == 11); //12ª linha tem "n"
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_map_player_orientation_invalid_character)
{
	LOG_INFO(BHBLU "Test: map_player_orientation_invalid_character" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_player_orientation_invalid_character.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == 11); //12ª linha tem "X"
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_map_player_no_position)
{
	LOG_INFO(BHBLU "Test: map_player_no_position" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_player_no_position.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	//@TODO: isto está mal - não devia dar erro;
	//@FIXME: o programa não está a mandar erro quando falta a posição do jogador
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == EXPECTED_ROWS);
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

//size
//@TODO: o que é um mapa demasiado pequeno?
MU_TEST(test_extract_map_data_map_smallest)
{
	LOG_INFO(BHBLU "Test: map_data_map_smallest" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_smallest.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == false);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == 3); //@NOTE: o mapa mais pequeno tem 3l
	mu_check(map->player_position_is_set == true);
	int expected_map[][MAX_COLS_TEST] = {
		{1, -1},
		{1, 87, 1, -1},
		{1, -1}
	};
	test_extracted_map(expected_map, map->rows);
	cleanup_map(map);
}

//@TODO: o que é um mapa demasiado grande?
//tenho de definir o tamanho se não o for definido por mim
MU_TEST(test_extract_map_data_map_too_big)
{
	LOG_INFO(BHBLU "Test: map_data_map_too_big" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_data_map_too_big.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == EXPECTED_ROWS);
	mu_check(map->player_position_is_set == false);
	//@TODO: test extracted map
	cleanup_map(map);
}

//shape
//@TODO: criar array de ints congruentes com os novos mapas, mudar rows->len
//todos estes maps são válidos
MU_TEST(test_extract_map_data_map_non_rectangular1)
{
	LOG_INFO(BHBLU "Test: map_non_rectangular1" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_non_rectangular1.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == false);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == 13); //@ASSUMPTION: este mapa tem 13l
	mu_check(map->player_position_is_set == true);
	int expected_map[][MAX_COLS_TEST] = {
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 1},
		{1, 0, 0, 1},
		{1, 1},
		{-1}
	};
	test_extracted_map(expected_map, map->rows);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_map_non_rectangular2)
{
	LOG_INFO(BHBLU "Test: map_non_rectangular2" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_non_rectangular2.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == false);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == 15); //@ASSUMPTION: mapa tem 15l
	mu_check(map->player_position_is_set == true);
	int expected_map[][MAX_COLS_TEST] = {
		{1, 1, 1, 1, 1,-1},
		{1, 0, 0, 83, 0, 0, 0,-1},
		{1, 0, 0, 0, 0, -1},
		{1, 0, 0, 0, 1, -1},
		{1, 0, 0, 0, 1, -1},
		{1, 0, 1, -1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 1},
		{1, 0, 0, 1},
		{1, 1},
		{-1}
	};
	test_extracted_map(expected_map, map->rows);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_map_longest_rows)
{
	LOG_INFO(BHBLU "Test: map_longest_rows" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_longest_rows.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == false);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == 7); //@ASSUMPTION: este mapa tem 7l
	mu_check(map->player_position_is_set == true);
	//@TODO - NOW
	//@BUG: porque é que isto não estã a passar os 1s todos da primeira linha?
	//para no 8º
	//-> é provável que tenha a ver com realocações no darr
	test_extracted_map(g_expected_map_longest_rows, map->rows);
	LOG_DEBUG_MAP_NUMS(map);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_map_row_too_long)
{
	LOG_INFO(BHBLU "Test: map_row_too_long" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_row_too_long.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == 2); //@ASSUMPTION: erro na 2ª linha
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_map_max_rows)
{
	LOG_INFO(BHBLU "Test: map_max_rows" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_max_rows.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == false);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == MAX_ROWS);
	mu_check(map->player_position_is_set == true);
	test_extracted_map(g_expected_map_max_rows, map->rows);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_map_too_many_rows)
{
	LOG_INFO(BHBLU "Test: map_too_many_rows" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_too_many_rows.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == MAX_ROWS);
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

//walls
//todos estes maps são inválidos
//@TODO: não sei bem se este é inválido? só paredes?
MU_TEST(test_extract_map_data_map_only_walls)
{
	LOG_INFO(BHBLU "Test: map_data_map_only_walls" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_data_map_only_walls.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == EXPECTED_ROWS);
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_map_no_walls)
{
	LOG_INFO(BHBLU "Test: map_data_map_no_walls" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_data_map_no_walls.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == EXPECTED_ROWS);
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_map_open_middle_top)
{
	LOG_INFO(BHBLU "Test: map_data_map_open_middle_top" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_data_map_open_middle_top.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == 0);
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_map_open_middle_right)
{
	LOG_INFO(BHBLU "Test: map_data_map_open_middle_right" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_data_map_open_middle_right.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == 6); //@ASSUMPTION: falha na linha 6
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_map_open_middle_left)
{
	LOG_INFO(BHBLU "Test: map_data_map_open_middle_left" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_data_map_open_middle_left.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == 6); //@ASSUMPTION: falha na linha 6
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_map_open_middle_bottom)
{
	LOG_INFO(BHBLU "Test: map_data_map_open_middle_bottom" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_data_map_open_middle_bottom.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == EXPECTED_ROWS); //@ASSUMPTION: falha na última linha
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_map_open_top_right_corner)
{
	LOG_INFO(BHBLU "Test: map_data_map_open_top_right_corner" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_data_map_open_top_right_corner.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == 1); //@ASSUMPTION: falha na 1a linha
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_map_open_top_left_corner)
{
	LOG_INFO(BHBLU "Test: map_data_map_open_top_left_corner" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_data_map_open_top_left_corner.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == 1); //@ASSUMPTION: falha na 1a linha
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_map_open_bottom_right_corner)
{
	LOG_INFO(BHBLU "Test: map_data_map_open_bottom_right_corner" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_data_map_open_bottom_right_corner.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == EXPECTED_ROWS); //@ASSUMPTION: falha na útlima linha
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_map_open_bottom_left_corner)
{
	LOG_INFO(BHBLU "Test: map_data_map_open_bottom_left_corner" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_data_map_open_bottom_left_corner.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == true);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == EXPECTED_ROWS); //@ASSUMPTION: falha na útlima linha
	mu_check(map->player_position_is_set == false);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_map_one_space_in_each_line)
{
	LOG_INFO(BHBLU "Test: map_one_space_in_each_line" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_one_space_in_each_line.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == false);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == EXPECTED_ROWS);
	mu_check(map->player_position_is_set == true);
	test_extracted_map(g_expected_map, map->rows);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_map_multiple_spaces_in_each_line)
{
	LOG_INFO(BHBLU "Test: map_multiple_spaces_in_each_line" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_multiple_spaces_in_each_line.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == false);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == EXPECTED_ROWS);
	mu_check(map->player_position_is_set == true);
	test_extracted_map(g_expected_map, map->rows);
	cleanup_map(map);
}

MU_TEST(test_extract_map_data_map_tabs_in_each_line)
{
	LOG_INFO(BHBLU "Test: map_tabs_in_each_line" RESET);
	const char *test_file = TEST_ASSETS_DIR "map_tabs_in_each_line.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	int fd = open(test_file, O_RDONLY);
	if (fd <= 2)
	{
		fprintf(stderr, "Error: Can't find %s\n", test_file);
		exit(1);
	}
	t_map *map = init_map();
	extract_map_data(fd, map);
	mu_check(map != NULL);
	mu_check(map->parse_error == false);
	test_extracted_textures(g_expected_textures, map->textures);
	test_extracted_rgb(g_expected_floor, map->floor);
	test_extracted_rgb(g_expected_ceiling, map->ceiling);
	mu_check(map->rows->len == EXPECTED_ROWS);
	mu_check(map->player_position_is_set == true);
	test_extracted_map(g_expected_map, map->rows);
	cleanup_map(map);
}

MU_TEST_SUITE(test_suite_map)
{
	//testar linhas de cores e texturas sem ordem lógica
	//file position
	//MU_RUN_TEST(test_extract_map_data_map_at_beginning_of_file);
	//MU_RUN_TEST(test_extract_map_data_map_at_middle_of_file);
	//@FIXME: devia dar erro
	//as linhas vazias não podem existir
	//na fase de extração do mapa
	//MU_RUN_TEST(test_extract_map_data_map_with_empty_lines);
	//MU_RUN_TEST(test_extract_map_data_map_divided_in_multiple_parts_throughout_file);
	//player
	//MU_RUN_TEST(test_extract_map_data_map_with_multiple_player_positions);
	//MU_RUN_TEST(test_extract_map_data_map_player_position_case_sensitivity);
	//MU_RUN_TEST(test_extract_map_data_map_player_orientation_invalid_character);
	//@FIXME - dá erro porque não considera
	//"parse_error" se faltar a posição do jogador
	//MU_RUN_TEST(test_extract_map_data_map_player_no_position);
	////size
	///@TODO NOW estes:
	//MU_RUN_TEST(test_extract_map_data_map_smallest);
	MU_RUN_TEST(test_extract_map_data_map_longest_rows);
	//MU_RUN_TEST(test_extract_map_data_map_row_too_long);
	//MU_RUN_TEST(test_extract_map_data_map_max_rows);
	//MU_RUN_TEST(test_extract_map_data_map_too_many_rows);
	////shape
	//MU_RUN_TEST(test_extract_map_data_map_non_rectangular1);
	//MU_RUN_TEST(test_extract_map_data_map_non_rectangular2);
	////walls
	///@TODO: fazer func que teste a validade das linhas
	///durante o parsing
	//MU_RUN_TEST(test_extract_map_data_map_only_walls);
	//MU_RUN_TEST(test_extract_map_data_map_no_walls);
	//MU_RUN_TEST(test_extract_map_data_map_open_middle_top);
	//MU_RUN_TEST(test_extract_map_data_map_open_middle_right);
	//MU_RUN_TEST(test_extract_map_data_map_open_middle_left);
	//MU_RUN_TEST(test_extract_map_data_map_open_middle_bottom);
	//MU_RUN_TEST(test_extract_map_data_map_open_top_right_corner);
	//MU_RUN_TEST(test_extract_map_data_map_open_top_left_corner);
	//MU_RUN_TEST(test_extract_map_data_map_open_bottom_right_corner);
	//MU_RUN_TEST(test_extract_map_data_map_open_bottom_left_corner);
	//whitespaces
	//MU_RUN_TEST(test_extract_map_data_map_one_space_in_each_line);
	//MU_RUN_TEST(test_extract_map_data_map_multiple_spaces_in_each_line);
	//MU_RUN_TEST(test_extract_map_data_map_tabs_in_each_line);
}

int main()
{
	logger_initConsoleLogger(stderr);
	logger_setLevel(0);
	// Grupos de Testes
	//MU_RUN_TEST(test_suite_error_extract_textures);
	//MU_RUN_TEST(test_suite_error_extract_colors);
	//
	//MU_RUN_TEST(test_suite_textures_and_colors_mixed);
	MU_RUN_TEST(test_suite_map);
	MU_REPORT();
	//@NOTE: limpo todos os fds abertos no fim
	//tenho de fazer isto senão get_next_line()
	//guarda linhas do último teste
	//o que leva a falsos negativos
	size_t i;
	i = 3;
	while (i < MAX_FDS)
		close(i++);
	return MU_EXIT_CODE;
}

// utils
void test_extracted_textures(const char **expected_textures, t_string *map_textures[])
{
	for (int i = 0; i < MAX_TEXTURES && map_textures[i]; i++)
	{
		mu_check(map_textures[i] != NULL);
		mu_check(map_textures[i]->data != NULL);
		mu_assert_string_eq(expected_textures[i], map_textures[i]->data);
	}
}

void test_extracted_rgb(t_rgb expected_rgb, t_rgb map_rgb)
{
	LOG_TRACE("\nTesting:\nexpected: %d %d %d\nextracted: %d %d %d",\
			expected_rgb.r,expected_rgb.g, expected_rgb.b,\
			map_rgb.r,map_rgb.g, map_rgb.b);
	mu_assert_int_eq(expected_rgb.r, map_rgb.r);
	mu_assert_int_eq(expected_rgb.g, map_rgb.g);
	mu_assert_int_eq(expected_rgb.b, map_rgb.b);
}

void test_extracted_map(const int expected_map[MAX_ROWS_TEST][MAX_COLS_TEST], t_dynamic_array *map)
{
	mu_check(map != NULL);
	mu_check(map->len != 0);
	LOG_DEBUG("test extraction");
	LOG_TRACE("map len: %zu", map->len);
	int i = 0;
	for (i = 0; i < MAX_ROWS_TEST && expected_map[i][0] != -1 && i < map->len; i++)
	{
		int map_to_test_total_cols = get_map_row_size(map, i);
		for (int j = 0; j < map_to_test_total_cols && expected_map[i][j] != -1; j++)
		{
			int expected_int = expected_map[i][j];
			int test_int = get_map_value(map, i, j);
			LOG_TRACE("comparing: expected: %zu test: %zu at [%d][%d]", expected_int, test_int, i, j);
			mu_assert_int_eq(expected_int, test_int);
		}
	}
	mu_check(i == map->len);
}
