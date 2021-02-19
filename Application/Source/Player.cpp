#include "Player.h"


Player::Player(Scene* scene, Vector3 position, std::string name) : Entity(scene, ENTITYTYPE::CUSTOM, name) {
	Box* box = new Box(Vector3(-1.5, 0, 1.5), Vector3(1.5, 3, -1.5));
	this->driving = false;
	this->hitBox = new HitBox(box);
	this->type = PLAYER;
}

Player::~Player() {
	
}

bool Player::isDriving()
{
	return driving;
}

void Player::setDriving(Car* car, bool drive) {
	driving = drive;
	currentDrivingCar = car;
}

Car* Player::getCar()
{
	return this->currentDrivingCar;
}

void Player::Update(double dt) {
}

void Player::Render() {
	this->scene->modelStack.PushMatrix();
		this->loadOriginTRSIntoStacknHitBox();
	this->scene->modelStack.PopMatrix();
}
 
