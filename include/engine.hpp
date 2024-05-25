#pragma once

#include "window.hpp"
#include <optional>
#include <vulkan/vulkan_core.h>

struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;

  bool isComplete() { return graphicsFamily.has_value(); }
};

class Engine {
public:
  Engine();
  Engine(int width, int height);

  ~Engine();

  // TODO: Abstract out into separate class
  void initVulkan();
  void setupDebugMessenger();
  void pickPhysicalDevice();
  void findQueueFamily();
  void createLogicalDevice();

  void loop();
  void input();
  void update();
  void render();

private:
  VkDebugUtilsMessengerEXT _debugMessenger;
  VkInstance _instance;
  VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
  QueueFamilyIndices _indices;
  VkDevice _device;
  VkQueue _graphicsQueue;

  float _deltaTime = 0.0f;
  float _lastFrame = 0.0f;
  float _currentFrame = 0.0f;
  float _elapsedTime = 0.0f;

  Window _window;
};
