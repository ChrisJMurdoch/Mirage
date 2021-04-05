
#include "model/instance.hpp"

static glm::mat4 const IDENTITY = glm::mat4(1.0f);

Instance::Instance(Model const &model) : model(model), position(0.0f, 0.0f, 0.0f), orientation(0.0f, 0.0f, 0.0f, 1.0f)
{
}

void Instance::render(glm::mat4 const &view, glm::mat4 const &projection) const
{
    // Build model matrix
    glm::mat4 modelMatrix = glm::toMat4(orientation);

    // Render model
    model.render(modelMatrix, view, projection);
}

void Instance::rotate(glm::vec3 axis, float radians)
{
    // Rotate
    orientation = glm::rotate(orientation, radians, axis);

    // Clear accumulated error every 10 rotations
    static int i = 0;
    orientation = (i++ % 10 == 0) ? glm::normalize(orientation) : orientation;
}
