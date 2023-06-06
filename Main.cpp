#include "BoardGenerator.h"
#include "GameBoard.h"
#include "Position.h"
#include "Puzzle.h"
#include <SFML/Graphics.hpp>
#include <iostream>

const int BOARD_SIZE = 10;
const int CELL_SIZE = 80;
const int BOARD_WIDTH = 800;
const sf::Color BORDER_COLOR(190, 190, 190);
const sf::Color CELL_COLOR(220, 220, 220);
const sf::Color PUZZLE_COLOR(123, 12, 43);
const sf::Color CAN_PLACE_PUZZLE_COLOR(0, 0, 0);
const sf::Color CANNOT_PLACE_PUZZLE_COLOR(255, 0, 0);
const int BORDER_SIZE = 3;
const int ASIDE_SIZE = 600;
const int PADDING_BETWEEN = 50;
const int CELL_SMALL = 18;

const int CELL_PUZZLE_SIZE = 150;

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

int activePuzzleIndex = 0;

void handleArrow(GameBoard& gameBoard, Position position);
void handleSpace(GameBoard& gameBoard);

int main()
{
	//TODO: check if no of puzzles is not too big

	BoardGenerator boardGenerator(BOARD_SIZE, BOARD_SIZE);
	vector<Puzzle> puzzles = boardGenerator.getPuzzles(); // puzzleID = index -b 1

	GameBoard gameBoard(BOARD_SIZE, BOARD_SIZE, puzzles);
	int puzzlesNumber = puzzles.size();

	// GUI ///////////////////////////////////////////////////////////////////////////////////////////////
	sf::RenderWindow window(sf::VideoMode(BOARD_SIZE * CELL_SIZE + ASIDE_SIZE + PADDING_BETWEEN, BOARD_SIZE * CELL_SIZE), "Puzzle");

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
			for (int y = 0; y < BOARD_SIZE; ++y)
			{
				for (int x = 0; x < BOARD_SIZE; ++x)
				{
					sf::RectangleShape tile(sf::Vector2f(CELL_SIZE - BORDER_SIZE, CELL_SIZE - BORDER_SIZE));
					tile.setPosition(x * CELL_SIZE + BORDER_SIZE / 2, y * CELL_SIZE + BORDER_SIZE / 2);
					tile.setOutlineColor(BORDER_COLOR);
					tile.setOutlineThickness(BORDER_SIZE);

					int puzzleID = board[y][x];

					if (puzzleID != activePuzzleID)
					{
						if (puzzleID > 0)
						{
							tile.setFillColor(COLORS[puzzleID]);
						}
						else
						{
							tile.setFillColor(CELL_COLOR);
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
				sf::RectangleShape tile(sf::Vector2f(CELL_SIZE - BORDER_SIZE, CELL_SIZE - BORDER_SIZE));
				tile.setPosition(x * CELL_SIZE + BORDER_SIZE / 2, y * CELL_SIZE + BORDER_SIZE / 2);
				if (gameBoard.canPlacePuzzle() || gameBoard.isPuzzleOnBoard(gameBoard.getActivePuzzleID()))
				{
					tile.setOutlineColor(CAN_PLACE_PUZZLE_COLOR);
				}
				else
				{
					tile.setOutlineColor(CANNOT_PLACE_PUZZLE_COLOR);
				}
				tile.setOutlineThickness(BORDER_SIZE);

				int puzzleID = board[y][x];
				if (puzzleID > 0)
				{
					tile.setFillColor(COLORS[puzzleID]);
				}
				else
				{
					tile.setFillColor(CELL_COLOR);
				}

				window.draw(tile);
			}

			// drawing puzzles to choose
			int maxX = 3;
			int maxY = puzzlesNumber / maxX;
			if (puzzlesNumber % maxX > 0)
			{
				maxY++;
			}

			int index = 0;

			// Draw aside grid
			for (int y = 0; y < maxY; ++y)
			{
				for (int x = 0; x < maxX; ++x)
				{
					if (index == puzzlesNumber)
					{
						break;
					}
					sf::RectangleShape asideTile(sf::Vector2f(CELL_PUZZLE_SIZE - BORDER_SIZE, CELL_PUZZLE_SIZE - BORDER_SIZE));
					int cellX = x * CELL_PUZZLE_SIZE + BORDER_SIZE / 2 + BOARD_WIDTH + PADDING_BETWEEN;
					int cellY = y * CELL_PUZZLE_SIZE + BORDER_SIZE / 2;

					asideTile.setPosition(cellX, cellY);
					asideTile.setOutlineColor(BORDER_COLOR);
					asideTile.setOutlineThickness(BORDER_SIZE);

					if (activePuzzleIndex == index)
					{
						asideTile.setFillColor(sf::Color(250, 20, 20));
					}
					else
					{
						asideTile.setFillColor(CELL_COLOR);
					}

					// Get puzzle dimensions and positions

					Puzzle puzzle = puzzles[index];
					int width = puzzle.getLowerRightCorner().getX() + 1;
					int height = puzzle.getLowerRightCorner().getY() + 1;
					vector<Position> positions = puzzle.getPositions();

					// Calculate the position of the figure in the middle of the cell
					int figureX = cellX + (CELL_PUZZLE_SIZE - BORDER_SIZE / 2 - width * CELL_SMALL) / 2;
					int figureY = cellY + (CELL_PUZZLE_SIZE - BORDER_SIZE / 2 - height * CELL_SMALL) / 2;

					window.draw(asideTile);
					// Draw each position of the figure as a small rectangle
					for (Position position : positions)
					{
						int rectX = figureX + position.getX() * CELL_SMALL;
						int rectY = figureY + position.getY() * CELL_SMALL;

						sf::RectangleShape rect(sf::Vector2f(CELL_SMALL, CELL_SMALL));
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

void handleArrow(GameBoard& gameBoard, Position position)
{
	int puzzleID = gameBoard.getActivePuzzleID();
	if (gameBoard.getActivePuzzleID() != -1 && !gameBoard.isPuzzleOnBoard(puzzleID))
	{
		gameBoard.move(position);
	}
};

void handleSpace(GameBoard& gameBoard)
{
	if (gameBoard.getActivePuzzleID() != -1 && gameBoard.canPlacePuzzle())
	{
		gameBoard.placeActivePuzzle();
		// TODO: switch to next free puzzle
	}
}
