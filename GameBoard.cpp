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
	boardLowerRightCorner_ = Position(width_ - 1, height_ - 1);
	puzzles_ = puzzles;

	// TODO: how about that
	activePuzzleID_ = 0;

	minusOneBoards();
}

void GameBoard::minusOneBoards()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			cleanBoardPosition(i, j);
		}
	}

	// TODO its not -1 XD
	for (int i = 0; i < 100; i++)
	{
		puzzlesOnBoard_[i] = false;
	}
}

void GameBoard::move(Position coordinates)
{
	Puzzle &activePuzzle = puzzles_[activePuzzleID_];
	Position coordinatesStart = activePuzzle.getCoordinatesStart();

	Position tmpUpperLeftCorner = activePuzzle.getUpperLeftCorner() + coordinatesStart + coordinates;
	Position tmpLowerRightCorner = activePuzzle.getLowerRightCorner() + coordinatesStart + coordinates;

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

void GameBoard::placeActivePuzzle()
{
	Puzzle activePuzzle = puzzles_[activePuzzleID_];
	vector<Position> positions = getActivePositions();

	for (Position position : positions)
	{
		int x = position.getX();
		int y = position.getY();
		board_[y][x] = activePuzzleID_;
	}

	puzzlesOnBoard_[activePuzzleID_] = true;
}

void GameBoard::pickUpPuzzle(int puzzleID)
{
	Puzzle &puzzle = puzzles_[activePuzzleID_];
	vector<Position> positions = puzzle.getPositions();
	Position start = puzzle.getCoordinatesStart();

	for (Position position : positions)
	{
		Position tmp = position + start;
		int x = tmp.getX();
		int y = tmp.getY();

		cleanBoardPosition(x, y);
		puzzlesOnBoard_[puzzleID] = false;
	}
}

void GameBoard::cleanBoardPosition(int x, int y)
{
	board_[y][x] = -1;
}

bool GameBoard::canPlacePuzzle(Position coordinates)
{
	vector<Position> activePositions = getActivePositions();

	for (Position activePosition : activePositions)
	{
		Position position = activePosition + coordinates;
		if (!isFree(position))
		{
			return false;
		}
	}
	return true;
}

bool GameBoard::canPlacePuzzle()
{
	return canPlacePuzzle(Position(0, 0));
}

bool GameBoard::isFree(Position position)
{
	int x = position.getX();
	int y = position.getY();
	return board_[y][x] == -1;
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

vector<Position> GameBoard::getActivePositions()
{
	vector<Position> activePosiotns;

	Puzzle activePuzzle = puzzles_[activePuzzleID_];
	vector<Position> positions = activePuzzle.getPositions();
	Position coordinatesStart = activePuzzle.getCoordinatesStart();

	for (Position position : positions)
	{
		Position activePosition = position + coordinatesStart;
		activePosiotns.push_back(activePosition);
	}

	return activePosiotns;
}

int GameBoard::getActivePuzzleID()
{
	return activePuzzleID_;
}

void GameBoard::setActivePuzzleID(int puzzleID)
{
	activePuzzleID_ = puzzleID;
}

bool GameBoard::isPuzzleOnBoard(int puzzleID)
{
	return puzzlesOnBoard_[puzzleID];
}

bool GameBoard::gameWon()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			Position position(i, j);

			// isFree for (x,y)
			if (isFree(position))
			{
				return false;
			}
		}
	}
	return true;
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

vector<Puzzle> GameBoard::getPuzzles()
{
	return puzzles_;
}