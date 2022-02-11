#include "glew.h"
#include "freeglut.h"
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Camera.h"
#include "Texture.h"
#include <random>

GLuint programColor;
GLuint programTexture;
GLuint programTexture2;
GLuint textureFish;
GLuint textureShip;
GLuint texturePlant;
GLuint textureTerrain;
GLuint textureChest;
GLuint textureFishV2;
GLuint textureUnderwater;
GLuint textureBubble;

Core::Shader_Loader shaderLoader;

obj::Model shipModel;
obj::Model fishModel;
obj::Model plantModel;
obj::Model terrainModel;
obj::Model chestModel;
obj::Model fishV2Model;
obj::Model sphereModel2;
obj::Model sphereModel;
float cameraMove = 1;
float cameraAngle = 0;
float cameraRotate = 0;
glm::vec3 cameraPos = glm::vec3(-100, 0, 0);
glm::vec3 cameraDir;

glm::mat4 cameraMatrix, perspectiveMatrix;

glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -0.9f, -1.0f));
glm::vec3 skyColor = glm::vec3(0.0f, 0.0f, 0.45f); //kolor mg³y
float fogDensity = 0.004; 
float fogGradient = 1.5;
int bubbleStartingCords[501][3];
glm::vec3 fishXYZ;
glm::vec3 terrainXYZ;
glm::vec3 chestXYZ;
glm::vec3 plantXYZ;
glm::vec3 plant2XYZ;
glm::vec3 plant3XYZ;
glm::vec3 plant4XYZ;
glm::vec3 plant5XYZ;
glm::vec3 plant6XYZ;
glm::vec3 plant7XYZ;
glm::vec3 plant8XYZ;
glm::vec3 fish2XYZ;
glm::vec3 fishV2XYZ;

int a = 0;
int i = 0;
int bubble = 0;
float c = 0;
float b = 0;

void keyboard(unsigned char key, int x, int y)
{
	float angleSpeed = 0.1f;
	float moveSpeed = 0.8f;
	float time = glutGet(GLUT_ELAPSED_TIME) / 5000.0f;
	switch (key)
	{
		case 'z': cameraAngle -= angleSpeed; break; //obrot w lewo
		case 'x': cameraAngle += angleSpeed; break; //obrot w prawo
		case 'q': cameraPos -= angleSpeed; break; //dziala tak samo jak p/o
		case 'e': cameraPos += angleSpeed; break;
		case 'w': cameraPos += cameraDir * moveSpeed; break; //lot do przodu
		case 's': cameraPos -= cameraDir * moveSpeed; break; //lot do tylu
		case 'd': cameraPos += glm::cross(cameraDir, glm::vec3(0, 1, 0)) * moveSpeed; break; //lot w lewo
		case 'a': cameraPos -= glm::cross(cameraDir, glm::vec3(0, 1, 0)) * moveSpeed; break; //lot w prawo
		case 'p': cameraPos += glm::cross(cameraDir, glm::vec3(1, 0, 1)) * moveSpeed; break; //lot w dol
		case 'o': cameraPos -= glm::cross(cameraDir, glm::vec3(1, 0, 1)) * moveSpeed; break; //lot w gore 
		case 't': cameraMove -= 0.1; break; //lot do tylu bez kamery
		case 'y': cameraMove += 0.1; break; //lot do przodu bez kamery
		
	}
}

glm::mat4 createCameraMatrix()
{
	// Obliczanie kierunku patrzenia kamery (w plaszczyznie x-z) przy uzyciu zmiennej cameraAngle kontrolowanej przez klawisze.
	cameraDir = glm::vec3(cosf(cameraAngle), 0.0f, sinf(cameraAngle));
	glm::vec3 up = glm::vec3(0, 1, 0);

	return Core::createViewMatrix(cameraPos, cameraDir, up);
}

void drawObjectColor(obj::Model * model, glm::mat4 modelMatrix, glm::vec3 color)
{
	GLuint program = programColor;

	glUseProgram(program);

	glUniform1f(glGetUniformLocation(program, "fogDensity"), fogDensity);
	glUniform1f(glGetUniformLocation(program, "fogGradient"), fogGradient);

	glUniform3f(glGetUniformLocation(program, "objectColor"), color.x, color.y, color.z);
	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
	glUniform3f(glGetUniformLocation(program, "skyColor"), skyColor.x, skyColor.y, skyColor.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "perspectiveMatrix"), 1, GL_FALSE, (float*)&perspectiveMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}

