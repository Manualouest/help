/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <manutea.birou@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 12:32:22 by mbirou            #+#    #+#             */
/*   Updated: 2024/07/29 18:45:11 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

#include <stdio.h>

t_pos_list	*cd_clear_pos_list(t_pos_list *pos_list)
{
	t_pos_list	*tp;
	t_pos_list	*next_save;

	tp = pos_list->next;
	while (tp)
	{
		next_save = tp->next;
		free(tp);
		tp = next_save;
	}
	pos_list->x = -1;
	pos_list->y = -1;
	pos_list->next = NULL;
	return (pos_list);
}

int	cd_is_segment(t_pos_list *pos_list, float x, float y)
{
	t_pos_list	*tp;

	tp = pos_list;
	while (tp)
	{
		if (tp->x == x && tp->y == y)
			return (1);
		tp = tp->next;
	}
	return (0);
}

int	cd_is_coord_possible(char **tab, int x, int y)
{
	int	i;
	int	ii;

	i = 0;
	if (y >= cd_tab_len(tab) || y < 0 || x < 0)
		return (0);
	while(tab[i] && i <= y)
	{
		ii = 0;
		if (x >= (int)ft_strlen(tab[i]))
			return (0);
		while (tab[i][ii] && ii < x)
			ii ++;
		if (tab[i][ii] && i == y && ii == x)
		{
			// printf("check ii,x: %d,%d, i,y: %d,%d\n", ii, x, i, y);
			return (1);
		}
		i ++;
	}
	return (0);
}

int	cd_is_between_intersects(int mx, int my, t_pos_list *pos_list, t_info *info)
{
	t_pos_list	*tp;

	tp = pos_list;
	while (tp)
	{
		if (cd_round(cd_get_dist(info->p_pos->x * info->mlx_info->s_width, info->p_pos->y * info->mlx_info->s_height, mx, my), 100) + cd_round(cd_get_dist(tp->x * info->mlx_info->s_width, tp->y * info->mlx_info->s_height, mx, my), 100) == cd_round(cd_get_dist(info->p_pos->x * info->mlx_info->s_width, info->p_pos->y * info->mlx_info->s_height, tp->x * info->mlx_info->s_width, tp->y * info->mlx_info->s_height), 100))
			return (1);
		tp = tp->next;
	}
	return (0);
}

int	cd_is_in_triangle(t_info *info, t_pos_list *p2, t_pos_list *p3, t_pos p4)
{
	float	denominator;
	float	a;
	float	b;
	float	c;

	denominator = ((p2->y * info->mlx_info->s_height - p3->y * info->mlx_info->s_height) * (info->p_pos->x * info->mlx_info->s_width - p3->x * info->mlx_info->s_width) + (p3->x * info->mlx_info->s_width - p2->x * info->mlx_info->s_width) * (info->p_pos->y * info->mlx_info->s_height - p3->y * info->mlx_info->s_height));
	a = ((p2->y * info->mlx_info->s_height - p3->y * info->mlx_info->s_height) * (p4.x - p3->x * info->mlx_info->s_width) + (p3->x * info->mlx_info->s_width - p2->x * info->mlx_info->s_width) * (p4.y - p3->y * info->mlx_info->s_height)) / denominator;
	b = ((p3->y * info->mlx_info->s_height - info->p_pos->y * info->mlx_info->s_height) * (p4.x - p3->x * info->mlx_info->s_width) + (info->p_pos->x * info->mlx_info->s_width - p3->x * info->mlx_info->s_width) * (p4.y - p3->y * info->mlx_info->s_height)) / denominator;
	c = 1 - a - b;
	return ((0 <= a && a <= 1) && (0 <= b && b <= 1) && (0 <= c && c <= 1));
}

int	cd_is_in_sight(int mx, int my, t_pos_list *pos_list, t_info *info)
{
	t_pos_list	*tp;
	t_pos		pos;

	tp = pos_list;
	while (tp)
	{
		pos.x = mx;
		pos.y = my;
		if (tp->next && cd_is_in_triangle(info, tp, tp->next, pos))
			return (1);
		tp = tp->next;
	}
	return (0);
}

int	cd_get_pixel_color(t_frame_info *frame, int x, int y)
{
	char	*dst;

	dst = frame->addr + (y * frame->line_size + x * (frame->bits_per_pixel / 8));
	if (dst)
		return (*(unsigned int*)dst);
	return (0);
}

int	cd_is_on_intersects_end(int mx, int my, t_pos_list *pos_list, t_info *info)
{
	t_pos_list	*tp;

	tp = pos_list;
	while (tp)
	{
		if (mx >= tp->x * info->mlx_info->s_width - 2 && mx <= tp->x * info->mlx_info->s_width + 2 && my >= tp->y * info->mlx_info->s_height - 2 && my <= tp->y * info->mlx_info->s_height + 2)
			return (1);
		tp = tp->next;
	}
	return (0);
}

void	cd_add_minimap(t_info *info)
{
	float	m_width;
	float	m_height;
	int		mx;
	int		my;
	int		i;
	int		ii;

	m_width = info->mlx_info->w_width / 4;
	m_height = info->mlx_info->w_height / 4;
	i = -1;
	while(++i <= m_height)
	{
		ii = -1;
		while(++ii <= m_width)
		{
			mx = (int)(floor((((float)ii) * (12.0 / m_width)) * info->mlx_info->s_width) + (info->p_pos->x - 6.0) * info->mlx_info->s_width);
			my = (int)(floor((((float)i) * (8.0 / m_height)) * info->mlx_info->s_height) + (info->p_pos->y - 4.0) * info->mlx_info->s_height);
			if ((i >= (m_height / 2) - 2 && i <= (m_height / 2) + 2) && (ii >= (m_width / 2) - 2 && ii <= (m_width / 2) + 2))
					cd_change_pixel_color(info->mlx_info->main_frame, ii, i, cd_basic_f_clamp(info->mlx_info->floor + info->mlx_info->roof, 0, 0xFFFFFFFF));
			else if (cd_is_coord_possible(info->map, (int)floor(mx / info->mlx_info->s_width), (int)floor(my / info->mlx_info->s_height)) && info->map[(int)(my / info->mlx_info->s_height)][(int)(mx / info->mlx_info->s_width)] == '1')
				cd_change_pixel_color(info->mlx_info->main_frame, ii, i, cd_basic_f_clamp(info->mlx_info->floor + info->mlx_info->roof + 0x00808080, 0, 0xFFFFFFFF));
			else
				cd_change_pixel_color(info->mlx_info->main_frame, ii, i, 1 - info->mlx_info->floor);
			// if (cd_is_segment(info->mlx_info->mmap_info->pos_list, (int)floor(mx / info->mlx_info->s_width), (int)floor(my / info->mlx_info->s_height)))
			// 	cd_change_pixel_color(info->mlx_info->main_frame, ii, i, 0x00FF0000);
			
			// if (cd_is_between_intersects(mx, my, info->mlx_info->mmap_info->pos_list, info))
			// 	cd_change_pixel_color(info->mlx_info->main_frame, ii, i, 0x00000000);
			
			if (cd_is_in_sight(mx, my, info->mlx_info->mmap_info->pos_list, info) && cd_get_pixel_color(info->mlx_info->main_frame, ii, i) != cd_basic_f_clamp(info->mlx_info->floor + info->mlx_info->roof + 0x00808080, 0, 0xFFFFFFFF))
				cd_change_pixel_color(info->mlx_info->main_frame, ii, i, info->mlx_info->floor);
			
			if (cd_is_on_intersects_end(mx, my, info->mlx_info->mmap_info->pos_list, info))
				cd_change_pixel_color(info->mlx_info->main_frame, ii, i, 0x00FFFF00);
			
			// printf("x: %d, y: %d, map: %c\n", (int)floor(((float)ii * (12.0 / (float)m_width))), (int)floor(((float)i * (8.0 / (float)m_height))), info->map[(int)floor(((float)i * (8.0 / (float)m_height)))][(int)floor(((float)ii * (12.0 / (float)m_width)))]);
		}
	}
	info->mlx_info->mmap_info->pos_list = cd_clear_pos_list(info->mlx_info->mmap_info->pos_list);
}