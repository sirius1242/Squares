#include "board.hpp"

template <typename T,typename U>
std::pair<T,U> operator+(const std::pair<T,U> & l,const std::pair<T,U> & r) {
    return {l.first+r.first,l.second+r.second};
}

void squares::init()
{
	memset(board, 0, BWIDTH*BHEIGHT*sizeof(int));
	for(int i=0; i<PNUM; i++)
	{
		for(int j=0; j<CHESSNUM; j++)
		{
			chesses[i][j].cmshape = chessshapes[j];
			chesses[i][j].use = false;
		}
	}
}

bool squares::insert(int cmnum, int rotation, pair<int, int> coor_lt, int np, bool first_round)
{
	bool canplace = false;
	squares::shape tmp = rotate(cmnum, rotation);
	if(first_round)
	{
		pair<int, int> corner = {0, 0};
		switch (np)
		{
		case 1:
			corner.second = BWIDTH;
			break;

		case 2:
			corner.first = BHEIGHT;
			break;

		case 3:
			corner.first = BHEIGHT;
			corner.second = BWIDTH;

		default:
			break;
		}
		for(int i=0; i<tmp.size; i++)
		{
			tmp.grids[i].first += coor_lt.first;
			tmp.grids[i].second += coor_lt.second;
			if(tmp.grids[i].first < 0 || tmp.grids[i].first > BHEIGHT || tmp.grids[i].second < 0 || tmp.grids[i].second > BWIDTH)
				return false;
			if(tmp.grids[i] == corner)
				canplace = true;
		}
	}
	else
	{
		set<pair<int, int>> inchess;
		for(int i=0; i<tmp.size; i++)
		{
			tmp.grids[i].first += coor_lt.first;
			tmp.grids[i].second += coor_lt.second;
			if(tmp.grids[i].first < 0 || tmp.grids[i].first > BHEIGHT || tmp.grids[i].second < 0 || tmp.grids[i].second > BWIDTH)
				return false;
			if(board[tmp.grids[i].first][tmp.grids[i].second]) // no other chessman on that grid
				return false;
			inchess.insert(tmp.grids[i]);
			pair<int, int> check;
			for(int j=0; j<4; j++) // check corner
			{
				check = tmp.grids[i] + corners[j];
				if(check.first < 0 || check.first > BHEIGHT || check.second < 0 || check.second > BWIDTH)
					continue;
				if(board[check.first][check.second] == np)
					canplace = true;
			}
			for(int j=0; j<4; j++) // check edge
			{
				check = tmp.grids[i] + edges[j];
				if(check.first < 0 || check.first > BHEIGHT || check.second < 0 || check.second > BWIDTH)
					continue;
				if(board[check.first][check.second] == np)
					canplace = false;
			}
		}
	}
	return canplace;
}

squares::shape squares::rotate(int cmnum, int rotation) // 1 is 90, 2 is 180, 3 is 270, clockwise
{
	squares::shape dst;
	squares::shape *src = &chessshapes[cmnum];
	dst.size = src->size;
	if(rotation == 2 || rotation == 0)
	{
		dst.height = src->height;
		dst.width = src->width;
	}
	else
	{
		dst.height = src->width;
		dst.width = src->height;
	}
	for(int i=0;i<dst.size;i++)
	{
		switch (rotation)
		{
		case 1:
			dst.grids[i].first = src->grids[i].second - 1;
			dst.grids[i].second = dst.height - src->grids[i].first - 1;
			break;

		case 2:
			dst.grids[i].first = dst.height - src->grids[i].first - 1;
			dst.grids[i].second = dst.width - src->grids[i].second - 1;
			break;

		case 3:
			dst.grids[i].first = dst.width - src->grids[i].second;
			dst.grids[i].second = src->grids[i].first;
			break;

		default:
			dst.grids[i].first = src->grids[i].first;
			dst.grids[i].second = src->grids[i].second;
			break;
		}
	}
	return dst;
}
