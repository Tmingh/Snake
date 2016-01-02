
#ifndef NEW_SNAKE_SNAKE_H
#define NEW_SNAKE_SNAKE_H
#include <SDL2/SDL_render.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define NODE_WIDTH 20
#define NODE_HEIGHT 20
#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 50
enum {
    UP, DOWN, RIGHT, LEFT
}direction;

struct node {
    int x;
    int y;
    int direction;
    struct node *previous;
    struct node *next;
}*head, *tail;
typedef struct node Node;

struct object {
    int x;
    int y;
    struct object *next;
}*map_wall, *all_grass, *all_bomb;
typedef struct object Wall;
typedef struct object Grass;
typedef struct object Bomb;

#endif //NEW_SNAKE_SNAKE_H
