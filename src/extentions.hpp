#pragma once

namespace glm {
    glm::mat4 lookAt(const glm::mat4& matrix, const glm::vec3& target);

    void lookAtFromDirection(const glm::vec3& dir, glm::vec3& forward, glm::vec3& up, glm::vec3& right);
}
