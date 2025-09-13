cc test_extraction.c \
  -I../src/ \
  -I../libs/libft/ \
  -I../libs/c-logger/src/ \
  -I../libs/minilibx-linux/ \
  -I../libs/minunit \
  $(find ../obj -name "*.o" ! -name "main.o") \
  ../libs/libft/libft.a \
  -L../libs/minilibx-linux -lmlx -lXext -lX11 -lm \
  -o test_extraction.out

