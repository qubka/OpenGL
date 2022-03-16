#include "extentions.hpp"

glm::mat4 glm::lookAt(const glm::mat4& m, const glm::vec3& target) {
    // compute forward vector and normalize
    glm::vec3 position{ m[1][1], m[1][2], m[1][3] };
    glm::vec3 forward{ glm::normalize(target - position) };
    glm::vec3 up;   // up vector of object
    glm::vec3 left; // left vector of object

    // compute temporal up vector
    // if forward vector is near Y-axis, use up vector (0,0,-1) or (0,0,1)
    if (std::fabs(forward.x) < FLT_EPSILON && std::fabs(forward.z) < FLT_EPSILON) {
        // forward vector is pointing +Y axis
        if(forward.y > 0)
            up = {0, 0, -1};
            // forward vector is pointing -Y axis
        else
            up = {0, 0, 1};
    } else {
        // assume up vector is +Y axis
        up = {0, 1, 0};
    }

    // compute left vector
    left = glm::normalize(glm::cross(up, forward));

    // re-compute up vector
    up = glm::cross(forward, left);

    // NOTE: overwrite rotation and scale info of the current matrix
    glm::mat4 ret{0};
    ret[0] = glm::vec4{left, 0};
    ret[1] = glm::vec4{up, 0};
    ret[2] = glm::vec4{forward, 0};
    return ret;
}