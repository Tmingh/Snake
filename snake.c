
#include "snake.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Window *gWindow;
SDL_Renderer *gRenderer;
int act_flag = -1;
int move_flag = -1;
int grass_count = 0;
int bomb_count = 0;
int goal = 1;
int food_x;
int food_y;


int snake_init();
int sdl_init();
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

int main()
{
    bool quit = false;
    SDL_Event e;
    sdl_init();
    snake_init();
    srand((unsigned)time(NULL));
    all_grass = (Grass *)malloc(sizeof(Grass));
    all_grass->next = NULL;
    all_bomb = (Bomb *)malloc(sizeof(Bomb));
    all_bomb->next = NULL;
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

        wall_build();
        food_action();
        grass_action();
        bomb_action();

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

    p1->x = 220;
    p1->y = 200;
    p1->direction = RIGHT;

    p2->x = 200;
    p2->y = 200;
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
    snake_bite_self();
    snake_bump_wall();
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

int snake_bite_self()
{
    Node *temp = head->next;
    while (temp != NULL){
        if (temp->x == head->x && temp->y == head->y){
            printf("you lost\n");
            exit(0);
        }
        temp = temp->next;
    }
}

int snake_bump_wall()
{
    Wall *temp = map_wall;
    while (temp != NULL){
        if (temp->x == head->x && temp->y == head->y){
            printf("you lost\n");
            exit(0);
        }
        temp = temp->next;
    }
}

int snake_draw()
{
    Node *temp = head->next;

    SDL_Rect head_block_1 = {head->x, head->y, NODE_WIDTH, NODE_HEIGHT};
    SDL_Rect head_block_2 = {temp->x, temp->y, NODE_WIDTH, NODE_HEIGHT};
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xff, 0x00, 0xff);
    SDL_RenderFillRect(gRenderer, &head_block_1);
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x8B, 0x8B, 0xff);
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

int snake_grass()
{
    Node *temp = tail->previous;
    temp->next = NULL;
    free(tail);
    tail = temp;
    return 0;
}

int snake_bomb()
{
    int i;
    Node *temp;
    int sum = goal - goal/2;
    goal /= 2;
    for (i = 0; i < sum; i++){
        temp = tail->previous;
        free(tail);
        tail = temp;
        tail->next = NULL;
    }
}

int food_action()
{
    if (move_flag == -1){
        food_init();
        move_flag = 1;
    }
    if (food_eat() == 0){
        food_init();
    }
    food_draw();
}

int food_eat()
{
    if (head->x == food_x && head->y == food_y){
        goal++;
        snake_food();
        return 0;
    } else {
        return -1;
    }
}

int food_init()
{
    Node *temp = head;
    food_x = rand() % 32 * 20;
    food_y = rand() % 24 * 20;
    while (temp != NULL){
        if (food_x == temp->x && food_y == temp->y){
            food_x = rand() % 32 * 20;
            food_y = rand() % 24 * 20;
            temp = head;
            continue;
        }
        temp = temp->next;
    }
    Wall *temp2 = map_wall;
    while (temp2 != NULL){
        if (food_x == temp2->x && food_y == temp2->y){
            food_x = rand() % 32 * 20;
            food_y = rand() % 24 * 20;
            temp2 = map_wall;
            continue;
        }
        temp2 = temp2->next;
    }
    return 0;
}

int food_draw()
{
    SDL_Rect food_block = {food_x, food_y, NODE_WIDTH, NODE_HEIGHT};
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xff, 0xff);
    SDL_RenderFillRect(gRenderer, &food_block);
}

int wall_build()
{
    wall_write();
    wall_read();
    wall_draw();
}

