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
	vector<Puzzle> puzzles_; // puzzles in aescending order form 0 puzzleID
	int activePuzzleID_;
	Position boardUpperLeftCorner_;
	Position boardLowerRightCorner_;
	bool puzzlesOnBoard_[25]; // puzzleOnBoard_[puzzleID] = true -> puzzle on board

public:
	GameBoard();
	GameBoard(int width, int height, const std::vector<Puzzle> &puzzles);
	void printBoard();
	int (*getBoard())[10];
	vector<Puzzle> getPuzzles();
	bool isActivePuzzle(int x, int y);
	void move(Position directionPosition);
	bool canPlacePuzzle();
	int getActivePuzzleID();
	vector<Position> getActivePositions();
	void placeActivePuzzle();
	void setActivePuzzleID(int puzzleID);
	bool isPuzzleOnBoard(int puzzleID);
	bool gameWon();
	void pickUpPuzzle(int puzzleID);

private:
	void minusOneBoards();
	bool canPlacePuzzle(Position startPosition);
	bool isFree(Position position);
	void cleanBoardPosition(int x, int y);
};

#endif
