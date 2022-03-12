#pragma once

class Shader {
public:
    Shader();
    ~Shader();

    void setUniform(const std::string& name, int value) const;
    void setUniform(const std::string& name, float value) const;
    void setUniform(const std::string& name, const glm::vec2& value) const;
    void setUniform(const std::string& name, const glm::vec3& value) const;
    void setUniform(const std::string& name, const glm::vec4& value) const;
    void setUniform(const std::string& name, const glm::mat2& value) const;
    void setUniform(const std::string& name, const glm::mat2& value, int count) const;
    void setUniform(const std::string& name, const glm::mat3& value) const;
    void setUniform(const std::string& name, const glm::mat3& value, int count) const;
    void setUniform(const std::string& name, const glm::mat4& value) const;
    void setUniform(const std::string& name, const glm::mat4& value, int count) const;

    void createVertexShader(const std::string& path);
    void createFragmentShader(const std::string& path);
    /*void createGeometryShader(const std::string& path);
    void createTessControlShader(const std::string& path);
    void createTessEvaluationShader(const std::string& path);*/

    void link() const;
    void use() const;
    void unuse() const;

private:
    GLuint programId;
    GLuint vertexId;
    GLuint fragmentId;

    GLint findUniform(const std::string& name) const;
    GLuint createShader(const std::string& shaderCode, GLuint shaderType) const;
    static std::string ReadFile(const std::string& path);
};
