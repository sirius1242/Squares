#include <iostream>
#include <SDL2/SDL.h>

#include "board.hpp"

using namespace std;
#define GRID_SIZE 36 // grid size on board
#define W_SELECTOR 16 // selector width
#define H_ROTATOR 4 // rotator height
#define S_SELECELL 24 // selector cell size
#define S_ROTACELL 16 // rotator cell size
#define W_ROTATOR 6 // rotator width
#define FPS_LIMIT 60
#define SDL_TICKSPEED 1000

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
int rotation;
set<int>lostplayers;
// strawberry, chartreuse, arctic, yellow
SDL_Color grid_cursor_ghost_colors[4] = {{0xF7, 0x4F, 0x52, 127}, {0xB3, 0xFB, 0x3B, 127}, {0x89, 0xEC, 0xFC, 127}, {0xFC, 0xE5, 0x4F, 127}};
// crimson, shamrock, azure, canary
SDL_Color grid_cursor_colors[4] = {{0xB2, 0x1B, 0x1A, 255}, {0x2C, 0xAA, 0x18, 255}, {0x1E, 0x28, 0xA4, 255}, {0xF9, 0xC8, 0x02, 255}};
//pair<int, int> selector_pos[CHESSNUM] = {{2, 3},{2, 6}, {2, 9}, {2, 12}};
pair<int, int> selector_pos[CHESSNUM] = {{1, 1},{1, 3}, {1, 5}, {1, 7}, {1, 10}, {1, 12}, {1, 15}, {1, 18}, {6, 1}, {10, 1}, {6, 4}, {6, 7}, {6, 10}, {6, 13}, {6, 17}, {12, 3}, {12, 10}, {12, 14}, {9, 15}, {12, 18}, {12, 7}};
Uint32 timeleft(Uint32 next_time);
int is_selector(int x, int y, int xoffset);
int is_rotator(int x, int y, int xoffset, int yoffset, int cmnum);
bool render_ghost(SDL_Rect &topleft, int yoffset, int xoffset, int cmnum, int rotation);
void insert(SDL_Renderer *renderer, int cmnum, SDL_Rect &topleft, SDL_Color grid_color, int rotation);
void render_board(SDL_Renderer *renderer);
void render_selector(SDL_Renderer *renderer, int xoffset, int id);
void render_rotator(SDL_Renderer *renderer, int yoffset, int xoffset, int cmnum, int id);
