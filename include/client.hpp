#include <iostream>
#include <SDL2/SDL.h>

#include "board.hpp"

using namespace std;
#define GRID_SIZE 36
#define W_SELECTOR 16
#define H_ROTATOR 4
#define S_SELECELL 24
#define S_ROTACELL 16
#define W_ROTATOR 6

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
//pair<int, int> selector_pos[CHESSNUM] = {{2, 3},{2, 6}, {2, 9}, {2, 12}};
pair<int, int> selector_pos[CHESSNUM] = {{1, 1},{1, 3}, {1, 5}, {1, 7}, {1, 10}, {1, 12}, {1, 15}, {1, 18}, {6, 1}, {10, 1}, {6, 4}, {6, 7}, {6, 10}, {6, 13}, {6, 17}, {12, 3}, {12, 10}, {12, 14}, {9, 15}, {12, 18}, {12, 7}};
int is_selector(int x, int y, int xoffset);
int is_rotator(int x, int y, int xoffset, int yoffset, int cmnum);
bool render_ghost(SDL_Rect &topleft, int yoffset, int xoffset, int cmnum, int rotation);
void insert(SDL_Renderer *renderer, int cmnum, SDL_Rect &topleft, SDL_Color grid_color, int rotation);
void render_board(SDL_Renderer *renderer);
void render_selector(SDL_Renderer *renderer, int xoffset, int id);
void render_rotator(SDL_Renderer *renderer, int yoffset, int xoffset, int cmnum, int id);
