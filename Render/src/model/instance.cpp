
#include "model/instance.h"

static glm::mat4 const IDENTITY = glm::mat4(1.0f);

Instance::Instance(Model const &model) : model(model)
{

}

void Instance::render(glm::mat4 const &view, glm::mat4 const &projection) const
{
    // Calulate rotation
    glm::mat4 modelMatrix = glm::rotate(IDENTITY, glm::radians(rotateY), glm::vec3(0.0f, 1.0f, 0.0f));

    // Render model
    model.render(modelMatrix, view, projection);
}

void Instance::physics()
{
    // Rotate
    rotateY += 0.5;
}
