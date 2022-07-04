#include "client.hpp"

Uint32 timeleft(Uint32 next_time)
{
    Uint32 now;
    now = SDL_GetTicks();
    if (next_time <= now)
        return 0;
    else
        return next_time - now;
}

int is_selector(int x, int y, int xoffset)
{
    for (int i = 0; i < CHESSNUM; i++)
    {
        if (x >= selector_pos[i].first * S_SELECELL + xoffset &&
            x <= selector_pos[i].first * S_SELECELL + xoffset + board.rotate(i, 0).width * S_SELECELL &&
            y >= selector_pos[i].second * S_SELECELL &&
            y <= selector_pos[i].second * S_SELECELL + board.rotate(i, 0).height * S_SELECELL)
        {
            // cout << "selected chessman No." << i << endl;
            return i;
        }
    }
    return -1;
}

int is_rotator(int x, int y, int xoffset, int yoffset, int cmnum)
{
    for (int i = 0; i < 8; i++)
    {
        if (x >= (i % 4 * W_ROTATOR + 2) * S_ROTACELL + xoffset &&
            x <= (i % 4 * W_ROTATOR + 2) * S_ROTACELL + xoffset + board.rotate(cmnum, i).width * S_ROTACELL &&
            y >= yoffset - ((2 - i / 4) * W_ROTATOR + 1) * S_ROTACELL &&
            y <= yoffset - ((2 - i / 4) * W_ROTATOR + 1) * S_ROTACELL + board.rotate(cmnum, i).height * S_ROTACELL)
            return i;
    }
    return -1;
}

void insert(SDL_Renderer *renderer, int cmnum, SDL_Rect &topleft, SDL_Color grid_color, int rotation)
{
    SDL_SetRenderDrawColor(renderer, grid_color.r, grid_color.g, grid_color.b, grid_color.a);
    squares::shape chessman = board.rotate(cmnum, rotation);
    for (int i = 0; i < chessman.size; i++)
    {
        SDL_Rect tmp = topleft;
        tmp.x += chessman.grids[i].second * GRID_SIZE;
        tmp.y += chessman.grids[i].first * GRID_SIZE;
        SDL_RenderFillRect(renderer, &tmp);
    }
}

void render_board(SDL_Renderer *renderer)
{
    for (int i = 0; i < BHEIGHT; i++)
        for (int j = 0; j < BWIDTH; j++)
        {
            if (board.getelem(i, j) >= 0)
            {
                SDL_Color grid_color = grid_cursor_colors[board.getelem(i, j)];
                SDL_SetRenderDrawColor(renderer, grid_color.r, grid_color.g, grid_color.b, grid_color.a);
                SDL_Rect tmp;
                tmp.x = j * GRID_SIZE;
                tmp.y = i * GRID_SIZE;
                tmp.w = GRID_SIZE;
                tmp.h = GRID_SIZE;
                SDL_RenderFillRect(renderer, &tmp);
            }
        }
}

void render_selector(SDL_Renderer *renderer, int xoffset, int id)
{
    for (int i = 0; i < CHESSNUM; i++)
    {
        SDL_Rect base;
        SDL_Color grid_color;
        if (board.checkused(i, id))
            grid_color = grid_wrong_color;
        else if (chessman == i)
            grid_color = grid_cursor_ghost_colors[id];
        else
            grid_color = grid_cursor_colors[id];
        SDL_SetRenderDrawColor(renderer, grid_color.r, grid_color.g, grid_color.b, grid_color.a);
        base.x = selector_pos[i].first * S_SELECELL + xoffset;
        base.y = selector_pos[i].second * S_SELECELL;
        base.w = S_SELECELL;
        base.h = S_SELECELL;
        for (int j = 0; j < board.rotate(i, 0).size; j++)
        {
            SDL_Rect draw = base;
            draw.x += board.rotate(i, 0).grids[j].second * S_SELECELL;
            draw.y += board.rotate(i, 0).grids[j].first * S_SELECELL;
            SDL_RenderFillRect(renderer, &draw);
        }
    }
}

