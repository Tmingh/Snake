//
// Created by minghua on 15-12-27.
//

#ifndef NEW_SNAKE_SNAKE_H
#define NEW_SNAKE_SNAKE_H
#include <SDL2/SDL_render.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define NODE_WIDTH 20
#define NODE_HEIGHT 20
enum {
    UP, DOWN, RIGHT, LEFT
}direction;

/*蛇的结构体*/
struct node {
    int x;
    int y;
    int direction;
    struct node *previous;
    struct node *next;
}*head, *tail;
typedef struct node Node;

#endif //NEW_SNAKE_SNAKE_H
