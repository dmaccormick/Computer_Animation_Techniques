#include "Player.h"

//Adds points to this player
void Player::addPoints(int points)
{
	this->points += points;
}
//Takes points away from this player
void Player::takePoints(int points)
{
	this->points -= points;
}
//Gets the number of points that this player has
int Player::getPoints()
{
	return points;
}
//Transfers *amount* points from this player to the target player
void Player::transferPointsTo(Player target, int amount)
{
	this->points -= amount;
	target.points += amount;
}
//Transfers *amount* points from the target player to this player
void Player::takePointsFrom(Player target, int amount)
{
	this->points += amount;
	target.points -= amount;
}

//Calls parent update
void Player::update(float deltaTime)
{
	Kinematic::update(deltaTime);
	cout << "Pos: " << position.x << ", " << position.y << ", " << position.z << endl;
}