int wall_write()
{
    int i;
    char *str;
    FILE *fp = fopen("/home/minghua/ClionProjects/new_snake/map/map.json", "w");
    cJSON *root = cJSON_CreateArray();
    cJSON *map = cJSON_CreateArray();

    cJSON_AddItemToArray(root, map);
    for (i = 0; i < WINDOW_WIDTH / NODE_WIDTH; i++){
        cJSON *wall_node_1 = cJSON_CreateObject();
        cJSON_AddNumberToObject(wall_node_1, "x", i * NODE_WIDTH);
        cJSON_AddNumberToObject(wall_node_1, "y", 0);
        cJSON_AddItemToArray(map, wall_node_1);

        cJSON *wall_node_2 = cJSON_CreateObject();
        cJSON_AddNumberToObject(wall_node_2, "x", i * NODE_WIDTH);
        cJSON_AddNumberToObject(wall_node_2, "y", WINDOW_HEIGHT - NODE_HEIGHT);
        cJSON_AddItemToArray(map, wall_node_2);

    }

    for (i = 1; i < WINDOW_HEIGHT / NODE_HEIGHT; i++){
        cJSON *wall_node_1 = cJSON_CreateObject();
        cJSON_AddNumberToObject(wall_node_1, "x", 0);
        cJSON_AddNumberToObject(wall_node_1, "y", i * NODE_HEIGHT);
        cJSON_AddItemToArray(map, wall_node_1);

        cJSON* wall_node_2 = cJSON_CreateObject();
        cJSON_AddNumberToObject(wall_node_2, "x", WINDOW_WIDTH - NODE_WIDTH);
        cJSON_AddNumberToObject(wall_node_2, "y", i * NODE_HEIGHT);
        cJSON_AddItemToArray(map, wall_node_2);
    }

    str = cJSON_Print(root);
    fputs(str, fp);
    fclose(fp);
}

int wall_read()
{
    long len;
    int wall_sum, i;
    cJSON *root;
    FILE *fp = fopen("/home/minghua/ClionProjects/new_snake/map/map.json", "r");
    map_wall = (Wall *)malloc(sizeof(Wall));
    map_wall->next = NULL;
    Wall *temp = map_wall;

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    rewind(fp);
    char str[len+1];
    str[len] = 0;
    fread(str, 1, (size_t)len, fp);
    root = cJSON_Parse(str);

    cJSON *map = cJSON_GetArrayItem(root, 0);
    wall_sum = cJSON_GetArraySize(map);

    for (i = 0; i < wall_sum; i++){
        cJSON *p1 = cJSON_GetArrayItem(map, i);
        cJSON *p2 = cJSON_GetObjectItem(p1, "x");
        temp->x = p2->valueint;
        p2 = cJSON_GetObjectItem(p1, "y");
        temp->y = p2->valueint;
        temp->next = (Wall *)malloc(sizeof(Wall));
        temp = temp->next;
        temp->next = NULL;
    }
    fclose(fp);
    return 0;
}

int wall_draw()
{
    Wall *temp = map_wall;

    while (temp != NULL){
        SDL_Rect wall_block = {temp->x, temp->y, NODE_WIDTH, NODE_HEIGHT};
        SDL_SetRenderDrawColor(gRenderer, 0x8B, 0x89, 0x89, 0xff);
        SDL_RenderFillRect(gRenderer, &wall_block);
        temp = temp->next;
    }
}



int grass_action()
{
    if (move_flag == 1) {
        grass_init();
        move_flag = 2;
    }
    if (grass_count == 1) {
        grass_init();
        grass_count = 0;
    }
    grass_eat();
    grass_draw();
}

int grass_init()
{
    int grass_mount, i;
    Grass *temp_grass, *p;
    Wall *temp_wall;
    temp_grass = (Grass *)malloc(sizeof(Grass));
    all_grass->next = temp_grass;
    grass_mount = rand() % 4 + 3;
    for (i = 0; i < grass_mount; i++){
        temp_wall = map_wall;
        temp_grass->x = rand() % 32 * 20;
        temp_grass->y = rand() % 24 * 20;
        while (temp_wall != NULL){
            if ((temp_grass->x == temp_wall->x && temp_grass->y == temp_wall->y) ||
                    (food_x == temp_grass->x && food_y == temp_grass->y)){
                temp_grass->x = rand() % 32 * 20;
                temp_grass->y = rand() % 24 * 20;
                temp_wall = map_wall;
            } else {
                temp_wall = temp_wall->next;
            }
        }
        p = (Grass *)malloc(sizeof(Grass));
        p->next = NULL;
        temp_grass->next = p;
        temp_grass = p;
    }
    return 0;
}

