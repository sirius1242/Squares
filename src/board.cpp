#include "board.hpp"

template <typename T, typename U>
std::pair<T, U> operator+(const std::pair<T, U>& l, const std::pair<T, U>& r)
{
    return {l.first + r.first, l.second + r.second};
}

void squares::init()
{
    // memset(board, 0, BWIDTH*BHEIGHT*sizeof(int));
    lostplayers.clear();
    for (int i = 0; i < BHEIGHT; i++)
        for (int j = 0; j < BWIDTH; j++)
            board[i][j] = -1;
    for (int i = 0; i < PNUM; i++)
    {
        for (int j = 0; j < CHESSNUM; j++)
        {
            chesses[i][j].cmshape = chessshapes[j];
            chesses[i][j].use = false;
        }
    }
}

bool squares::tryinsert(int cmnum, int rotation, coord coor_lt, int np, bool first_round)
{
    // cout << "try to insert chessman in (" << coor_lt.first << ", " << coor_lt.second << ")" << endl;
    bool canplace = false;
    squares::shape tmp = rotate(cmnum, rotation);
    if (chesses[np][cmnum].use)
        return false;
    if (first_round)
    {
        coord corner = {0, 0};
        switch (np)
        {
            case 1:
                corner.second = BWIDTH - 1;
                break;

            case 3:
                corner.first = BHEIGHT - 1;
                break;

            case 2:
                corner.first = BHEIGHT - 1;
                corner.second = BWIDTH - 1;

            default:
                break;
        }
        for (int i = 0; i < tmp.size; i++)
        {
            tmp.grids[i] = tmp.grids[i] + coor_lt;
            if (!in_range(tmp.grids[i]))
                return false;
            if (tmp.grids[i] == corner)
                canplace = true;
        }
    }
    else
    {
        set<coord> inchess;
        for (int i = 0; i < tmp.size; i++)
        {
            tmp.grids[i] = tmp.grids[i] + coor_lt;
            if (!in_range(tmp.grids[i]))
                return false;
            if (at(tmp.grids[i]) >= 0) // cell is already occupied
                return false;
            inchess.insert(tmp.grids[i]);
        }
        for (int i = 0; i < tmp.size; i++)
        {
            coord check;
            for (const auto& edge : edges) // check edges
            {
                check = tmp.grids[i] + edge;
                if (!in_range(check))
                    continue;
                if (!inchess.count(check) && board[check.first][check.second] == np)
                {
                    return false;
                }
            }
            for (const auto& corner : corners) // check corners
            {
                check = tmp.grids[i] + corner;
                if (!in_range(check))
                    continue;
                if (!inchess.count(check) && at(check) == np)
                {
                    canplace = true;
                    break;
                }
            }
        }
    }
    return canplace;
}

void squares::insert(int cmnum, int rotation, coord coor_lt, int np, bool first_round)
{
    squares::shape tmp = rotate(cmnum, rotation);
    for (int i = 0; i < tmp.size; i++)
        at(tmp.grids[i] + coor_lt) = np;
    chesses[np][cmnum].use = true;
}

squares::shape squares::rotate(int cmnum, int rotation) // 1 is 90, 2 is 180, 3 is 270, clockwise
{
    squares::shape dst;
    const squares::shape& src = chessshapes[cmnum];
    dst.size = src.size;
    if (rotation % 2 == 0)
    {
        dst.height = src.height;
        dst.width = src.width;
    }
    else
    {
        dst.height = src.width;
        dst.width = src.height;
    }
    for (int i = 0; i < dst.size; i++)
    {
        switch (rotation)
        {
            case 1:
                dst.grids[i].first = src.grids[i].second;
                dst.grids[i].second = dst.width - src.grids[i].first - 1;
                break;

            case 2:
                dst.grids[i].first = dst.height - src.grids[i].first - 1;
                dst.grids[i].second = dst.width - src.grids[i].second - 1;
                break;

            case 3:
                dst.grids[i].first = dst.height - src.grids[i].second - 1;
                dst.grids[i].second = src.grids[i].first;
                break;

            case 4:
                dst.grids[i].first = src.grids[i].first;
                dst.grids[i].second = dst.width - src.grids[i].second - 1;
                break;

            case 5:
                dst.grids[i].first = dst.height - src.grids[i].second - 1;
                dst.grids[i].second = dst.width - src.grids[i].first - 1;
                break;

            case 6:
                dst.grids[i].first = dst.height - src.grids[i].first - 1;
                dst.grids[i].second = src.grids[i].second;
                break;

            case 7:
                dst.grids[i].first = src.grids[i].second;
                dst.grids[i].second = src.grids[i].first;
                break;

            default:
                dst.grids[i].first = src.grids[i].first;
                dst.grids[i].second = src.grids[i].second;
                break;
        }
    }
    return dst;
}

/**
 * Check if a player has any valid move available
 *
 * @return Whether the player can place a piece
 */
bool squares::checkplayer(int np)
{
    /** bstatus: Board status
     *	 0: This cell is not adjacent to any existing pieces
     *	 1: This cell is diagonally adjacent to an existing piece
     *	 2: This cell is orthogonally adjacent to, or already covered by, an existing piece
     */
    int bstatus[BHEIGHT][BWIDTH] = {};
    for (int y = 0; y < BHEIGHT; y++)
    {
        for (int x = 0; x < BWIDTH; x++)
        {
            coord pos = {y, x};
            // Mark occupied cells
            if (at(pos) >= 0)
                bstatus[pos.first][pos.second] = 2;
            // Calculate adjacency only for same-color cells
            if (at(pos) != np)
                continue;

            for (const auto& edge : edges)
            {
                coord newpos = pos + edge;
                if (in_range(newpos))
                    bstatus[newpos.first][newpos.second] = 2;
            }
            for (const auto& corner : corners)
            {
                coord newpos = pos + corner;
                if (in_range(newpos) && bstatus[newpos.first][newpos.second] < 2)
                    bstatus[newpos.first][newpos.second] = 1;
            }
        }
    }
    std::vector<coord> musts;
    for (int y = 0; y < BHEIGHT; y++)
    {
        for (int x = 0; x < BWIDTH; x++)
        {
            coord pos = {y, x};
            if (bstatus[pos.first][pos.second] == 1)
                musts.push_back(pos);
        }
    }

    // Enumerate all remaining pieces over all "must cover" cells
    for (int i = 0; i < CHESSNUM; i++)
    {
        if (checkused(i, np))
            continue;
        for (const coord& must : musts)
        {
            for (int r = 0; r < ROTATIONS; r++)
            {
                int loop_w = chessshapes[i].width,
                    loop_h = chessshapes[i].height;
                if (r % 2 == 1)
                {
                    int tmp = loop_w;
                    loop_w = loop_h;
                    loop_h = tmp;
                }
                for (int y = 0; y < loop_h; y++)
                {
                    for (int x = 0; x < loop_w; x++)
                    {
                        coord off = {-y, -x}; // Move up and left
                        coord pos = must + off;
                        if (in_range(pos) && tryinsert(i, r, pos, np, false))
                            return true;
                    }
                }
            }
        }
    }
    return false;
}

int squares::check()
{
    int loseplayers = 0;
    for (int i = 0; i < PNUM; i++)
    {
        if (lostplayers.count(i) == 0 && !checkplayer(i))
        {
            loseplayers |= 1 << i;
            lostplayers.insert(i);
        }
    }
    return loseplayers;
}
