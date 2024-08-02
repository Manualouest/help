/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <manutea.birou@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 19:50:50 by mbirou            #+#    #+#             */
/*   Updated: 2024/07/29 14:03:47 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

typedef struct s_pos
{
	float	x;
	float	y;
	float	orientation; // N = 0, E = 0.5*PI, S = PI, W = 1.5*PI
}			t_pos;

typedef struct s_img_info
{
	void	*img;
	int		height;
	int		width;
}			t_img_info;

typedef struct s_frame_info
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_size;
	int		endian;
	int		height;
	int		width;
}			t_frame_info;

typedef struct s_pos_list
{
	float				x;
	float				y;
	float				angle;
	int					side;
	struct s_pos_list	*next;
}			t_pos_list;

typedef struct s_minimap_info
{
	t_pos_list	*pos_list;
	int			side;
}			t_minimap_info;

typedef struct s_mlx_info
{
	void			*mlx;
	void			*mlx_win;
	float			w_width;
	float			w_height;
	float			s_width;
	float			s_height;
	float			fov;
	t_frame_info	*main_frame;
	t_frame_info	*refresh_frame;
	t_frame_info	*north;
	t_frame_info	*south;
	t_frame_info	*west;
	t_frame_info	*east;
	int				floor;
	int				roof;
	t_minimap_info	*mmap_info;
}					t_mlx_info;

typedef struct s_keys
{
	int	z;
	int	s;
	int	q;
	int	d;
}		t_keys;

typedef struct s_info
{
	char		*north;
	char		*south;
	char		*west;
	char		*east;
	char		*floor;
	char		*ceiling;
	char		**map;
	t_pos		*p_pos;
	t_mlx_info	*mlx_info;
	t_keys		*keys;
}				t_info;

typedef struct s_ray_info
{
	float	p_x;
	float	p_y;
	float	x;
	float	y;
	int		side;
	float	angle;
	float	distance;
	float	wall_height;
}			t_ray_info;

# include <stdlib.h>
# include <fcntl.h>
# include <math.h>

# include "libft.h"
# include "get_next_line_bonus.h"

# include <mlx.h>

//______src/struct_setup______
// struct_setup_main.c
int		cd_setup_info(t_info *info, char *filename);

// struct_setup_utils.c
char	*cd_re_malloc(char *src, char *new);
int		cd_tab_len(char **tab);
void	cd_free_tab_content(char **tab);
void	cd_dup_tab(char **dest, char **src);
char	**cd_tab_append(char **src, char *elem);

// mlx_setup.c
void	cd_change_pixel_color(t_frame_info *frame, int x, int y, int color);
void	cd_setup_mlx_main(t_info *info);

//____________________________

//______src/frees______
// free_struct.c
void	cd_free_tab(char **tab);
void	cd_free_info(t_info *info);

//_____________________

//______src/math_utils______
// color_utils.C
int		cd_get_hex(int t, int r, int g, int b);
float	cd_basic_f_clamp(float num, float min, float max);
float	cd_f_abs(float num);
float	cd_get_dist(float x1, float y1, float x2, float y2);
float	cd_round(float num, float round);
//__________________________

//______src/hooks______
// hook_setup.c
void	cd_hook_init(t_info *info);
int		cd_move_player(t_info *info);
int		key_tests(int keycode, t_info *info);

// handle_background.c
int		cd_update_frame(t_info *info);

// raycast.c
void	cd_raycast(t_info *info);

// minimap.c
void	cd_add_minimap(t_info *info);

//_____________________

#endif