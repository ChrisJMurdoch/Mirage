
#pragma once

#include "model/model.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Instance
{
private:
	Model const &model;
	glm::vec3 position;
	glm::quat orientation;
public:
	Instance(Model const &model);
	void render(glm::mat4 const &view, glm::mat4 const  &projection) const;
	void physics();
};
