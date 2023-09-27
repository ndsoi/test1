#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2\soil2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "ImportedModel.h"
#include "Utils.h"
#include "Transform.h"
using namespace std;

#define numVAOs 1
#define numVBOs 3

float cameraX, cameraY, cameraZ;
float objLocX, objLocY, objLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint shuttleTexture;

// variable allocation for display
GLuint mvLoc, projLoc,nLoc;

GLuint globalAmbLoc, ambLoc, difLoc, specLoc, posLoc, mAmbLoc, mDiffLoc, mSpecLoc, mShiLoc;

int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat,invTrMat;

glm::vec3 currentLightPos, lightPosV;

float lightPos[3];

glm::vec3 initialLightLoc = glm::vec3(5.0f, 2.0f, 2.0f);

//白光特性


float radx, rady, radz;

ImportedModel myModel("./mode/fandisk.obj");

float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void setupVertices(void) {
 	std::vector<glm::vec3> vert = myModel.getVertices();
	//std::vector<glm::vec2> tex = myModel.getTextureCoords();
	//std::vector<glm::vec3> norm = myModel.getNormals();

	std::vector<float> pvalues;
	//std::vector<float> tvalues;
	//std::vector<float> nvalues;

	for (int i = 0; i < myModel.getNumVertices(); i++) {
		pvalues.push_back((vert[i]).x);
		pvalues.push_back((vert[i]).y);
		pvalues.push_back((vert[i]).z);
		//tvalues.push_back((tex[i]).s);
		//tvalues.push_back((tex[i]).t);
		//nvalues.push_back((norm[i]).x);
		//nvalues.push_back((norm[i]).y);
		//nvalues.push_back((norm[i]).z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	//glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
	//设置好相机、物体的位置
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 1.5f;
	objLocX = 0.0f; objLocY = 0.0f; objLocZ = 0.0f;

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

	setupVertices();
	//shuttleTexture = Utils::loadTexture("spstob_1.jpg");

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(objLocX, objLocY, objLocZ));


	//旋转
	radx = 0.0f;
	rady = 135.0f;
	radz = 35.0f;
	mMat = glm::rotate(mMat, toRadians(radx), glm::vec3(1.0f, 0.0f, 0.0f));
	mMat = glm::rotate(mMat, toRadians(rady), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::rotate(mMat, toRadians(radz), glm::vec3(0.0f, 0.0f, 1.0f));
}

void display(GLFWwindow* window, double currentTime) {


	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");		//模式-视图矩阵
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");	//投影矩阵


	mvMat = vMat * mMat;	

	
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));	

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);	//激活第0个顶点的顶点属性

	//glDisable(GL_TEXTURE_2D);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(1);

	//glActiveTexture(GL_TEXTURE0);	//纹理设置  注释掉好似无区别
	//glBindTexture(GL_TEXTURE_2D, shuttleTexture);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, myModel.getNumVertices());
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}


Transform* transform;


bool isrotate = false;
float rad = 2.f;

void ExampleGLFWkeyfun(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		if (isrotate == false)  isrotate = true;
		else isrotate = false;
	}
	
	
	if (isrotate == false)
	{
		//平移处理方式
		if (key == GLFW_KEY_W && action == GLFW_PRESS)
		{
			cameraY += 0.5f;
		}
		else if (key == GLFW_KEY_S && action == GLFW_PRESS)
		{
			cameraY -= 0.5f;
		}
		else if (key == GLFW_KEY_A && action == GLFW_PRESS)
		{
			cameraX += 0.5f;
		}
		else if (key == GLFW_KEY_D && action == GLFW_PRESS)
		{
			cameraX -= 0.5f;
		}
		else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		{
			cameraZ += 0.5f;
		}
		else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		{
			cameraZ -= 0.5f;
		}
		vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
		//cout << "平移中" << "x = " << -cameraX << " y=" << -cameraY << " z=" << -cameraZ << endl;
	}
	else if(isrotate == true)
	{	//旋转
		//cout << "旋转 rad = " << rad;
		if (key == GLFW_KEY_W && action == GLFW_PRESS)
		{
		
			mMat = glm::rotate(mMat, toRadians(rad), glm::vec3(0.0f, 1.0f, 0.0f));
	
		}
		else if (key == GLFW_KEY_S && action == GLFW_PRESS)
		{
		
			mMat = glm::rotate(mMat, toRadians(-rad), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (key == GLFW_KEY_A && action == GLFW_PRESS)
		{
			
			mMat = glm::rotate(mMat, toRadians(rad), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (key == GLFW_KEY_D && action == GLFW_PRESS)
		{
		
			mMat = glm::rotate(mMat, toRadians(-rad), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		{
			
			mMat = glm::rotate(mMat, toRadians(rad), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		{

			mMat = glm::rotate(mMat, toRadians(-rad), glm::vec3(0.0f, 0.0f, 1.0f));
		}
	}


	

}



float factor = 1;





int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }	//加载glfw库
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);	//窗口版本设置
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(800, 800, "Chapter6 - program1", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	//窗口大小发生改变时的响应
	glfwSetWindowSizeCallback(window, window_size_callback);
	// 创建 Transform 对象并进行初始化
	//transform = new Transform(window);

	// 设置回调函数

	glfwSetKeyCallback(window, ExampleGLFWkeyfun);

	init(window);

	


	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}