#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>
#include <chrono>

#include "Camera.h"
#include "LightRenderer.h"
#include "MeshRenderer.h"
#include "TextRenderer.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"

bool bIsGrounded;
bool bIsGameOver;

int score;

GLuint flatShaderProgram;
GLuint litTexturedShaderProgram;
GLuint textureShaderProgram;
GLuint textProgram;
GLuint sphereMeshTexture;
GLuint groundMeshTexture;

btDiscreteDynamicsWorld* dynamicsWorld;
btRigidBody* sphereRigidBody;
btRigidBody* groundRigidBody;
btRigidBody* enemyRigidBody;

Camera* camera;
LightRenderer* light;
MeshRenderer* sphereMesh;
MeshRenderer* groundMesh;
MeshRenderer* enemyMesh;
TextRenderer* scoreText;

void AddRigidBodies();
void AddUIText();
void HandleCollisions();
void InitGame();
void InitPhysics();
void RenderScene();
void TickCallback(btDynamicsWorld* dynamicsWorld, btScalar timeStep);
void UpdateKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(int argc, char **argv)
{
	// Init GLFW
	glfwInit();
	
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello OpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, UpdateKeyboard);

	// Init GLEW
	glewInit();

	// Init Game
	InitGame();

	// Start Game Clock
	std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(window)) {
		// Handle Frame Tick
		std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - previousTime).count();
		dynamicsWorld->stepSimulation(dt);
		previousTime = currentTime;

		RenderScene();

		// render our scene
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	delete camera;
	delete light;
	delete sphereMesh;
	delete groundMesh;
	delete enemyMesh;
	delete scoreText;
	delete sphereRigidBody;
	delete groundRigidBody;
	delete enemyRigidBody;
	delete dynamicsWorld;

	return 0;
}

void AddRigidBodies()
{
	// Create Sphere Rigid Body
	btCollisionShape* sphereShape = new btSphereShape(1.0f);
	btDefaultMotionState* sphereMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0.5f, 0)));

	btScalar mass = 13.0f;
	btVector3 sphereInertia(0, 0, 0);
	sphereShape->calculateLocalInertia(mass, sphereInertia);

	btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(mass, sphereMotionState, sphereShape, sphereInertia);
	sphereRigidBody = new btRigidBody(sphereRigidBodyCI);
	sphereRigidBody->setRestitution(0.0f);
	sphereRigidBody->setFriction(1.0f);
	sphereRigidBody->setActivationState(DISABLE_DEACTIVATION);
	dynamicsWorld->addRigidBody(sphereRigidBody);

	// Create Sphere Mesh
	sphereMesh = new MeshRenderer(MeshType::kSphere, camera, sphereRigidBody, "hero", light, 0.1f, 0.5f);
	sphereMesh->setProgram(litTexturedShaderProgram);
	sphereMesh->setTexture(sphereMeshTexture);
	sphereMesh->setScale(glm::vec3(1.0f));
	
	sphereRigidBody->setUserPointer(sphereMesh);

	// Create Ground Rigid Body
	btCollisionShape* groundShape = new btBoxShape(btVector3(4.0f, 0.5f, 4.0f));
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1.0f, 0)));

	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0.0f, groundMotionState, groundShape, btVector3(0, 0, 0));
	groundRigidBody = new btRigidBody(groundRigidBodyCI);
	groundRigidBody->setFriction(1.0f);
	groundRigidBody->setRestitution(0.0f);
	groundRigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);

	dynamicsWorld->addRigidBody(groundRigidBody);

	// Create Ground Mesh
	groundMesh = new MeshRenderer(MeshType::kCube, camera, groundRigidBody, "ground", light, 0.1f, 0.5f);
	groundMesh->setProgram(litTexturedShaderProgram);
	groundMesh->setTexture(groundMeshTexture);
	groundMesh->setScale(glm::vec3(4.0f, 0.5f, 4.0f));

	groundRigidBody->setUserPointer(groundMesh);
	
	// Create Enemy Rigid Body
	btCollisionShape* enemyShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
	btDefaultMotionState* enemyMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(18, 1.0f, 0)));

	btRigidBody::btRigidBodyConstructionInfo enemyRigidBodyCI(0.0f, enemyMotionState, enemyShape, btVector3(0, 0, 0));
	enemyRigidBody = new btRigidBody(enemyRigidBodyCI);
	enemyRigidBody->setFriction(1.0f);
	enemyRigidBody->setRestitution(0.0f);
	//enemyRigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
	enemyRigidBody->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

	dynamicsWorld->addRigidBody(enemyRigidBody);

	// Create Enemy Mesh
	enemyMesh = new MeshRenderer(MeshType::kCube, camera, enemyRigidBody, "enemy", light, 0.1f, 0.5f);
	enemyMesh->setProgram(litTexturedShaderProgram);
	enemyMesh->setTexture(groundMeshTexture);
	enemyMesh->setScale(glm::vec3(1.0f, 1.0f, 1.0f));

	enemyRigidBody->setUserPointer(enemyMesh);

	return;
}

void AddUIText()
{
	// Create Score Text
	scoreText = new TextRenderer("Score: 0", "Assets/fonts/gooddog.ttf", 64, glm::vec3(1.0f, 0.0f, 0.0f), textProgram);
	scoreText->setPosition(glm::vec2(320.0f, 500.0f));

	return;
}

