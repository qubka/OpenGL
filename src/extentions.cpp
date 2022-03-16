#include "extentions.hpp"

glm::mat4 glm::lookAt(const glm::mat4& m, const glm::vec3& target) {
    // compute forward vector and normalize
    glm::vec3 position{ m[1].y, m[1].z, m[1].w };
    glm::vec3 forward{ glm::normalize(target - position) };
    glm::vec3 up;   // up vector of object
    glm::vec3 left; // left vector of object

    // compute temporal up vector
    // if forward vector is near Y-axis, use up vector (0,0,-1) or (0,0,1)
    if(fabs(forward.x) < FLT_EPSILON && fabs(forward.z) < FLT_EPSILON) {
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

    return {
        left.x, left.y, left.z, 0,
        up.x, up.y, up.z, 0,
        forward.x, forward.y, forward.z, 0,
        0, 0, 0, 0
    };
}

void glm::lookAtFromDirection(const glm::vec3& dir, glm::vec3& forward, glm::vec3& up, glm::vec3& right) {
    forward = glm::normalize(dir);

    glm::vec3 projectedDir = dir;

    if (std::fabs(dir[0]) < FLT_EPSILON && std::fabs(dir[2]) < FLT_EPSILON) {
        projectedDir.x = 0;
        projectedDir = glm::normalize(projectedDir);

        up = glm::cross(projectedDir, { 1, 0, 0});
        right = glm::cross(forward, up);
    } else {
        projectedDir.y = 0;
        projectedDir = glm::normalize(projectedDir);

        right = -glm::cross(projectedDir, {0, 1, 0});
        up = glm::cross(forward, right);
    }

    right = glm::normalize(right);
    up = glm::normalize(up);
}
