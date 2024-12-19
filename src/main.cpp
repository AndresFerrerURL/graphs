/*  Base code by Alun Evans 2016 LaSalle (aevanss@salleurl.edu) modified by: Conrado Ruiz, Ferran Ruiz 2024*/

// student name: Andrés Ferrer Camarasalta && David Casanovas

//include some standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

//include OpenGL libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//include some custom code files
#include "glfunctions.h"	//include all OpenGL stuff
#include "Shader.h"			// class to compile shaders



using namespace std;
using namespace glm;

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


const char filename[] = "assets/sphere.obj";
vector <tinyobj::shape_t> shapes;

const char filename2[] = "assets/sphere.obj";
vector <tinyobj::shape_t> shapes2;

//global variables to help us do things
int g_ViewportWidth = 512; int g_ViewportHeight = 512; // Default window size, in pixels
double mouse_x, mouse_y;	//variables storing mouse position
const vec3 g_backgroundColor(0.2f, 0.2f, 0.2f); // background colour - a GLM 3-component vector

GLuint g_simpleShader = 0;	//shader identifier
GLuint g_Vao = 0;			//vao
GLuint g_Vao2 = 0;			//vao
GLuint g_NumTriangles = 0;	 //  Number of triangles we are painting.
GLuint g_NumTriangles2 = 0;


mat4 model = translate(mat4(1.0f), vec3(-0.7, -0.7, 0.0));
mat4 model2 = scale(translate(mat4(1.0f), vec3(2.0, 0.0, 0.0)), vec3(5.0, 5.0, 5.0));

bool usePerspective = true;  // True for perspective, false for orthographic


