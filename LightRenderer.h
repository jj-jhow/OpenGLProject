#pragma once
//#include <vector>

#include <GL/glew.h>
#include "Dependencies/glm/glm/glm.hpp"
#include "Dependencies/glm/glm/gtc/matrix_transform.hpp"
#include "Dependencies/glm/glm/gtc/type_ptr.hpp"

#include "Mesh.h"
#include "Camera.h"

class LightRenderer
{
public:
	LightRenderer(MeshType meshType, Camera* camera);
	~LightRenderer();

	void Draw();

	void setColor(glm::vec3 _color);
	void setPosition(glm::vec3 _position);
	void setProgram(GLuint _program);

	glm::vec3 getPosition() const;
	glm::vec3 getColor() const;

private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	glm::vec3 position;
	glm::vec3 color;

	GLuint vbo; // Vertex Buffer Object
	GLuint ebo; // Element Buffer Object;
	GLuint vao; // Vertex Array Object
	GLuint program;

	Camera* camera;

	virtual void SetupModelViewProjectionMatrix();
	void HandleMeshType(MeshType _meshType);
	void HandleGLSetup();
};

