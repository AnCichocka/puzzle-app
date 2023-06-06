#include "Position.h"
#include <iostream>

using namespace std;

Position::Position()
{}

Position::Position(int x, int y)
{
	x_ = x;
	y_ = y;
	//cout << "position (" << x_ << "," << y_ << ") created!\n";
}

bool Position::operator==(const Position& other) const
{
	return (x_ == other.x_ && y_ == other.y_);
}

Position Position::operator+(Position& other) const
{
	Position result = Position(x_ + other.getX(), y_ + other.getY());
	return result;
}

Position Position::operator-(Position& other) const
{
	Position result = Position(x_ - other.getX(), y_ - other.getY());
	return result;
}

bool Position::operator<(Position& other) const
{
	return (x_ <= other.x_ && y_ <= other.y_);
}

bool Position::operator>(Position& other) const
{
	return (x_ >= other.x_ && y_ >= other.y_);
}

int Position::getX()
{
	return x_;
}

int Position::getY()
{
	return y_;
}

void Position::print()
{
	cout << "(" << x_ << "," << y_ << ")" << endl;
}