vec3 cameraPos = vec3(1.0f, 1.0f, 1.0f);
vec3 cameraCenter = vec3(0.0f, 0.0f, -2.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

mat4 view_matrix = glm::lookAt(
	cameraPos, // the position of your camera, in world space
	cameraCenter, // where you want to look at, in world space
	cameraUp // probably glm::vec3(0,1,0)
);

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint texture_id;
GLuint texture2_id;

vec3 g_light_dir(10.0, 10.0, 10.0);










// ------------------------------------------------------------------------------------------
// This function manually creates a square geometry (defined in the array vertices[])
// ------------------------------------------------------------------------------------------
void load()
{
	//**********************
	// CODE TO SET GEOMETRY
	//**********************

	bool ret = tinyobj::LoadObj(shapes, filename);

	if (ret)
		cout << "OBJ File: " << filename << " sucessfully loaded\n";
	else
		cout << "OBJ File: " << filename << " cannot be found or is not a valid OBJ\n";

	bool ret2 = tinyobj::LoadObj(shapes2, filename2);

	if (ret2)
		cout << "OBJ File: " << filename2 << " sucessfully loaded\n";
	else
		cout << "OBJ File: " << filename2 << " cannot be found or is not a valid OBJ\n";


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// textura 1 para la tetera
	char marble_path[] = "textures/marble.bmp";
	int width, height, numChannels;
	stbi_set_flip_vertically_on_load(true); // Invertir la textura si es necesario
	unsigned char* pixels = stbi_load(marble_path, &width, &height, &numChannels, 0);
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	stbi_image_free(pixels);

	// textura 2 para el conejo
	char bunny_texture_path[] = "textures/earth.bmp";
	pixels = stbi_load(bunny_texture_path, &width, &height, &numChannels, 0);
	if (!pixels) {
		std::cerr << "Error: No se pudo cargar la textura del conejo." << std::endl;
		return;
	}
	glGenTextures(1, &texture2_id);
	glBindTexture(GL_TEXTURE_2D, texture2_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	stbi_image_free(pixels);



	//the positions of the corner of the square, each a 3-component vector, x, y, z
	


 	//**********************
	// CODE TO LOAD EVERYTHING INTO MEMORY
	//**********************

	//load the shader
	Shader simpleShader("src/shader.vert", "src/shader.frag");
	g_simpleShader = simpleShader.program;

	// Create the VAO where we store all geometry (stored in g_Vao)
// Crear VAO para la teapot
	g_Vao = gl_createAndBindVAO();

	// Vincular posiciones de los vértices
	gl_createAndBindAttribute(
		&(shapes[0].mesh.positions[0]),
		shapes[0].mesh.positions.size() * sizeof(float),
		g_simpleShader,
		"a_vertex",
		3
	);

	// Vincular coordenadas UV
	gl_createAndBindAttribute(
		&(shapes[0].mesh.texcoords[0]),
		shapes[0].mesh.texcoords.size() * sizeof(float),
		g_simpleShader,
		"a_uv",
		2
	);
	//Create and add a new VBO to VAO, containing the normals 
	gl_createAndBindAttribute(&(shapes[0].mesh.normals[0]),
		shapes[0].mesh.normals.size() * sizeof(float),
		g_simpleShader,
		"a_normal", 3);

	// Vincular índice del buffer
	gl_createIndexBuffer(
		&(shapes[0].mesh.indices[0]),
		shapes[0].mesh.indices.size() * sizeof(unsigned int)
	);

	// Desvincular VAO
	gl_unbindVAO();

	// Almacenar el número de triángulos
	g_NumTriangles = shapes[0].mesh.indices.size() / 3;


	//SEGUNDO ---------------------------------

	// Crear VAO para el conejo (bunny)
	g_Vao2 = gl_createAndBindVAO();

	// Vincular posiciones de los vértices
	gl_createAndBindAttribute(
		&(shapes2[0].mesh.positions[0]),
		shapes2[0].mesh.positions.size() * sizeof(float),
		g_simpleShader,
		"a_vertex",
		3
	);

	// Vincular coordenadas UV
	gl_createAndBindAttribute(
		&(shapes2[0].mesh.texcoords[0]),
		shapes2[0].mesh.texcoords.size() * sizeof(float),
		g_simpleShader,
		"a_uv",
		2
	);

	//Create and add a new VBO to VAO, containing the normals 
	gl_createAndBindAttribute(&(shapes2[0].mesh.normals[0]),
		shapes2[0].mesh.normals.size() * sizeof(float),
		g_simpleShader,
		"a_normal", 3);

	// Vincular índice del buffer
	gl_createIndexBuffer(
		&(shapes2[0].mesh.indices[0]),
		shapes2[0].mesh.indices.size() * sizeof(unsigned int)
	);

	// Desvincular VAO
	gl_unbindVAO();

	// Almacenar el número de triángulos
	g_NumTriangles2 = shapes2[0].mesh.indices.size() / 3;


}

// ------------------------------------------------------------------------------------------
// This function actually draws to screen and called non-stop, in a loop
// ------------------------------------------------------------------------------------------
void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Activar el shader
	glUseProgram(g_simpleShader);

	// Pasar transformaciones al shader
	GLuint model_loc = glGetUniformLocation(g_simpleShader, "u_model");
	GLuint view_loc = glGetUniformLocation(g_simpleShader, "u_view");
	GLuint projection_loc = glGetUniformLocation(g_simpleShader, "u_projection");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));

	GLuint light_loc = glGetUniformLocation(g_simpleShader, "u_light_dir");
	glUniform3f(light_loc, g_light_dir.x, g_light_dir.y, g_light_dir.z);

	// **Pasar los parámetros de iluminación** al shader

	// Iluminación ambiental (ejemplo: color gris oscuro)
	GLuint ambient_loc = glGetUniformLocation(g_simpleShader, "u_ambient");
	glUniform3f(ambient_loc, 0.1f, 0.1f, 0.1f);  // Gris oscuro

	// Iluminación difusa (ejemplo: luz blanca)
	GLuint diffuse_loc = glGetUniformLocation(g_simpleShader, "u_diffuse");
	glUniform3f(diffuse_loc, 1.0f, 1.0f, 1.0f);  // Blanca

	// Iluminación especular (ejemplo: luz blanca)
	GLuint specular_loc = glGetUniformLocation(g_simpleShader, "u_specular");
	glUniform3f(specular_loc, 1.0f, 1.0f, 1.0f); // Blanca

	// Brillo del material (ejemplo: valor 32)
	GLuint shininess_loc = glGetUniformLocation(g_simpleShader, "u_shininess");
	glUniform1f(shininess_loc, 32.0f); // Brillo del material

	// Posición de la cámara (para Blinn-Phong)
	GLuint cam_pos_loc = glGetUniformLocation(g_simpleShader, "u_cam_pos");
	glUniform3f(cam_pos_loc, cameraPos.x, cameraPos.y, cameraPos.z);  // Usamos la posición de la cámara

	// Calcular la Normal Matrix
	mat4 normalMatrix = transpose(inverse(model));  // Calculo de la Normal Matrix
	GLuint normal_matrix_loc = glGetUniformLocation(g_simpleShader, "u_normal_matrix");
	glUniformMatrix4fv(normal_matrix_loc, 1, GL_FALSE, value_ptr(normalMatrix));

	// Seleccionar proyección
	mat4 projection_matrix;
	if (usePerspective) {
		projection_matrix = perspective(45.0f, (float)g_ViewportWidth / g_ViewportHeight, 0.1f, 50.0f);
	}
	else {
		projection_matrix = ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 50.0f);
	}
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	// Ajustar la cámara para centrar la escena
	view_matrix = glm::lookAt(
		vec3(0.0f, 0.0f, 10.0f), // Posición de la cámara
		vec3(0.0f, 0.0f, 0.0f),  // Hacia dónde mira
		vec3(0.0f, 1.0f, 0.0f)   // Vector "arriba"
	);

	// Rotación de la Tierra sobre su propio eje
	static float earth_rotation_angle = 0.0f; // Ángulo de rotación para la Tierra
	earth_rotation_angle += 0.01f;             // Incremento del ángulo para la rotación de la Tierra
	mat4 earth_rotation = rotate(mat4(1.0f), earth_rotation_angle, vec3(0.0f, 1.0f, 0.0f)); // Rotación en Y (auto-rotación)

	// Dibujar la Tierra con rotación sobre su propio eje
	mat4 earth_model = earth_rotation * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f)); // Mantener tamaño de la Tierra
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture2_id);
	GLuint texture_loc = glGetUniformLocation(g_simpleShader, "u_texture");
	glUniform1i(texture_loc, 0);
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(earth_model));
	gl_bindVAO(g_Vao2);
	glDrawElements(GL_TRIANGLES, g_NumTriangles2 * 3, GL_UNSIGNED_INT, 0);

	// Animar y escalar la Luna
	static float moon_angle = 0.0f; // Ángulo de rotación para la órbita de la Luna
	moon_angle += 0.01f;            // Incremento del ángulo para animar la órbita de la Luna

	// Rotación de la Luna en su órbita
	mat4 moon_rotation = rotate(mat4(1.0f), moon_angle, vec3(0.0f, 1.0f, 0.0f)); // Rotación en Y (órbita)
	mat4 moon_translation = translate(mat4(1.0f), vec3(3.0f, 0.0f, 0.0f));      // Posición relativa de la Luna respecto a la Tierra
	mat4 moon_scale = scale(mat4(1.0f), vec3(0.3f, 0.3f, 0.3f));                   // Escalar la Luna para que sea más pequeña

	// Aquí se añade la rotación de la Luna sobre su propio eje (para que siempre tenga una cara estática)
	mat4 self_rotation = rotate(mat4(1.0f), moon_angle * 0.5f, vec3(0.0f, 1.0f, 0.0f));  // Rotación sobre su eje propio

	// Transformación final de la Luna
	mat4 moon_model = earth_model * moon_rotation * moon_translation * moon_scale * self_rotation; // Transformación final de la Luna

	// Dibujar la Luna
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glUniform1i(texture_loc, 0);
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(moon_model));
	gl_bindVAO(g_Vao);
	glDrawElements(GL_TRIANGLES, g_NumTriangles * 3, GL_UNSIGNED_INT, 0);
}






