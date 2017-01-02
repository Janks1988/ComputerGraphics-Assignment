#ifndef TERRAIN_H
#define TERRAIN_H

#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include "Camera.h"

class Terrain
{
public:
	// Constructors
	Terrain();
	~Terrain();

	struct Vertex
	{
		glm::vec4 position;
		glm::vec2 texCoord;
	};

	void genGrid(int a_rows, int a_col);	// Generate a grid for the domain
	void setupShaders();	// Setup Shaders
	void SetupPerlinData();		//Setup Procedural Generation

	void update(float delatTime);	//Update Function
	void render(Camera* a_camera); // Render Terrain

protected:

	unsigned int m_program; // Shader program
	
	// Object Buffers
	unsigned int m_vbo;
	unsigned int m_vao;
	unsigned int m_ibo;

	// Perlin Data
	int m_perlinDim;
	unsigned int m_perlinTexture;
	unsigned int indexCount;
	float*		 m_perlinData;
	glm::vec2	 m_minMaxHeight;

	// Texture Data
	unsigned int m_texture;
	unsigned int m_texture2;

	// Shaders
	unsigned int vertexShader;
	unsigned int fragmentShader;
};

#endif //TERRAIN_H