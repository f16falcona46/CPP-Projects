#include "Entity.h"

void Entity::Draw()
{
	this->draw_internal();
}

void Entity::Tick(float time)
{
	this->tick_internal(time);
	this->update_state_vectors(time);
}

void Entity::update_state_vectors(float time)
{
	this->pos += time * this->vel;
}
