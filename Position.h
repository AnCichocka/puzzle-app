#ifndef POSITION_H
#define POSITION_H

class Position
{
private:
	int x_;
	int y_;

public:
	Position();
	Position(int x, int y);
	bool operator==(const Position& other) const;
	Position operator+(Position& other) const;
	Position operator-(Position& other) const;
	bool operator<(Position& other) const;
	bool operator>(Position& other) const;
	int getX();
	int getY();
	void print();
};

#endif
