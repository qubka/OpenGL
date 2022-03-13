#include "shader.hpp"
#include "opengl.hpp"

Shader::Shader() : programId{glCall_(glCreateProgram)} {
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

#ifndef NDEBUG
    GLint success;

    glCall(glGetProgramiv, programId, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glCall(glGetShaderInfoLog, programId, 1024, nullptr, infoLog);
        std::cerr << "ERROR: Linking Shader code: " << infoLog << std::endl;
    }
#endif

    if (vertexId != 0) {
        glCall(glDetachShader, programId, vertexId);
    }
    if (fragmentId != 0) {
        glCall(glDetachShader, programId, fragmentId);
    }

    glCall(glValidateProgram, programId);

#ifndef NDEBUG
    glCall(glGetProgramiv, programId, GL_VALIDATE_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glCall(glGetShaderInfoLog, programId, 1024, nullptr, infoLog);
        std::cerr << "ERROR: Validating Shader code: " << infoLog << std::endl;
    }
#endif
}

void Shader::createVertexShader(const std::string& path) {
    vertexId = createShader(ReadFile(path), GL_VERTEX_SHADER);
}

void Shader::createFragmentShader(const std::string& path) {
    fragmentId = createShader(ReadFile(path), GL_FRAGMENT_SHADER);
}

GLuint Shader::createShader(const std::string& shaderCode, GLuint shaderType) const {
    GLuint shaderId = glCall(glCreateShader, shaderType);
    if (!shaderId) {
        std::cerr << "ERROR: creating shader. Type: " << (shaderType == GL_FRAGMENT_SHADER ? "FRAGMENT" : "VERTEX") << std::endl;
        return 0;
    }

    const GLchar* code = shaderCode.c_str();
    glCall(glShaderSource, shaderId, 1, &code, nullptr);
    glCall(glCompileShader, shaderId);

#ifndef NDEBUG
    GLint success;
    glCall(glGetShaderiv, shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glCall(glGetShaderInfoLog, shaderId, 1024, nullptr, infoLog);
        std::cerr << "ERROR: compiling Shader code: " << infoLog << std::endl;
        return 0;
    }
#endif

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
    GLint uniformLocation = glCall(glGetUniformLocation, programId, name.c_str());
    if (uniformLocation < 0) {
        std::cerr << "ERROR: Could not find uniform: " << name << std::endl;
    }
    return uniformLocation;
}

std::string Shader::ReadFile(const std::string& path) {
    assert(std::filesystem::exists(path) && "Could not load file");

    std::ifstream in{path};
    if (!in.is_open()) {
        std::cerr << "ERROR: Cannot opened file: " << path << std::endl;
        return "";
    }

    std::stringstream stream;
    stream << in.rdbuf();
    in.close();
    return stream.str();
}