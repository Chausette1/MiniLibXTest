#include "MLX_LINUX/mlx.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <X11/keysym.h>

#define WIDTH_WIN 500
#define HEIHGT_WIN 500
#define SQUARE_SIZE 100
#define SQUARE_SPEED 10
#define X_START 200
#define Y_START 200

enum e_keycodes
{
    KEY_ESC = XK_Escape,
    KEY_Z = XK_z,
    KEY_Q = XK_q,
    KEY_S = XK_s,
    KEY_D = XK_d,
    KEY_UP = XK_Up,
    KEY_LEFT = XK_Left,
    KEY_DOWN = XK_Down,
    KEY_RIGHT = XK_Right,
    KEY_TAB = XK_Tab,
};

typedef struct s_img
{
    void *img_ptr;
    char *addr;
    int bits_per_pixel;
    int line_length;
    int endian;
} t_img;

typedef struct s_mlx_data
{
    void *mlx_connection;
    void *mlx_window;
    t_img img;
    int x;
    int y;
} t_mlx_data;

int draw_square(t_mlx_data *data, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            mlx_pixel_put(data->mlx_connection, data->mlx_window, data->x + i, data->y + j, 0x00FF0000);
        }
    }

    return 0;
}

void put_pixel_to_image(t_img *img, int x, int y, int color)
{
    int offset;

    offset = (y * img->line_length) + (x * (img->bits_per_pixel / 8));

    *((unsigned int *)(offset + img->addr)) = color;
}

void generate_image(t_mlx_data *data)
{
    for (int y = 0; y < HEIHGT_WIN; y++)
    {
        for (int x = 0; x < WIDTH_WIN; x++)
        {
            if (x > data->x && x < data->x + SQUARE_SIZE && y > data->y && y < data->y + SQUARE_SIZE)
            {
                put_pixel_to_image(&data->img, x, y, 0x00FF0000);
            }
            else
            {
                put_pixel_to_image(&data->img, x, y, 0x00000000);
            }
        }
    }
}

void free_memory(t_mlx_data *mlx_data)
{
    if (mlx_data->mlx_connection)
    {
        free(mlx_data->mlx_connection);
    }
    if (mlx_data)
    {
        free(mlx_data);
    }
}

int clean_end(t_mlx_data *mlx_data)
{
    if (mlx_data->mlx_connection && mlx_data->mlx_window)
    {
        mlx_destroy_window(mlx_data->mlx_connection, mlx_data->mlx_window);
        mlx_destroy_display(mlx_data->mlx_connection);
        free_memory(mlx_data);
    }
    printf("Cleaning up and exiting\n");
    exit(0);
}

int key_hook(int keycode, t_mlx_data *param)
{
    printf("Keycode: %d\n", keycode);
    enum e_keycodes key = keycode;
    if (key == KEY_ESC)
    {
        clean_end(param);
    }
    else if (key == KEY_Z || key == KEY_UP)
    {
        if (param->y - SQUARE_SPEED >= 0)
        {
            param->y -= SQUARE_SPEED;
        }
        else
        {
            param->y = 0;
        }
        generate_image(param);
        mlx_put_image_to_window(param->mlx_connection, param->mlx_window, param->img.img_ptr, 0, 0);
    }
    else if (key == KEY_Q || key == KEY_LEFT)
    {
        if (param->x - SQUARE_SPEED >= 0)
        {
            param->x -= SQUARE_SPEED;
        }
        else
        {
            param->x = 0;
        }
        generate_image(param);
        mlx_put_image_to_window(param->mlx_connection, param->mlx_window, param->img.img_ptr, 0, 0);
    }
    else if (key == KEY_S || key == KEY_DOWN)
    {
        if (param->y + SQUARE_SPEED <= HEIHGT_WIN - SQUARE_SIZE)
        {
            param->y += SQUARE_SPEED;
        }
        else
        {
            param->y = HEIHGT_WIN - SQUARE_SIZE;
        }
        generate_image(param);
        mlx_put_image_to_window(param->mlx_connection, param->mlx_window, param->img.img_ptr, 0, 0);
    }
    else if (key == KEY_D || key == KEY_RIGHT)
    {
        if (param->x + SQUARE_SPEED <= WIDTH_WIN - SQUARE_SIZE)
        {
            param->x += SQUARE_SPEED;
        }
        else
        {
            param->x = WIDTH_WIN - SQUARE_SIZE;
        }
        generate_image(param);
        mlx_put_image_to_window(param->mlx_connection, param->mlx_window, param->img.img_ptr, 0, 0);
    }
    return 0;
}

int main()
{
    t_mlx_data *mlx_data;

    mlx_data = malloc(sizeof(t_mlx_data));
    mlx_data->x = X_START;
    mlx_data->y = Y_START;

    mlx_data->mlx_connection = mlx_init();
    if (mlx_data->mlx_connection == NULL)
    {
        clean_end(mlx_data);
    }

    mlx_data->mlx_window = mlx_new_window(mlx_data->mlx_connection, WIDTH_WIN, HEIHGT_WIN, "My first window");
    if (mlx_data->mlx_window == NULL)
    {
        clean_end(mlx_data);
    }

    mlx_data->img.img_ptr = mlx_new_image(mlx_data->mlx_connection, WIDTH_WIN, HEIHGT_WIN);
    mlx_data->img.addr = mlx_get_data_addr(mlx_data->img.img_ptr, &mlx_data->img.bits_per_pixel, &mlx_data->img.line_length, &mlx_data->img.endian);

    generate_image(mlx_data);
    mlx_put_image_to_window(mlx_data->mlx_connection, mlx_data->mlx_window, mlx_data->img.img_ptr, 0, 0);

    mlx_hook(mlx_data->mlx_window, 17, 0, clean_end, mlx_data);
    mlx_key_hook(mlx_data->mlx_window, key_hook, mlx_data);

    mlx_loop(mlx_data->mlx_connection);
    return 0;
}