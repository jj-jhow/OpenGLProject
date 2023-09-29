#pragma once
#include <vector>
#include "Dependencies/glm/glm/glm.hpp"

enum MeshType
{
	kTriangle = 0,
	kQuad,
	kCube,
	kSphere,
};

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texture_coordinate;

};

class Mesh
{
public:
	static void SetTriangleData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
	static void SetQuadData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
	static void SetCubeData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
	static void SetSphereData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
};

