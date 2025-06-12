#pragma once

#include "SceneObject.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

struct CubeData {
	alignas(16) glm::mat4 model;
	alignas(16) glm::vec4 color;
};

class VulkanRenderer {
public:
	bool framebufferResized = false;

	void setup(GLFWwindow* window);
	void init();
	void render(const std::vector<SceneObject>& objects, const Camera& camera);
	void cleanup();

	Mesh genCube(Color color = { 255,255, 255, 255 });

private:
	const int MAX_FRAMES_IN_FLIGHT = 2;
	const int MAX_MODELS = 1024;

	GLFWwindow* window;
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkSurfaceKHR surface;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	VkFormat depthFormat;
	std::vector<VkImage> depthImages;
	std::vector<VkDeviceMemory> depthImagesMemory;
	std::vector<VkImageView> depthImagesViews;
	VkRenderPass renderPass;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	uint32_t currentFrame = 0;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	std::vector<VkBuffer> buffers;
	std::vector<VkDeviceMemory> buffersMemory;

	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	std::vector<std::vector<VkBuffer>> uniformBuffers;
	std::vector<std::vector<VkDeviceMemory>> uniformBuffersMemory;
	std::vector<std::vector<void*>> uniformBuffersMapped;
	std::vector<std::vector<VkDescriptorSet>> descriptorSets;

	std::vector<CubeData> cubes;
	VkDescriptorSetLayout cubesDescriptorSetLayout;
	VkDescriptorPool cubesDescriptorPool;
	std::vector<VkBuffer> cubesBuffers;
	std::vector<VkDescriptorSet> cubesDescriptorSets;
	std::vector<VkDeviceMemory> cubesBuffersMemory;

	void createInstance();
	void setupDebugMessenger();
	void createSurface();
	void pickPhysicalDevice();
	void createLogicalDevice();
	void createSwapChain();
	void createDepthBuffers();
	void createRenderPass();
	void createImageViews();
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffers();
	void createSyncObjects();
	void createDescriptorSetLayout();
	void createGraphicsPipeline();
	void createDescriptorPool();
	void createUniformBuffers();
	void createDescriptorSets();
	void createCubesBuffers();

	void updateUniformBuffer(uint32_t currentImage, const std::vector<SceneObject>& objects, const Camera& camera);
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, const std::vector<SceneObject>& objects);
	void recreateSwapChain();
	void cleanupSwapChain();
	VkShaderModule createShaderModule(const std::vector<char>& code);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};