#include <iostream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "common.h"
#include "cube.h"
#include "shader.h"

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#define CAPTION "60 FPS"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

#define VERTEX_COORD_ATTRIB 0
#define NORMAL_ATTRIB 1
#define TEXTURE_COORD_ATTRIB 2

GLuint VaoId, VboId[4];
Shader *basicShader;
GLint UniformId;

/////////////////////////////////////////////////////////////////////// VAOs &
/// VBOs

void createBufferObjects() {
  GLCall(glGenVertexArrays(1, &VaoId));
  GLCall(glBindVertexArray(VaoId));

  GLCall(glGenBuffers(4, VboId));

  // vertex coordinates buffer
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, VboId[0]));
  GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
  GLCall(glEnableVertexAttribArray(VERTEX_COORD_ATTRIB));
  GLCall(glVertexAttribPointer(VERTEX_COORD_ATTRIB, 4, GL_FLOAT, 0, 0, 0));

  // normals buffer
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, VboId[2]));
  GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW));
  GLCall(glEnableVertexAttribArray(NORMAL_ATTRIB));
  GLCall(glVertexAttribPointer(NORMAL_ATTRIB, 3, GL_FLOAT, 0, 0, 0));

  // texture coordinates buffer
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, VboId[1]));
  GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW));
  GLCall(glEnableVertexAttribArray(TEXTURE_COORD_ATTRIB));
  GLCall(glVertexAttribPointer(TEXTURE_COORD_ATTRIB, 2, GL_FLOAT, 0, 0, 0));

  // index buffer
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[3]));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faceIndex), faceIndex,
               GL_STATIC_DRAW));

  // unbind the VAO
  GLCall(glBindVertexArray(0));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
  GLCall(glDisableVertexAttribArray(VERTEX_COORD_ATTRIB));
  GLCall(glDisableVertexAttribArray(NORMAL_ATTRIB));
  GLCall(glDisableVertexAttribArray(TEXTURE_COORD_ATTRIB));

}

void destroyBufferObjects() {
  GLCall(glDisableVertexAttribArray(VERTEX_COORD_ATTRIB));
  GLCall(glDisableVertexAttribArray(NORMAL_ATTRIB));
  GLCall(glDisableVertexAttribArray(TEXTURE_COORD_ATTRIB));

  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
  GLCall(glBindVertexArray(0));

  GLCall(glDeleteBuffers(4, VboId));
  GLCall(glDeleteVertexArrays(1, &VaoId));
}

/////////////////////////////////////////////////////////////////////// SCENE

void renderScene() {
  GLCall(glBindVertexArray(VaoId));
  basicShader->Bind();
  glm::mat4 proj = glm::perspective(30.0f, (float)WinX / (float)WinY, 1.0f, 5000.f);
  glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 2.0f, 2.0f), glm::vec3(0.0f), glm::vec3(0, 0, 1));

  basicShader->SetUniformMat4f("Matrix", proj * view * glm::mat4(1));
  GLCall(glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_INT, (GLvoid*)0));

  //glm::mat4 M = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, 0.0f));
  glm::mat4 model =
      glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, .0f)) *
      glm::rotate(glm::mat4(1),glm::radians(.0f), glm::vec3(0.5f, 0.5f, 0.f));
  auto mvp = proj * view * model;
  basicShader->SetUniformMat4f("Matrix", mvp);
  GLCall(glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_INT, (GLvoid*)0));

  basicShader->Unbind();
  GLCall(glBindVertexArray(0));
}

///////////////////////////////////////////////////////////////////////
/// CALLBACKS

void cleanup() {
  destroyBufferObjects();
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

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks() {
  glutCloseFunc(cleanup);
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutReshapeFunc(reshape);
  glutTimerFunc(0, timer, 0);
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
  basicShader = new Shader("res/shaders/basic");
  createBufferObjects();
  setupCallbacks();
}

int main(int argc, char* argv[]) {
  init(argc, argv);
  glutMainLoop();
  exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////