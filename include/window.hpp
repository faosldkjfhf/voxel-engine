#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Window {
public:
  Window(int width, int height, const char *title);
  ~Window();

  GLFWwindow *getWindow() const { return _window; }

private:
  int _width, _height;
  const char *_title;

  GLFWwindow *_window = nullptr;
};
