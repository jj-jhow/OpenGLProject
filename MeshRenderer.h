#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>
#include <btBulletDynamicsCommon.h>

#include "Dependencies/glm/glm/glm.hpp"
#include "Dependencies/glm/glm/gtc/matrix_transform.hpp"
#include "Dependencies/glm/glm/gtc/type_ptr.hpp"

#include "Camera.h"
#include "LightRenderer.h"

class MeshRenderer
{
public:
	MeshRenderer(MeshType _meshType, Camera* _camera, btRigidBody* _rigidBody, std::string _name, LightRenderer* _light, float _specularStrength, float _ambientStrength);
	~MeshRenderer();
	
	void Draw() ;

	void SetupLighting();

	void setPosition(glm::vec3 _position);
	void setScale(glm::vec3 _scale);
	void setProgram(GLuint _program);
	void setTexture(GLuint _textureID);

	std::string getName() const;

	btRigidBody* getRigidBody() const;

private:
	float ambientStrength;
	float specularStrength;

	std::string name;

	glm::vec3 scale;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	glm::vec3 position;

	GLuint vbo; // Vertex Buffer Object
	GLuint ebo; // Element Buffer Object;
	GLuint vao; // Vertex Array Object
	GLuint program;
	GLuint texture;

	Camera* camera;

	btRigidBody* rigidBody;

	LightRenderer* light;

	void HandleMeshType(MeshType _meshType);
	void HandleGLSetup();
	void SetupModelViewProjectionMatrix();
};

