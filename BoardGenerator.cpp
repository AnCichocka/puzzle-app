#include "BoardGenerator.h"
#include "Position.h"
#include "Puzzle.h"
#include <chrono>
#include <iostream>
#include <random>

using namespace std;

const vector<Position> coordinatesToAdd = {
	Position(0, 1),
	Position(0, -1),
	Position(1, 0),
	Position(-1, 0)};

const int MIN_PUZZLE_SIZE = 4;
const int MAX_PUZZLE_SIZE = 10;

BoardGenerator::BoardGenerator()
{
}

BoardGenerator::BoardGenerator(int width, int height)
{
	width_ = width;
	height_ = height;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			board_[i][j] = -1;
		}
	}

	cout << "generating free positions..." << endl;
	generateFreePositions();

	cout << "generating whole puzzles..." << endl;
	generateWholePuzzles();
	printBoard();

	cout << "scale holes with whole puzzles.." << endl;
	scaleHolesWithWholePuzzles();
	printBoard();

	cout << "reindexing puzzles..." << endl;
	reindexPuzzles();
	printBoard();

	cout << "scaling puzzles positions..." << endl;
	scalePuzzles();
}

void BoardGenerator::generateFreePositions()
{
	for (int x = width_ - 1; x > -1; x--)
	{
		for (int y = height_ - 1; y > -1; y--)
		{
			freePositions_.push_back(Position(x, y));
		}
	}
}

void BoardGenerator::generateWholePuzzles()
{
	int puzzleID = 0;

	while (freePositions_.size() > 0)
	{
		// create new puzzle
		Puzzle puzzle = Puzzle(puzzleID);
		int puzzleExpectedSize = getRandomFromInterval(MIN_PUZZLE_SIZE, MAX_PUZZLE_SIZE);

		// get start position
		Position startPosition = move(freePositions_.back());
		freePositions_.pop_back();

		vector<Position> freeNeighbourPositions;
		addFreeNeighbourPositions(freeNeighbourPositions, startPosition);

		// TODO: swtich to reference
		puzzle.addPosition(startPosition);

		while (puzzle.getSize() != puzzleExpectedSize)
		{
			int n = freeNeighbourPositions.size();
			if (n == 0)
			{
				break;
			}
			int index = getRandomFromInterval(0, n - 1);

			Position position = freeNeighbourPositions[index];
			freeNeighbourPositions.erase(freeNeighbourPositions.begin() + index);
			removeFromFreePosiitons(position);
			puzzle.addPosition(position);
			board_[position.getY()][position.getX()] = puzzleID;
			addFreeNeighbourPositions(freeNeighbourPositions, position);
		}

		// add puzzle to puzzles or holes
		addPuzzleToPuzzlesOrHoles(puzzle, puzzleExpectedSize);
		puzzleID++;
	}
}

void BoardGenerator::addFreeNeighbourPositions(vector<Position> &neighbourPositions, Position position)
{
	for (Position coordinates : coordinatesToAdd)
	{
		Position newPosition = position + coordinates;
		if (isInBoardRange(newPosition) && isFree(newPosition))
		{
			bool isAlreadyNeighbourPosition = false;
			for (Position neighbourPosition : neighbourPositions)
			{
				if (neighbourPosition == newPosition)
				{
					isAlreadyNeighbourPosition = true;
					break;
				}
			}

			if (!isAlreadyNeighbourPosition)
			{
				neighbourPositions.push_back(newPosition);
			}
		}
	}
}

bool BoardGenerator::isInBoardRange(Position position)
{
	int x = position.getX();
	int y = position.getY();
	return 0 <= x && x < width_ && 0 <= y && y < height_;
}

bool BoardGenerator::isFree(Position position)
{
	for (Position freePosition : freePositions_)
	{
		if (freePosition == position)
		{
			return true;
		}
	}
	return false;
}

