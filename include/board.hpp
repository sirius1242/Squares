#include <iostream>
#include <vector>
#include <set>
using namespace std;;

#define BWIDTH 21
#define BHEIGHT 21
#define PNUM 4 // Max player number
#define CHESSNUM 21 // chessman numbers
#define MAXSIZE 5 // chessman max grid size

class squares{
	public:
		typedef struct {
			pair<int, int> grids[MAXSIZE];
			int width;
			int height;
			int size;
		} shape;

		void init();
		bool tryinsert(int cmnum, int rotation, pair<int, int> coor_lt, int np, bool first_round);
		void insert(int cmnum, int rotation, pair<int, int> coor_lt, int np, bool first_round);
		shape rotate(int cmnum, int rotation);
		bool check(vector<int> &loseplayers); // check if someone lose
		int getelem(int y, int x) {return board[y][x];};
		bool checkused(int cmnum, int np) {return chesses[np][cmnum].use;};

	private:
		int board[BWIDTH][BHEIGHT];

		shape chessshapes[21] = {
			{{{0, 0}}, 1, 1, 1}, // *
			{{{0, 0}, {0, 1}}, 2, 1, 2}, // **
			{{{0, 0}, {0, 1}, {0, 2}}, 3, 1, 3}, //***
			{{{0, 0}, {0, 1}, {1, 0}}, 2, 2, 3}, //**
												 //*
			{{{0, 0}, {0, 1}, {0, 2}, {0, 3}}, 4, 1, 4}, //****
			{{{0, 0}, {0, 1}, {0, 2}, {1, 0}}, 3, 2, 4}, //***
														 //*
			{{{0, 0}, {0, 1}, {1, 1}, {1, 2}}, 3, 2, 4}, //**
														 // **
			{{{0, 0}, {0, 1}, {0, 2}, {1, 1}}, 3, 2, 4}, //***
														 // *
			{{{0,0}, {0, 1}, {1, 0}, {1, 1}}, 2, 2, 4}, //**
													    //**
			{{{0,0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}}, 5, 1, 5}, //*****
			{{{0,0}, {0, 1}, {0, 2}, {0, 3}, {1, 0}}, 4, 2, 5}, //****
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
			{{{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 2}}, 3, 2, 5} //***
															 	//* *
		};

		typedef struct {
			bool use;
			shape cmshape;
		} chessman;

		chessman chesses[PNUM][CHESSNUM];
		//char colors[4] = {'r', 'y', 'b', 'g'};
		pair<int, int> corners[4] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
		pair<int, int> edges[4] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};
};
