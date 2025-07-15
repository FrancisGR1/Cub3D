NAME = cub3d

CFLAGS = -Wall -Werror -Wextra
OPTIMIZE = -O3

# directories
SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = libs/libft
MLX_DIR = libs/minilibx-linux

# sources
SRC_FILES = \
	    main.c
SRC = $(addprefix $(SRC_DIR)/,$(SRC_FILES))

# objects
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# headers
HEADER_FILES = \
	       cub3d.h

HEADERS = $(addprefix $(SRC_DIR)/,$(HEADER_FILES))

MK = Makefile

# libs
LIBFT = $(LIBFT_DIR)/libft.a
MLX = $(MLX_DIR)/libmlx_Linux.a

# linking flags
MLX_LINKS = -L$(MLX_DIR) -lmlx_Linux -L/usr/lib -lXext -lX11 -lm -lz
LIBFT_LINKS = -L$(LIBFT_DIR) -lft
INCLUDES = -I$(SRC_DIR) -I$(LIBFT_DIR) -I$(MLX_DIR) -I/usr/include

# tests
TEST_MAP = maps/2x2.cub

# Colors for pretty output
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
MAGENTA = \033[0;35m
CYAN = \033[0;36m
WHITE = \033[0;37m
RESET = \033[0m

all: $(NAME) 

# make executable
$(NAME): $(OBJ) $(LIBFT) $(MLX)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJ) -o $(NAME) $(LIBFT_LINKS) $(MLX_LINKS)

# objects
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR) 
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ): $(HEADERS) $(MK)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(MLX):
	$(MAKE) -C $(MLX_DIR)

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean
	$(MAKE) -C $(MLX_DIR)   clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(MAKE) -C $(MLX_DIR) fclean

re: fclean all

leaks: $(NAME)
	valgrind --track-origins=yes --show-leak-kinds=all --leak-check=full $(NAME) $(TEST_MAP)

norm:
	@norminette $(SRC_DIR) | grep -E "(Error|Warning)" || echo "$(GREEN)Norminette OK!$(RESET)"

# quick test runs
test:
	./$(NAME) $(TEST_MAP)

debug: CFLAGS += -fsanitize=address -fsanitize=undefined -g3
debug: OPTIMIZE = -00
debug: re