void render_rotator(SDL_Renderer *renderer, int yoffset, int xoffset, int cmnum, int id)
{
    for (int i = 0; i < 8; i++)
    {
        SDL_Rect base;
        SDL_Color grid_color;
        squares::shape rotator;
        if (board.checkused(cmnum, id))
            grid_color = grid_wrong_color;
        else if (rotation == i)
            grid_color = grid_cursor_ghost_colors[id];
        else
            grid_color = grid_cursor_colors[id];
        SDL_SetRenderDrawColor(renderer, grid_color.r, grid_color.g, grid_color.b, grid_color.a);
        base.x = (i % 4 * W_ROTATOR + 2) * S_ROTACELL + xoffset;
        base.y = yoffset - ((2 - i / 4) * W_ROTATOR + 1) * S_ROTACELL;
        base.w = S_ROTACELL;
        base.h = S_ROTACELL;
        rotator = board.rotate(cmnum, i);
        for (int j = 0; j < rotator.size; j++)
        {
            SDL_Rect draw = base;
            draw.x += rotator.grids[j].second * S_ROTACELL;
            draw.y += rotator.grids[j].first * S_ROTACELL;
            SDL_RenderFillRect(renderer, &draw);
        }
    }
}

bool render_ghost(SDL_Rect &topleft, int yoffset, int xoffset, int cmnum, int rotation)
{
    if (rotation % 2)
    {
        if (topleft.x + board.rotate(cmnum, 0).height * GRID_SIZE > xoffset || topleft.y + board.rotate(cmnum, 0).width * GRID_SIZE > yoffset)
            return false;
    }
    else
    {
        if (topleft.x + board.rotate(cmnum, 0).width * GRID_SIZE > xoffset || topleft.y + board.rotate(cmnum, 0).height * GRID_SIZE > yoffset)
            return false;
    }
    return true;
}

