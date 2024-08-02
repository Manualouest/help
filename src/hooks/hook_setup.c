/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_setup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <manutea.birou@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 15:21:19 by mbirou            #+#    #+#             */
/*   Updated: 2024/07/25 17:44:42 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>
#include <stdio.h>
int	cd_stop_window(t_info *info)
{
	mlx_loop_end(info->mlx_info->mlx);
	return (0);
}

void	cd_go_forward(t_info *info, float speed)
{
	float	new_x;
	float	new_y;

	new_x = info->p_pos->x + (cos(info->p_pos->orientation) * speed) * 2;
	new_y = info->p_pos->y + (sin(info->p_pos->orientation) * speed) * 2;
	if (info->map[(int)floor(new_y)][(int)floor(new_x)] == '0')
	{
		info->p_pos->x += cos(info->p_pos->orientation) * speed;
		info->p_pos->y += sin(info->p_pos->orientation) * speed;
	}
}

void	cd_go_backward(t_info *info, float speed)
{
	float	new_x;
	float	new_y;

	new_x = info->p_pos->x - (cos(info->p_pos->orientation) * speed) * 2;
	new_y = info->p_pos->y - (sin(info->p_pos->orientation) * speed) * 2;
	if (info->map[(int)floor(new_y)][(int)floor(new_x)] == '0')
	{
		info->p_pos->x -= cos(info->p_pos->orientation) * speed;
		info->p_pos->y -= sin(info->p_pos->orientation) * speed;
	}
}

// int	key_tests(int keycode, t_info *info)
// {
// 	if (keycode == 65307)
// 		cd_stop_window(info);
// 	else if (keycode == 122)
// 		cd_go_forward(info, speed);
// 	else if (keycode == 115)
// 		cd_go_backward(info, speed);
// 	else if (keycode == 113)
// 		info->p_pos->orientation -= turn_speed;
// 	else if (keycode == 100)
// 		info->p_pos->orientation += turn_speed;

// 	return (0);
// }

int	cd_key_pressed(int keycode, t_info *info)
{
	if (keycode == 65307)
		cd_stop_window(info);
	if (keycode == 122)
		info->keys->z = 1;
	else if (keycode == 115)
		info->keys->s = 1;
	else if (keycode == 113)
		info->keys->q = 1;
	else if (keycode == 100)
		info->keys->d = 1;
	// printf("keys pressed, z: %d, s: %d, q: %d, d: %d\n", keys->z, keys->s, keys->q, keys->d);
	return (0);
}

int	cd_key_released(int keycode, t_info *info)
{
	if (keycode == 122)
		info->keys->z = 0;
	else if (keycode == 115)
		info->keys->s = 0;
	else if (keycode == 113)
		info->keys->q = 0;
	else if (keycode == 100)
		info->keys->d = 0;
	// printf("keys release, z: %d, s: %d, q: %d, d: %d\n", keys->z, keys->s, keys->q, keys->d);
	return (0);
}

int	cd_move_player(t_info *info)
{
	float	speed;
	float	turn_speed;

	speed = 0.075;
	turn_speed = floor(info->mlx_info->fov / 50.0 * 1000.0) / 1000.0;
	// printf("keys movement, z: %d, s: %d, q: %d, d: %d\n", info->keys->z, info->keys->s, info->keys->q, info->keys->d);
	if (info->keys->z && !info->keys->s)
		cd_go_forward(info, speed);
	if (info->keys->s && !info->keys->z)
		cd_go_backward(info, speed);
	if (info->keys->q && !info->keys->d)
		info->p_pos->orientation -= turn_speed;
	if (info->keys->d && !info->keys->q)
		info->p_pos->orientation += turn_speed;
	if (info->p_pos->orientation < 0)
		info->p_pos->orientation += 2.0 * M_PI;
	if (info->p_pos->orientation > 2.0 * M_PI)
		info->p_pos->orientation -= 2.0 * M_PI;
	// 	cd_go_forward(info, speed);
	// else if (keycode == 115)
	// 	cd_go_backward(info, speed);
	// else if (keycode == 113)
	// 	info->p_pos->orientation -= turn_speed;
	// else if (keycode == 100)
	// 	info->p_pos->orientation += turn_speed;
	
	return (0);
}

void	cd_hook_init(t_info *info)
{
	info->mlx_info->mlx_win = mlx_new_window(info->mlx_info->mlx,
		info->mlx_info->w_width, info->mlx_info->w_height, "Cub3d");

	// mlx_key_hook(info->mlx_info->mlx_win, key_tests, info);

	mlx_hook(info->mlx_info->mlx_win, 2, 1L<<0, cd_key_pressed, info);
	mlx_hook(info->mlx_info->mlx_win, 3, 1L<<1, cd_key_released, info);
	// mlx_loop_hook(info->mlx_info->mlx, cd_move_player, info);

	mlx_hook(info->mlx_info->mlx_win, 17, 1L<<0, cd_stop_window, info);

	mlx_loop_hook(info->mlx_info->mlx, cd_update_frame, info);
	// cd_update_frame(info);
	
	mlx_loop(info->mlx_info->mlx);
	mlx_destroy_window(info->mlx_info->mlx, info->mlx_info->mlx_win);
}