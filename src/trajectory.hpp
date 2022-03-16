#pragma once

struct Trajectory {
    void setup(const std::vector<glm::vec3>& spline = {});
    void advance(float increment);

    glm::vec3 camPos;
    glm::vec3 camTarget;
    glm::mat4 camOrient;
    float angle;
    float posRadius;
    float targetRadius;
    glm::vec3 tangent;
    glm::vec3 normal;
    glm::vec3 binormal;
    std::vector<glm::vec3> splineCtrlPoint;
    uint32_t segmentIndex;
    float t;
    glm::mat4 localOrient; // matrix used to xform world-space objects to the local camera-on-tube space
};