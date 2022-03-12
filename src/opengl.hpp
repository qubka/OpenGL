#pragma once

/// https://indiegamedev.net/2020/01/17/c-opengl-function-call-wrapping/

template<typename FuncPtrType>
bool check_gl_errors(FuncPtrType& lastFunc, const std::string& filename, const uint_fast32_t line) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n";
        switch (error) {
            case GL_INVALID_ENUM:
                std::cerr << "GL_INVALID_ENUM: an invalid enum value was passed to an OpenGL function";
                break;
            case GL_INVALID_VALUE:
                std::cerr << "GL_INVALID_VALUE: an invalid value was passed to an OpenGL function";
                break;
            case GL_INVALID_OPERATION:
                std::cerr << "GL_INVALID_OPERATION: a bad operation (ID) was passed to an OpenGL function";
                break;
            case GL_STACK_OVERFLOW:
                std::cerr << "GL_STACK_OVERFLOW: an stack overflow was happen during an OpenGL execution";
                break;
            case GL_STACK_UNDERFLOW:
                std::cerr << "GL_STACK_UNDERFLOW: an stack underflow was happen during an OpenGL execution";
                break;
            case GL_OUT_OF_MEMORY:
                std::cerr << "AL_OUT_OF_MEMORY: the requested operation resulted in OpenGL running out of memory";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION: an invalid framebuffer operation was passed to an OpenGL function";
                break;
            case GL_CONTEXT_LOST:
                std::cerr << "GL_CONTEXT_LOST: an context was lost during an OpenGL execution";
                break;
            default:
                std::cerr << "UNKNOWN GL ERROR: " << error;
        }
        std::cerr << std::endl;
        return false;
    }
    return true;
}

template<typename glFunction, typename... Params>
auto glCallImpl(const char* filename,
                const uint_fast32_t line,
                glFunction function,
                Params... params)
-> typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool> {
    function (std::forward<Params>(params)...);
    return check_gl_errors(function, filename, line);
}

template<typename glFunction, typename ReturnType, typename... Params>
auto glCallImpl(const char* filename,
                const uint_fast32_t line,
                glFunction function,
                ReturnType& returnValue,
                Params... params)
-> typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, bool> {
    returnValue = function(std::forward<Params>(params)...);
    return check_gl_errors(function, filename, line);
}

#define glCall(function, ...) glCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)