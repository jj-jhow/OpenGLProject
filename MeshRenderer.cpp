#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(MeshType _meshType, Camera* _camera, btRigidBody* _rigidBody, std::string _name, LightRenderer* _light, float _specularStrength, float _ambientStrength)
{
	this->ambientStrength = _ambientStrength;
	this->specularStrength = _specularStrength;
	this->name = _name;
	this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	this->position = glm::vec3(0.0, 0.0, 0.0);

	this->ebo = 0;
	this->vbo = 0;
	this->vao = 0;
	this->program = 0;
	this->texture = 0;

	this->camera = _camera;
	this->rigidBody = _rigidBody;
	this->light = _light;

	HandleMeshType(_meshType);
	HandleGLSetup();

	return;
}

MeshRenderer::~MeshRenderer()
{
	return;
}

void MeshRenderer::Draw()
{
	glUseProgram(program);

	SetupModelViewProjectionMatrix();

	// Set Texture
	glBindTexture(GL_TEXTURE_2D, texture);

	SetupLighting();

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	// Unbinds
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	return;
}

void MeshRenderer::SetupLighting()
{
	// Set Lighting
	GLuint cameraPosLoc = glGetUniformLocation(program, "cameraPos");
	glUniform3f(cameraPosLoc, camera->GetCameraPosition().x, camera->GetCameraPosition().y, camera->GetCameraPosition().z);

	GLuint lightPosLoc = glGetUniformLocation(program, "lightPos");
	glUniform3f(lightPosLoc, this->light->getPosition().x, this->light->getPosition().y, this->light->getPosition().z);

	GLuint lightColorLoc = glGetUniformLocation(program, "lightColor");
	glUniform3f(lightColorLoc, this->light->getColor().x, this->light->getColor().y, this->light->getColor().z);

	GLuint specularStrengthLoc = glGetUniformLocation(program, "specularStrength");
	glUniform1f(specularStrengthLoc, specularStrength);

	GLuint ambientStrengthLoc = glGetUniformLocation(program, "ambientStrength");
	glUniform1f(ambientStrengthLoc, ambientStrength);

	return;
}

// Setters

void MeshRenderer::setScale(glm::vec3 _scale)
{
	this->scale = _scale;
	return;
}

void MeshRenderer::setTexture(GLuint _texture)
{
	this->texture = _texture;
	return;
}

void MeshRenderer::setPosition(glm::vec3 _position) {

	this->position = _position;
	return;
}

void MeshRenderer::setProgram(GLuint _program) {

	this->program = _program;
	return;
}

// Getters

std::string MeshRenderer::getName() const
{
	return name;
}

btRigidBody* MeshRenderer::getRigidBody() const
{
	return rigidBody;
}

// Private //

void MeshRenderer::SetupModelViewProjectionMatrix()
{
	// Rigid Body Transform
	btTransform t;
	rigidBody->getMotionState()->getWorldTransform(t);
	const btQuaternion kRotation = t.getRotation();
	const btVector3 kTranslate = t.getOrigin();

	const glm::mat4 kRotationMatrix = glm::rotate(
		glm::mat4(1.0f),
		kRotation.getAngle(),
		glm::vec3(
			kRotation.getAxis().getX(),
			kRotation.getAxis().getY(),
			kRotation.getAxis().getZ()
		)
	);

	const glm::mat4 kTranslationMatrix = glm::translate(
		glm::mat4(1.0f),
		glm::vec3(
			kTranslate.getX(),
			kTranslate.getY(),
			kTranslate.getZ()
		)
	);

	const glm::mat4 kScaleMatrix = glm::scale(glm::mat4(1.0f), scale);


	// Transform Matrix
	//const glm::mat4 kModel = glm::translate(glm::mat4(1.0), position);
	const glm::mat4 kModel = kTranslationMatrix * kRotationMatrix * kScaleMatrix;
	GLint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(kModel));

	const glm::mat4 kVP = camera->GetProjectionMatrix() * camera->GetViewMatrix();
	GLint vpLoc = glGetUniformLocation(program, "vp");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(kVP));

	return;
}

void MeshRenderer::HandleMeshType(MeshType _meshType)
{
	switch (_meshType) {
	case kTriangle:
		Mesh::SetTriangleData(vertices, indices);
		break;
	case kQuad:
		Mesh::SetQuadData(vertices, indices);
		break;
	case kCube:
		Mesh::SetCubeData(vertices, indices);
		break;
	case kSphere:
		Mesh::SetSphereData(vertices, indices);
		break;
	}
}

void MeshRenderer::HandleGLSetup()
{

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Vertex::texture_coordinate)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Vertex::normal)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
