
#include "snake.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Window *gWindow;
SDL_Renderer *gRenderer;
int act_flag = -1;
int move_flag = -1;
int food_x = 400;
int food_y = 300;
int goal_count = 0;


int snake_init();
int sdl_init();
int snake_action();
int snake_move();
int snake_draw();
int snake_food();

int food_action();
int food_eat();
int food_init();
int food_draw();

int main()
{
    bool quit = false;
    SDL_Event e;
    sdl_init();
    snake_init();
    while (!quit)
    {
        while (SDL_PollEvent(&e) != NULL)
        {
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (act_flag == 0) {
                switch (e.key.keysym.sym) {
                    case SDLK_LEFT:
                        if (head->direction != RIGHT){
                            head->direction = LEFT;
                            move_flag = 0;
                        }
                        break;
                    case SDLK_RIGHT:
                        if (head->direction != LEFT){
                            head->direction = RIGHT;
                            move_flag = 0;
                        }
                        break;
                    case SDLK_UP:
                        if (head->direction != DOWN){
                            head->direction = UP;
                            move_flag = 0;
                        }
                        break;
                    case SDLK_DOWN:
                        if (head->direction != UP) {
                            head->direction = DOWN;
                            move_flag = 0;
                        }
                        break;
                }
                act_flag = -1;
            }

        }
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xff);
        SDL_RenderClear(gRenderer);

        food_action();
        act_flag = snake_action();
        SDL_RenderPresent(gRenderer);
        SDL_Delay(300);
    }
    return 0;
}

int sdl_init()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    gWindow = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
    int imgFlags = IMG_INIT_PNG;
    IMG_Init(imgFlags);
}

int snake_init()
{
    Node *p1 = NULL, *p2 = NULL;
    p1 = (Node *)malloc(sizeof(Node));
    if (p1 == NULL){
        return -1;
    }
    p2 = (Node *)malloc(sizeof(Node));
    if (p2 == NULL){
        return -1;
    }

    p1->x = 40;
    p1->y = 20;
    p1->direction = RIGHT;

    p2->x = 20;
    p2->y = 20;
    p2->direction = RIGHT;

    p1->next = p2;
    p1->previous = NULL;
    p2->next = NULL;
    p2->previous = p1;

    head = p1;
    tail = p2;

    return 0;
}

int snake_action()
{
    if (move_flag == 0) {
        snake_move();
    }

    snake_draw();

    return 0;
}

int snake_move()
{
    Node *temp = tail;
    while (temp->previous != NULL) {
        temp->x = temp->previous->x;
        temp->y = temp->previous->y;
        temp->direction = temp->previous->direction;
        temp = temp->previous;
    }
    if (head->direction == UP) {
        head->y -= NODE_HEIGHT;
    }
    else if (head->direction == DOWN) {
        head->y += NODE_HEIGHT;
    }
    else if (head->direction == RIGHT) {
        head->x += NODE_WIDTH;
    }
    else if (head->direction == LEFT) {
        head->x -= NODE_WIDTH;
    }
    return 0;
}

int snake_draw()
{
    Node *temp = head->next;

    SDL_Rect head_block_1 = {head->x, head->y, NODE_WIDTH, NODE_HEIGHT};
    SDL_Rect head_block_2 = {temp->x, temp->y, NODE_WIDTH, NODE_HEIGHT};
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xff, 0x00, 0xff);
    SDL_RenderFillRect(gRenderer, &head_block_1);
    SDL_RenderFillRect(gRenderer, &head_block_2);

    while (temp->next != NULL){
        SDL_Rect after_block = {temp->next->x, temp->next->y, NODE_WIDTH, NODE_HEIGHT};
        SDL_SetRenderDrawColor(gRenderer, 0xff, 0x00, 0x00, 0xff);
        SDL_RenderFillRect(gRenderer, &after_block);
        temp = temp->next;
    }
    return 0;
}

int snake_food()
{
    Node *p = NULL;
    p = (Node *)malloc(sizeof(Node));
    if (p == NULL){
        return  -1;
    }

    if (tail->direction == RIGHT){
        p->x = tail->x - 20;
        p->y = tail->y;
    }
    else if (tail->direction == LEFT){
        p->x = tail->x + 20;
        p->y = tail->y;
    }
    else if (tail->direction == UP){
        p->x = tail->x;
        p->y = tail->y + 20;
    }
    else if (tail->direction == DOWN){
        p->x = tail->x;
        p->y = tail->y - 20;
    }

    p->direction = tail->direction;
    p->previous = tail;
    p->next = NULL;
    tail->next = p;
    tail = p;

    return 0;
}



int food_action()
{
    if (food_eat() == 0){
        food_init();
    }
    food_draw();
}

int food_eat()
{
    if (head->x == food_x && head->y == food_y){
        snake_food();
        return 0;
    } else {
        return -1;
    }
}

int food_init()
{
    Node *temp = head;
    srand((unsigned)time(NULL));
    food_x = rand() % 27 * 20;
    food_y = rand() % 19 * 20;
    while (temp != NULL){
        if (food_x == temp->x && food_y == temp->y){
            food_x = rand() % 27 * 20;
            food_y = rand() % 19 * 20;
            temp = head;
        }
        temp = temp->next;
    }
    return 0;
}

int food_draw()
{
    SDL_Rect food_block = {food_x, food_y, NODE_WIDTH, NODE_HEIGHT};
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xff, 0xff);
    SDL_RenderFillRect(gRenderer, &food_block);
}