int BoardGenerator::getRandomFromInterval(int min, int max)
{
	// interval (0,0)
	if (min == max && min == 0)
	{
		return 0;
	}

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	uniform_int_distribution<int> distribution(min, max);

	return distribution(generator);
}

void BoardGenerator::removeFromFreePosiitons(Position position)
{
	int index = 0;
	for (Position freePosition : freePositions_)
	{
		if (freePosition == position)
		{
			freePositions_.erase(freePositions_.begin() + index);
			break;
		}
		index++;
	}
}

void BoardGenerator::addPuzzleToPuzzlesOrHoles(Puzzle &puzzle, int expectedPuzzleSize)
{
	vector<Position> positions = puzzle.getPositions();
	int puzzleID = puzzle.getID();

	// puzzle is too small - add puzzle to holes
	if (puzzle.getSize() != expectedPuzzleSize)
	{
		puzzleID *= -1;
		puzzle.setID(puzzleID);

		holes_.push_back(puzzle);
	}

	// everything is fine - add puzlle to board
	else
	{
		puzzles_.push_back(puzzle);
	}

	// draw on board
	for (Position position : positions)
	{
		int x = position.getX();
		int y = position.getY();
		board_[y][x] = puzzleID;
	}
}

void BoardGenerator::scaleHolesWithWholePuzzles()
{
	for (Puzzle hole : holes_)
	{
		vector<Position> holePositions = hole.getPositions();
		vector<int> neighbourPuzzleIDs = getNeighbourPuzzleIDs(hole);

		int index = getRandomFromInterval(0, neighbourPuzzleIDs.size() - 1);
		int puzzleID = neighbourPuzzleIDs[index];

		for (Puzzle puzzle : puzzles_)
		{
			if (puzzle.getID() == puzzleID)
			{

				for (Position holePosition : holePositions)
				{
					puzzle.addPosition(holePosition);
					int x = holePosition.getX();
					int y = holePosition.getY();
					board_[y][x] = puzzleID;
				}
				break;
			}
		}
	}
}

// TODO: puzzle reference
vector<int> BoardGenerator::getNeighbourPuzzleIDs(Puzzle puzzle)
{
	vector<Position> holePositions = puzzle.getPositions();
	vector<int> neighbourIDs;

	for (Position holePosition : holePositions)
	{
		for (Position coordinates : coordinatesToAdd)
		{
			Position positionToCheck = holePosition + coordinates;
			int x = positionToCheck.getX();
			int y = positionToCheck.getY();

			if (isInBoardRange(positionToCheck) && !isHole(x, y))
			{
				int neighbourPuzzleID = board_[y][x];

				bool isAlreadyNeighbour = false;
				for (int neighbourID : neighbourIDs)
				{
					if (neighbourID == neighbourPuzzleID)
					{
						isAlreadyNeighbour = true;
						break;
					}
				}

				if (!isAlreadyNeighbour)
				{
					neighbourIDs.push_back(neighbourPuzzleID);
				}
			}
		}
	}

	return neighbourIDs;
}

bool BoardGenerator::isHole(int x, int y)
{
	return board_[y][x] < 0;
}

void BoardGenerator::printBoard()
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

int (*BoardGenerator::getBoard())[10]
{
	return board_;
}

vector<Puzzle> BoardGenerator::getPuzzles()
{
	return puzzles_;
}

void BoardGenerator::reindexPuzzles()
{
	int newID = 0;

	for (Puzzle &puzzle : puzzles_)
	{
		int oldID = puzzle.getID();

		if (oldID != newID)
		{
			vector<Position> positions = puzzle.getPositions();
			for (Position position : positions)
			{
				int x = position.getX();
				int y = position.getY();
				board_[y][x] = newID;
			}
		}

		puzzle.setID(newID);
		newID++;
	}
}

void BoardGenerator::scalePuzzles()
{
	for (int i = 0; i < (int)puzzles_.size(); i++)
	{
		puzzles_[i].scalePuzzle();
	}
}