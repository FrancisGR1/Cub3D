NAME = cub3D

CFLAGS = -Wall -Werror -Wextra #-g
OPTIMIZE = -O3

# directories
SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = libs/libft
MLX_DIR = libs/minilibx-linux
LOGGER_DIR = libs/c-logger/src

# log level default 
LOG_LEVEL ?= 7
CFLAGS += -DLOG_LEVEL=$(LOG_LEVEL)


# bonus for the project
ifdef BONUS
    CFLAGS += -DBONUS=1
endif

# sources
SRC_FILES = \
	    data_extraction.c \
	    data_extraction_utils.c \
	    draw_utils.c \
	    draw_vertical.c \
	    dynamic_array_to_fixed_array.c \
	    game.c \
	    game_loop.c \
	    getters.c \
	    is_valid.c \
	    is_valid_map.c \
	    is_valid_p2.c \
	    keys.c \
	    main.c \
	    minimap_bonus.c \
	    player.c \
	    raycast.c \
	    render.c \
	    rgb.c \
	    setters.c \
	    textures.c \
	    update.c \
	    vector.c \
	    vector_operations.c \
	    window.c

# need this for print debugging
LOGGER_FILE = \
	    logger.c

# put src files and logger files in src
SRC = $(addprefix $(SRC_DIR)/,$(SRC_FILES)) \
      $(addprefix $(LOGGER_DIR)/,$(LOGGER_FILE))

# objects
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(filter $(SRC_DIR)/%.c,$(SRC))) \
      $(patsubst $(LOGGER_DIR)/%.c, $(OBJ_DIR)/logger_%.o, $(filter $(LOGGER_DIR)/%.c,$(SRC)))

# headers
HEADER_FILES = \
	       cub3d.h \
	       vector.h

HEADERS = $(addprefix $(SRC_DIR)/,$(HEADER_FILES))

MK = Makefile

# libs
LIBFT = $(LIBFT_DIR)/libft.a
MLX = $(MLX_DIR)/libmlx_Linux.a

# linking flags
MLX_LINKS = -L$(MLX_DIR) -lmlx_Linux -L/usr/lib -lXext -lX11 -lm -lz
LIBFT_LINKS = -L$(LIBFT_DIR) -lft
INCLUDES = -I$(SRC_DIR) -I$(LOGGER_DIR) -I$(LIBFT_DIR) -I$(MLX_DIR) -I/usr/include

# tests
TEST_MAP = assets/maps/textures_faulty_xpm_content.cub

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

$(OBJ_DIR)/logger_%.o: $(LOGGER_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ): $(HEADERS) $(MK)

# libs
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(MLX):
	$(MAKE) -C $(MLX_DIR)

# rules
clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean
	$(MAKE) -C $(MLX_DIR)   clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(MAKE) -C $(MLX_DIR) fclean

re: fclean all

bonus:
	rm -f $(NAME)
	rm -rf $(OBJ_DIR)
	$(MAKE) all BONUS=1

fast: clean $(NAME)


# go grab the assets and the test script
REPO_URL = https://github.com/FrancisGR1/Cub3D.git
TMP_DIR = ../tmp_repo
ASSETS_DIR = assets
TEST_SCRIPT = test_errors.sh
TESTS_DIR = tests

.PHONY: tests
tests:
	git clone $(REPO_URL) $(TMP_DIR); \
	mv $(TMP_DIR)/assets ./; \
	mkdir -p $(TESTS_DIR); \
	mv $(TMP_DIR)/tests/$(TEST_SCRIPT) $(TESTS_DIR)/; \
	chmod +x $(TESTS_DIR)/$(TEST_SCRIPT); \
	rm -rf $(TMP_DIR); \
	cd $(TESTS_DIR) && ./$(TEST_SCRIPT)
