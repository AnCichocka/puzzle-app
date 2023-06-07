#include "BoardGenerator.h"
#include "GameBoard.h"
#include "Position.h"
#include "Puzzle.h"
#include <SFML/Graphics.hpp>
#include <iostream>

const int asideColNumber = 3;
int activePuzzleIndex = 0;

void handleArrow(GameBoard &gameBoard, Position position);
void handleSpace(GameBoard &gameBoard);

// CONSTANTS ///////////////////////////////////////////////////////
// board and cell size
const int BOARD_CELLS_WIDTH_NUMBER = 10;
const int BOARD_CELL_SIZE = 80;
const int BOARD_WIDTH = BOARD_CELLS_WIDTH_NUMBER * BOARD_CELL_SIZE;

// board cells' colors
const sf::Color BOARD_EMPTY_CELL_COLOR(220, 220, 220);
const sf::Color INACTIVE_BORDER_COLOR(190, 190, 190);

// board borders
const int BOARD_BORDER_SIZE = 3;
const sf::Color CAN_PLACE_PUZZLE_BORDER_COLOR(0, 0, 0);
const sf::Color CANNOT_PLACE_PUZZLE_BORDER_COLOR(255, 0, 0);

const int PADDING_BETWEEN = 50;

// aside - puzzle choice
const int ASIDE_CELL_SIZE = 150;
const int ASIDE_SIZE = asideColNumber * ASIDE_CELL_SIZE;
const int ASIDE_DRAW_PUZZLE_CELL = 18;

// aside cells' colors
const sf::Color ASIDE_ACTIVE_PUZZLE_CELL_COLOR(250, 20, 20);
const sf::Color ASIDE_ON_BOARD_PUZZLE_CELL_COLOR(0, 0, 0);

int counter = 0;

