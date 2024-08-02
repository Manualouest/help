/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_background.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <manutea.birou@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 15:20:36 by mbirou            #+#    #+#             */
/*   Updated: 2024/07/24 15:04:33 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	cd_refresh_background(t_mlx_info *mlx_info)
{
	int	y;
	int	x;

	y = -1;
	while (++y < mlx_info->w_height)
	{
		x = -1;
		while (++x < mlx_info->w_width)
			cd_change_pixel_color(mlx_info->main_frame, x, y, mlx_info->roof);
	}
}

int	cd_update_frame(t_info *info)
{
	cd_move_player(info);
	// mlx_put_image_to_window(info->mlx_info->mlx, info->mlx_info->mlx_win,
	// 	info->mlx_info->refresh_frame->img, 0, 0);
	cd_refresh_background(info->mlx_info);
	cd_raycast(info);
	cd_add_minimap(info);
	mlx_put_image_to_window(info->mlx_info->mlx, info->mlx_info->mlx_win,
		info->mlx_info->main_frame->img, 0, 0);
	return (0);
}