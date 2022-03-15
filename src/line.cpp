#include "line.hpp"
#include "common.hpp"

Line::Line() : direction{0}, point{0} {
}

Line::Line(const glm::vec3& v, const glm::vec3& p) : direction{v}, point{p} {
}

Line::Line(float slope, float intercept) {
    set(slope, intercept);
}

Line::Line(const glm::vec2& direction, const glm::vec2& point) {
    set(direction, point);
}

void Line::set(const glm::vec3& v, const glm::vec3& p) {
    direction = v;
    point = p;
}

void Line::set(const glm::vec2& v, const glm::vec2& p) {
    // convert 2D to 3D
    direction = glm::vec3{v.x, v.y, 0};
    point = glm::vec3{p.x, p.y, 0};
}

void Line::set(float slope, float intercept) {
    // convert slope-intercept form (2D) to parametric form (3D)
    direction = glm::vec3{1, slope, 0};
    point = glm::vec3{0, intercept, 0};
}

glm::vec3 Line::intersect(const Line& line) const {
    const glm::vec3& v1 = direction;
    const glm::vec3& v2 = line.getDirection();
    const glm::vec3& p1 = point;
    const glm::vec3& p2 = line.getPoint();

    glm::vec3 result = vec3::nan; // default with NaN

    // find v3 = (p2 - p1) x V2
    glm::vec3 v3{ glm::cross(p2 - p1, v2) };

    // find v4 = V1 x V2
    glm::vec3 v4{ glm::cross(v1, v2) };

    // find (V1xV2) . (V1xV2)
    float dot = glm::dot(v4, v4);

    // if both V1 and V2 are same direction, return NaN point
    if (dot == 0)
        return result;

    // find a = ((p2-p1)xV2).(V1xV2) / (V1xV2).(V1xV2)
    float alpha = glm::dot(v3, v4) / dot;

    // find intersect point
    result = p1 + (alpha * v1);
    return result;
}

bool Line::isIntersected(const Line& line) const {
    // if 2 lines are same direction, the magnitude of cross product is 0
    glm::vec3 v{ glm::cross(direction, line.getDirection()) };
    if (v == vec3::zero)
        return false;
    else
        return true;
}
