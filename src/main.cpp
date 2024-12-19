/*  Base code by Alun Evans 2016 LaSalle (aevanss@salleurl.edu) modified by: Conrado Ruiz, Ferran Ruiz 2024*/

// student name: 

//include some standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

//include OpenGL libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//include some custom code files
#include "glfunctions.h"	//include all OpenGL stuff
#include "Shader.h"			// class to compile shaders

using namespace std;
using namespace glm;

//global variables to help us do things
int g_ViewportWidth = 512; int g_ViewportHeight = 512; // Default window size, in pixels
double mouse_x, mouse_y;	//variables storing mouse position
const vec3 g_backgroundColor(0.2f, 0.2f, 0.2f); // background colour - a GLM 3-component vector

GLuint g_simpleShader = 0;	//shader identifier
GLuint g_Vao = 0;			//vao
GLuint g_NumTriangles = 0;	 //  Number of triangles we are painting.

// ------------------------------------------------------------------------------------------
// This function manually creates a square geometry (defined in the array vertices[])
// ------------------------------------------------------------------------------------------
void load()
{
	//**********************
	// CODE TO SET GEOMETRY
	//**********************

	//the positions of the corner of the square, each a 3-component vector, x, y, z
	const GLfloat vertices[] = 
		{	0.5f, -0.5f, 0.0f,		// vertex 0
			0.5f,  0.5f, 0.0f,		// vertex 1
		   -0.5f,  0.5f, 0.0f,		// vertex 2
		   -0.5f, -0.5f, 0.0f  };	// vertex 3

	//we assign a colour to each corner (each colour is RGB)
	const GLfloat colors[] = 
		{	1.0f, 0.0f, 0.0f,		// color of vertex 1 (Red)
			0.f, 1.0f, 0.0f,		// color of vertex 1 (Green)
			0.0f, 0.0f, 1.0f,		// color of vertex 1 (Blue)
			1.0f, 1.0f, 0.0f };		// color of vertex 1 (Yellow)

	// The index buffer references the vertices we paint, in order
	//here we have two triangles
	const GLuint indices[] = 
		{	0, 1, 2,				// triangle 0
			0, 2, 3  };				// triangle 1


 	//**********************
	// CODE TO LOAD EVERYTHING INTO MEMORY
	//**********************

	//load the shader
	Shader simpleShader("src/shader.vert", "src/shader.frag");
	g_simpleShader = simpleShader.program;

	// Create the VAO where we store all geometry (stored in g_Vao)
	g_Vao = gl_createAndBindVAO();
	std::cout << "vao: " << g_Vao;

	//create vertex buffer for positions, colors, and indices, and bind them to shader
	gl_createAndBindAttribute(vertices, sizeof(vertices), g_simpleShader, "a_vertex", 3);
	gl_createAndBindAttribute(colors, sizeof(colors), g_simpleShader, "a_color", 3);
	gl_createIndexBuffer(indices, sizeof(indices));

	//unbind everything
	gl_unbindVAO();

	//store number of triangles (use in draw())
	g_NumTriangles = sizeof(indices) / (sizeof(GLuint) * 3);
}

// ------------------------------------------------------------------------------------------
// This function actually draws to screen and called non-stop, in a loop
// ------------------------------------------------------------------------------------------
void draw()
{
	//clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// activate shader
	glUseProgram(g_simpleShader);

	//bind the geometry
	gl_bindVAO(g_Vao);

	// Draw to screen
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);

}

// ------------------------------------------------------------------------------------------
// This function is called every time you press a screen
// ------------------------------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    //quit
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
	//reload
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
		load();
}

// ------------------------------------------------------------------------------------------
// This function is called every time you click the mouse
// ------------------------------------------------------------------------------------------
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        cout << "Left mouse down at" << mouse_x << ", " << mouse_y << endl;
    }
}

int main(void)
{
	//setup window and boring stuff, defined in glfunctions.cpp
	GLFWwindow* window;
	if (!glfwInit())return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(g_ViewportWidth, g_ViewportHeight, "Hello OpenGL!", NULL, NULL);
	if (!window) {glfwTerminate();	return -1;}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	//input callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	glClearColor(g_backgroundColor.x, g_backgroundColor.y, g_backgroundColor.z, 1.0f);

	//load all the resources
	load();

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
		draw();

        // Swap front and back buffers
        glfwSwapBuffers(window);
        
        // Poll for and process events
        glfwPollEvents();
        
        //mouse position must be tracked constantly (callbacks do not give accurate delta)
        glfwGetCursorPos(window, &mouse_x, &mouse_y);
    }

    //terminate glfw and exit
    glfwTerminate();
    return 0;
}


