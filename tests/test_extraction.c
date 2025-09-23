/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_extraction.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 11:23:28 by frmiguel          #+#    #+#             */
/*   Updated: 2025/09/13 18:59:57 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_extraction.h"
#include "cub3d.h"
#include "vector.h"
#include "minunit.h"

#define MAX_ROWS_TEST MAX_ROWS + 1
#define MAX_COLS_TEST MAX_COLS + 1

# define LOG_LEVEL 4

#define	MAPS_DIR	"assets/maps/"
#define	TEXTURES_DIR	"assets/textures/"

// ===============
// EXPECTED VALUES
// ===============

// Texture paths initialization
const char *g_expected_textures[MAX_TEXTURES] = {
    TEXTURES_DIR "bricks.xpm", // NO
    TEXTURES_DIR "bricks.xpm", // SO
    TEXTURES_DIR "bricks.xpm", // WE
    TEXTURES_DIR "bricks.xpm"  // EA
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
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,1,-1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,0,0,1,0,0,0,1,-1},
	{1,1,0,0,0,0,0,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,1,0,78,0,1,1,1,-1},
	{1,1,1,1,0,1,1,1,1,1,1,0,1,0,1,1,0,1,1,1,1,0,1,0,0,0,1,-1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1},
	{-1}
};

#define EXPECTED_ROWS ((sizeof(g_expected_map) - 1) / sizeof(g_expected_map[0]))

t_file_data *init_map(void)
{
	return (malloc(sizeof(t_file_data)));
}

//Nota: tive de fazer isto para a gnl() não deixar restos e
//dar falsos positivos
void empty_gnl(const char *path)
{
	t_string *str;

	int fd = open(path, O_RDONLY);
	while((str = get_next_line_to_str(fd)) != NULL)
		str_deallocate(get_next_line_to_str(fd));
	close(fd);
}


// Utils
void test_extracted_textures(const char **expected_textures, t_string *map_textures[]);
void test_extracted_rgb(t_rgb expected_rgb, t_rgb map_rgb);
void test_extracted_map(const int expected_map[][MAX_COLS_TEST], t_dynamic_array *map);
// TEXTURAS
MU_TEST(test_extract_map_data_textures_2_equal_ids)
{
	LOG_INFO(BHBLU "textures_2_equal_ids" RESET);
	const char *test_file = MAPS_DIR "textures_error_2_equal_ids.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data *ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	//@TODO este check mais os dois últimos antes de cleanup_map() 
	//podem ser só uma função
	mu_check(ed->parser_error == true);
	mu_check(ed->textures[NORTH] != NULL);
	mu_check(ed->textures[SOUTH] != NULL);
	mu_check(ed->textures[WEST] == NULL); //@ASSUMPTION: dá erro a partir do west
	mu_check(ed->textures[EAST] == NULL);
	test_extracted_rgb(g_expected_nothing, ed->floor);
	test_extracted_rgb(g_expected_nothing, ed->ceiling);
	mu_check(ed->rows->len == 0);
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}

MU_TEST(test_extract_map_data_textures_wrong_id)
{
	LOG_INFO(BHBLU "textures_wrong_id" RESET);
	// testar 2 ids das iguais das texturas
	const char *test_file = MAPS_DIR "textures_error_wrong_id.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	mu_check(ed->textures[NORTH] != NULL);
	mu_check(ed->textures[SOUTH] != NULL);
	mu_check(ed->textures[WEST] == NULL); //dá erro a partir do west
	mu_check(ed->textures[EAST] == NULL);
	test_extracted_rgb(g_expected_nothing, ed->floor);
	test_extracted_rgb(g_expected_nothing, ed->ceiling);
	mu_check(ed->rows->len == 0);
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}

MU_TEST(test_extract_map_data_textures_wrong_path)
{
	LOG_INFO(BHBLU "textures_wrong_path" RESET);
	// testar 2 ids das iguais das texturas
	const char *test_file = MAPS_DIR "textures_error_wrong_path.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	mu_check(ed->textures[NORTH] != NULL);
	mu_check(ed->textures[SOUTH] != NULL);
	mu_check(ed->textures[WEST] != NULL);
	mu_check(ed->textures[EAST] == NULL); //dá erro a partir do east
	test_extracted_rgb(g_expected_nothing, ed->floor);
	test_extracted_rgb(g_expected_nothing, ed->ceiling);
	mu_check(ed->rows->len == 0);
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}

MU_TEST(test_extract_map_data_textures_wrong_extension)
{
	LOG_INFO(BHBLU "textures_error_wrong_extension" RESET);
	// testar 2 ids das iguais das texturas
	const char *test_file = MAPS_DIR "textures_error_wrong_extension.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	mu_check(ed->textures[NORTH] != NULL);
	mu_check(ed->textures[SOUTH] == NULL); //@ASSUMPTION: dá error a partir de SO
	mu_check(ed->textures[WEST] == NULL);
	mu_check(ed->textures[EAST] == NULL);
	test_extracted_rgb(g_expected_nothing, ed->floor);
	test_extracted_rgb(g_expected_nothing, ed->ceiling);
	mu_check(ed->rows->len == 0);
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_textures_more_than_2_strings)
{
	LOG_INFO(BHBLU "textures_more_than_2_strings" RESET);
	// testar 2 ids das iguais das texturas
	const char *test_file = MAPS_DIR "textures_error_more_than_2_strings.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	mu_check(ed->textures[NORTH] != NULL);
	mu_check(ed->textures[SOUTH] == NULL); //@ASSUMPTION: dá erro a partir de south
	mu_check(ed->textures[WEST] == NULL);
	mu_check(ed->textures[EAST] == NULL);
	test_extracted_rgb(g_expected_nothing, ed->floor);
	test_extracted_rgb(g_expected_nothing, ed->ceiling);
	mu_check(ed->rows->len == 0);
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_textures_1_string)
{
	LOG_INFO(BHBLU "textures_1_string" RESET);
	// testar 2 ids das iguais das texturas
	const char *test_file = MAPS_DIR "textures_error_1_string.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	mu_check(ed->textures[NORTH] != NULL);
	mu_check(ed->textures[SOUTH] != NULL); 
	mu_check(ed->textures[WEST] == NULL); //dá erro a partir de south
	mu_check(ed->textures[EAST] == NULL);
	test_extracted_rgb(g_expected_nothing, ed->floor);
	test_extracted_rgb(g_expected_nothing, ed->ceiling);
	mu_check(ed->rows->len == 0);
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST_SUITE(test_suite_error_extract_textures)
{
	// testar texturas em formato errado
	MU_RUN_TEST(test_extract_map_data_textures_2_equal_ids);
	MU_RUN_TEST(test_extract_map_data_textures_wrong_id);
	MU_RUN_TEST(test_extract_map_data_textures_wrong_path);
	MU_RUN_TEST(test_extract_map_data_textures_wrong_extension);
	MU_RUN_TEST(test_extract_map_data_textures_more_than_2_strings);
	MU_RUN_TEST(test_extract_map_data_textures_1_string);
}
//
//// CORES
MU_TEST(test_extract_map_data_colors_more_than_3_values)
{
	LOG_INFO(BHBLU "Test: colors_more_than_3_values" RESET);
	const char *test_file = MAPS_DIR "colors_more_than_3_values.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data *ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_nothing, ed->ceiling); // o erro acontece aqui
	mu_check(ed->rows->len == 0);
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_colors_less_than_3_values)
{
	LOG_INFO(BHBLU "Test: colors_less_than_3_values" RESET);
	const char *test_file = MAPS_DIR "colors_less_than_3_values.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_nothing, ed->floor); // o erro acontece aqui
	test_extracted_rgb(g_expected_nothing, ed->ceiling);
	mu_check(ed->rows->len == 0);
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_colors_wrong_id)
{
	LOG_INFO(BHBLU "Test: colors_wrong_id" RESET);
	const char *test_file = MAPS_DIR "colors_wrong_id.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_nothing, ed->ceiling); // o erro acontece aqui
	mu_check(ed->rows->len == 0);
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_colors_equal_ids)
{
	LOG_INFO(BHBLU "Test: colors_equal_ids" RESET);
	const char *test_file = MAPS_DIR "colors_equal_ids.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_nothing, ed->ceiling); // o erro acontece aqui
	mu_check(ed->rows->len == 0);
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_colors_more_than_2_strings)
{
	LOG_INFO(BHBLU "Test: colors_more_than_2_strings" RESET);
	const char *test_file = MAPS_DIR "colors_more_than_2_strings.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_nothing, ed->ceiling); // o erro acontece aqui
	mu_check(ed->rows->len == 0);
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_colors_1_string)
{
	LOG_INFO(BHBLU "Test: colors_1_string" RESET);
	const char *test_file = MAPS_DIR "colors_1_string.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_nothing, ed->ceiling); // o erro acontece aqui
	mu_check(ed->rows->len == 0);
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_colors_above_255)
{
	LOG_INFO(BHBLU "Test: colors_above_255" RESET);
	const char *test_file = MAPS_DIR "colors_above_255.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	test_extracted_textures(g_expected_textures, ed->textures);
	t_rgb expected_floor = {220, 100, 255};
	test_extracted_rgb(expected_floor, ed->floor);
	test_extracted_rgb(g_expected_nothing, ed->ceiling); // o erro acontece aqui
	mu_check(ed->rows->len == 0);
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_colors_below_0)
{
	LOG_INFO(BHBLU "Test: colors_below_0" RESET);
	const char *test_file = MAPS_DIR "colors_below_0.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	test_extracted_textures(g_expected_textures, ed->textures);
	t_rgb expected_floor = {220, 100, 255};
	test_extracted_rgb(expected_floor, ed->floor);
	test_extracted_rgb(g_expected_nothing, ed->ceiling); // o erro acontece aqui
	mu_check(ed->rows->len == 0);
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_colors_multiple_comas)
{
	LOG_INFO(BHBLU "Test: colors_multiple_comas" RESET);
	const char *test_file = MAPS_DIR "colors_multiple_comas.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	test_extracted_textures(g_expected_textures, ed->textures);
	t_rgb expected_floor = {220, 100, 255};
	test_extracted_rgb(g_expected_nothing, ed->floor); // o erro acontece aqui
	test_extracted_rgb(g_expected_nothing, ed->ceiling);
	mu_check(ed->rows->len == 0);
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
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
	MU_RUN_TEST(test_extract_map_data_colors_multiple_comas);
}
//
//
//// test mixed lines of colors and textures
MU_TEST(test_extract_map_data_mixed_colors_first)
{
	LOG_INFO(BHBLU "Test: mixed_colors_first" RESET);
	const char *test_file = MAPS_DIR "mixed_colors_first.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == false);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == EXPECTED_ROWS);
	mu_check(ed->player_position_is_set == true);
	test_extracted_map(g_expected_map, ed->rows);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_mixed_colors_in_between_textures)
{
	LOG_INFO(BHBLU "Test: mixed_colors_in_between_textures" RESET);
	const char *test_file = MAPS_DIR "mixed_colors_first.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == false);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == EXPECTED_ROWS);
	mu_check(ed->player_position_is_set == true);
	test_extracted_map(g_expected_map, ed->rows);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST_SUITE(test_suite_textures_and_colors_mixed)
{
	// testar linhas de cores e texturas sem ordem lógica
	MU_RUN_TEST(test_extract_map_data_mixed_colors_first);
	MU_RUN_TEST(test_extract_map_data_mixed_colors_in_between_textures);
}
//
//// MAPA
////file position
MU_TEST(test_extract_map_data_map_at_beginning_of_file)
{
	LOG_INFO(BHBLU "Test: map_at_beginning_of_file" RESET);
	const char *test_file = MAPS_DIR "map_at_beginning_of_file.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	mu_check(ed->textures[NORTH] == NULL);
	mu_check(ed->textures[SOUTH] == NULL);
	mu_check(ed->textures[WEST] == NULL);
	mu_check(ed->textures[EAST] == NULL);
	test_extracted_rgb(g_expected_nothing, ed->floor);
	test_extracted_rgb(g_expected_nothing, ed->ceiling);
	mu_check(ed->rows->len == 0); //@ASSUMPTION: fails at first line
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_at_middle_of_file)
{
	LOG_INFO(BHBLU "Test: ed_at_middle_of_file" RESET);
	const char *test_file = MAPS_DIR "map_at_middle_of_file.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_nothing, ed->floor);
	test_extracted_rgb(g_expected_nothing, ed->ceiling);
	mu_check(ed->rows->len == 0);
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_with_empty_lines)
{
	LOG_INFO(BHBLU "Test: map_with_empty_lines" RESET);
	const char *test_file = MAPS_DIR "map_with_empty_lines.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == 1);
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_divided_in_multiple_parts_throughout_file)
{
	LOG_INFO(BHBLU "Test: map_divided_in_multiple_parts_throughout_file" RESET);
	const char *test_file = MAPS_DIR "map_divided_in_multiple_parts_throughout_file.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	mu_check(ed->textures[NORTH] != NULL);
	mu_check(ed->textures[NORTH]->data != NULL);
	mu_assert_string_eq(g_expected_textures[NORTH], ed->textures[NORTH]->data);
	mu_check(ed->textures[SOUTH] == NULL);
	mu_check(ed->textures[WEST] == NULL);
	mu_check(ed->textures[EAST] == NULL);
	test_extracted_rgb(g_expected_nothing, ed->floor);
	test_extracted_rgb(g_expected_nothing, ed->ceiling);
	mu_check(ed->rows->len == 0); //ASSUMPTION: doesnt allocate space to any row
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
//
MU_TEST(test_extract_map_data_map_with_multiple_player_positions)
{
	LOG_INFO(BHBLU "Test: map_with_multiple_player_positions" RESET);
	const char *test_file = MAPS_DIR "map_with_multiple_player_positions.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == 8); //A 2ª posição está na 9a linha
	mu_check(ed->player_position_is_set == true);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_player_position_case_sensitivity)
{
	LOG_INFO(BHBLU "Test: map_player_position_case_sensitivity" RESET);
	const char *test_file = MAPS_DIR "map_player_position_case_sensitivity.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == 11); //12ª linha tem "n"
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_player_orientation_invalid_character)
{
	LOG_INFO(BHBLU "Test: map_player_orientation_invalid_character" RESET);
	const char *test_file = MAPS_DIR "map_player_orientation_invalid_character.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == 11); //12ª linha tem "X"
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_player_no_position)
{
	LOG_INFO(BHBLU "Test: map_player_no_position" RESET);
	const char *test_file = MAPS_DIR "map_player_no_position.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == EXPECTED_ROWS);
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
//size
MU_TEST(test_extract_map_data_map_smallest)
{
	LOG_INFO(BHBLU "Test: map_data_map_smallest" RESET);
	const char *test_file = MAPS_DIR "map_smallest.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == false);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == 3); //@NOTE: o eda mais pequeno tem 3l
	mu_check(ed->player_position_is_set == true);
	int expected_ed[][MAX_COLS_TEST] = {
		{1, -1},
		{1, 87, 1, -1},
		{1, -1}
	};
	test_extracted_map(expected_ed, ed->rows);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_too_big)
{
	LOG_INFO(BHBLU "Test: map_data_map_too_big" RESET);
	const char *test_file = MAPS_DIR "map_data_map_too_big.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == EXPECTED_ROWS);
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
//shape
MU_TEST(test_extract_map_data_map_non_rectangular1)
{
	LOG_INFO(BHBLU "Test:  map_non_rectangular1" RESET);
	const char *test_file = MAPS_DIR "map_non_rectangular1.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == false);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == 13); //@ASSUMPTION: este eda tem 13l
	mu_check(ed->player_position_is_set == true);
	int expected_ed[][MAX_COLS_TEST] = {
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
	test_extracted_map(expected_ed, ed->rows);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_non_rectangular2)
{
	LOG_INFO(BHBLU "Test: map_non_rectangular2" RESET);
	const char *test_file = MAPS_DIR "map_non_rectangular2.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == false);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == 5); //@ASSUMPTION: eda tem 5l
	mu_check(ed->player_position_is_set == true);
	int expected_ed[][MAX_COLS_TEST] = {
		{1, 1, 1, 1, 1,-1},
		{1, 0, 0, 83, 0, 0, 1,-1},
		{1, 0, 0, 0, 1, -1},
		{1, 0, 1, -1},
		{1, -1},
		{-1}
	};
	test_extracted_map(expected_ed, ed->rows);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_longest_rows)
{
	LOG_INFO(BHBLU "Test: map_longest_rows" RESET);
	const char *test_file = MAPS_DIR "map_longest_rows.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == false);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == 7); //@ASSUMPTION: este mapa tem 7l
	mu_check(ed->player_position_is_set == true);
	test_extracted_map(g_expected_map_longest_rows, ed->rows);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_row_too_long)
{
	LOG_INFO(BHBLU "Test: map_row_too_long" RESET);
	const char *test_file = MAPS_DIR "map_row_too_long.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == 1); //@ASSUMPTION: erro na 2ª linha
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_max_rows)
{
	LOG_INFO(BHBLU "Test: map_max_rows" RESET);
	const char *test_file = MAPS_DIR "map_max_rows.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == false);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == MAX_ROWS - 1);
	mu_check(ed->player_position_is_set == true);
	test_extracted_map(g_expected_map_max_rows, ed->rows);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_too_many_rows)
{
	LOG_INFO(BHBLU "Test: map_too_many_rows" RESET);
	const char *test_file = MAPS_DIR "map_too_many_rows.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == MAX_ROWS);
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
//walls
//todos estes maps são inválidos
//@QUESTION: não sei bem se este é inválido? só paredes?
MU_TEST(test_extract_map_data_map_only_walls)
{
	LOG_INFO(BHBLU "Test: map_only_walls" RESET);
	const char *test_file = MAPS_DIR "map_only_walls.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == false); //@ASSUMPTION: só paredes não dá erro
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == EXPECTED_ROWS);
	mu_check(ed->player_position_is_set == true);
	test_extracted_map(g_expected_map_only_walls, ed->rows);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_no_walls)
{
	LOG_INFO(BHBLU "Test: map_no_walls" RESET);
	const char *test_file = MAPS_DIR "map_no_walls.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == false);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == EXPECTED_ROWS);
	mu_check(ed->player_position_is_set == true);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_open_middle_top)
{
	LOG_INFO(BHBLU "Test: map_open_middle_top" RESET);
	const char *test_file = MAPS_DIR "map_open_middle_top.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == false);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == EXPECTED_ROWS);
	mu_check(ed->player_position_is_set == true);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_open_middle_right)
{
	LOG_INFO(BHBLU "Test: map_open_middle_right" RESET);
	const char *test_file = MAPS_DIR "map_open_middle_right.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == false);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == EXPECTED_ROWS);
	mu_check(ed->player_position_is_set == true);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_open_middle_left)
{
	LOG_INFO(BHBLU "Test: map_open_middle_left" RESET);
	const char *test_file = MAPS_DIR "map_open_middle_left.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == false);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == EXPECTED_ROWS);
	mu_check(ed->player_position_is_set == true);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_open_middle_bottom)
{
	LOG_INFO(BHBLU "Test: map_open_middle_bottom" RESET);
	const char *test_file = MAPS_DIR "map_open_middle_bottom.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == false);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == EXPECTED_ROWS);
	mu_check(ed->player_position_is_set == true);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_open_top_right_corner)
{
	LOG_INFO(BHBLU "Test: map_open_top_right_corner" RESET);
	const char *test_file = MAPS_DIR "map_open_top_right_corner.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == false);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == EXPECTED_ROWS);
	mu_check(ed->player_position_is_set == true);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_open_top_left_corner)
{
	LOG_INFO(BHBLU "Test: map_open_top_left_corner" RESET);
	const char *test_file = MAPS_DIR "map_open_top_left_corner.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == false);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == EXPECTED_ROWS);
	mu_check(ed->player_position_is_set == true);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_open_bottom_right_corner)
{
	LOG_INFO(BHBLU "Test: map_open_bottom_right_corner" RESET);
	const char *test_file = MAPS_DIR "map_open_bottom_right_corner.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == false);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == EXPECTED_ROWS);
	mu_check(ed->player_position_is_set == true);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_open_bottom_left_corner)
{
	LOG_INFO(BHBLU "Test: map_open_bottom_left_corner" RESET);
	const char *test_file = MAPS_DIR "map_open_bottom_left_corner.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == false);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == EXPECTED_ROWS);
	mu_check(ed->player_position_is_set == true);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_one_space_in_each_line)
{
	LOG_INFO(BHBLU "Test: map_one_space_in_each_line" RESET);
	const char *test_file = MAPS_DIR "map_one_space_in_each_line.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == false);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == EXPECTED_ROWS);
	mu_check(ed->player_position_is_set == true);
	test_extracted_map(g_expected_map, ed->rows);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_multiple_spaces_in_each_line)
{
	LOG_INFO(BHBLU "Test: map_multiple_spaces_in_each_line" RESET);
	const char *test_file = MAPS_DIR "map_multiple_spaces_in_each_line.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == false);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == EXPECTED_ROWS);
	mu_check(ed->player_position_is_set == true);
	test_extracted_map(g_expected_map, ed->rows);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_tabs_in_each_line)
{
	LOG_INFO(BHBLU "Test: map_tabs_in_each_line" RESET);
	const char *test_file = MAPS_DIR "map_tabs_in_each_line.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == false);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == EXPECTED_ROWS);
	mu_check(ed->player_position_is_set == true);
	test_extracted_map(g_expected_map, ed->rows);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_non_binary_nums)
{
	LOG_INFO(BHBLU "Test: ed_non_binary_nums" RESET);
	const char *test_file = MAPS_DIR "map_non_binary_nums.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data*ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == 1); //@ASSUMPTION: erro na segunda linha
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}
//
MU_TEST(test_extract_map_data_map_negative_nums)
{
	LOG_INFO(BHBLU "Test: map_negative_nums" RESET);
	const char *test_file = MAPS_DIR "map_negative_nums.cub";
	LOG_DEBUG("Testing with file: %s", test_file);
	t_file_data *ed = alloc_init_extracted_data();
	extract_file_data(test_file, ed);
	mu_check(ed != NULL);
	mu_check(ed->parser_error == true);
	test_extracted_textures(g_expected_textures, ed->textures);
	test_extracted_rgb(g_expected_floor, ed->floor);
	test_extracted_rgb(g_expected_ceiling, ed->ceiling);
	mu_check(ed->rows->len == 1); //@ASSUMPTION: erro na segunda linha
	mu_check(ed->player_position_is_set == false);
	cleanup_extracted_data(ed);
	empty_gnl(test_file);
}

MU_TEST_SUITE(test_suite_map)
{
//	//testar linhas de cores e texturas sem ordem lógica
//	//file position
	MU_RUN_TEST(test_extract_map_data_map_at_beginning_of_file);
	MU_RUN_TEST(test_extract_map_data_map_at_middle_of_file);
	MU_RUN_TEST(test_extract_map_data_map_with_empty_lines);
	MU_RUN_TEST(test_extract_map_data_map_divided_in_multiple_parts_throughout_file);
	//player
	MU_RUN_TEST(test_extract_map_data_map_with_multiple_player_positions);
	MU_RUN_TEST(test_extract_map_data_map_player_position_case_sensitivity);
	MU_RUN_TEST(test_extract_map_data_map_player_orientation_invalid_character);
	MU_RUN_TEST(test_extract_map_data_map_player_no_position);
	//size
	MU_RUN_TEST(test_extract_map_data_map_smallest);
	MU_RUN_TEST(test_extract_map_data_map_longest_rows);
	MU_RUN_TEST(test_extract_map_data_map_row_too_long);
	MU_RUN_TEST(test_extract_map_data_map_max_rows);
	MU_RUN_TEST(test_extract_map_data_map_too_many_rows);
	//shape
	MU_RUN_TEST(test_extract_map_data_map_non_rectangular1);
	MU_RUN_TEST(test_extract_map_data_map_non_rectangular2);
	//walls
	MU_RUN_TEST(test_extract_map_data_map_only_walls);
	//@NOTE: nestes não é suposto dar erro no momento de extração 
	MU_RUN_TEST(test_extract_map_data_map_no_walls);
	MU_RUN_TEST(test_extract_map_data_map_open_middle_top);
	MU_RUN_TEST(test_extract_map_data_map_open_middle_right);
	MU_RUN_TEST(test_extract_map_data_map_open_middle_left);
	MU_RUN_TEST(test_extract_map_data_map_open_middle_bottom);
	MU_RUN_TEST(test_extract_map_data_map_open_top_right_corner);
	MU_RUN_TEST(test_extract_map_data_map_open_top_left_corner);
	MU_RUN_TEST(test_extract_map_data_map_open_bottom_right_corner);
	MU_RUN_TEST(test_extract_map_data_map_open_bottom_left_corner);
	//whitespaces
	MU_RUN_TEST(test_extract_map_data_map_one_space_in_each_line);
	MU_RUN_TEST(test_extract_map_data_map_multiple_spaces_in_each_line);
	MU_RUN_TEST(test_extract_map_data_map_tabs_in_each_line);
	MU_RUN_TEST(test_extract_map_data_map_non_binary_nums);
	MU_RUN_TEST(test_extract_map_data_map_negative_nums);
	//@TODO: adicionar teste com conteúdo de textura xpm mal escrito
}

// @NOTE: mudamos de diretório de modo 
// a ter acesso à pasta "assets/"
int setup_test_environment() {
	if (chdir("..") != 0) {
		perror("Failed to change to project root");
		return -1;
	}
	return 0;
}

int main()
{
	setup_test_environment();
	logger_initConsoleLogger(stderr);
	logger_setLevel(2);
	printf("testing with log level: %d\n", LOG_LEVEL);
	// Grupos de Testes
	MU_RUN_TEST(test_suite_error_extract_textures);
	MU_RUN_TEST(test_suite_error_extract_colors);
	MU_RUN_TEST(test_suite_textures_and_colors_mixed);
	MU_RUN_TEST(test_suite_map);
	MU_REPORT();
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
