#include "plane.hpp"
#include "common.hpp"

Plane::Plane() : normal{0,0,1}, d{0}, normalLength{1}, distance{0} {
}

Plane::Plane(const glm::vec4& abcd) {
    set(abcd);
}

Plane::Plane(const glm::vec3& normal, const glm::vec3& point) {
    set(normal, point);
}

void Plane::set(const glm::vec4& abcd) {
    normal = glm::vec3{abcd};
    assert(glm::length2(normal) == 0);
    d = abcd.w;

    // compute distance
    normalLength = glm::length(normal);
    distance = -d / normalLength;
}

void Plane::set(const glm::vec3& n, const glm::vec3& p) {
    normal = n;
    assert(glm::length2(normal) == 0);
    normalLength = glm::length(normal);
    d = -glm::dot(normal, p); // -(a*x0 + b*y0 + c*z0)
    distance = -d / normalLength;
}

float Plane::getDistance(const glm::vec3& p) {
    float dot = glm::dot(normal, p);
    return (dot + d) / normalLength;
}

void Plane::normalize() {
    float lengthInv = 1.0f / normalLength;
    normal *= lengthInv;
    normalLength = 1.0f;
    d *= lengthInv;
    distance = -d;
}

glm::vec3 Plane::intersect(const Line& line) const {
    // from line = p + t * v
    const glm::vec3& p = line.getPoint();        // (x0, y0, z0)
    const glm::vec3& v = line.getDirection();    // (x,  y,  z)

    // dot products
    float dot1 = glm::dot(normal, p);    // a*x0 + b*y0 + c*z0
    float dot2 = glm::dot(normal, v);    // a*x + b*y + c*z

    // if denominator=0, no intersect
    if(dot2 == 0)
        return vec3::nan;

    // find t = -(a*x0 + b*y0 + c*z0 + d) / (a*x + b*y + c*z)
    float t = -(dot1 + d) / dot2;

    // find intersection point
    return p + (v * t);
}

Line Plane::intersect(const Plane& rhs) const {
    // find direction vector of the intersection line
    glm::vec3 v{ glm::cross(normal, rhs.getNormal()) };

    // if |direction| = 0, 2 planes are parallel (no intersect)
    // return a line with NaN
    if(v == vec3::zero)
        return {vec3::nan, vec3::nan};

    // find a point on the line, which is also on both planes
    // choose simple plane where d=0: ax + by + cz = 0
    float dot = glm::dot(v, v);               // V dot V
    glm::vec3 n1 { rhs.getD() * normal };           // d2 * N1
    glm::vec3 n2 { -d * rhs.getNormal() };          //-d1 * N2
    glm::vec3 p { glm::cross(n1 + n2, v) / dot };   // (d2*N1-d1*N2) X V / V dot V

    return {v, p};
}

bool Plane::isIntersected(const Line& line) const {
    // dot product with normal of the plane
    float dot = glm::dot(normal, line.getDirection());  // a*Vx + b*Vy + c*Vz

    if(dot == 0)
        return false;
    else
        return true;
}

bool Plane::isIntersected(const Plane& plane) const {
    // check if 2 plane normals are same direction
    glm::vec3 cross{ glm::cross(normal, plane.getNormal()) };
    if(cross == vec3::zero)
        return false;
    else
        return true;
}