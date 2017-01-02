#include "Terrain.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <gl_core_4_4.h>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

// Default contstructor will accept a 64x64 grid by default
Terrain::Terrain()
{
	genGrid(64, 64);
	setupShaders();
	SetupPerlinData();
}

// Delete shader Program once done
Terrain::~Terrain()
{
	glDeleteProgram(m_program);
}

void Terrain::genGrid(int a_rows, int a_col)
{
	// Dimensions
	int row = a_rows;
	int col = a_col;

	// Serup vertices
	Vertex* vertices = new Vertex[row*col];

	// Position each verticie on the cartesian Graph
	for (int r = 0; r < row; ++r)
		for (int c = 0; c < col; ++c)
		{
			vertices[r*col + c].position = vec4(c - col*0.5f, 0, r - row*0.5f, 1);

			vertices[r*col + c].texCoord = vec2(c*(1.0f / col), r*(1.f / row)); // set per vertex
		}

	// Count of vertices
	indexCount = (row - 1)*(col - 1) * 6;

	// Setup Indicies (Index) 
	unsigned int* indicies = new unsigned int[indexCount];

	unsigned int index = 0;
	for (int r = 0; r < (row - 1); ++r)
		for (int c = 0; c < (col - 1); ++c)
		{
			// Essentially getting the quad data 
			//triangle 1
			indicies[index++] = r*col + c;
			indicies[index++] = (r + 1)*col + c;
			indicies[index++] = (r + 1)*col + (c + 1);

			//Triangle 2
			indicies[index++] = r * col + c;
			indicies[index++] = (r + 1) * col + (c + 1);
			indicies[index++] = r * col + (c + 1);
		}

	// Generate Buffers
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	// Generate Vertex Array
	glGenVertexArrays(1, &m_vao);

	// Apply Data here
	glBindVertexArray(m_vao);

	// Bind Vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, (row * col) * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// Bind Indx Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indicies, GL_STATIC_DRAW);

	// Postion
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// TexCoords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(vec4));

	//safety
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Cleanup

	delete[] vertices;
	delete[] indicies;
}

void Terrain::setupShaders()
{
	const char* vsSource = "#version 430\n \
							layout(location=0) in vec4 Position; \
							layout(location=1) in vec2 TexCoord; \
							out vec2 vTexCoord; \
							uniform mat4 ProjectionView; \
							uniform sampler2D perlinTexture; \
							void main() { \
							vec4 pos = Position; \
							pos.y += texture(perlinTexture, TexCoord).r * 10; \
							vTexCoord = TexCoord; \
							gl_Position = ProjectionView * pos;}";

	const char* fsSource = "#version 430\n \
							in vec2 vTexCoord; \
							out vec4 FragColor; \
							uniform sampler2D noiseTexture; \
							void main() { \
							FragColor = texture(noiseTexture, vTexCoord).rrrr; \
							FragColor.a =1; }";


	int success = GL_FALSE; // Check if shader is compiled

	//Create and compile shaders
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);

	//Create Shader Program
	m_program = glCreateProgram();
	glAttachShader(m_program, vertexShader);
	glAttachShader(m_program, fragmentShader);
	glLinkProgram(m_program);

	// The following section is for debugging
	/// Check if the shaders have linked properly or not
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infolog = new char[infoLogLength];

		glGetProgramInfoLog(m_program, infoLogLength, 0, infolog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infolog);
		delete[] infolog;
	}

	//Clean up shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Terrain::SetupPerlinData()
{
	// Dimensions of the data
	m_perlinDim = 64;

	// Float array of differnet Perlin Values
	m_perlinData = new float[m_perlinDim * m_perlinDim];

	// Scale the point down
	// Do this because Perlin Noise always returns 0 on integer boundaries.
	float scale = (1.0f / m_perlinDim) * 3; // Control Perlin Noise

	int octaves = 6;

	for (unsigned int x = 0; x < m_perlinDim; ++x) {
		for (unsigned int y = 0; y < m_perlinDim; ++y)
		{
			float amplitude = 1.0f;
			float persistence = 0.3f;

			m_perlinData[y* m_perlinDim + x] = 0;

			for (int o = 0; o < octaves; ++o)
			{
				float freq = powf(5, (float)o);
				// Perlion noise funtion returns a calue between -1 and 1
				// We want a number between 0 and 1 for our height map
				// We do this by multiplying 0.5 and then add 0.5
				float perlin_sample = glm::perlin(vec2((float)x, (float)y) * scale * freq) * 0.5f + 0.5f;
				m_perlinData[y * m_perlinDim + x] += perlin_sample * amplitude;
				amplitude *= persistence;
			}

			if (m_perlinData[x * m_perlinDim + y] > m_minMaxHeight.y)
				m_minMaxHeight.y = m_perlinData[x * m_perlinDim + y];
			if (m_perlinData[x * m_perlinDim + y] < m_minMaxHeight.x)
				m_minMaxHeight.x = m_perlinData[x * m_perlinDim + y];
		}
	}

	// Create Perlin Texture from the Data Above
	glGenTextures(1, &m_perlinTexture);
	glBindTexture(GL_TEXTURE_2D, m_perlinTexture);

	//Bind data as float for a single channel
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 64, 64, 0, GL_RED, GL_FLOAT, m_perlinData);

	//enable blending else samples must be "exact" centre of texels
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set wrap to stop errors at edge of noise sampling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// unbind Texture
	//glBindTexture(GL_TEXTURE_2D, 0);
}

void Terrain::update(float delatTime)
{
}

void Terrain::render(Camera* a_camera)
{
	// Selected Shader program to use
	glUseProgram(m_program);

	int matUniform = glGetUniformLocation(m_program, "ProjectionView");
	glUniformMatrix4fv(matUniform, 1, GL_FALSE, &(a_camera->getProjectionView()[0][0]));

	int texUniform = glGetUniformLocation(m_program, "noiseTexture");

	glUniform1i(texUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_perlinTexture);

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}
