#include "engine.hpp"
#include "debug.hpp"
#include <iostream>
#include <vulkan/vulkan_core.h>

Engine::Engine() : _window(640, 480, "Window") {}

Engine::Engine(int width, int height) : _window(width, height, "Window") {}

Engine::~Engine() {
  vkDestroyDevice(_device, nullptr);

  if (enableValidationLayers) {
    DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);
  }

  vkDestroyInstance(_instance, nullptr);
}

void Engine::initVulkan() {
  if (enableValidationLayers && !checkValidationLayerSupport()) {
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

  if (enableValidationLayers) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
  } else {
    createInfo.enabledLayerCount = 0;
  }

  auto extensions = getRequiredExtensions();

  createInfo.enabledExtensionCount = extensions.size();
  createInfo.ppEnabledExtensionNames = extensions.data();
  createInfo.enabledLayerCount = 0;

  // create our vulkan instance
  VkResult result = vkCreateInstance(&createInfo, nullptr, &_instance);

  if (result != VK_SUCCESS) {
    std::cout << "Failed to create Vulkan instance" << std::endl;
  }

  setupDebugMessenger();
  pickPhysicalDevice();
  createLogicalDevice();

  glfwCreateWindowSurface(_instance, _window.getWindow(), nullptr, &_surface);
}

void Engine::createLogicalDevice() {
  VkDeviceQueueCreateInfo queueCreateInfo = {};
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.queueFamilyIndex = _indices.graphicsFamily.value();
  queueCreateInfo.queueCount = 1;

  float queuePriority = 1.0f;
  queueCreateInfo.pQueuePriorities = &queuePriority;

  VkPhysicalDeviceFeatures deviceFeatures = {};

  VkDeviceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.pQueueCreateInfos = &queueCreateInfo;
  createInfo.queueCreateInfoCount = 1;
  createInfo.pEnabledFeatures = &deviceFeatures;
  createInfo.enabledExtensionCount = 0;

  if (enableValidationLayers) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(validationLayers.size());
  } else {
    createInfo.enabledLayerCount = 0;
  }

  if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create logical device!");
  }

  vkGetDeviceQueue(_device, _indices.graphicsFamily.value(), 0,
                   &_graphicsQueue);
}

void Engine::pickPhysicalDevice() {
  // get the number of devices
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);

  if (deviceCount == 0) {
    throw std::runtime_error("failed to find GPUs with Vulkan support!");
  }

  // get the physical devices
  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());

  // find a suitable device
  for (const auto &d : devices) {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(d, &properties);

    if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
      _physicalDevice = d;
      break;
    }
  }

  if (_physicalDevice == VK_NULL_HANDLE) {
    throw std::runtime_error("failed to find a suitable GPU!");
  }

  findQueueFamily();
}

void Engine::findQueueFamily() {
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount,
                                           nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount,
                                           queueFamilies.data());

  int i = 0;
  for (const auto &q : queueFamilies) {
    if (q.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      _indices.graphicsFamily = i;
    }

    if (_indices.isComplete()) {
      break;
    }

    i++;
  }
}

void Engine::setupDebugMessenger() {
  if (!enableValidationLayers) {
    return;
  }

  VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = debugCallback;
  createInfo.pUserData = nullptr;

  if (CreateDebugUtilsMessengerEXT(_instance, &createInfo, nullptr,
                                   &_debugMessenger) != VK_SUCCESS) {
    throw std::runtime_error("failed to set up debug messenger!");
  }
}

void Engine::loop() {
  // initialize vulkan
  initVulkan();

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
