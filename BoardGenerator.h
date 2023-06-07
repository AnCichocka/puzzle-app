#ifndef BOARDGENERATOR_H
#define BOARDGENERATOR_H

#include "Position.h"
#include "Puzzle.h"
#include <vector>

using namespace std;

class BoardGenerator
{
private:
	int width_;
	int height_;
	// TODO: MAX_SIZE
	int board_[10][10];
	vector<Position> freePositions_;
	vector<Puzzle> puzzles_;
	vector<Puzzle> holes_;
	bool canPlace_;

public:
	BoardGenerator();
	BoardGenerator(int width, int height);
	void printBoard();
	int (*getBoard())[10];
	vector<Puzzle> getPuzzles();

private:
	void generateFreePositions();
	void generateWholePuzzles();
	bool isInBoardRange(Position position);
	bool isPositionFree(Position position);
	void addFreeNeighbourPositions(vector<Position> &neighbours, Position position);
	int getRandomFromInterval(int min, int max);
	void removeFromFreePosiitons(Position position);
	void handlePuzzle(Puzzle &puzzle, int expectedSize);
	void scaleHolesWithWholePuzzles();
	vector<int> getPuzzleNeighbourNumbers(Puzzle puzzle);
	void reindexPuzzles();
	void scalePuzzles();
};

#endif // BOARDGENERATOR_H