void drawObjectTexture(obj::Model * model, glm::mat4 modelMatrix, GLuint tex)
{
	GLuint program = programTexture;

	glUseProgram(program);

	glUniform1f(glGetUniformLocation(program, "fogDensity"), fogDensity);
	glUniform1f(glGetUniformLocation(program, "fogGradient"), fogGradient);

	Core::SetActiveTexture(tex, "uni", program, 0);
													
	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
	glUniform3f(glGetUniformLocation(program, "skyColor"), skyColor.x, skyColor.y, skyColor.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "perspectiveMatrix"), 1, GL_FALSE, (float*)&perspectiveMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}
void drawObjectTexture2(obj::Model * model, glm::mat4 modelMatrix, GLuint tex) //do skyboxa
{
	GLuint program = programTexture2;

	glUseProgram(program);

	Core::SetActiveTexture(tex, "uni", program, 0);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}

void renderScene()
{
	float time = glutGet(GLUT_ELAPSED_TIME) / 5000.0f;

	fishXYZ = glm::vec3(cos(time * 5) * 80, 0, sin(time * 5) * 80);
	plantXYZ = glm::vec3(80,-250,80);
	plant2XYZ = glm::vec3(230, -250, 230);
	plant3XYZ = glm::vec3(-230, -250, 230);
	plant4XYZ = glm::vec3(100, -250, 170);
	plant5XYZ = glm::vec3(10, -250, 10);
	plant6XYZ = glm::vec3(-230, -250, 330);
	plant7XYZ = glm::vec3(150, -250, -200);
	plant8XYZ = glm::vec3(-50, -250, -400);
	terrainXYZ = glm::vec3(100, -200, 100);
	chestXYZ = glm::vec3(100, -250, 100);
	fish2XYZ = glm::vec3((cos(0.5f + time * 3) * 80) + 200, 30, (sin(0.5f + time * 3) * 80) + 10);
	fishV2XYZ = glm::vec3(cos(time * 5) * 40 + 70, 0, sin(time * 5) * 40 - 70);


	// Aktualizacja macierzy widoku i rzutowania
	cameraMatrix = createCameraMatrix();
	perspectiveMatrix = Core::createPerspectiveMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(skyColor.x, skyColor.y, skyColor.z, 1.0f); //kolor mg³y
	
	// przyczepienie kamery do statku
	glm::mat4 shipModelMatrix = glm::translate(cameraPos + cameraMove*cameraDir * 0.5f + glm::vec3(0, -0.25f, 0)) * glm::rotate(-cameraAngle + cameraRotate + glm::radians(90.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.0008f));
	
	drawObjectTexture(&shipModel, shipModelMatrix * glm::rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(-90.0f), glm::vec3(0, 0, 1)) * glm::scale(glm::vec3(0.5f)), textureShip);
	
	
	bubble++;

	glm::mat4 FishModelMatrix = glm::translate(fishXYZ) * glm::rotate((-time * 5), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(5.0f));
	glm::mat4 Fish2ModelMatrix = glm::translate(fish2XYZ) * glm::rotate((-time * 3), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(3.0f));
	glm::mat4 FishV2ModelMatrix = glm::translate(fishV2XYZ) * glm::rotate((-time * 5), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0))* glm::scale(glm::vec3(3.0f));
	glm::mat4 UnderwaterModelMatrix = glm::translate(glm::vec3(0,0,0)) * glm::scale(glm::vec3(500.0f));
	glm::mat4 PlantModelMatrix = glm::translate(plantXYZ) * glm::rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0)) *glm::scale(glm::vec3(0.5f));
	glm::mat4 Plant2ModelMatrix = glm::translate(plant2XYZ) * glm::rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(0.3f));
	glm::mat4 Plant3ModelMatrix = glm::translate(plant3XYZ) * glm::rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(0.6f));
	glm::mat4 Plant4ModelMatrix = glm::translate(plant4XYZ) * glm::rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(0.2f));
	glm::mat4 Plant5ModelMatrix = glm::translate(plant5XYZ) * glm::rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(0.5f));
	glm::mat4 Plant6ModelMatrix = glm::translate(plant6XYZ) * glm::rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(0.6f));
	glm::mat4 Plant7ModelMatrix = glm::translate(plant7XYZ) * glm::rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(0.2f));
	glm::mat4 Plant8ModelMatrix = glm::translate(plant8XYZ) * glm::rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(0.5f));
	glm::mat4 TerrainModelMatrix = glm::translate(terrainXYZ) * glm::rotate(glm::radians(90.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(5000.0f));
	glm::mat4 ChestModelMatrix = glm::translate(chestXYZ) * glm::rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(0.5f));
	//bubble
	for (int z = 0; z < 500; z++) {
		int z1 = bubbleStartingCords[0][z];
		int z2 = bubbleStartingCords[1][z];
		int z3 = bubbleStartingCords[2][z];
		drawObjectTexture(&sphereModel2, glm::translate(glm::vec3(z1,(-300 + z2 + bubble)%1000,z3)) * glm::scale(glm::vec3(1.0f)), textureBubble);
	}
  
	drawObjectTexture(&fishModel, FishModelMatrix, textureFish);
	drawObjectTexture(&plantModel, PlantModelMatrix, texturePlant);
	drawObjectTexture(&plantModel, Plant2ModelMatrix, texturePlant);
	drawObjectTexture(&plantModel, Plant3ModelMatrix, texturePlant);
	drawObjectTexture(&plantModel, Plant4ModelMatrix, texturePlant);
	drawObjectTexture(&plantModel, Plant5ModelMatrix, texturePlant);
	drawObjectTexture(&plantModel, Plant6ModelMatrix, texturePlant);
	drawObjectTexture(&plantModel, Plant7ModelMatrix, texturePlant);
	drawObjectTexture(&plantModel, Plant8ModelMatrix, texturePlant);
	drawObjectTexture(&terrainModel, TerrainModelMatrix, textureTerrain);
	drawObjectTexture(&chestModel, ChestModelMatrix, textureChest);
	drawObjectTexture(&fishModel, Fish2ModelMatrix, textureFish);
	drawObjectTexture(&fishV2Model, FishV2ModelMatrix, textureFishV2);
	drawObjectTexture2(&sphereModel2, UnderwaterModelMatrix, textureUnderwater);

	glutSwapBuffers();
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	programColor = shaderLoader.CreateProgram("shaders/shader_color.vert", "shaders/shader_color.frag");
	programTexture = shaderLoader.CreateProgram("shaders/shader_tex.vert", "shaders/shader_tex.frag");
	programTexture2 = shaderLoader.CreateProgram("shaders/shader_tex2.vert", "shaders/shader_tex2.frag");
	fishModel = obj::loadModelFromFile("models/fish.obj");
	plantModel = obj::loadModelFromFile("models/plant.obj");
	terrainModel = obj::loadModelFromFile("models/terrain.obj");
	chestModel = obj::loadModelFromFile("models/chest.obj");
	fishV2Model = obj::loadModelFromFile("models/fish2.obj");
	sphereModel = obj::loadModelFromFile("models/sphere.obj");
	sphereModel2 = obj::loadModelFromFile("models/sphere2.obj");
	shipModel = obj::loadModelFromFile("models/ship.obj");
	textureFish = Core::LoadTexture("textures/fish_texture.png");
	textureShip = Core::LoadTexture("textures/ship.png");
	texturePlant = Core::LoadTexture("textures/plant.png");
	textureTerrain = Core::LoadTexture("textures/terrain.png");
	textureChest = Core::LoadTexture("textures/chest.png");
	textureFishV2 = Core::LoadTexture("textures/fish2.png");
	textureUnderwater = Core::LoadTexture("textures/underwater.png");
	textureBubble = Core::LoadTexture("textures/babul.png");
}

void shutdown()
{
	shaderLoader.DeleteProgram(programColor);
	shaderLoader.DeleteProgram(programTexture);
}

void idle()
{
	glutPostRedisplay();
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(00, 00);
	glutInitWindowSize(900, 900);
	glutCreateWindow("OpenGL");
	glewInit();

	init();
	glutKeyboardFunc(keyboard); // nasluchuje na klawisze

	
	// generowanie poczatkowych cordow bubli
	for (int z = 0; z < 500; z++) {
		int sign = 0;
		int signHelper = rand() % 2;
		if (signHelper == 0) { //losowanie dodatnia/ujemna
			sign = 1;
		}
		else {
			sign = -1;
		}
		bubbleStartingCords[0][z] = (rand() % 1001) * sign;
		bubbleStartingCords[1][z] = rand() % 1001;
		bubbleStartingCords[2][z] = (rand() % 1001) * sign;
	}
	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);

	glutMainLoop();

	shutdown();

	return 0;
}
