/**
 * @file lista1.cpp
 * @brief based on triangle.cpp
 * Draws a one or two triangle.
 *
 * Uses the basics for drawing in OpenGL, shaders, arrays, buffers, etc.
 *
 * @author Matheus Martins Batista
 */

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>


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
int triangles_number = 1;

/** Variavel global tipo primitivo*/
int type_primitive = GL_TRIANGLES;

/** Variavel global tipo poligono*/
int type_polygon = GL_FILL;

/** Vertex shader. */
const char *vertex_code = "\n"
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

/** Fragment shader. */
const char *fragment_code = "\n"
"#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\0";

/* Functions. */
void display(void);
void reshape(int, int);
void keyboard(unsigned char, int, int);
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

    	// Define the vertices will be draw and draws the triangles (1 or 2)
    	glDrawElements(type_primitive, 3*triangles_number, GL_UNSIGNED_INT, 0);

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
		case 27:
				exit(0);
		case 'q':
		case 'Q':
				exit(0);
		case 'o':
		case 'O':
				if(triangles_number == 1)
					triangles_number = 2;
				else
					triangles_number = 1;
				glutPostRedisplay();
				break;
				
		case 'p':
		case 'P':
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

/**
 * Init vertex data.
 *
 * Defines the coordinates for vertices, creates the arrays for OpenGL.
 */
void initData()
{   float vertices[12] = {
		-0.5f,  0.5f, 0.0f,  // top left
		-0.5f, -0.5f, 0.0f,  // bottom left
		0.5f, -0.5f, 0.0f,  // bottom right
		0.5f,  0.5f, 0.0f   // top right
	};
	unsigned int indices[6] = {
		0, 1, 2,   // first triangle
		0, 2, 3    // second triangle
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    
	// Desabilita VAO	
	glEnableVertexAttribArray(0);

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
    program  = glCreateProgram();
    int vertex   = glCreateShader(GL_VERTEX_SHADER);
    int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Set shaders source
    glShaderSource(vertex, 1, &vertex_code, NULL);
    glShaderSource(fragment, 1, &fragment_code, NULL);

    // Compile shaders
    glCompileShader(vertex);
    glCompileShader(fragment);

    // Attach shader objects to the program
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    // Link the program
    glLinkProgram(program);

    // Get rid of shaders (not needed anymore)
    glDetachShader(program, vertex);
    glDetachShader(program, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
    // Set the program to be used.
    glUseProgram(program);
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