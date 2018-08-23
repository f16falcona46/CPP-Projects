#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>

class Entity {
public:
	virtual ~Entity() {}
	void Draw();
	void Tick(float time);
	glm::vec3 pos;
	glm::vec3 vel;
protected:
	Entity() {}
	virtual void draw_internal() {}
	virtual void tick_internal(float time) {}
private:
	void update_state_vectors(float time);
};

#endif
