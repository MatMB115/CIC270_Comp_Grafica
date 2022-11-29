/**
 * 	@file lista2.cpp
 * 	@brief based on triangle.cpp
 *
 * 	Uses the basics for drawing in OpenGL, shaders, arrays, buffers, etc.
 *
 * 	@author Adriano Lucas Ferreira
 *	@author Matheus Martins Batista
 */

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "./libs/utils.h"

/* Globals */
/** Window width. */
int win_width  = 800;
/** Window height. */
int win_height = 600;

/** Program variable. */
int program;
/** Vertex array object. */
unsigned int VAO;
/** Vertex buffer object. */
unsigned int VBO;

/** Element buffer object */
unsigned int EBO;

/** Variavel global num triangulos*/
//int triangles_number = 1;

/** Variavel global tipo primitivo*/
int type_primitive = GL_TRIANGLES;

/** Variavel global tipo poligono*/
int type_polygon = GL_FILL;

/** Translate Y */
float mov_y = 0.0f;

/** Translate X */
float mov_x = 0.0f;

/** Rotation angle. */
float angle = 0.0f;

/** Rotation increment. */
float angle_inc = 0.5f;

/** Rotation decrement. */
float angle_dec = -0.5f;

/** Scale. */
float scale = 0.0;

/** Vertex shader. */
const char *vertex_code = "\n"
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"\n"
"out vec3 vColor;\n"
"\n"
"uniform mat4 transform;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = transform * vec4(position, 1.0);\n"
"    vColor = color;\n"
"}\0";

/** Fragment shader. */
const char *fragment_code = "\n"
"#version 330 core\n"
"\n"
"in vec3 vColor;\n"
"out vec4 FragColor;\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = vec4(vColor, 1.0f);\n"
"}\0";

/* Functions. */
void display(void);
void reshape(int, int);
void keyboard(unsigned char, int, int);
void rotation(int);
void initData(void);
void initShaders(void);

/** 
 * Drawing function.
 *
 * Draws a triangle.
 */
void display()
{
    	glClearColor(1.0, 1.0, 1.0, 1.0);
    	glClear(GL_COLOR_BUFFER_BIT);

    	glUseProgram(program);
    	glBindVertexArray(VAO);

		// Translation. 
		glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f + mov_x,0.0f + mov_y,0.0f));
		// Rotation around z-axis.
		glm::mat4 Rz = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f,0.0f,1.0f));
		// Scale.
		glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(1 + scale, 1 + scale, 1.0));

		glm::mat4 M = glm::mat4(1.0f);
		M = T*Rz*S;

		// Retrieve location of tranform variable in shader.
		unsigned int loc = glGetUniformLocation(program, "transform");

		// Send matrix to shader.
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(M));

    	// Define the vertices will be draw 
    	glDrawElements(type_primitive, 6, GL_UNSIGNED_INT, 0);

    	glutSwapBuffers();
}

/**
 * Reshape function.
 *
 * Called when window is resized.
 *
 * @param width New window width.
 * @param height New window height.
 */
void reshape(int width, int height)
{
    win_width = width;
    win_height = height;
    glViewport(0, 0, width, height);
    glutPostRedisplay();
}


/** 
 * Keyboard function.
 *
 * Called to treat pressed keys.
 *
 * @param key Pressed key.
 * @param x Mouse x coordinate when key pressed.
 * @param y Mouse y coordinate when key pressed.
 */
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{	
		case '1':
				type_primitive = GL_TRIANGLES;
				glutPostRedisplay();
				break;
		case '2':
				type_primitive = GL_TRIANGLE_FAN;
				glutPostRedisplay();
				break;
		case '3':
				type_primitive = GL_TRIANGLE_STRIP;
				glutPostRedisplay();
				break;
		case 27:
				exit(0);

		// Translação
		case 'w':
		case 'W':
				mov_y += 0.01;
				glutPostRedisplay();
				break;
		case 's':
		case 'S':
				mov_y -= 0.01;
				glutPostRedisplay();
				break;
		case 'd':
		case 'D':
				mov_x += 0.01;
				glutPostRedisplay();
				break;
		case 'a':
		case 'A':
				mov_x -= 0.01;
				glutPostRedisplay();
				break;
		
		// Rotação
		case 'p':
		case 'P':
				rotation(1);
				break;
		case 'n':
		case 'N':
				rotation(0);
				break;
		
		// Escala
		case 'i':
		case 'I':
				scale += 0.025;
				glutPostRedisplay();
				break;
		case 'o':
		case 'O':
				scale -= 0.025;
				glutPostRedisplay();
				break;
		case 'q':
		case 'Q':
				exit(0);
				
		case 'v':
		case 'V':
				if(type_primitive == GL_POINTS){
					type_primitive = GL_TRIANGLES;
				}else{
					glPointSize(10.0);
					type_primitive = GL_POINTS;
				}
				glutPostRedisplay();
				break;

		case 'f':
		case 'F':
				if(type_polygon == GL_FILL){
					type_polygon = GL_LINE;
				}else{
					type_polygon = GL_FILL;
				}
				glPolygonMode(GL_FRONT_AND_BACK, type_polygon);
				glutPostRedisplay();
				break;
	}
}

void rotation(int n)
{	if(n){
		angle = ((angle+angle_inc) < 360.0f) ? angle+angle_inc : 360.0-angle+angle_inc;
	}else{
		angle = ((angle+angle_dec) < 360.0f) ? angle+angle_dec : 360.0-angle+angle_dec;
	}

    glutPostRedisplay();
}

/**
 * Init vertex data.
 *
 * Defines the coordinates for vertices, creates the arrays for OpenGL.
 */
void initData()
{   float vertices[36] = {
		-0.0f,  0.0f, 0.0f, 0.5f, 0.25f, 0.25f, // top left
		-0.5f, -0.5f, 0.0f, 0.25f, 0.25f, 0.5f, // bottom left
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,// bottom right
		-0.5f,  0.5f, 0.0f , 0.25f, 0.25f, 0.5f,// top right
		0.5f,  0.5f, 0.0f , 0.0f, 1.0f, 0.0f,// top right
		0.0f,  -0.0f, 0.0f , 0.0f, 0.5f, 0.0f,// top right
	};
	unsigned int indices[6] = {
		0, 1, 2,   // first triangle
		3, 4, 5    // second triangle
	};  
    // Vertex array.
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex buffer
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
	// Element buffer - not use/allocate duplicated vertices
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set attributes.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    
	// Desabilita VAO	
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind Vertex Array Object.
    glBindVertexArray(0);
}

/** Create program (shaders).
 * 
 * Compile shaders and create the program.
 */
void initShaders()
{
    // Request a program and shader slots from GPU
    program = createShaderProgram(vertex_code, fragment_code);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(win_width,win_height);
	glutCreateWindow(argv[0]);
	glewExperimental = GL_TRUE; 
	glewInit();

	// Init vertex data for the triangle.
	initData();

	// Create shaders.
	initShaders();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
}