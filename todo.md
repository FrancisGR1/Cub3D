- [x] substituir espaços por paredes
- [x] verificar middle lines
- [x] reorganizar header e norminette
- [ ] fazer commit das mudanças
- [ ] mudar tests em tests.sh

flood_fill algo
void    flood_fill(t_data *data, int x, int y, char **map_copy)
{
    if (data->map.map[y][x] == '1' || map_copy[y][x] == 'V')
        return ;
    map_copy[y][x] = 'V';
    flood_fill(data, x + 1, y, map_copy);
    flood_fill(data, x - 1, y, map_copy);
    flood_fill(data, x, y + 1, map_copy);
    flood_fill(data, x, y - 1, map_copy);
}
