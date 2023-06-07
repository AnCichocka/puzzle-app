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

public:
	BoardGenerator();
	BoardGenerator(int width, int height);
	void generateBoard();
	void printBoard();
	int (*getBoard())[10];
	vector<Puzzle> getPuzzles();
	void printAnswer(vector<int> puzzleIDs);

private:
	void generateFreePositions();
	void generateWholePuzzles();
	bool isInBoardRange(Position position);
	bool isFree(Position position);
	void addFreeNeighbourPositions(vector<Position> &neighbours, Position position);
	int getRandomFromInterval(int min, int max);
	void removeFromFreePosiitons(Position position);
	void addPuzzleToPuzzlesOrHoles(Puzzle &puzzle, int expectedSize);
	void scaleHolesWithWholePuzzles();
	vector<int> getNeighbourPuzzleIDs(Puzzle puzzle);
	bool isHole(int x, int y);
	void reindexPuzzles();
	void scalePuzzles();
};

#endif // BOARDGENERATOR_H
