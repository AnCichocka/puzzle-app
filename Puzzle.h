#ifndef PUZZLE_H
#define PUZZLE_H

#include "Position.h"
#include <vector>

using namespace std;

class Puzzle
{
private:
	int puzzleID_;
	int size_;
	vector<Position> positions;
	Position upperLeftCorner_;
	Position lowerRightCorner_;
	Position coordinatesStart_;

public:
	Puzzle();
	Puzzle(int puzzleID);
	void addPosition(Position position);
	bool check(Position position);
	int getSize();
	void setID(int puzzleID);
	int getID();
	vector<Position> getPositions();
	void print();
	void scalePuzzle();
	Position getUpperLeftCorner();
	Position getLowerRightCorner();
	Position getCoordinatesStart();
	void setCoordinatesStart(Position position);
};

#endif // PUZZLE_H
