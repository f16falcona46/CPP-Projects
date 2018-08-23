#ifndef ROTATING_ENTITY_H
#define ROTATING_ENTITY_H

#include "OGLEntity.h"
#include <glm/gtx/transform.hpp>

class RotatingEntity : public OGLEntity {
public:
	RotatingEntity(const std::string& filename, const GLESData& data, float x_rate=0.0f, float y_rate=0.0f, float scale=1.0f) : OGLEntity(filename, data),
		x_rate(x_rate), y_rate(y_rate), scale(scale), x_angle(0.0f), y_angle(0.0f) {}
	float x_rate, y_rate, scale;
protected:
	float x_angle, y_angle;
	virtual void tick_internal(float time) override {
		this->x_angle += time * this->x_rate;
		this->y_angle += time * this->y_rate;
		this->M_rotate = glm::scale(glm::vec3(this->scale, this->scale, this->scale));
		this->M_rotate = glm::rotate(this->M_rotate, this->x_angle, glm::vec3(0.0f, 0.0f, 1.0f));
		this->M_rotate = glm::rotate(this->M_rotate, this->y_angle, glm::vec3(1.0f, 0.0f, 0.0f));
		OGLEntity::tick_internal(time);
	}
};


#endif //ROTATING_ENTITY_H
