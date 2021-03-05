
#include "model/instance.h"

static glm::mat4 const IDENTITY = glm::mat4(1.0f);

Instance::Instance(Model const &model) : model(model), position(0.0f, 0.0f, 0.0f), orientation(0.0f, 0.0f, 0.0f, 1.0f)
{

}

void Instance::render(glm::mat4 const &view, glm::mat4 const &projection)const
{
    // Build model matrix
    glm::mat4 modelMatrix = glm::toMat4(orientation);

    // Render model
    model.render(modelMatrix, view, projection);
}

void Instance::physics()
{
    // Rotate
    orientation = glm::rotate(orientation, 0.007f, glm::vec3(-0.5f, 1.0f, 0.0f));

    // Clear accumulated error
    orientation = glm::normalize(orientation);
}
