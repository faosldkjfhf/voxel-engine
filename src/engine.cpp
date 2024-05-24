#include "engine.hpp"
#include "debug.hpp"
#include <iostream>

Engine::Engine() : _window(640, 480, "Window") { initVulkan(); }

Engine::Engine(int width, int height) : _window(width, height, "Window") {
  initVulkan();
}

Engine::~Engine() { vkDestroyInstance(_instance, nullptr); }

void Engine::initVulkan() {
  if (debug::enableValidationLayers && !debug::checkValidationLayerSupport()) {
    std::cout << "Validation layers requested, but not available!" << std::endl;
  }

  // instance setup
  VkApplicationInfo appInfo = {};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Hello Triangle";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;
  createInfo.enabledLayerCount = 0;

  // create our vulkan instance
  VkResult result = vkCreateInstance(&createInfo, nullptr, &_instance);

  if (result != VK_SUCCESS) {
    std::cout << "Failed to create Vulkan instance" << std::endl;
  } else {
    std::cout << "Vulkan instance created successfully" << std::endl;
  }
}

void Engine::loop() {
  while (!glfwWindowShouldClose(_window.getWindow())) {
    input();
    update();
    render();
  }
}

void Engine::input() { glfwPollEvents(); }

void Engine::update() {
  _currentFrame = glfwGetTime();
  _deltaTime = _currentFrame - _lastFrame;
  _lastFrame = _currentFrame;
  _elapsedTime += _deltaTime;

  if (_elapsedTime >= 1.0f) {
    std::cout << "FPS: " << 1.0f / _deltaTime << std::endl;
    _elapsedTime = 0.0f;
  }
}

void Engine::render() { glfwSwapBuffers(_window.getWindow()); }
