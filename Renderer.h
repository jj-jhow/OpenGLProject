#pragma once
//#include <vector>

#include <GL/glew.h>
#include "Dependencies/glm/glm/glm.hpp"
#include "Dependencies/glm/glm/gtc/matrix_transform.hpp"
#include "Dependencies/glm/glm/gtc/type_ptr.hpp"

#include "Mesh.h"
#include "Camera.h"

class Renderer
{
public:
	Renderer(MeshType _meshType, Camera* _camera);
	~Renderer();

	virtual void Draw();


	virtual void setPosition(glm::vec3 _position);
	virtual void setProgram(GLuint _program);

	virtual glm::vec3 getPosition() const;
	
protected:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	glm::vec3 position;

	GLuint vbo; // Vertex Buffer Object
	GLuint ebo; // Element Buffer Object;
	GLuint vao; // Vertex Array Object
	GLuint program;

	Camera* camera;

	virtual void SetupModelViewProjectionMatrix();

private:
	void HandleMeshType(MeshType _meshType);
	void HandleGLSetup();

};

