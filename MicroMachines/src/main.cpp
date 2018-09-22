#include <iostream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "common.h"
#include "cube.h"
#include "shader.h"
#include "gameEventHandler.h"
#include "inputControl.h"
#include "mesh.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>  
#include <glm/vec3.hpp>    
#include <glm/vec4.hpp>    


extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	//makes visual studio use the gpu
}

#define CAPTION "60 FPS"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

Shader* basicShader;
Mesh* mesh;
GameEventHandler eventHandler;
InputControl horizontal, vertical;

/////////////////////////////////////////////////////////////////////// SCENE

void renderScene() {
	if (horizontal.GetAmt() != 0) return;
	basicShader->Bind();
	glm::mat4 proj = glm::perspective(30.0f, (float)WinX / (float)WinY, 1.0f, 5000.f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 2.0f, 2.0f), glm::vec3(0.0f), glm::vec3(0, 0, 1));

	basicShader->SetUniformMat4f("Matrix", proj * view * glm::mat4(1));
	mesh->Draw();

	glm::mat4 model =
		glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, .0f)) *
		glm::rotate(glm::mat4(1), glm::radians(.0f), glm::vec3(0.5f, 0.5f, 0.f));
	auto mvp = proj * view * model;
	basicShader->SetUniformMat4f("Matrix", mvp);

	mesh->Draw();
	basicShader->Unbind();
	GLCall(glBindVertexArray(0));

}


///////////////////////////////////////////////////////////////////////
/// CALLBACKS

void cleanup() {
	// destroyBufferObjects();
}

void display() {
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderScene();
	glutSwapBuffers();
}


void idle() { glutPostRedisplay(); }

void reshape(int w, int h) {
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
}

void timer(int value) {
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY
		<< ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
	glutTimerFunc(1000, timer, 0);
}

void KeyboardDown(unsigned char key, int x, int y) {
	eventHandler.OnKeyDown(key, false);
}

void KeyboardUp(unsigned char key, int x, int y) {
	eventHandler.OnKeyUp(key, false);
}

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks() {
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutKeyboardFunc(KeyboardDown);
	glutKeyboardUpFunc(KeyboardUp);
}

void setupOpenGL() {
	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void setupGLEW() {
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void setupGLUT(int argc, char* argv[]) {
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if (WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[]) {
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	setupCallbacks();
}

int main(int argc, char* argv[]) {
	init(argc, argv);

	basicShader = new Shader("res/shaders/basic");
	mesh = new Mesh("res/meshes/suzanne.obj");

	eventHandler.AddKeyControl('a', horizontal, -1.0f);
	eventHandler.AddKeyControl('d', horizontal, 1.0f);
	eventHandler.AddKeyControl('w', vertical, -1.0f);
	eventHandler.AddKeyControl('s', vertical, 1.0f);

	glutMainLoop();

	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////