// ------------------------------------------------------------------------------------------
// This function is called every time you press a screen
// ------------------------------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {



	// Toggle between perspective and orthographic projection
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		usePerspective = !usePerspective;  // Toggle the projection mode
		if (usePerspective) {
			cout << "Now on PERSPECTIVE mode\n\n";
		}
		else {
			cout << "Now on ORTOGRAPHIC mode\n\n";
		}
	}

		
	// Move camera UP
	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		cameraPos.y += 0.1f; // Move camera up
		view_matrix = glm::lookAt(cameraPos, cameraCenter, cameraUp); // Recalculate view matrix
	}

	// Move camera DOWN
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		cameraPos.y -= 0.1f; // Move camera down
		view_matrix = glm::lookAt(cameraPos, cameraCenter, cameraUp); // Recalculate view matrix
	}



	//quit
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	    glfwSetWindowShouldClose(window, 1);

	//reload
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	    load();


	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		model = glm::rotate(model, 45.0f, vec3(0.0, 0.0, 1.0));
		model2 = glm::rotate(model2, 45.0f, vec3(0.0, 0.0, 1.0));
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		model = glm::rotate(model, 45.0f, vec3(0.0, 0.0, -1.0));
		model2 = glm::rotate(model2, 45.0f, vec3(0.0, 0.0, -1.0));
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		model = glm::translate(model, vec3(0.0f, 0.0f, -1.0f));
		model2 = glm::translate(model2, vec3(0.0f, 0.0f, -1.0f));
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		model = glm::translate(model, vec3(0.0f, 0.0f, 1.0f));
		model2 = glm::translate(model2, vec3(0.0f, 0.0f, 1.0f));
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		model = glm::translate(model, vec3(-0.1f, 0.0f, 0.0f));
		model2 = glm::translate(model2, vec3(-0.1f, 0.0f, 0.0f));
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		model = glm::translate(model, vec3(0.1f, 0.0f, 0.0f));
		model2 = glm::translate(model2, vec3(0.1f, 0.0f, 0.0f));
	}
	if (key == GLFW_KEY_DOWN) {
		model = glm::scale(model, vec3(0.5, 0.5, 0.5));
		model2 = glm::scale(model2, vec3(0.5, 0.5, 0.5));
	}
	if (key == GLFW_KEY_UP) {
		model = glm::scale(model, vec3(1.5, 1.5, 1.5));
		model2 = glm::scale(model2, vec3(1.5, 1.5, 1.5));
	}


	// TRANSLATE THE 3D mesh when a key is PRESSED


}


// ------------------------------------------------------------------------------------------
// This function is called every time you click the mouse
// ------------------------------------------------------------------------------------------
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		cout << "Left mouse down at" << mouse_x << ", " << mouse_y << endl;
		//ifloat a[]
		//mat4(1.0f) -> Identidad
		//Recuera que asi la rotacion no se guarda, habra que hacer esto en el draw guardando las variables de rotacion, posicion y eso
		
		float normalized_x = (mouse_x / g_ViewportWidth) * 2.0f - 1.0f;
		float normalized_y = 1.0f - (mouse_y / g_ViewportHeight) * 2.0f;
		model = glm::translate(mat4(1.0f), vec3(normalized_x, normalized_y, 0.0f));
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