int grass_eat()
{
    Grass *temp = all_grass;
    Grass *del_temp;
    while (temp->next != NULL){
        if (head->x == temp->next->x && head->y == temp->next->y){
            snake_grass();
            del_temp = temp->next;
            temp->next = del_temp->next;
            free(del_temp);
            grass_count++;
            goal--;
            if (goal <= 0){
                printf("you lost\n");
                exit(0);
            }
            return 0;
        }
        temp = temp->next;
    }
    return 0;
}

int grass_draw()
{
    Grass *temp = all_grass->next;

    while (temp != NULL){
        SDL_Rect grass_block = {temp->x, temp->y, NODE_WIDTH, NODE_HEIGHT};
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0xE5, 0xEE, 0xff);
        SDL_RenderFillRect(gRenderer, &grass_block);
        temp = temp->next;
    }
    return 0;
}

int bomb_action()
{
    if (move_flag == 2) {
        bomb_init();
        move_flag = 3;
    }
    if (bomb_count == 1) {
        bomb_init();
        bomb_count = 0;
    }
    bomb_eat();
    bomb_draw();
}

int bomb_init()
{
    int bomb_mount, i;
    Grass *temp_grass, *temp_bomb, *p;
    Wall *temp_wall;
    temp_bomb = (Bomb *)malloc(sizeof(Bomb));
    all_bomb->next = temp_bomb;
    bomb_mount = rand() % 3 + 2;
    for (i = 0; i < bomb_mount; i++){
        temp_wall = map_wall;
        temp_grass = all_grass->next;
        temp_bomb->x = 20;//rand() % 32 * 20;
        temp_bomb->y = 0;//rand() % 24 * 20;
        while (temp_grass  != NULL){
            while (temp_wall != NULL){
                if ((temp_bomb->x == temp_wall->x && temp_bomb->y == temp_wall->y) ||
                        (food_x == temp_bomb->x && food_y == temp_bomb->y) ||
                        (temp_bomb->x == temp_grass->x && temp_bomb->y == temp_grass->y)){
                    temp_bomb->x = rand() % 32 * 20;
                    temp_bomb->y = rand() % 24 * 20;
                    temp_wall = map_wall;
                    temp_grass = all_grass->next;
                } else{
                    temp_wall = temp_wall->next;
                }
            }
            temp_grass = temp_grass->next;
        }
        p = (Bomb *)malloc(sizeof(Bomb));
        p->next = NULL;
        temp_bomb->next = p;
        temp_bomb = p;
    }
    return 0;
}

int bomb_eat()
{
    Bomb *temp = all_bomb;
    Bomb *del_temp;
    while (temp->next != NULL){
        if (head->x == temp->next->x && head->y == temp->next->y){
            snake_bomb();
            del_temp = temp->next;
            temp->next = del_temp->next;
            free(del_temp);
            bomb_count++;
            if (goal == 0){
                printf("you lost\n");
                exit(0);
            }
            return 0;
        }
        temp = temp->next;
    }
    return 0;
}

int bomb_draw()
{
    Bomb *temp = all_bomb->next;

    while (temp != NULL){
        SDL_Rect bomb_block = {temp->x, temp->y, NODE_WIDTH, NODE_HEIGHT};
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x7F, 0x24, 0xff);
        SDL_RenderFillRect(gRenderer, &bomb_block);
        temp = temp->next;
    }
    return 0;
}