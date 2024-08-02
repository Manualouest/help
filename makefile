# valgrind --suppressions=ms.supp --leak-check=full --show-leak-kinds=all ./minishell

NAME = cub3D

CC_FLAGS = -Wall -Werror -Wextra -g -I$(INCLUDES) -Imlx_linux -O3
OTHER_LIBS = -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz

INCLUDES = includes/
LIBFT_FOLDER = $(INCLUDES)libft
LIBFT_A = $(INCLUDES)libft/libft.a

SRC_DIR = src/
SRC_NAME = main.c\
			struct_setup/struct_setup_main.c\
			struct_setup/struct_setup_utils.c\
			struct_setup/mlx_setup.c\
			frees/free_struct.c\
			math_utils/color_utils.c\
			hooks/hook_setup.c\
			hooks/handle_background.c\
			hooks/raycast.c\
			hooks/minimap.c

SRC = $(addprefix $(SRC_DIR), $(SRC_NAME))

OBJ_DIR = obj/
OBJ_NAME = $(SRC_NAME:.c=.o)
OBJ = $(patsubst %, $(OBJ_DIR)%, $(OBJ_NAME))
$(OBJ_DIR)%.o:$(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	cc $(CC_FLAGS) $< -c -o $@

$(NAME): $(OBJ)
	@make full -C $(LIBFT_FOLDER)
	cc -o $(NAME) $(OBJ) $(LIBFT_A) $(OTHER_LIBS)

clean:
	@make clean -C $(LIBFT_FOLDER)
	rm -rf $(OBJ_DIR)

fclean: clean
	@make fclean -C $(LIBFT_FOLDER)
	rm -f $(NAME)

all: $(NAME)

re: fclean all