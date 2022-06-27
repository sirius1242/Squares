#include <iostream>
#include <SDL2/SDL.h>

#include "board.hpp"

using namespace std;
#define GRID_SIZE 36

// Dark theme.
// SDL_Color grid_background = {22, 22, 22, 255}; // Barely Black
// SDL_Color grid_line_color = {44, 44, 44, 255}; // Dark grey
// SDL_Color grid_cursor_ghost_color = {44, 44, 44, 255};
// SDL_Color grid_cursor_color = {255, 255, 255, 255}; // White

// Light Theme.
SDL_Color grid_background = {233, 233, 233, 255}; // Barely white
SDL_Color grid_line_color = {200, 200, 200, 255}; // Very light grey
SDL_Color grid_cursor_ghost_color;
SDL_Color grid_cursor_color;
SDL_Color grid_wrong_color = {127, 127, 127, 255};

squares board;
int id;
int chessman;
SDL_Color grid_cursor_ghost_colors[4] = {{255, 127, 127, 127}, {127, 255, 127, 127}, {127, 127, 255, 127}, {255, 255, 159, 127}};
SDL_Color grid_cursor_colors[4] = {{255, 63, 63, 255}, {63, 255, 63, 255}, {63, 63, 255, 255}, {255, 255, 63, 255}};
void insert(SDL_Renderer *renderer, int cmnum, SDL_Rect &topleft, SDL_Color grid_color, int rotation);
void render_board(SDL_Renderer *renderer);
