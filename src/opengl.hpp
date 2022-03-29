#pragma once

/// https://indiegamedev.net/2020/01/17/c-opengl-function-call-wrapping/

template<typename Error, typename Function, typename... Params>
auto glCallImpl(const char* filename,
              uint32_t line,
              Error error,
              Function function,
              Params... params)
-> typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))> {
    auto ret = function(std::forward<Params>(params)...);
#ifndef NDEBUG
    error(filename, line);
#endif
    return ret;
}

template<typename Error, typename Function, typename... Params>
auto glCallImpl(const char* filename,
              const uint32_t line,
              Error error,
              Function function,
              Params... params)
-> typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool> {
    function(std::forward<Params>(params)...);
#ifndef NDEBUG
    return error(filename, line);
#else
    return true;
#endif
}

bool check_gl_errors(const std::string& filename, uint32_t line);

#define glCall_(function) glCallImpl(__FILE__, __LINE__, check_gl_errors, function)
#define glCall(function, ...) glCallImpl(__FILE__, __LINE__, check_gl_errors, function, __VA_ARGS__)