const sf::Color COLORS[] = {
	sf::Color(255, 0, 0),	  // Red
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
	sf::Color(128, 255, 128), // Light Green
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

int main()
{
	// TODO: check if no of puzzles is not too big

	BoardGenerator boardGenerator(BOARD_CELLS_WIDTH_NUMBER, BOARD_CELLS_WIDTH_NUMBER);
	vector<Puzzle> puzzles = boardGenerator.getPuzzles(); // puzzleID = index -b 1
	int puzzlesNumber = puzzles.size();

	GameBoard gameBoard(BOARD_CELLS_WIDTH_NUMBER, BOARD_CELLS_WIDTH_NUMBER, puzzles);

	// GUI ///////////////////////////////////////////////////////////////////////////////////////////////
	sf::RenderWindow window(sf::VideoMode(BOARD_CELLS_WIDTH_NUMBER * BOARD_CELL_SIZE + ASIDE_SIZE + PADDING_BETWEEN, BOARD_CELLS_WIDTH_NUMBER * BOARD_CELL_SIZE), "Puzzle");

	while (window.isOpen())
	{
		if (gameBoard.gameWon())
		{
			cout << "YOU WIN!!!!!!!!!" << endl;
		}
		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Up)
				{
					handleArrow(gameBoard, Position(0, -1));
				}
				else if (event.key.code == sf::Keyboard::Down)
				{
					handleArrow(gameBoard, Position(0, 1));
				}
				else if (event.key.code == sf::Keyboard::Left)
				{
					handleArrow(gameBoard, Position(-1, 0));
				}
				else if (event.key.code == sf::Keyboard::Right)
				{
					handleArrow(gameBoard, Position(1, 0));
				}
				else if (event.key.code == sf::Keyboard::Space)
				{
					handleSpace(gameBoard);
				}
				else if (event.key.code == sf::Keyboard::X)
				{
					int newActiveIndex = (activePuzzleIndex + 1) % (puzzlesNumber);
					activePuzzleIndex = newActiveIndex;
					gameBoard.setActivePuzzle(newActiveIndex);
				}
				else if (event.key.code == sf::Keyboard::Z)
				{
					int newActiveIndex = (activePuzzleIndex - 1 + puzzlesNumber) % puzzlesNumber;
					activePuzzleIndex = newActiveIndex;
					gameBoard.setActivePuzzle(newActiveIndex);
				}
			}

			int(*board)[10] = gameBoard.getBoard();
			int activePuzzleID = gameBoard.getActivePuzzleID();

			window.clear();

			// draw inactive cells
			for (int y = 0; y < BOARD_CELLS_WIDTH_NUMBER; ++y)
			{
				for (int x = 0; x < BOARD_CELLS_WIDTH_NUMBER; ++x)
				{
					sf::RectangleShape tile(sf::Vector2f(BOARD_CELL_SIZE - BOARD_BORDER_SIZE, BOARD_CELL_SIZE - BOARD_BORDER_SIZE));
					tile.setPosition(x * BOARD_CELL_SIZE + BOARD_BORDER_SIZE / 2, y * BOARD_CELL_SIZE + BOARD_BORDER_SIZE / 2);
					tile.setOutlineColor(INACTIVE_BORDER_COLOR);
					tile.setOutlineThickness(BOARD_BORDER_SIZE);

					int puzzleID = board[y][x];

					if (puzzleID != activePuzzleID)
					{
						if (puzzleID > 0)
						{
							tile.setFillColor(COLORS[puzzleID]);
						}
						else
						{
							tile.setFillColor(BOARD_EMPTY_CELL_COLOR);
						}

						window.draw(tile);
					}
				}
			}

			// draw active cells
			for (Position position : gameBoard.getActivePositions())
			{
				int x = position.getX();
				int y = position.getY();
				sf::RectangleShape tile(sf::Vector2f(BOARD_CELL_SIZE - BOARD_BORDER_SIZE, BOARD_CELL_SIZE - BOARD_BORDER_SIZE));
				tile.setPosition(x * BOARD_CELL_SIZE + BOARD_BORDER_SIZE / 2, y * BOARD_CELL_SIZE + BOARD_BORDER_SIZE / 2);
				if (gameBoard.canPlacePuzzle() || gameBoard.isPuzzleOnBoard(gameBoard.getActivePuzzleID()))
				{
					tile.setOutlineColor(CAN_PLACE_PUZZLE_BORDER_COLOR);
				}
				else
				{
					tile.setOutlineColor(CANNOT_PLACE_PUZZLE_BORDER_COLOR);
				}
				tile.setOutlineThickness(BOARD_BORDER_SIZE);

				int puzzleID = board[y][x];
				if (puzzleID > 0)
				{
					tile.setFillColor(COLORS[puzzleID]);
				}
				else
				{
					tile.setFillColor(BOARD_EMPTY_CELL_COLOR);
				}

				window.draw(tile);
			}

			// drawing puzzles to choose

			int maxY = puzzlesNumber / asideColNumber;
			if (puzzlesNumber % asideColNumber > 0)
			{
				maxY++;
			}

			int index = 0;

			// Draw aside grid
			for (int y = 0; y < maxY; ++y)
			{
				for (int x = 0; x < asideColNumber; ++x)
				{
					if (index == puzzlesNumber)
					{
						break;
					}
					sf::RectangleShape asideTile(sf::Vector2f(ASIDE_CELL_SIZE - BOARD_BORDER_SIZE, ASIDE_CELL_SIZE - BOARD_BORDER_SIZE));
					int cellX = x * ASIDE_CELL_SIZE + BOARD_BORDER_SIZE / 2 + BOARD_WIDTH + PADDING_BETWEEN;
					int cellY = y * ASIDE_CELL_SIZE + BOARD_BORDER_SIZE / 2;

					asideTile.setPosition(cellX, cellY);
					asideTile.setOutlineColor(INACTIVE_BORDER_COLOR);
					asideTile.setOutlineThickness(BOARD_BORDER_SIZE);

					if (activePuzzleID == index + 1)
					{
						asideTile.setFillColor(ASIDE_ACTIVE_PUZZLE_CELL_COLOR);
					}
					else if (gameBoard.isPuzzleOnBoard(index + 1))
					{
						asideTile.setFillColor(ASIDE_ON_BOARD_PUZZLE_CELL_COLOR);
					}
					else
					{
						asideTile.setFillColor(BOARD_EMPTY_CELL_COLOR);
					}

					// Get puzzle dimensions and positions

					Puzzle puzzle = puzzles[index];
					int width = puzzle.getLowerRightCorner().getX() + 1;
					int height = puzzle.getLowerRightCorner().getY() + 1;
					vector<Position> positions = puzzle.getPositions();

					// Calculate the position of the figure in the middle of the cell
					int figureX = cellX + (ASIDE_CELL_SIZE - BOARD_BORDER_SIZE / 2 - width * ASIDE_DRAW_PUZZLE_CELL) / 2;
					int figureY = cellY + (ASIDE_CELL_SIZE - BOARD_BORDER_SIZE / 2 - height * ASIDE_DRAW_PUZZLE_CELL) / 2;

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

					index++;
				}
			}

			window.display();
		}
	}
	return 0;
}

void handleArrow(GameBoard &gameBoard, Position position)
{
	int puzzleID = gameBoard.getActivePuzzleID();
	if (gameBoard.getActivePuzzleID() != -1 && !gameBoard.isPuzzleOnBoard(puzzleID))
	{
		gameBoard.move(position);
	}
};

void handleSpace(GameBoard &gameBoard)
{
	int puzzleID = gameBoard.getActivePuzzleID();
	if (!gameBoard.isPuzzleOnBoard(puzzleID))
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
		gameBoard.pickUpPuzzle(puzzleID);
	}
}

// 1. randomize order on list
// 2. reslove indexes and IDs
// 3. ending game screen

// maybe to add:
// 1. remeber active position for puzzles which are not on board?
// 2. go to next free puzzle after placing puzzle to board?
