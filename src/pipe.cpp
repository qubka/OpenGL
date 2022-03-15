#include "pipe.hpp"
#include "line.hpp"
#include "plane.hpp"
#include "extentions.hpp"

Pipe::Pipe() {
}

Pipe::Pipe(const std::vector<glm::vec3>& pathPoints, const std::vector<glm::vec3>& contourPoints) {
    set(pathPoints, contourPoints);
}

void Pipe::set(const std::vector<glm::vec3>& pathPoints, const std::vector<glm::vec3>& contourPoints) {
    path = pathPoints;
    contour = contourPoints;
    generateContours();
}

void Pipe::setPath(const std::vector<glm::vec3>& pathPoints) {
    path = pathPoints;
    generateContours();
}

void Pipe::setContour(const std::vector<glm::vec3>& contourPoints) {
    contour = contourPoints;
    generateContours();
}

void Pipe::addPathPoint(const glm::vec3& point) {
    // add it to path first
    path.push_back(point);

    int count = static_cast<int>(path.size());
    if (count == 1) {
        transformFirstContour();
        normals.push_back(computeContourNormal(0));
    } else if (count == 2) {
        contours.push_back(projectContour(0, 1));
        normals.push_back(computeContourNormal(1));
    } else {
        // add dummy to match same # of contours/normals and path
        std::vector<glm::vec3> dummy;
        contours.push_back(dummy);
        normals.push_back(dummy);

        // re-project the previous contour
        contours[count - 2] = projectContour(count - 3, count - 2);
        normals[count - 2] = computeContourNormal(count - 2);

        // compute for new contour
        contours[count - 1] = projectContour(count - 2, count - 1);
        normals[count - 1] = computeContourNormal(count - 1);
    }
}

void Pipe::generateContours() {
    // reset
    contours.clear();
    normals.clear();

    // path must have at least a point
    int count = static_cast<int>(path.size());
    if (count < 1)
        return;

    // reserve
    contours.reserve(count);
    normals.reserve(count);

    // rotate and translate the contour to the first path point
    transformFirstContour();
    contours.push_back(contour);
    normals.push_back(computeContourNormal(0));

    // project contour to the plane at the next path point
    for (int i = 1; i < count; ++i) {
        contours.push_back(projectContour(i - 1, i));
        normals.push_back(computeContourNormal(i));
    }
}

std::vector<glm::vec3> Pipe::projectContour(int fromIndex, int toIndex) const {
    glm::vec3 dir1, dir2, normal;
    Line line;

    dir1 = path[toIndex] - path[fromIndex];
    if (toIndex == path.size() - 1)
        dir2 = dir1;
    else
        dir2 = path[toIndex + 1] - path[toIndex];

    normal = dir1 + dir2; // normal vector of plane at toIndex
    Plane plane{normal, path[toIndex]};

    // project each vertex of contour to the plane
    const std::vector<glm::vec3>& fromContour = contours[fromIndex];
    std::vector<glm::vec3> toContour;
    toContour.reserve(fromContour.size());

    for (auto& v : fromContour) {
        line.set(dir1, v);
        toContour.push_back(plane.intersect(line));
    }

    return toContour;
}

void Pipe::transformFirstContour() {
    int pathCount = (int) path.size();
    glm::mat4 matrix{1};

    if (pathCount > 0) {
        // transform matrix
        if (pathCount > 1) {
            matrix = glm::lookAt(matrix, path[1] - path[0]);
        }

        matrix = glm::translate(matrix, path[0]);

        // multiply matrix to the contour
        // NOTE: the contour vertices are transformed here
        //       MUST resubmit contour data if the path is reset to 0
        for (auto& v : contour) {
            v = glm::mat3{matrix} * v;
        }
    }
}

std::vector<glm::vec3> Pipe::computeContourNormal(int pathIndex) const {
    // get current contour and center point
    glm::vec3 center = path[pathIndex];
    const std::vector<glm::vec3>& contour = contours[pathIndex];

    std::vector<glm::vec3> contourNormal;
    contourNormal.reserve(contour.size());

    for (auto& v : contour) {
        contourNormal.push_back(glm::normalize(v - center));
    }

    return contourNormal;
}
