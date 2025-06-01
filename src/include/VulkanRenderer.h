#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class VulkanRenderer {
public:
	void init();
	void render();
	void cleanup();

private:
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;

	void createInstance();
	void setupDebugMessenger();
};