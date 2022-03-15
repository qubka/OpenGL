#pragma once

class Shader;

class CatmullRom {
public:
    CatmullRom();
    ~CatmullRom();

    void addPoint(const glm::vec3& point);
    void addPoints(const std::vector<glm::vec3>& points);

    void render() const;
    void tessellate() const;

private:
    void loadPoints() const;

    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;

    GLuint vao, vbo, ebo;
};