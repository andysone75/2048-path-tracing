#pragma once

#include "Engine.h"
#include "VulkanRenderer.h"

#include <array>
#include <string>
#include <memory>

using namespace std;

enum class LitShaderType {
    Color,
	Gradient,
};

enum class ModelType {
	GridCell,
	Level0,
	Level1,
	Level2,
	Level3,
	Level4,
	Level5,
	Level6,
	Level7,
	Level8,
	Level9,
};

class Resources {
public:
	void initialize(VulkanRenderer& vulkan);
	void unload();

	inline const Model& getModel(ModelType model) const { return models[static_cast<int>(model)]; }

private:
	array<Model, 12> models;
};