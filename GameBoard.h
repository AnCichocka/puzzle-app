#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "Position.h"
#include "Puzzle.h"
#include <vector>

class GameBoard
{
private:
	int width_;
	int height_;
	int board_[10][10];
	vector<Puzzle> puzzles_;
	int activePuzzleIndex_;
	Position boardUpperLeftCorner_;
	Position boardLowerRightCorner_;
	int puzzlesOnBoard_[25]; // puzzleOnBoard[puzzleID] == 1 -> puzzle is on board

public:
	GameBoard();
	GameBoard(int width, int height, const std::vector<Puzzle>& puzzles);
	void printBoard();
	int (*getBoard())[10];
	vector<Puzzle> getPuzzles();
	bool isActivePuzzle(int x, int y);
	void move(Position directionPosition);
	bool canPlacePuzzle();
	int getActivePuzzleID();
	vector<Position> getActivePositions();
	void placeActivePuzzle();
	void setActivePuzzle(int puzzleIndex);
	bool isPuzzleOnBoard(int puzzleID);
	bool gameWon();

private:
	void zeroBoard();
	bool canPlacePuzzle(Position startPosition);
	bool isPositionFree(Position position);
};

#endif
