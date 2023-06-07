#include "GameBoard.h"
#include <iostream>
#include <vector>

using namespace std;

GameBoard::GameBoard()
{
}

GameBoard::GameBoard(int width, int height, const std::vector<Puzzle> &puzzles)
{
	width_ = width;
	height_ = height;
	boardUpperLeftCorner_ = Position(0, 0);
	boardLowerRightCorner_ = Position(9, 9);
	puzzles_ = puzzles;
	activePuzzleIndex_ = 0;

	zeroBoard();
}

void GameBoard::printBoard()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			cout << board_[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

int (*GameBoard::getBoard())[10]
{
	return board_;
}

bool GameBoard::isActivePuzzle(int x, int y)
{
	Position positionToCheck(x, y);
	vector<Position> activePositions = getActivePositions();
	for (Position activePosition : activePositions)
	{
		if (positionToCheck == activePosition)
		{
			return true;
		}
	}
	return false;
}

vector<Puzzle> GameBoard::getPuzzles()
{
	return puzzles_;
}

void GameBoard::zeroBoard()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			board_[i][j] = 0;
		}
	}

	for (int i = 0; i < 100; i++)
	{
		puzzlesOnBoard_[i] = 0;
	}
}

void GameBoard::move(Position coordinates)
{
	// activePuzzle_.print();
	Puzzle &activePuzzle = puzzles_[activePuzzleIndex_];
	Position coordinatesStart = activePuzzle.getCoordinatesStart();

	Position tmpUpperLeftCorner = activePuzzle.getUpperLeftCorner() + coordinatesStart + coordinates;
	Position tmpLowerRightCorner = activePuzzle.getLowerRightCorner() + coordinatesStart + coordinates;

	// cout << "upperLeft: ";
	// tmpUpperLeftCorner.print();
	// cout << "lowerRight: ";
	// tmpLowerRightCorner.print();
	// newActivePositionStart.print();
	if (boardUpperLeftCorner_ < tmpUpperLeftCorner && tmpLowerRightCorner < boardLowerRightCorner_)
	{
		// cout << "can move..." << endl;
		activePuzzle.setCoordinatesStart(coordinatesStart + coordinates);
	}

	else
	{
		cout << "puzzle not in map range..." << endl;
	}
}

bool GameBoard::canPlacePuzzle(Position coordinates)
{
	vector<Position> activePositions = getActivePositions();

	for (Position activePosition : activePositions)
	{
		Position position = activePosition + coordinates;
		if (!isPositionFree(position))
		{
			return false;
		}
	}
	return true;
}
bool GameBoard::isPositionFree(Position position)
{
	int x = position.getX();
	int y = position.getY();
	return board_[y][x] == 0;
}

bool GameBoard::canPlacePuzzle()
{
	return canPlacePuzzle(Position(0, 0));
}

void GameBoard::placeActivePuzzle()
{
	Puzzle activePuzzle = puzzles_[activePuzzleIndex_];
	int puzzleID = activePuzzle.getID();
	vector<Position> positions = getActivePositions();

	for (Position position : positions)
	{
		// TODO: handle previous position
		// TODO: add to puzzles on map

		int x = position.getX();
		int y = position.getY();

		board_[y][x] = puzzleID;
	}

	// TODO: set as inactive puzzle ? im not sure now
	puzzlesOnBoard_[puzzleID] = 1;
}

int GameBoard::getActivePuzzleID()
{
	if (activePuzzleIndex_ == -1)
	{
		return -1;
	}
	return puzzles_[activePuzzleIndex_].getID();
}

vector<Position> GameBoard::getActivePositions()
{
	vector<Position> activePosiotns;
	if (activePuzzleIndex_ != -1)
	{
		Puzzle activePuzzle = puzzles_[activePuzzleIndex_];
		vector<Position> positions = activePuzzle.getPositions();
		Position coordinatesStart = activePuzzle.getCoordinatesStart();

		for (Position position : positions)
		{
			Position activePosition = position + coordinatesStart;
			activePosiotns.push_back(activePosition);
		}
	}

	return activePosiotns;
}

void GameBoard::setActivePuzzle(int puzzleIndex)
{
	activePuzzleIndex_ = puzzleIndex;
}

bool GameBoard::isPuzzleOnBoard(int puzzleID)
{
	return (puzzlesOnBoard_[puzzleID] > 0);
}

bool GameBoard::gameWon()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (board_[i][j] == 0)
			{
				return false;
			}
		}
	}
	return true;
}

void GameBoard::pickUpPuzzle(int puzzleID)
{
	Puzzle &puzzle = puzzles_[activePuzzleIndex_];
	vector<Position> positions = puzzle.getPositions();
	Position start = puzzle.getCoordinatesStart();

	for (Position position : positions)
	{
		Position tmp = position + start;
		int x = tmp.getX();
		int y = tmp.getY();
		board_[y][x] = 0;
		puzzlesOnBoard_[puzzleID] = 0;
	}

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			cout << board_[i][j] << " ";
		}
		cout << endl;
	}
}