void InitGame()
{
	glEnable(GL_DEPTH_TEST);

	InitPhysics();

	ShaderLoader shader;
	// Create Shaders
	flatShaderProgram = shader.CreateProgram("Assets/Shaders/FlatModel.vs", "Assets/Shaders/FlatModel.fs");
	litTexturedShaderProgram = shader.CreateProgram("Assets/Shaders/LitTexturedModel.vs", "Assets/Shaders/LitTexturedModel.fs");
	textureShaderProgram = shader.CreateProgram("Assets/Shaders/TexturedModel.vs", "Assets/Shaders/TexturedModel.fs");
	textProgram = shader.CreateProgram("Assets/Shaders/text.vs", "Assets/Shaders/text.fs");

	TextureLoader textureLoader;
	// Sphere Texture Loader
	sphereMeshTexture = textureLoader.getTexture("Assets/Textures/tennisBall.jpg");
	// Ground Texture Loader
	groundMeshTexture = textureLoader.getTexture("Assets/Textures/ground.jpg");
	
	camera = new Camera(45.0f, 800, 600, 0.1f, 100.0f, glm::vec3(0.0f, 4.0f, 30.0f));

	light = new LightRenderer(MeshType::kCube, camera);
	light->setProgram(flatShaderProgram);
	light->setPosition(glm::vec3(0.0f, 10.0f, 0.0f));

	AddRigidBodies();
	AddUIText();

	return;
}

void InitPhysics()
{
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();

	// Create Physics
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	// Set Physics Constants
	dynamicsWorld->setGravity(btVector3(0, -9.8f, 0));

	dynamicsWorld->setInternalTickCallback(TickCallback);
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);//clear yellow
	
	// Draw game objects here
	//light->Draw();
	sphereMesh->Draw();
	groundMesh->Draw();
	enemyMesh->Draw();
	
	// Drawn last because of alpha blending
	scoreText->Draw();

	return;
}

void TickCallback(btDynamicsWorld* dynamicsWorld, btScalar timeStep)
{
	if (bIsGameOver)
	{
		return;
	}

	// Get enemy transform
	btTransform enemyTransform(enemyMesh->getRigidBody()->getWorldTransform());
	// Set enemy position
	enemyTransform.setOrigin(enemyTransform.getOrigin() + btVector3(-15, 0, 0) * timeStep);
	// Check if offScreen
	if (enemyTransform.getOrigin().x() <= -18.0f) {
		enemyTransform.setOrigin(btVector3(18, 1, 0));

		scoreText->setText("Score: " + std::to_string(++score));
	}

	enemyMesh->getRigidBody()->setWorldTransform(enemyTransform);
	enemyMesh->getRigidBody()->getMotionState()->setWorldTransform(enemyTransform);

	HandleCollisions();
}

void HandleCollisions()
{
	const int kNumManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();

	for (int i = 0; i < kNumManifolds; i++)
	{
		btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const int kNumContacts = contactManifold->getNumContacts();
		if (kNumContacts > 0) {
			const btCollisionObject* objA = contactManifold->getBody0();
			const btCollisionObject* objB = contactManifold->getBody1();
			MeshRenderer* gModA = (MeshRenderer*)objA->getUserPointer();
			MeshRenderer* gModB = (MeshRenderer*)objB->getUserPointer();
			if (!gModA || !gModB)
			{
				continue;
			}

			// Player got hit
			if (
				(gModA->getName() == "hero" && gModB->getName() == "enemy") ||
				(gModA->getName() == "enemy" && gModB->getName() == "hero")
				)
			{
				//printf("collision: %s with %s \n", gModA->getName(), gModB->getName());

				if (gModB->getName() == "enemy")
				{
					btTransform b(gModB->getRigidBody()->getWorldTransform());
					b.setOrigin(btVector3(18, 1, 0));
					gModB->getRigidBody()->setWorldTransform(b);
					gModB->getRigidBody()->getMotionState()->setWorldTransform(b);
				}
				else {
					btTransform a(gModA->getRigidBody()->getWorldTransform());
					a.setOrigin(btVector3(18, 1, 0));
					gModA->getRigidBody()->setWorldTransform(a);
					gModA->getRigidBody()->getMotionState()->setWorldTransform(a);
				}

				score = 0;
				bIsGameOver = true;
				scoreText->setText("Score: " + std::to_string(score));

			}
			// Player is on the floor
			else if (
				(gModA->getName() == "hero" && gModB->getName() == "ground") ||
				(gModA->getName() == "ground" && gModB->getName() == "hero")
				)
			{
				if (!bIsGrounded)
				{
					bIsGrounded = true;
					//printf("collision: %s with %s \n", gModA->getName(), gModB->getName());
				}
			}
		}
	}
}

void UpdateKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		return;
	}

	if (bIsGameOver)
	{
		if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
		{
			bIsGameOver = false;
			return;
		}
		return;
	}

	if ((key == GLFW_KEY_UP || key == GLFW_KEY_W) && action == GLFW_PRESS)
	{
		if (bIsGrounded)
		{
			bIsGrounded = false;
			sphereMesh->getRigidBody()->applyImpulse(btVector3(0.0f, 100.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
			//printf("pressed up key \n");
		}
	}
}