#ifndef _BOARD_HPP
#define _BOARD_HPP

#include <iostream>
#include <set>
#include <vector>

using std::pair;

#define BSIZE 21
#define BWIDTH BSIZE
#define BHEIGHT BSIZE
#define PNUM 4      // Max player number
#define CHESSNUM 21 // chessman numbers
#define MAXSIZE 5   // chessman max grid size
#define ROTATIONS 8 // Number of possible rotations

using Coord = std::pair<int, int>;

struct Shape
{
    Coord grids[MAXSIZE];
    int width;
    int height;
    int size;
};

struct Chessman
{
    bool use;
    Shape cmshape;
};

extern const Shape chessshapes[CHESSNUM];
extern const Coord corners[4];
extern const Coord edges[4];

class Squares
{
   public:

    void init();
    Squares();
    bool tryinsert(int cmnum, int rotation, pair<int, int> coor_lt, int np, bool first_round);
    void insert(int cmnum, int rotation, pair<int, int> coor_lt, int np, bool first_round);
    Shape rotate(int cmnum, int rotation);
    bool checkplayer(int np);
    int check(); // check if someone loses
    inline int& getelem(int y, int x) { return board[y][x]; }
    inline int& at(const Coord& c) { return getelem(c.first, c.second); }
    bool checkused(int cmnum, int np) const { return chesses[np][cmnum].use; }
    inline bool in_range(const Coord& l) const
    {
        return l.first >= 0 && l.first < BHEIGHT && l.second >= 0 && l.second < BWIDTH;
    }

   private:
    int board[BWIDTH][BHEIGHT];
    Chessman chesses[PNUM][CHESSNUM];
    std::set<int> lostplayers;
};

#endif // _BOARD_HPP
