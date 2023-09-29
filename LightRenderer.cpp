#include "LightRenderer.h"

LightRenderer::LightRenderer(MeshType _meshType, Camera* _camera)
{
	this->color = glm::vec3(1);
	this->camera = _camera;
	this->position = glm::vec3(0.0, 0.0, 0.0);

	this->ebo = 0;
	this->vbo = 0;
	this->vao = 0;
	this->program = 0;

	HandleMeshType(_meshType);
	HandleGLSetup();

	return;
}

LightRenderer::~LightRenderer()
{
	return;
}

void LightRenderer::Draw()
{
	glUseProgram(program);

	SetupModelViewProjectionMatrix();

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);

	return;
}

void LightRenderer::setColor(glm::vec3 _color)
{
	this->color = _color;
	return;
}

void LightRenderer::setPosition(glm::vec3 _position)
{
	this->position = _position;
	return;
}

void LightRenderer::setProgram(GLuint _program)
{
	this->program = _program;
	return;
}

glm::vec3 LightRenderer::getPosition() const
{
	return position;
}

glm::vec3 LightRenderer::getColor() const
{
	return color;
}

// Private //

void LightRenderer::SetupModelViewProjectionMatrix()
{
	const glm::mat4 kModel = glm::translate(glm::mat4(1.0), position);
	GLint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(kModel));

	const glm::mat4 kView = camera->GetViewMatrix();
	GLint vLoc = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(kView));

	const glm::mat4 kProj = camera->GetProjectionMatrix();
	GLint pLoc = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(kProj));
}

void LightRenderer::HandleMeshType(MeshType _meshType)
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

void LightRenderer::HandleGLSetup()
{

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Vertex::color)));

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
