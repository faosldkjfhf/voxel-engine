#pragma once

#include "window.hpp"

class Engine {
public:
  Engine();
  Engine(int width, int height);

  ~Engine();

  void initVulkan();
  void loop();
  void input();
  void update();
  void render();

private:
  VkInstance _instance;

  float _deltaTime = 0.0f;
  float _lastFrame = 0.0f;
  float _currentFrame = 0.0f;
  float _elapsedTime = 0.0f;

  Window _window;
};
