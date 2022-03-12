#include "shader.hpp"
#include "opengl.hpp"

Shader::Shader() : vertexId{0}, fragmentId{0} {
    glCall(glCreateProgram, programId);
}

Shader::~Shader() {
    glCall(glUseProgram, 0);
    if (programId != 0) {
        glCall(glDeleteProgram, programId);
    }
}

void Shader::use() const {
    glCall(glUseProgram, programId);
}

void Shader::unuse() const {
    glCall(glUseProgram, 0);
}

void Shader::link() const {
    glCall(glLinkProgram, programId);

    GLint success;

    glCall(glGetProgramiv, programId, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glCall(glGetShaderInfoLog, programId, 1024, nullptr, infoLog);
        std::cerr << "Error linking Shader code: " << infoLog << std::endl;
    }

    if (vertexId != 0) {
        glCall(glDetachShader, programId, vertexId);
    }
    if (fragmentId != 0) {
        glCall(glDetachShader, programId, fragmentId);
    }

    glCall(glValidateProgram, programId);
    glCall(glGetProgramiv, programId, GL_VALIDATE_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glCall(glGetShaderInfoLog, programId, 1024, nullptr, infoLog);
        std::cerr << "Warning validating Shader code: " << infoLog << std::endl;
    }
}

void Shader::createVertexShader(const std::string& path) {
    vertexId = createShader(ReadFile(path), GL_VERTEX_SHADER);
}

void Shader::createFragmentShader(const std::string& path) {
    fragmentId = createShader(ReadFile(path), GL_FRAGMENT_SHADER);
}

GLuint Shader::createShader(const std::string& shaderCode, GLuint shaderType) const {
    GLuint shaderId;
    if (!glCall(glCreateShader, shaderId, shaderType)) {
        std::cerr << "Error creating shader. Type: " << (shaderType == GL_FRAGMENT_SHADER ? "FRAGMENT" : "VERTEX") << std::endl;
        return 0;
    }

    const GLchar* code = shaderCode.c_str();
    glCall(glShaderSource, shaderId, 1, &code, nullptr);
    glCall(glCompileShader, shaderId);

    GLint success;
    glCall(glGetShaderiv, shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glCall(glGetShaderInfoLog, shaderId, 1024, nullptr, infoLog);
        std::cerr << "Error compiling Shader code: " << infoLog << std::endl;
        return 0;
    }

    glCall(glAttachShader, programId, shaderId);

    return shaderId;
}

void Shader::setUniform(const std::string& name, int value) const {
    glCall(glUniform1i, findUniform(name), value);
}

void Shader::setUniform(const std::string& name, float value) const {
    glCall(glUniform1f, findUniform(name), value);
}

void Shader::setUniform(const std::string& name, const glm::vec2& value) const {
    glCall(glUniform2f, findUniform(name), value.x, value.y);
}

void Shader::setUniform(const std::string& name, const glm::vec3& value) const {
    glCall(glUniform3f, findUniform(name), value.x, value.y, value.z);
}

void Shader::setUniform(const std::string& name, const glm::vec4& value) const {
    glCall(glUniform4f, findUniform(name), value.x, value.y, value.z, value.w);
}

void Shader::setUniform(const std::string& name, const glm::mat2& value) const {
    glCall(glUniformMatrix2fv, findUniform(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, const glm::mat2& value, int count) const {
    glCall(glUniformMatrix2fv, findUniform(name), count, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, const glm::mat3& value) const {
    glCall(glUniformMatrix3fv, findUniform(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, const glm::mat3& value, int count) const {
    glCall(glUniformMatrix3fv, findUniform(name), count, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, const glm::mat4& value) const {
    glCall(glUniformMatrix4fv, findUniform(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, const glm::mat4& value, int count) const {
    glCall(glUniformMatrix4fv, findUniform(name), count, GL_FALSE, glm::value_ptr(value));
}

GLint Shader::findUniform(const std::string& name) const {
    GLint uniformLocation;
    glCall(glGetUniformLocation, uniformLocation, programId, name.c_str());
    if (uniformLocation < 0) {
        std::cerr << "Could not find uniform: " << name << std::endl;
    }
    return uniformLocation;
}

std::string Shader::ReadFile(const std::string& path) {
    std::ifstream file{path};
    if (file) {
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        return stream.str();
    } else {
        std::cerr << "Cannot opened file: " << path << std::endl;
        return "";
    }
}