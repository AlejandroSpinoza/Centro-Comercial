#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	GLfloat getmuevez() { return muevez; }
	GLfloat getluzHeli() { return luzHeli; }
	bool animacionActiva; // Agrega esta línea para la variable de animación

	bool getRightMouseClick() { return rightMouseClick; }
	void setRightMouseClick(bool value) { rightMouseClick = value; }
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	GLfloat getmueveh() { return mueveh; }
	GLboolean getlinterna() { return linterna; }
	GLint getarreglos() { return arreglos; }
	GLfloat getpaletaizq() { return paletaizq; }
	GLfloat getpaletader() { return paletader; }
	GLfloat getpaletacentral() { return paletacentral; }

	GLfloat getruedat() { return ruedat; }
	GLboolean getlampara() { return lampara; }
	GLboolean getcarroX() { return carroX; }
	GLboolean getcarroZ() { return carroZ; }

	GLboolean gethelip() { return helip; }

	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);
	}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }

	~Window();
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	GLfloat muevez;
	GLfloat luzHeli;
	GLfloat mueveh;
	GLboolean linterna;
	GLboolean lampara;
	GLboolean planta;
	GLfloat ruedat = 0.0f;
	GLboolean carroX;
	GLboolean carroZ;

	GLfloat paletaizq = 0.0f;
	GLfloat paletader = 0.0f;
	GLfloat paletacentral = 0.0f;

	GLboolean helip;
	GLint arreglos;
	bool rightMouseClick;

	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};