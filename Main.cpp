#include "BoardGenerator.h"
#include "GameBoard.h"
#include "Position.h"
#include "Puzzle.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <iostream>
#include <random>
#include <unistd.h>

const int asideRowNumber = 5;
int asideColNumber;

// board and cell size
const int BOARD_CELLS_WIDTH_NUMBER = 10;
const int BOARD_CELL_SIZE = 86;
const int BOARD_WIDTH = BOARD_CELLS_WIDTH_NUMBER * BOARD_CELL_SIZE;

// board cells' colors
const sf::Color BOARD_EMPTY_CELL_COLOR(220, 220, 220);
const sf::Color INACTIVE_BORDER_COLOR(190, 190, 190);

// board borders
const int BOARD_BORDER_SIZE = 3;
const sf::Color CAN_PLACE_PUZZLE_BORDER_COLOR(0, 0, 0);
const sf::Color CANNOT_PLACE_PUZZLE_BORDER_COLOR(255, 0, 0);

const int PADDING_BETWEEN = 0;

// aside - puzzle choice
int asideSize;
const int ASIDE_CELL_SIZE = 2 * BOARD_CELL_SIZE;
const int ASIDE_DRAW_PUZZLE_CELL = 20;
const int ASIDE_BORDER_SIZE = 4;

// aside cells' colors
const sf::Color ASIDE_EMPTY_CELL_COLOR(220, 220, 220);
const sf::Color ASIDE_ACTIVE_PUZZLE_CELL_COLOR(255, 204, 255);
const sf::Color ASIDE_ON_BOARD_PUZZLE_CELL_COLOR = INACTIVE_BORDER_COLOR;

const sf::Color COLORS[] = {
	sf::Color(0, 255, 0),	  // Green
	sf::Color(0, 0, 255),	  // Blue
	sf::Color(255, 255, 0),	  // Yellow
	sf::Color(255, 0, 255),	  // Magenta
	sf::Color(0, 255, 255),	  // Cyan
	sf::Color(255, 128, 0),	  // Orange
	sf::Color(128, 0, 255),	  // Purple
	sf::Color(0, 255, 128),	  // Lime
	sf::Color(128, 255, 0),	  // Chartreuse
	sf::Color(255, 0, 128),	  // Pink
	sf::Color(0, 128, 255),	  // Sky Blue
	sf::Color(128, 255, 255), // Light Blue
	sf::Color(255, 128, 128), // Light Red
	sf::Color(128, 128, 255), // Light Purple
	sf::Color(255, 128, 255), // Light Magenta
	sf::Color(128, 255, 255), // Light Cyan
	sf::Color(255, 255, 128), // Pale Yellow
	sf::Color(255, 128, 64),  // Salmon
	sf::Color(64, 255, 128),  // Spring Green
	sf::Color(128, 64, 255),  // Violet
	sf::Color(128, 255, 64),  // Lime Green
	sf::Color(255, 64, 128),  // Hot Pink
	sf::Color(64, 128, 255)	  // Deep Sky Blue
};

int activePuzzleIndex = 0;
int activePuzzleID;
int (*board)[10];
int puzzlesNumber;
BoardGenerator boardGenerator(BOARD_CELLS_WIDTH_NUMBER, BOARD_CELLS_WIDTH_NUMBER);
vector<Puzzle> puzzles;
GameBoard gameBoard;
vector<int> puzzleIDs;

vector<int> getPuzzleIDs(int n, bool shufflePuzzles);
void handleArrow(Position position);
void handleSpace();
bool isEmptyCell(int x, int y);
bool isActiveCell(int x, int y);
bool isActiveBorderColor();
void drawInactiveCells(sf::RenderWindow &window);
void drawActiveCells(sf::RenderWindow &window);
void drawPuzzlesToChoose(sf::RenderWindow &window);

