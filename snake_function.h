#ifndef NEW_SNAKE_SNAKE_FUNCTION_H
#define NEW_SNAKE_SNAKE_FUNCTION_H
int menu_main();
int text_set();
int sdl_init();
int mouse_motion(int);
int mouse_down(int);
int close_game();

int snake_main();
int snake_init();
int snake_menu();
int snake_save();
int snake_text_set();
int snake_mouse_motion(int);
int snake_mouse_down(int);
int snake_action();
int snake_move();
int snake_draw();
int snake_food();
int snake_grass();
int snake_bomb();
int snake_bite_self();
int snake_bump_wall();

int food_action();
int food_eat();
int food_init();
int food_draw();

int wall_build();
int wall_read();
int wall_write();
int wall_draw();

int grass_action();
int grass_eat();
int grass_init();
int grass_draw();

int bomb_action();
int bomb_eat();
int bomb_init();
int bomb_draw();
#endif //NEW_SNAKE_SNAKE_FUNCTION_H
