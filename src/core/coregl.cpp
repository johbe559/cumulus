#include "coregl.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace core {

void CoreGL::printError(const char *functionName)
{
   GLenum error;
   while (( error = glGetError() ) != GL_NO_ERROR)
   {
	  fprintf (stderr, "GL error 0x%X detected in %s\n", error, functionName);
   }
}

void CoreGL::loadShaders() {
	shaderManager = ShaderManager();
	shaderManager.loadShaders("basic.vert", "basic.frag", ShaderManager::shaderId::BASIC);
}

core::CoreGL::CoreGL() {
}

void core::CoreGL::render(glm::mat4 trans, glm::mat4 proj) {
	glm::mat4 mvp = proj * trans; // * glm::scale(glm::mat4(1.0f), glm::vec3(10.f));;

	GLuint matrixLoc = 0;
	GLuint id = shaderManager.getId(ShaderManager::shaderId::BASIC);
	glUseProgram(id);
	
	glUniformMatrix4fv(
		glGetUniformLocation(id, "camTrans"), 1, 
		GL_FALSE, glm::value_ptr(mvp));
	objectLoader.drawModels();
	printError("Core render");

	//glPushMatrix();
	//glLoadIdentity();
	glBegin(GL_QUADS);
	glVertex3f(-1.0f,-1.0f, -1.0f);
	glVertex3f( 1.0f,-1.0f, -1.0f);
	glVertex3f( 1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();
	//glPopMatrix();
}

core::CoreGL *core::CoreGL::creator(std::string path) {
    core::CoreGL *instance = new core::CoreGL();
    instance->initialize(path);
    return instance;
}

void core::CoreGL::initialize(std::string path) {
    std::cout << "Loading object " << path.c_str() << std::endl;
	loadShaders();
	printError("Load Shaders");
	objectLoader = ObjLoader();
	objectLoader.loadObj(path, shaderManager.getId(ShaderManager::shaderId::BASIC));
	printError("LoadObj");
	std::cout << "Loading done.\n";
}
} // namespace core
