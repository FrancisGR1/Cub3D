#include "cub3d.h"

/*
 * Isto faz o seguinte:
 * no arr de incubação:
 * substitui os ' ' e os 0 por 2
 * conta o nº de susbtituições (células marcadas)
 * regista no pointer spaces o nº de espaços encontrados (células marcadas que eram espaços)
 */
static int flood_fill(int arr[INCUBATION_ROWS][INCUBATION_COLS], int x, int y, int *spaces)
{
	int marked_cells;

	if (x < 0 || x >= INCUBATION_COLS 
			|| y < 0 || y >= INCUBATION_ROWS)
		return 0;
	if (arr[y][x] == 1 || arr[y][x] == 2)
		return (0);
	if (arr[y][x] == ' ')
		(*spaces)++;
	arr[y][x] = 2;
	marked_cells = 1;
	marked_cells += flood_fill(arr, x + 1, y, spaces);
	marked_cells += flood_fill(arr, x - 1, y, spaces);
	marked_cells += flood_fill(arr, x, y + 1, spaces);
	marked_cells += flood_fill(arr, x, y - 1, spaces);
	return (marked_cells);
}

int break_in_map_from_outside(t_file_data *fdata, int incubation_arr[INCUBATION_ROWS][INCUBATION_COLS])
{
	//incubar mapa
	//copiar para array stático
	int i = 0;
	int j;
	//preencher mapa com -1
	while (i < INCUBATION_ROWS)
	{
		j = 0;
		while (j < INCUBATION_COLS)
		{
			incubation_arr[i][j] = -1;
			j++;
		}
		i++;
	}
	//passar nºs para mapa
	//tem de estar rodeado de -1
	//começamos na 1ª linha 
	i = 1;
//	printf("test_map: passing numbers to incubation array\n");
//	printf("map_size: %d\n", get_map_size(fdata->rows));
	while (i < get_map_size(fdata->rows) + 1)
	{
		//2ª coluna
		j = 1;
		while (j < get_map_row_size(fdata->rows, i-1) + 1)
		{
			incubation_arr[i][j] = get_map_value(fdata->rows, i-1, j-1);
			//printf("%d", incubation_arr[i][j]);
			j++;
		}
		//printf("\n");
		i++;
	}
	//printf("show incubation array\n");
	i = 0;
	while (i < INCUBATION_ROWS)
	{
		j = 0;
		while (j < INCUBATION_COLS)
		{
			//printf("%d", incubation_arr[i][j]);
			j++;
		}
		//printf("\n");
		i++;
	}
	int expected_marked_cells;
	int marked_cells;

	//calcular nº correto de pontos marcados esperados
	int map_size = get_map_total_cells(fdata->rows);
	//printf("map size: %d\n", map_size);
	//int outside_spaces = count_spaces_from_outside_and_mark(incubation_arr, 0, 0);
	//printf("counted spaces: %d\n", outside_spaces);
	expected_marked_cells = INCUBATION_ROWS * INCUBATION_COLS - map_size;
	//printf("incub size %d\n", INCUBATION_ROWS * INCUBATION_COLS );

	int spaces = 0;
	marked_cells = flood_fill(incubation_arr, 0, 0, &spaces) - spaces;
	i = 0;
	while (i < INCUBATION_ROWS)
	{
		j = 0;
		while (j < INCUBATION_COLS)
		{
			//printf("%d", incubation_arr[i][j]);
			j++;
		}
		//printf("\n");
		i++;
	}
	if (marked_cells != expected_marked_cells)
	{
		printf("Error: Expected: %d\nMarked: %d\n", expected_marked_cells, marked_cells);
		return (1);
	}
	else
	{
		printf("Success: Expected: %d\nMarked: %d\n", expected_marked_cells, marked_cells);
		return (0);
	}
}
