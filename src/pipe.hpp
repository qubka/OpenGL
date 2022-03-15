#pragma once

class Pipe {
public:
    Pipe();
    Pipe(const std::vector<glm::vec3>& pathPoints, const std::vector<glm::vec3>& contourPoints);

    void set(const std::vector<glm::vec3>& pathPoints, const std::vector<glm::vec3>& contourPoints);
    void setPath(const std::vector<glm::vec3>& pathPoints);
    void setContour(const std::vector<glm::vec3>& contourPoints);
    void addPathPoint(const glm::vec3& point);

    int getPathCount() const                                       { return static_cast<int>(path.size()); }
    const std::vector<glm::vec3>& getPathPoints() const            { return path; }
    const glm::vec3& getPathPoint(int index) const                 { return path.at(index); }
    int getContourCount() const                                    { return static_cast<int>(contours.size()); }
    const std::vector<std::vector<glm::vec3>>& getContours() const { return contours; }
    const std::vector<glm::vec3>& getContour(int index) const      { return contours.at(index); }
    const std::vector< std::vector<glm::vec3>>& getNormals() const { return normals; }
    const std::vector<glm::vec3>& getNormal(int index) const       { return normals.at(index); }

private:
    void generateContours();
    void transformFirstContour();
    std::vector<glm::vec3> projectContour(int fromIndex, int toIndex) const;
    std::vector<glm::vec3> computeContourNormal(int pathIndex) const;

    std::vector<glm::vec3> path;
    std::vector<glm::vec3> contour;
    std::vector<std::vector<glm::vec3>> contours;
    std::vector<std::vector<glm::vec3>> normals;
};