#include "window.hpp"
#include <iostream>

Window::Window(int width, int height, const char *title)
    : _width(width), _height(height), _title(title) {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  if (!glfwInit()) {
    std::cout << "Failed to initialize GLFW" << std::endl;
  }

  _window = glfwCreateWindow(_width, _height, _title, nullptr, nullptr);

  if (!_window) {
    glfwTerminate();
    std::cout << "Failed to create window" << std::endl;
  }

  glfwMakeContextCurrent(_window);
}

Window::~Window() {
  glfwDestroyWindow(_window);
  glfwTerminate();
}