int main()
{
    board.init();
    int grid_cell_size = GRID_SIZE;
    int grid_width = BWIDTH;
    int grid_height = BHEIGHT;
    id = 0;
    chessman = 0;
    rotation = 0;
    bool firstround = true;
    double interval = static_cast<double>(SDL_TICKSPEED) / FPS_LIMIT;
    Uint32 next_time;

    // + 1 so that the last grid lines fit in the screen.
    int window_width = (grid_width * grid_cell_size) + (W_SELECTOR * S_SELECELL) + 1;
    int board_width = (grid_width * grid_cell_size) + 1;
    int window_height = (grid_height * grid_cell_size) + 1;
    int selector_height = BSIZE * S_SELECELL;
    // int window_height = (grid_height * grid_cell_size) + (H_ROTATOR * S_SELECELL) + 1;
    // int board_height = (grid_height * grid_cell_size) + 1;

    // Place the grid cursor in the middle of the screen.
    SDL_Rect grid_cursor = {
        .x = (grid_width - 1) / 2 * grid_cell_size,
        .y = (grid_height - 1) / 2 * grid_cell_size,
        .w = grid_cell_size,
        .h = grid_cell_size,
    };

    // The cursor ghost is a cursor that always shows in the cell below the
    // mouse cursor.
    SDL_Rect grid_cursor_ghost = {grid_cursor.x, grid_cursor.y, grid_cell_size,
                                  grid_cell_size};

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Initialize SDL: %s",
                     SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window;
    SDL_Renderer *renderer;
    if (SDL_CreateWindowAndRenderer(window_width, window_height, 0, &window,
                                    &renderer) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Create window and renderer: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_SetWindowTitle(window, "Squares");

    SDL_bool quit = SDL_FALSE;
    SDL_bool mouse_active = SDL_FALSE;
    SDL_bool mouse_hover = SDL_FALSE;
    grid_cursor_ghost_color = grid_cursor_ghost_colors[id];

    next_time = SDL_GetTicks();
    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_w:
                        case SDLK_UP:
                            grid_cursor.y -= grid_cell_size;
                            break;
                        case SDLK_s:
                        case SDLK_DOWN:
                            grid_cursor.y += grid_cell_size;
                            break;
                        case SDLK_a:
                        case SDLK_LEFT:
                            grid_cursor.x -= grid_cell_size;
                            break;
                        case SDLK_d:
                        case SDLK_RIGHT:
                            grid_cursor.x += grid_cell_size;
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        if (event.motion.x < board_width)
                        {
                            grid_cursor.x = (event.motion.x / grid_cell_size) * grid_cell_size;
                            grid_cursor.y = (event.motion.y / grid_cell_size) * grid_cell_size;
                            if (board.tryinsert(chessman, rotation, make_pair(grid_cursor.y / GRID_SIZE, grid_cursor.x / GRID_SIZE), id, firstround))
                            {
                                board.insert(chessman, rotation, make_pair(grid_cursor.y / GRID_SIZE, grid_cursor.x / GRID_SIZE), id, firstround);
                                if (firstround)
                                    firstround = false;
                                /* four clients on one client
                                if(!firstround)
                                {
                                        if(int loseplayers = board.check())
                                        {
                                                int i=0;
                                                while(loseplayers)
                                                {
                                                        if(loseplayers % 2 == 1)
                                                        {
                                                                lostplayers.insert(i);
                                                                if(lostplayers.size() == PNUM)
                                                                        cout << "player " << i << "win!" << endl;
                                                                else
                                                                        cout << "player " << i << " lost!" << endl;
                                                        }
                                                        loseplayers >>= 1;
                                                        i++;
                                                }
                                        }
                                }
                                if (firstround && id==3)
                                        firstround = false;
                                id++;
                                if(!lostplayers.empty())
                                {
                                        if(lostplayers.size() == PNUM)
                                                break;
                                        while(lostplayers.count(id))
                                        {
                                                id++;
                                                id%=4;
                                        }
                                }
                                id%=4;
                                */
                            }
                        }
                        else if (event.motion.y < selector_height)
                        {
                            if (is_selector(event.motion.x, event.motion.y, board_width) >= 0)
                            {
                                chessman = is_selector(event.motion.x, event.motion.y, board_width);
                                rotation = 0;
                            }
                        }
                        else
                        {
                            if (is_rotator(event.motion.x, event.motion.y, board_width, window_height, chessman) >= 0)
                                rotation = is_rotator(event.motion.x, event.motion.y, board_width, window_height, chessman);
                        }
                    }
                    else if (event.button.button == SDL_BUTTON_RIGHT)
                    {
                        rotation++;
                        rotation %= 8;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    grid_cursor_ghost.x = (event.motion.x / grid_cell_size) * grid_cell_size;
                    grid_cursor_ghost.y = (event.motion.y / grid_cell_size) * grid_cell_size;

                    if (!mouse_active)
                        mouse_active = SDL_TRUE;
                    break;
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_ENTER && !mouse_hover)
                        mouse_hover = SDL_TRUE;
                    else if (event.window.event == SDL_WINDOWEVENT_LEAVE && mouse_hover)
                        mouse_hover = SDL_FALSE;
                    break;
                case SDL_QUIT:
                    quit = SDL_TRUE;
                    break;
            }
        }

        // Draw grid background.
        SDL_SetRenderDrawColor(renderer, grid_background.r, grid_background.g,
                               grid_background.b, grid_background.a);
        SDL_Delay(timeleft(next_time));
        next_time += interval;
        SDL_RenderClear(renderer);

        // Draw grid lines.
        SDL_SetRenderDrawColor(renderer, grid_line_color.r, grid_line_color.g,
                               grid_line_color.b, grid_line_color.a);

        for (int x = 0; x < 1 + grid_width * grid_cell_size;
             x += grid_cell_size)
        {
            SDL_RenderDrawLine(renderer, x, 0, x, window_height);
        }

        for (int y = 0; y < 1 + grid_height * grid_cell_size;
             y += grid_cell_size)
        {
            SDL_RenderDrawLine(renderer, 0, y, board_width, y);
        }
        // Draw selector break line
        for (int i = 0; i < 3; i++)
            SDL_RenderDrawLine(renderer, board_width, selector_height + i, window_width, selector_height + i);

        // Draw grid ghost cursor.
        if (mouse_active && mouse_hover && render_ghost(grid_cursor_ghost, window_height, board_width, chessman, rotation))
        {
            if (board.tryinsert(chessman, rotation, make_pair(grid_cursor_ghost.y / GRID_SIZE, grid_cursor_ghost.x / GRID_SIZE), id, firstround))
                insert(renderer, chessman, grid_cursor_ghost, grid_cursor_ghost_colors[id], rotation);
            else
                insert(renderer, chessman, grid_cursor_ghost, grid_wrong_color, rotation);
            /*
            SDL_SetRenderDrawColor(renderer, grid_cursor_ghost_color.r,
                                                       grid_cursor_ghost_color.g,
                                                       grid_cursor_ghost_color.b,
                                                       grid_cursor_ghost_color.a);
            SDL_RenderFillRect(renderer, &grid_cursor_ghost);
            */
        }

        // Draw grid cursor.
        // insert(renderer, 10, grid_cursor, grid_cursor_color, rotation);
        /*
        SDL_SetRenderDrawColor(renderer, grid_cursor_color.r,
                                                   grid_cursor_color.g, grid_cursor_color.b,
                                                   grid_cursor_color.a);
        SDL_RenderFillRect(renderer, &grid_cursor);
        */
        render_selector(renderer, board_width, id);
        render_rotator(renderer, window_height, board_width, chessman, id);
        render_board(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
