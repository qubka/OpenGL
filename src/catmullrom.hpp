#pragma once

class Shader;

class CatmullRom {
public:
    CatmullRom(std::vector<glm::vec3>&& points,  int numSamples);
    ~CatmullRom();

    void render(const std::unique_ptr<Shader>& shader) const;

public:
    GLuint vao, vbo;

    void uniformlySampleControlPoints(int numSamples);
    void generateTubeSurface(float radius, int steps);
    void computeLengthsAlongControlPoints();
    bool sample(float d, glm::vec3& p);

    static glm::vec3 interpolate(const glm::vec3& start, const glm::vec3& end, const glm::vec3& tanPoint1, const glm::vec3& tanPoint2, float t);

    std::vector<glm::vec3> vertices;
    std::vector<float> distances;
    std::vector<glm::vec3> controlPoints;
    std::vector<glm::vec3> centrelinePoints;
};