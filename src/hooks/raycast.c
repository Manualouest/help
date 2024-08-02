/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <manutea.birou@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 00:04:37 by mbirou            #+#    #+#             */
/*   Updated: 2024/07/29 19:40:14 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

#include <stdio.h>
// float	cd_cast_ray(t_info *info, float ray_angle)
// {
// 	float	x;
// 	float	y;
// 	float	distance;

// 	x = info->p_pos->x;
// 	y = info->p_pos->y;
// 	// printf("start\n");
// 	while (info->map[(int)floor(y)][(int)floor(x)] && info->map[(int)floor(y)][(int)floor(x)] == '0')
// 	{
// 		x += cos(ray_angle) * 0.005;
// 		y += sin(ray_angle) * 0.005;
// 		// printf("x: %f, y: %f, elem: %c\n", x, y, info->map[(int)floor(y)][(int)floor(x)]);
// 	}
// 	distance = sqrt(pow((x - info->p_pos->x), 2.0) + pow((y - info->p_pos->y), 2.0));
// 	// printf("distance: %f, end\n", distance);
// 	return ((info->mlx_info->w_height * 1.5) / distance);
// }

float	cd_get_angle(float x, float y, t_pos *pos)
{
	float	v1_x;
	float	v1_y;
	float	v2_x;
	float	v2_y;
	float	angle;

	v1_x = x - pos->x;
	v1_y = y - pos->y;
	v2_x = cos(pos->orientation);
	v2_y = sin(pos->orientation);
	angle = atan2(v1_y, v1_x) - atan2(v2_y, v2_x);
	return (cd_basic_f_clamp(angle + pos->orientation, 0, 2 * M_PI));
}

void	cd_add_to_pos_list(t_info	*info, float x, float y, float ray_angle, int force_angle)
{
	t_pos_list	*tp_list;
	t_pos_list	*prev;

	tp_list = info->mlx_info->mmap_info->pos_list;
	prev = tp_list;
	while (tp_list)
	{
		if (tp_list->x == x && tp_list->y == y)
			return ;
		prev = tp_list;
		tp_list = tp_list->next;
	}
	if (prev && prev->x != -1)
	{
		prev->next = malloc(sizeof(*prev));
		prev->next->x = x;
		prev->next->y = y;
		if (force_angle)
			prev->next->angle = ray_angle;
		else
			prev->next->angle = cd_get_angle(x, y, info->p_pos);
		prev->next->next = NULL;
	}
	else
	{
		info->mlx_info->mmap_info->pos_list->x = x;
		info->mlx_info->mmap_info->pos_list->y = y;
		if (force_angle)
			info->mlx_info->mmap_info->pos_list->angle = ray_angle;
		else
			info->mlx_info->mmap_info->pos_list->angle = cd_get_angle(x, y, info->p_pos);
	}
}

void	cd_add_to_pos_list_offsets(t_info	*info, float x, float y, float ray_angle)
{
	cd_add_to_pos_list(info, cd_round(x - 0.01, 100), cd_round(y - 0.01, 100), cd_basic_f_clamp(ray_angle - 0.0001, 0, 2.0 * M_PI), 1);
	cd_add_to_pos_list(info, cd_round(x, 100), cd_round(y, 100), ray_angle, 0);
	cd_add_to_pos_list(info, cd_round(x + 0.01, 100), cd_round(y + 0.01, 100), cd_basic_f_clamp(ray_angle + 0.0001, 0, 2.0 * M_PI), 1);
}

t_ray_info	cd_cast_ray(t_info *info, t_ray_info ray_info, int force_add)
{
	double	dx;
	double	dy;
	double	ddx;
	double	ddy;
	int		step_x;
	int		step_y;
	double	side_dx;
	double	side_dy;

	dx = cos(ray_info.angle);
	dy = sin(ray_info.angle);

	ray_info.x = (int)info->p_pos->x;
	ray_info.y = (int)info->p_pos->y;

	ddx = fabs(1 / dx);
	ddy = fabs(1 / dy);

	if (dx < 0)
	{
		step_x = -1;
		side_dx = (info->p_pos->x - ray_info.x) * ddx;
	}
	else
	{
		step_x = 1;
		side_dx = (ray_info.x + 1 - info->p_pos->x) * ddx;
	}
	if (dy < 0)
	{
		step_y = -1;
		side_dy = (info->p_pos->y - ray_info.y) * ddy;
	}
	else
	{
		step_y = 1;
		side_dy = (ray_info.y + 1 - info->p_pos->y) * ddy;
	}

	ray_info.side = 0;
	while (info->map[(int)ray_info.y][(int)ray_info.x])
	{
		if (side_dx < side_dy)
		{
			side_dx += ddx;
			ray_info.x += step_x;
			ray_info.side = 0;
		}
		else
		{
			side_dy += ddy;
			ray_info.y += step_y;
			ray_info.side = 1;
		}
		if (info->map[(int)ray_info.y][(int)ray_info.x] == '1')
			break;
	}
	if (ray_info.side == 0)
		ray_info.distance = ((float)ray_info.x - info->p_pos->x + (float)(1.0 - step_x) / 2.0) / dx;
	else
		ray_info.distance = ((float)ray_info.y - info->p_pos->y + (float)(1.0 - step_y) / 2.0) / dy;
	ray_info.distance = floor(ray_info.distance * 1000)/1000;
	// printf("x: %f, y: %f, side_dx: %f, side_dy: %f\n", ray_info.x, ray_info.y, side_dx, side_dy);
	ray_info.wall_height = ((float)info->mlx_info->w_height) / (ray_info.distance * cos(cd_basic_f_clamp(info->p_pos->orientation - ray_info.angle , 0, 2.0 * M_PI)));
	
	if (force_add == 1)
	{
		cd_add_to_pos_list(info, ray_info.p_x + ray_info.distance * cos(ray_info.angle), ray_info.p_y + ray_info.distance * sin(ray_info.angle), ray_info.angle, 1);
	}
	if (ray_info.side == 0)
	{
		if (cos(ray_info.angle) > 0)
		{
			cd_add_to_pos_list_offsets(info, ray_info.x, ray_info.y, ray_info.angle);
			// if (info->map[(int)ray_info.y + 1][(int)ray_info.x] == '1')
			// {
			// 	cd_add_to_pos_list_offsets(info, ray_info.x, ray_info.y + 1, ray_info.angle);
			// }
		}
		else
		{
			cd_add_to_pos_list_offsets(info, ray_info.x + 1, ray_info.y, ray_info.angle);
		}

		// if (info->map[(int)ray_info.y - 1][(int)ray_info.x] == '1')
		// 	cd_add_to_pos_list(info, ray_info.x, ray_info.y - 1, ray_info.angle, 0);
	}
	else
	{
		if (sin(ray_info.angle) > 0)
		{
			cd_add_to_pos_list_offsets(info, ray_info.x, ray_info.y, ray_info.angle);
			if (info->map[(int)ray_info.y][(int)ray_info.x + 1] == '1')
				cd_add_to_pos_list_offsets(info, ray_info.x + 1, ray_info.y, ray_info.angle);
		}
		else
		{
			cd_add_to_pos_list_offsets(info, ray_info.x, ray_info.y + 1, ray_info.angle);
			cd_add_to_pos_list_offsets(info, ray_info.x + 1, ray_info.y + 1, ray_info.angle);
		}

		// if (info->map[(int)ray_info.y][(int)ray_info.x + 1] == '1')
		// 	cd_add_to_pos_list(info, ray_info.x + 1, ray_info.y, ray_info.angle, 0);
	}

	if (force_add == 2)
	{
		cd_add_to_pos_list(info, ray_info.p_x + ray_info.distance * cos(ray_info.angle), ray_info.p_y + ray_info.distance * sin(ray_info.angle), ray_info.angle, 1);
	}

	info->mlx_info->mmap_info->side = ray_info.side;
	

	return (ray_info);
}

int	cd_get_texture(t_mlx_info *mlx_info, t_ray_info ray_info, int y, int max_y)
{
	float			t_x;
	t_frame_info	*texture;
	char			*dst;

	if (ray_info.side == 0)
	{
		if (cos(ray_info.angle) > 0)
			texture = mlx_info->east;
		else
			texture = mlx_info->west;
	}
	else
	{
		if (sin(ray_info.angle) > 0)
			texture = mlx_info->south;
		else
			texture = mlx_info->north;
	}
	if (ray_info.side == 0)
		t_x = ray_info.p_y + ray_info.distance * sin(ray_info.angle);
	else
		t_x = ray_info.p_x + ray_info.distance * cos(ray_info.angle);
	t_x = t_x - floor(t_x);
	t_x = t_x * (float)texture->width;
	
	if ((ray_info.side == 0 && cos(ray_info.angle) < 0) || (ray_info.side == 1 && sin(ray_info.angle) > 0))
		t_x = (float)texture->width - t_x - 1.0;

	y = (float)y * ((float)texture->height / (float)max_y);
	dst = texture->addr + (y * texture->line_size + (int)floor(t_x) * (texture->bits_per_pixel / 8));
	return (*(unsigned int *)dst);
}

void	cd_draw_wall(t_mlx_info *mlx_info, int i, t_ray_info ray_info)
{
	int	start;
	int	end;
	int	n;

	start = floor(-ray_info.wall_height / 2.0 + (float)mlx_info->w_height / 2.0);
	end = floor(ray_info.wall_height / 2.0 + (float)mlx_info->w_height / 2.0);
	n = start;
	if (n < 0)
		n = 0;
	while (n < end && n < mlx_info->w_height)
	{
		cd_change_pixel_color(mlx_info->main_frame, i, n, cd_get_texture(mlx_info, ray_info, n - start, end - start - 1));
		n ++;
	}
	if (n < mlx_info->w_height)
		n --;
	while (n < mlx_info->w_height)
	{
		cd_change_pixel_color(mlx_info->main_frame, i, n, mlx_info->floor);
		n ++;
	}
}

t_pos_list	*cd_pos_list_last(t_pos_list *pos_list)
{
	if (pos_list == NULL)
		return (0);
	if (pos_list->next == NULL)
		return (pos_list);
	return (cd_pos_list_last(pos_list->next));
}

void	cd_check_line(t_info *info, t_pos_list *pos)
{
	double	dx;
	double	dy;
	double	ddx;
	double	ddy;
	int		step_x;
	int		step_y;
	double	side_dx;
	double	side_dy;
	float	distance;

	dx = cos(pos->angle);
	dy = sin(pos->angle);

	pos->x = (int)info->p_pos->x;
	pos->y = (int)info->p_pos->y;

	ddx = fabs(1 / dx);
	ddy = fabs(1 / dy);

	if (dx < 0)
	{
		step_x = -1;
		side_dx = (info->p_pos->x - pos->x) * ddx;
	}
	else
	{
		step_x = 1;
		side_dx = (pos->x + 1 - info->p_pos->x) * ddx;
	}
	if (dy < 0)
	{
		step_y = -1;
		side_dy = (info->p_pos->y - pos->y) * ddy;
	}
	else
	{
		step_y = 1;
		side_dy = (pos->y + 1 - info->p_pos->y) * ddy;
	}

	pos->side = 0;
	while (info->map[(int)pos->y][(int)pos->x])
	{
		if (side_dx < side_dy)
		{
			side_dx += ddx;
			pos->x += step_x;
			pos->side = 0;
		}
		else
		{
			side_dy += ddy;
			pos->y += step_y;
			pos->side = 1;
		}
		if (info->map[(int)pos->y][(int)pos->x] == '1')
			break;
	}
	if (pos->side == 0)
		distance = ((float)pos->x - info->p_pos->x + (float)(1.0 - step_x) / 2.0) / dx;
	else
		distance = ((float)pos->y - info->p_pos->y + (float)(1.0 - step_y) / 2.0) / dy;
	distance = floor(distance * 1000)/1000;
	pos->x = info->p_pos->x + distance * cos(pos->angle);
	pos->y = info->p_pos->y + distance * sin(pos->angle);
	// printf("new x: %f, y: %f\n", pos->x, pos->y);
}

void	cd_check_segment_angles(t_info *info, t_pos_list *pos_list)
{
	t_pos_list	*prev;
	t_pos_list	*tp;
	float		left_angle;
	float		right_angle;

	left_angle = pos_list->angle;
	right_angle = cd_pos_list_last(pos_list)->angle;
	tp = pos_list;
	prev = NULL;
	while (tp)
	{
		if ((left_angle < right_angle && (tp->angle < left_angle || tp->angle > right_angle))
			|| (left_angle > right_angle && (tp->angle < left_angle && tp->angle > right_angle)))
		{
			prev->next = tp->next;
			free(tp);
			tp = prev;
		}
		else if (tp->angle != left_angle && tp->angle != right_angle)
			cd_check_line(info, tp);
		// printf("x: %f, y: %f, a: %f\n", tp->x, tp->y, tp->angle);
		prev = tp;
		tp = tp->next;
	}
	// printf("\n");
	// printf("lax: %f, lay: %f, rax: %f, ray: %f\n", pos_list->x, pos_list->y, cd_pos_list_last(pos_list)->x, cd_pos_list_last(pos_list)->y);
}

void	cd_raycast(t_info *info)
{
	float		rays;
	float		i;
	t_ray_info	ray_info;

	rays = info->mlx_info->w_width; //info->mlx_info->w_width;
	i = 0;
	// printf("start\n");
	ray_info.p_x = info->p_pos->x;
	ray_info.p_y = info->p_pos->y;
	while (i < rays)
	{
		ray_info.angle = (info->p_pos->orientation - (info->mlx_info->fov / 2.0) + i * (info->mlx_info->fov / rays)); //(M_PI / 4.0) is the fov
		ray_info.angle = cd_basic_f_clamp(ray_info.angle, 0, 2.0 * M_PI);
		if (i == 0)
			ray_info = cd_cast_ray(info, ray_info, 1);
		else if (i == rays - 1)
			ray_info = cd_cast_ray(info, ray_info, 2);
		else
			ray_info = cd_cast_ray(info, ray_info, 0);
		// printf("ray_angle: %f, wall_height: %f\n", ray_angle, wall_height);
		cd_draw_wall(info->mlx_info, i, ray_info);
		i ++;
	}

	cd_check_segment_angles(info, info->mlx_info->mmap_info->pos_list);
	// t_pos_list	*tp;
	// tp = info->mlx_info->mmap_info->pos_list;
	// while(tp)
	// {
	// 	printf("mmap, x: %f, y: %f\n", tp->x, tp->y);
	// 	tp = tp->next;
	// }


	// printf("end\n");
}
