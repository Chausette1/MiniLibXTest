CC = gcc

C_FLAGS = -Wall -Wextra -Werror -I/usr/include -Iminilibx-linux -O3

TARGET = main

MLX_LIB = -LMLX_LINUX -lmlx_Linux
X11_LIB = -L/usr/lib -lXext -lX11
MATH_LIB = -lm
ZLIB = -lz

OBJS = main.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(MLX_LIB) $(X11_LIB) $(MATH_LIB) $(ZLIB) -o $(TARGET)
	
%.o: %.c
	$(CC) $(C_FLAGS) -c $< -o $@


clean:
	rm -f $(OBJS) $(TARGET)

re: clean all