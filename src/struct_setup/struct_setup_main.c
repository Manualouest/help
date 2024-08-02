/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_setup_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <manutea.birou@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 12:27:58 by mbirou            #+#    #+#             */
/*   Updated: 2024/07/22 16:09:39 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

char	**cd_get_map(int fd);
void	cd_setup_player(t_info *info);
char	*cd_clean_gnl(int fd);

void	cd_innit_keys(t_keys *keys)
{
	keys->z = 0;
	keys->s = 0;
	keys->q = 0;
	keys->d = 0;
}

int	cd_setup_info(t_info *info, char *filename)
{
	int	fd;
	t_keys	*keys;

	keys = malloc(sizeof(*keys));
	cd_innit_keys(keys);
	info->keys = keys;
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (0);
	info->north = cd_clean_gnl(fd);
	info->south = cd_clean_gnl(fd);
	info->west = cd_clean_gnl(fd);
	info->east = cd_clean_gnl(fd);
	info->floor = cd_clean_gnl(fd);
	info->ceiling = cd_clean_gnl(fd);
	info->map = cd_get_map(fd);
	info->p_pos = malloc(sizeof(*info->p_pos));
	cd_setup_player(info);
	close(fd);
	info->mlx_info = malloc(sizeof(*info->mlx_info));
	cd_setup_mlx_main(info);
	return (1);
}

char	**cd_get_map(int fd)
{
	char	*line;
	char	**map;
	char	**tp_map;
	int		i;

	i = 0;
	map = NULL;
	tp_map = NULL;
	line = cd_clean_gnl(fd);
	while(line)
	{
		tp_map = cd_tab_append(tp_map, ft_strdup(line));
		map = ft_calloc(sizeof(char *), i + 2);
		cd_dup_tab(map, tp_map);
		cd_free_tab_content(tp_map);
		// free(line);
		line = cd_re_malloc(line, cd_clean_gnl(fd));
		if (line)
		{
			cd_dup_tab(tp_map, map);
			cd_free_tab(map);
		}
		i ++;
	}
	free(tp_map);
	return (map);
}

float	cd_get_orientation(char orient)
{
	if (orient == 'N')
		return (1.5 * M_PI);
	else if (orient == 'S')
		return (0.5 * M_PI);
	else if (orient == 'W')
		return (M_PI);
	else if (orient == 'E')
		return (0);
	else
		return (-1);
}

void	cd_setup_player(t_info *info)
{
	int	i;
	int	ii;

	i = -1;
	ii = 0;
	while (info->map && info->map[++i])
	{
		ii = -1;
		while (info->map[i][++ii] && ft_isdigit(info->map[i][ii]))
			;
		if (info->map[i][ii] && !ft_isdigit(info->map[i][ii]))
			break;
	}
	info->p_pos->x = ii + 0.5;
	info->p_pos->y = i + 0.5;
	if (i != -1)
		info->p_pos->orientation = cd_get_orientation(info->map[i][ii]);
	else
		info->p_pos->orientation = -1;
	if (info->p_pos->orientation != -1)
		info->map[i][ii] = '0';
}

char	*cd_clean_gnl(int fd)
{
	char	*line;

	line = get_next_line(fd);
	if (!line)
		return (line);
	if (line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = 0;
	return (line);
}
