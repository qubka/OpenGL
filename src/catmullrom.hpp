#pragma once

class Shader;

class CatmullRom {
public:
    CatmullRom(std::vector<glm::vec3>&& points, int numSamples);
    ~CatmullRom();

    void render(const std::unique_ptr<Shader>& shader) const;

public:
    GLuint vao, vbo;

    void uniformlySampleControlPoints(int numSamples);
    void computeLengthsAlongControlPoints();
    bool sample(float d, glm::vec3& p, glm::vec3& up); // Return a point on the centreline based on a certain distance along the control curve.

    std::vector<float> distances;

    std::vector<glm::vec3> controlPoints;		// Control points, which are interpolated to produce the centreline points
    std::vector<glm::vec3> controlUpVectors;	// Control upvectors, which are interpolated to produce the centreline upvectors
    std::vector<glm::vec3> centrelinePoints;	// Centreline points
    std::vector<glm::vec3> centrelineUpVectors;// Centreline upvectors
};