int main()
{
	// SETUP -------------------------------------------------------------------------
	boardGenerator.generateBoard();
	puzzles = boardGenerator.getPuzzles();
	puzzlesNumber = puzzles.size();

	gameBoard = GameBoard(BOARD_CELLS_WIDTH_NUMBER, BOARD_CELLS_WIDTH_NUMBER, puzzles);

	asideColNumber = puzzlesNumber / asideRowNumber;
	if (puzzlesNumber % asideRowNumber > 0)
	{
		asideColNumber++;
	}

	asideSize = asideColNumber * ASIDE_CELL_SIZE;

	cout << "puzzles: " << puzzlesNumber << endl;

	puzzleIDs = getPuzzleIDs(puzzles.size(), false);

	cout << "printing answer..." << endl;
	boardGenerator.printAnswer(puzzleIDs);

	cout << "puzzleID order..." << endl;
	for (int i = 0; i < puzzles.size(); i++)
	{
		std::cout << puzzleIDs[i] << " ";
	}
	std::cout << std::endl;

	// GUI -------------------------------------------------------------------------
	sf::RenderWindow window(sf::VideoMode(BOARD_CELLS_WIDTH_NUMBER * BOARD_CELL_SIZE + asideSize + PADDING_BETWEEN, BOARD_CELLS_WIDTH_NUMBER * BOARD_CELL_SIZE), "Puzzle");

	while (window.isOpen())
	{
		if (gameBoard.gameWon())
		{
			cout << "YOU WIN!!!!!!!!!" << endl;
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Up:
					handleArrow(Position(0, -1));
					break;

				case sf::Keyboard::Down:
					handleArrow(Position(0, 1));
					break;

				case sf::Keyboard::Left:
					handleArrow(Position(-1, 0));
					break;

				case sf::Keyboard::Right:
					handleArrow(Position(1, 0));
					break;

				case sf::Keyboard::Space:
					handleSpace();
					break;

				case sf::Keyboard::X:
					activePuzzleIndex = (activePuzzleIndex + 1) % puzzlesNumber;
					activePuzzleID = puzzleIDs[activePuzzleIndex];
					gameBoard.setActivePuzzleID(activePuzzleID);
					break;

				case sf::Keyboard::Z:
					activePuzzleIndex = (activePuzzleIndex - 1 + puzzlesNumber) % puzzlesNumber;
					activePuzzleID = puzzleIDs[activePuzzleIndex];
					gameBoard.setActivePuzzleID(activePuzzleID);
					break;
				case sf::Keyboard::Enter:
					execl("./puzzle-app", "puzzle-app", NULL);
					break;
				}
			}
		}
		board = gameBoard.getBoard();

		window.clear(BOARD_EMPTY_CELL_COLOR);

		// draw inactive cells
		drawInactiveCells(window);

		// draw active cells
		drawActiveCells(window);

		// drawing puzzles to choose from
		drawPuzzlesToChoose(window);

		window.display();
	}
	return 0;
}

void handleArrow(Position position)
{
	int puzzleID = gameBoard.getActivePuzzleID();
	if (!gameBoard.isPuzzleOnBoard(puzzleID))
	{
		gameBoard.move(position);
	}
}

void handleSpace()
{
	if (!gameBoard.isPuzzleOnBoard(activePuzzleID))
	{
		if (gameBoard.canPlacePuzzle())
		{
			gameBoard.placeActivePuzzle();
			// TODO: switch to next free puzzle
		}
	}
	else
	{
		// puzzle is on board -> pickup
		gameBoard.pickUpPuzzle(activePuzzleID);
	}
}

bool isEmptyCell(int x, int y)
{
	return board[y][x] == -1;
}

bool isActiveCell(int x, int y)
{
	return board[y][x] == activePuzzleID;
}

bool isActiveBorderColor()
{
	return gameBoard.canPlacePuzzle() || gameBoard.isPuzzleOnBoard(gameBoard.getActivePuzzleID());
}

void drawInactiveCells(sf::RenderWindow &window)
{
	for (int y = 0; y < BOARD_CELLS_WIDTH_NUMBER; ++y)
	{
		for (int x = 0; x < BOARD_CELLS_WIDTH_NUMBER; ++x)
		{
			sf::RectangleShape tile(sf::Vector2f(BOARD_CELL_SIZE - BOARD_BORDER_SIZE, BOARD_CELL_SIZE - BOARD_BORDER_SIZE));
			tile.setPosition(x * BOARD_CELL_SIZE + BOARD_BORDER_SIZE / 2, y * BOARD_CELL_SIZE + BOARD_BORDER_SIZE / 2);
			tile.setOutlineColor(INACTIVE_BORDER_COLOR);
			tile.setOutlineThickness(BOARD_BORDER_SIZE);

			if (!isActiveCell(x, y))
			{
				if (isEmptyCell(x, y))
				{
					tile.setFillColor(BOARD_EMPTY_CELL_COLOR);
				}
				else
				{
					int currentPuzzleID = board[y][x];
					tile.setFillColor(COLORS[currentPuzzleID]);
				}

				window.draw(tile);
			}
		}
	}
}

