#pragma once

namespace glm {
    /*
     * rotate matrix to face along the target direction
     * NOTE: This function will clear the previous rotation and scale info and
     * rebuild the matrix with the target vector. But it will keep the previous
     * translation values.
     * NOTE: It is for rotating object to look at the target, NOT for camera
     */
    glm::mat4 lookAt(const glm::mat4& matrix, const glm::vec3& target);
}
