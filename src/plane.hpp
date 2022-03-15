#pragma once

#include "line.hpp"

class Plane {
public:
    Plane();
    Plane(const glm::vec4& abcd);                           // 4 coeff of plane equation
    Plane(const glm::vec3& normal, const glm::vec3& point); // a point on the plane and normal vector

    // setters/getters
    void set(const glm::vec4& abcd);
    void set(const glm::vec3& normal, const glm::vec3& point);  // set with  a point on the plane and normal
    const glm::vec3& getNormal() const { return normal; }
    float getD() const { return d; }                            // return 4th coefficient
    float getNormalLength() const { return normalLength; }      // return length of normal
    float getDistance() const { return distance; };             // return distance from the origin
    float getDistance(const glm::vec3& point);                  // return distance from the point

    // convert plane equation with unit normal vector
    void normalize();

    // for intersection
    glm::vec3 intersect(const Line& line) const;              // intersect with a line
    Line intersect(const Plane& plane) const;                 // intersect with another plane
    bool isIntersected(const Line& line) const;
    bool isIntersected(const Plane& plane) const;

private:
    glm::vec3 normal;   // normal vector of a plane
    float d;            // coefficient of constant term: d = -(a*x0 + b*y0 + c*z0)
    float normalLength; // length of normal vector
    float distance;     // distance from origin to plane
};
