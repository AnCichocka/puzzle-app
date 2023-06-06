#include "Puzzle.h"
#include <iostream>

Puzzle::Puzzle()
{}

Puzzle::Puzzle(int puzzleID)
{
	puzzleID_ = puzzleID;
	size_ = 0;
	coordinatesStart_ = Position(0, 0);
	//cout << "new puzzle created!\n"
}

void Puzzle::addPosition(Position position)
{
	positions.push_back(position);
	size_++;
	//cout << "position added\n";
}

bool Puzzle::check(Position position)
{
	if (positions.size() == 0)
	{
		return false;
	}
	return position == positions[0];
}

int Puzzle::getSize()
{
	return size_;
}
vector<Position> Puzzle::getPositions()
{
	return positions;
}
int Puzzle::getID()
{
	return puzzleID_;
}
void Puzzle::setID(int puzzleID)
{
	puzzleID_ = puzzleID;
}
void Puzzle::print()
{
	cout << "puzzleID: " << puzzleID_ << endl;
	for (int i = 0; i < (int)positions.size(); i++)
	{
		positions[i].print();
	}
}

void Puzzle::scalePuzzle()
{
	int minX = -1;
	int maxX = -1;
	int minY = -1;
	int maxY = -1;

	for (int i = 0; i < (int)positions.size(); i++)
	{
		int x = positions[i].getX();
		int y = positions[i].getY();
		if (x < minX || minX == -1)
		{
			minX = x;
		}
		if (x > maxX)
		{
			maxX = x;
		}
		if (y < minY || minY == -1)
		{
			minY = y;
		}
		if (y > maxY)
		{
			maxY = y;
		}
	}

	upperLeftCorner_ = Position(minX, minY);
	lowerRightCorner_ = Position(maxX, maxY);

	for (int i = 0; i < (int)positions.size(); i++)
	{
		positions[i] = positions[i] - upperLeftCorner_;
	};
	lowerRightCorner_ = lowerRightCorner_ - upperLeftCorner_;
	upperLeftCorner_ = upperLeftCorner_ - upperLeftCorner_; // is supposed to be 0 :)
}

Position Puzzle::getUpperLeftCorner()
{
	return upperLeftCorner_;
}
Position Puzzle::getLowerRightCorner()
{
	return lowerRightCorner_;
}
Position Puzzle::getCoordinatesStart()
{
	return coordinatesStart_;
}
void Puzzle::setCoordinatesStart(Position position)
{
	coordinatesStart_ = position;
}