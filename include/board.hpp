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

class Squares
{
   public:
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

    static constexpr Shape chessshapes[21] = {
        {{{0, 0}}, 1, 1, 1},                                 // *
        {{{0, 0}, {0, 1}}, 2, 1, 2},                         // **
        {{{0, 0}, {0, 1}, {0, 2}}, 3, 1, 3},                 //***
        {{{0, 0}, {0, 1}, {1, 0}}, 2, 2, 3},                 //**
                                                             //*
        {{{0, 0}, {0, 1}, {0, 2}, {0, 3}}, 4, 1, 4},         //****
        {{{0, 0}, {0, 1}, {0, 2}, {1, 0}}, 3, 2, 4},         //***
                                                             //*
        {{{0, 0}, {0, 1}, {1, 1}, {1, 2}}, 3, 2, 4},         //**
                                                             // **
        {{{0, 0}, {0, 1}, {0, 2}, {1, 1}}, 3, 2, 4},         //***
                                                             // *
        {{{0, 0}, {0, 1}, {1, 0}, {1, 1}}, 2, 2, 4},         //**
                                                             //**
        {{{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}}, 5, 1, 5}, //*****
        {{{0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 0}}, 4, 2, 5}, //****
                                                             //*
        {{{0, 0}, {0, 1}, {1, 1}, {1, 2}, {1, 3}}, 4, 2, 5}, //**
                                                             // ***
        {{{0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 1}}, 4, 2, 5}, //****
                                                             // *
        {{{0, 0}, {0, 1}, {0, 2}, {1, 0}, {2, 0}}, 3, 3, 5}, //***
                                                             //*
                                                             //*
        {{{0, 0}, {0, 1}, {1, 1}, {2, 1}, {2, 2}}, 3, 3, 5}, //**
                                                             // *
                                                             // **
        {{{0, 0}, {1, 0}, {1, 1}, {1, 2}, {2, 1}}, 3, 3, 5}, //*
                                                             //***
                                                             // *
        {{{0, 1}, {1, 0}, {1, 1}, {1, 2}, {2, 1}}, 3, 3, 5}, // *
                                                             //***
                                                             // *
        {{{0, 0}, {0, 1}, {1, 1}, {1, 2}, {2, 2}}, 3, 3, 5}, //**
                                                             // **
                                                             //  *
        {{{0, 0}, {1, 0}, {1, 1}, {1, 2}, {2, 0}}, 3, 3, 5}, //*
                                                             //***
                                                             //*
        {{{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}}, 3, 2, 5}, //***
                                                             //**
        {{{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 2}}, 3, 2, 5}  //***
                                                             //* *
    };
    // const char colors[4] = {'r', 'y', 'b', 'g'};
    const Coord corners[4] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    const Coord edges[4] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};
};

#endif // _BOARD_HPP