void drawActiveCells(sf::RenderWindow &window)
{
	for (Position position : gameBoard.getActivePositions())
	{
		int x = position.getX();
		int y = position.getY();
		sf::RectangleShape tile(sf::Vector2f(BOARD_CELL_SIZE - BOARD_BORDER_SIZE, BOARD_CELL_SIZE - BOARD_BORDER_SIZE));
		tile.setPosition(x * BOARD_CELL_SIZE + BOARD_BORDER_SIZE / 2, y * BOARD_CELL_SIZE + BOARD_BORDER_SIZE / 2);

		if (isActiveBorderColor())
		{
			tile.setOutlineColor(CAN_PLACE_PUZZLE_BORDER_COLOR);
		}
		else
		{
			tile.setOutlineColor(CANNOT_PLACE_PUZZLE_BORDER_COLOR);
		}
		tile.setOutlineThickness(BOARD_BORDER_SIZE);

		if (isEmptyCell(x, y))
		{
			tile.setFillColor(BOARD_EMPTY_CELL_COLOR);
		}
		else
		{
			int currentPuzzleID = board[y][x];
			tile.setFillColor(COLORS[currentPuzzleID]);
		}

		window.draw(tile);
	}
}

void drawPuzzlesToChoose(sf::RenderWindow &window)
{
	int puzzleIndex = 0;
	int fullRows = puzzlesNumber % asideRowNumber;
	// cout << "fullRows: " << fullRows << endl;
	int done = 0;
	// Draw aside grid
	for (int y = 0; y < asideRowNumber; ++y)
	{
		for (int x = 0; x < asideColNumber; ++x)
		{
			if (!(puzzleIndex < puzzlesNumber))
			{
				return;
			}

			if (x == asideColNumber - 1 && (done >= fullRows && fullRows != 0))
			{
				break;
			}

			sf::RectangleShape asideTile(sf::Vector2f(ASIDE_CELL_SIZE - ASIDE_BORDER_SIZE, ASIDE_CELL_SIZE - ASIDE_BORDER_SIZE));
			int cellX = x * ASIDE_CELL_SIZE + ASIDE_BORDER_SIZE / 2 + BOARD_WIDTH + PADDING_BETWEEN;
			int cellY = y * ASIDE_CELL_SIZE + ASIDE_BORDER_SIZE / 2;

			asideTile.setPosition(cellX, cellY);
			asideTile.setOutlineColor(INACTIVE_BORDER_COLOR);
			asideTile.setOutlineThickness(ASIDE_BORDER_SIZE);

			int currentPuzzleID = puzzleIDs[puzzleIndex];
			if (activePuzzleID == currentPuzzleID)
			{
				asideTile.setFillColor(ASIDE_ACTIVE_PUZZLE_CELL_COLOR);
			}
			else if (gameBoard.isPuzzleOnBoard(currentPuzzleID))
			{
				asideTile.setFillColor(ASIDE_ON_BOARD_PUZZLE_CELL_COLOR);
			}
			else
			{
				asideTile.setFillColor(ASIDE_EMPTY_CELL_COLOR);
			}

			// Get puzzle dimensions and positions

			Puzzle puzzle = puzzles[puzzleIDs[puzzleIndex]];
			int width = puzzle.getLowerRightCorner().getX() + 1;
			int height = puzzle.getLowerRightCorner().getY() + 1;
			vector<Position> positions = puzzle.getPositions();

			// Calculate the position of the figure in the middle of the cell
			int figureX = cellX + (ASIDE_CELL_SIZE - ASIDE_BORDER_SIZE / 2 - width * ASIDE_DRAW_PUZZLE_CELL) / 2;
			int figureY = cellY + (ASIDE_CELL_SIZE - ASIDE_BORDER_SIZE / 2 - height * ASIDE_DRAW_PUZZLE_CELL) / 2;

			window.draw(asideTile);
			// Draw each position of the figure as a small rectangle
			for (Position position : positions)
			{
				int rectX = figureX + position.getX() * ASIDE_DRAW_PUZZLE_CELL;
				int rectY = figureY + position.getY() * ASIDE_DRAW_PUZZLE_CELL;

				sf::RectangleShape rect(sf::Vector2f(ASIDE_DRAW_PUZZLE_CELL, ASIDE_DRAW_PUZZLE_CELL));
				rect.setPosition(rectX, rectY);
				rect.setFillColor(COLORS[puzzle.getID()]);

				window.draw(rect);
			}

			puzzleIndex++;
		}
		done++;
	}
}

vector<int> getPuzzleIDs(int n, bool shufflePuzzles)
{
	vector<int> t(n);

	for (int i = 0; i < n; i++)
	{
		t[i] = i;
	}

	if (!shufflePuzzles)
	{
		return t;
	}

	random_device rd;
	mt19937 gen(rd());
	for (int i = n - 1; i > 0; i--)
	{
		uniform_int_distribution<int> dist(0, i);
		int j = dist(gen);
		swap(t[i], t[j]);
	}

	return t;
}

// maybe to add:
// 1. remeber active position for puzzles which are not on board?
// 2. go to next free puzzle after placing puzzle to board?
