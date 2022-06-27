#include "client.hpp"

void insert(SDL_Renderer *renderer, int cmnum, SDL_Rect &topleft, SDL_Color grid_color, int rotation)
{
	SDL_SetRenderDrawColor(renderer, grid_color.r, grid_color.g, grid_color.b, grid_color.a);
	squares::shape chessman = board.rotate(cmnum, rotation);
	for(int i=0; i<chessman.size; i++)
	{
		SDL_Rect tmp = topleft;
		tmp.x += chessman.grids[i].second * GRID_SIZE;
		tmp.y += chessman.grids[i].first * GRID_SIZE;
        SDL_RenderFillRect(renderer, &tmp);
	}
}

void render_board(SDL_Renderer *renderer)
{
	for(int i=0; i<BHEIGHT; i++)
		for(int j=0; j<BWIDTH; j++)
		{
			if(board.getelem(i, j) >= 0)
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

int main()
{
	board.init();
	int grid_cell_size = GRID_SIZE;
    int grid_width = BWIDTH;
    int grid_height = BHEIGHT;
	int rotation = 0;
	id = 0;
	chessman = 15;
	bool firstround = true;

    // + 1 so that the last grid lines fit in the screen.
    int window_width = (grid_width * grid_cell_size) + 1;
    int window_height = (grid_height * grid_cell_size) + 1;

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

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Initialize SDL: %s",
                     SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window;
    SDL_Renderer *renderer;
    if (SDL_CreateWindowAndRenderer(window_width, window_height, 0, &window,
                                    &renderer) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Create window and renderer: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_SetWindowTitle(window, "SDL Grid");

    SDL_bool quit = SDL_FALSE;
    SDL_bool mouse_active = SDL_FALSE;
    SDL_bool mouse_hover = SDL_FALSE;
	grid_cursor_ghost_color = grid_cursor_ghost_colors[id];

    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
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
				if(event.button.button == SDL_BUTTON_LEFT)
				{
					grid_cursor.x = (event.motion.x / grid_cell_size) * grid_cell_size;
					grid_cursor.y = (event.motion.y / grid_cell_size) * grid_cell_size;
					if(board.tryinsert(chessman, rotation, make_pair(grid_cursor.y/GRID_SIZE, grid_cursor.x/GRID_SIZE), id, firstround))
					{
						board.insert(chessman, rotation, make_pair(grid_cursor.y/GRID_SIZE, grid_cursor.x/GRID_SIZE), id, firstround);
						if(firstround)
							firstround = false;
					}
				}
				else if(event.button.button == SDL_BUTTON_RIGHT)
				{
					rotation++;
					rotation %= 4;
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
        SDL_RenderClear(renderer);

        // Draw grid lines.
        SDL_SetRenderDrawColor(renderer, grid_line_color.r, grid_line_color.g,
                               grid_line_color.b, grid_line_color.a);

        for (int x = 0; x < 1 + grid_width * grid_cell_size;
             x += grid_cell_size) {
            SDL_RenderDrawLine(renderer, x, 0, x, window_height);
        }

        for (int y = 0; y < 1 + grid_height * grid_cell_size;
             y += grid_cell_size) {
            SDL_RenderDrawLine(renderer, 0, y, window_width, y);
        }

        // Draw grid ghost cursor.
        if (mouse_active && mouse_hover) {
			if(board.tryinsert(chessman, rotation, make_pair(grid_cursor_ghost.y/GRID_SIZE, grid_cursor_ghost.x/GRID_SIZE), id, firstround))
				insert(renderer, chessman, grid_cursor_ghost, grid_cursor_ghost_color, rotation);
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
		//insert(renderer, 10, grid_cursor, grid_cursor_color, rotation);
		/*
        SDL_SetRenderDrawColor(renderer, grid_cursor_color.r,
                               grid_cursor_color.g, grid_cursor_color.b,
                               grid_cursor_color.a);
        SDL_RenderFillRect(renderer, &grid_cursor);
		*/
		render_board(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
