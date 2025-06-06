#include "Resources.h"
#include "Engine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#define TEXT_TEXTURE_RESOLUTION 128

Model loadModel(
	Mesh mesh, 
	//Shader shader, 
	glm::mat4 scaleMatrix = glm::mat4(1),
	glm::mat4 translateMatrix = glm::mat4(1)
) {
	Model model = Model::Load(mesh);
	model.transform = translateMatrix * scaleMatrix;
	//model.shader = shader;
	return model;
}

glm::vec3 colorToVector3(Color color) {
	return glm::vec3{
		(float)color.r / 255.0f,
		(float)color.g / 255.0f,
		(float)color.b / 255.0f,
	};
}

inline int getShaderIndex(LitShaderType shaderType) { return static_cast<int>(shaderType); }

#define MATRIX_SCALE(x, y, z) glm::scale(glm::mat4(1), glm::vec3(x, y, z))
#define MATRIX_TRANSLATE(x, y, z) glm::translate(glm::mat4(1), glm::vec3(x, y, z))

void Resources::initialize(VulkanRenderer& vulkan) {
	// Lit Shaders
	Color white = WHITE;
	Color gray		 = Color{ 186, 202, 211, 255 };
	Color lightGreen = Color{ 146, 255, 124, 255 };
	Color darkGreen  = Color{ 5,   181, 119, 255 };
	Color blue		 = Color{ 10,  226, 255, 255 };
	Color darkBlue	 = Color{ 42,  120, 255, 255 };
	Color purple	 = Color{ 202, 103, 255, 255 };
	Color pink		 = Color{ 158, 51,  216, 255 };
	Color red		 = Color{ 255, 110, 112, 255 };
	Color orange	 = Color{ 255, 159, 124, 255 };
	Color yellow	 = Color{ 255, 238, 124, 255 };

	//Shader shaderLit = Shader::Load(
	//	"shaders/lit-vs.glsl",
	//	"shaders/lit-fs.glsl",
	//	{}
	//);

	//Shader shaderGradient = Shader::Load(
	//	"shaders/gradient-vert.glsl",
	//	"shaders/gradient-frag.glsl",
	//	{}
	//);

	//litShaders[getShaderIndex(LitShaderType::Color)] = shaderLit;
	//litShaders[getShaderIndex(LitShaderType::Gradient)] = shaderGradient;

	//shaderGradient.use();
	//shaderGradient.setUniformVec3("color0", colorToVector3(darkBlue));
	//shaderGradient.setUniformVec3("color1", colorToVector3(purple));

	// Models
	Model gridCell = loadModel(vulkan.genCube(white),	MATRIX_SCALE(1, .3, 1), MATRIX_TRANSLATE(0, 0, 0));
	Model level0  = loadModel(vulkan.genCube(gray),		MATRIX_SCALE(.7, .7 * .2, .7), MATRIX_TRANSLATE(0, .7 * .2 * .5, 0));
	Model level1  = loadModel(vulkan.genCube(lightGreen), MATRIX_SCALE(.7, .7 * .2, .7), MATRIX_TRANSLATE(0, .7 * .2 * .5, 0));
	Model level2  = loadModel(vulkan.genCube(darkGreen), MATRIX_SCALE(.7, .7 * .2, .7), MATRIX_TRANSLATE(0, .7 * .2 * .5, 0));
	Model level3  = loadModel(vulkan.genCube(blue),		MATRIX_SCALE(.7, .7 * .2, .7), MATRIX_TRANSLATE(0, .7 * .2 * .5, 0));
	Model level4  = loadModel(vulkan.genCube(darkBlue),	MATRIX_SCALE(.7, .7 * .2, .7), MATRIX_TRANSLATE(0, .7 * .2 * .5, 0));
	Model level5  = loadModel(vulkan.genCube(purple),	MATRIX_SCALE(.7, .7 * .2, .7), MATRIX_TRANSLATE(0, .7 * .2 * .5, 0));
	Model level6  = loadModel(vulkan.genCube(pink),		MATRIX_SCALE(.7, .7 * .2, .7), MATRIX_TRANSLATE(0, .7 * .2 * .5, 0));
	Model level7  = loadModel(vulkan.genCube(red),		MATRIX_SCALE(.7, .7 * .2, .7), MATRIX_TRANSLATE(0, .7 * .2 * .5, 0));
	Model level8  = loadModel(vulkan.genCube(orange),	MATRIX_SCALE(.7, .7 * .2, .7), MATRIX_TRANSLATE(0, .7 * .2 * .5, 0));
	Model level9  = loadModel(vulkan.genCube(yellow),	MATRIX_SCALE(.7, .7 * .2, .7), MATRIX_TRANSLATE(0, .7 * .2 * .5, 0));
	Model level10 = loadModel(vulkan.genCube(),			MATRIX_SCALE(.7, .19 * 11, .7), MATRIX_TRANSLATE(0, .19 * 11 * .5, 0));

	models[0] = gridCell;
	models[1] = level0;
	models[2] = level1;
	models[3] = level2;
	models[4] = level3;
	models[5] = level4;
	models[6] = level5;
	models[7] = level6;
	models[8] = level7;
	models[9] = level8;
	models[10] = level9;
	models[11] = level10;
}

void Resources::unload() {
}