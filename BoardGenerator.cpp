#include "BoardGenerator.h"
#include "Position.h"
#include "Puzzle.h"
#include <chrono>
#include <iostream>
#include <random>

using namespace std;

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
			board_[i][j] = 0;
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

	cout << "scaling and reindexing puzzles positions..." << endl;
	scaleAndReindexPuzzles();
	printBoard();
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
	int puzzleID = 1;

	while (freePositions_.size() > 0)
	{
		// create new puzzle
		Puzzle puzzle = Puzzle(puzzleID);
		int puzzleExpectedSize = getRandomFromInterval(4, 10);

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
		handlePuzzle(puzzle, puzzleExpectedSize);
		puzzleID++;
	}
}

void BoardGenerator::addFreeNeighbourPositions(vector<Position>& neighbours, Position position)
{
	vector<Position> coordinatesToAdd;
	coordinatesToAdd.push_back(Position(0, 1));
	coordinatesToAdd.push_back(Position(0, -1));
	coordinatesToAdd.push_back(Position(1, 0));
	coordinatesToAdd.push_back(Position(-1, 0));

	for (int i = 0; i < (int)coordinatesToAdd.size(); i++)
	{
		Position newPosition = position + coordinatesToAdd[i];
		if (isInBoardRange(newPosition) && isPositionFree(newPosition))
		{
			bool has = false;
			for (int j = 0; j < (int)neighbours.size(); j++)
			{
				if (neighbours[j] == newPosition)
				{
					has = true;
				}
			}
			if (!has)
			{
				neighbours.push_back(newPosition);
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

bool BoardGenerator::isPositionFree(Position position)
{
	for (int i = 0; i < (int)freePositions_.size(); i++)
	{
		if (freePositions_[i] == position)
		{
			return true;
		}
	}
	return false;
}

int BoardGenerator::getRandomFromInterval(int min, int max)
{
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
	for (int i = 0; i < (int)freePositions_.size(); i++)
	{
		if (freePositions_[i] == position)
		{
			freePositions_.erase(freePositions_.begin() + i);
			return;
		}
	}
}

void BoardGenerator::handlePuzzle(Puzzle& puzzle, int expectedSize)
{
	vector<Position> hole = puzzle.getPositions();

	// something went wrong, puzzle is too small - add puzzle to holes
	if (puzzle.getSize() != expectedSize)
	{
		for (int i = 0; i < (int)hole.size(); i++)
		{
			int x = hole[i].getX();
			int y = hole[i].getY();
			board_[y][x] = -puzzle.getID();
		}
		puzzle.setID(-puzzle.getID());
		holes_.push_back(puzzle);
		//puzzle.print();
	}
	// everything is fine - add puzlle to board
	else
	{
		for (int i = 0; i < (int)hole.size(); i++)
		{
			int x = hole[i].getX();
			int y = hole[i].getY();
			board_[y][x] = puzzle.getID();
		}
		puzzles_.push_back(puzzle);
		//puzzle.print();
	}
}

void BoardGenerator::scaleHolesWithWholePuzzles()
{
	for (int i = 0; i < (int)holes_.size(); i++)
	{
		Puzzle hole = holes_[i];

		//cout << "hole ID: " << hole.getID() << endl;

		vector<Position> holePositions = hole.getPositions();

		// cout << "hole: ";
		// for (int i = 0; i < (int)holePositions.size(); i++)
		// {
		// 	holePositions[i].print();
		// }

		vector<int> neighbourPuzzleNumbers = getPuzzleNeighbourNumbers(hole);

		// cout << "neighbours: ";
		// for (int i = 0; i < (int)neighbourPuzzleNumbers.size(); i++)
		// {
		// 	cout << neighbourPuzzleNumbers[i] << " ";
		// }
		// cout << endl;
		int index = getRandomFromInterval(0, neighbourPuzzleNumbers.size() - 1);
		int puzzleID = neighbourPuzzleNumbers[index];

		//cout << "size: " << neighbourPuzzleNumbers.size() << " index: " << index << " puzzleID: " << puzzleID << endl;
		// TODO: exception size == 0
		for (int j = 0; j < (int)puzzles_.size(); j++)
		{
			if (puzzles_[j].getID() == puzzleID)
			{
				//cout << "FOUND PUZZLE" << endl;
				for (int k = 0; k < (int)holePositions.size(); k++)
				{
					//cout << "ADDING ";
					//holePositions[k].print();
					//cout << endl;
					puzzles_[j].addPosition(holePositions[k]);
					board_[holePositions[k].getY()][holePositions[k].getX()] = puzzleID;
				}
				//puzzles_[j].print();
				break;
			}
		}
	}
}

vector<int> BoardGenerator::getPuzzleNeighbourNumbers(Puzzle puzzle)
{
	vector<Position> coordinatesToAdd = {
		Position(0, 1),
		Position(0, -1),
		Position(1, 0),
		Position(-1, 0)
	};

	vector<Position> holePositions = puzzle.getPositions();
	vector<int> neighbours;
	bool alreadyInNeighbours;

	for (int i = 0; i < (int)holePositions.size(); i++)
	{
		for (int j = 0; j < (int)coordinatesToAdd.size(); j++)
		{
			Position positionToCheck = holePositions[i] + coordinatesToAdd[j];
			if (isInBoardRange(positionToCheck) && board_[positionToCheck.getY()][positionToCheck.getX()] > 0)
			{
				int puzzleNumber = board_[positionToCheck.getY()][positionToCheck.getX()];

				alreadyInNeighbours = false;
				for(int neighbourID : neighbours){
					if(neighbourID == puzzleNumber){
						alreadyInNeighbours = true;
						break;
					}
				}

				if(!alreadyInNeighbours){
					neighbours.push_back(puzzleNumber);
				}


//TODO
				//auto result = find(neighbours.begin(), neighbours.end(), puzzleNumber);
				//if (result == neighbours.end())
				//{
				//	neighbours.push_back(puzzleNumber);
				//}
			}
		}
	}

	return neighbours;
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

void BoardGenerator::scaleAndReindexPuzzles()
{
	for (int i = 0; i < (int)puzzles_.size(); i++)
	{
		puzzles_[i].scalePuzzle();
		puzzles_[i].setID(i + 1);
	}
}