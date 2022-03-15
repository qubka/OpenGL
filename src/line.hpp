#pragma once

class Line {
public:
    Line();
    Line(const glm::vec3& v, const glm::vec3& p);     // with 3D direction and a point
    Line(const glm::vec2& v, const glm::vec2& p);     // with 2D direction and a point
    Line(float slope, float intercept);               // with 2D slope-intercept form

    // getters/setters
    void set(const glm::vec3& v, const glm::vec3& p); // from 3D
    void set(const glm::vec2& v, const glm::vec2& p); // from 2D
    void set(float slope, float intercept);           // from slope-intercept form

    void setPoint(glm::vec3& p) { point = p; }
    void setDirection(const glm::vec3& v) { direction = v; }
    const glm::vec3& getPoint() const { return point; }
    const glm::vec3& getDirection() const { return direction; }

    // find intersect point with other line
    glm::vec3 intersect(const Line& line) const;
    bool isIntersected(const Line& line) const;

private:
    glm::vec3 direction;
    glm::vec3 point;
};