#ifndef _CLIENT_HPP
#define _CLIENT_HPP

#include <SDL2/SDL.h>

#include <iostream>

#include "board.hpp"

#define GRID_SIZE 36  // grid size on board
#define W_SELECTOR 16 // selector width
#define H_ROTATOR 4   // rotator height
#define S_SELECELL 24 // selector cell size
#define S_ROTACELL 16 // rotator cell size
#define W_ROTATOR 6   // rotator width
#define FPS_LIMIT 60
#define SDL_TICKSPEED 1000

extern SDL_Color grid_background;
extern SDL_Color grid_line_color;
extern SDL_Color grid_cursor_ghost_color;
extern SDL_Color grid_cursor_color;
extern SDL_Color grid_wrong_color;

extern Squares board;
extern int id;
extern int chessman;
extern int rotation;
extern int active_player;
extern int firstround;
extern std::set<int> lostplayers;
extern SDL_Color grid_cursor_ghost_colors[4];
extern SDL_Color grid_cursor_colors[4];
extern Coord selector_pos[CHESSNUM];

Uint32 timeleft(Uint32 next_time);
int is_selector(int x, int y, int xoffset);
int is_rotator(int x, int y, int xoffset, int yoffset, int cmnum);
bool render_ghost(SDL_Rect &topleft, int yoffset, int xoffset, int cmnum, int rotation);
void insert(SDL_Renderer *renderer, int cmnum, SDL_Rect &topleft, SDL_Color grid_color, int rotation);
void render_board(SDL_Renderer *renderer);
void render_selector(SDL_Renderer *renderer, int xoffset, int id);
void render_rotator(SDL_Renderer *renderer, int yoffset, int xoffset, int cmnum, int id);
bool after_move(bool single_client);

#endif // _CLIENT_HPP
