#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>
#include <thread>
#include <ctime>
#include "shaders.h"

float left = -2.0f, right = 2.0f, top = 2.0f, bottom = -2.0f;
float zNear = 0.0001f, zFar = 10.0f;
float scale = 1.0f;
float transX = 0.0f, transY = 0.0f;

unsigned program, vertexArray, vertexBuffer, basicIndicesBuffer;

glm::mat4x4 projMat;

void InitScene();
void DisplayScene();
void Display();
void Reshape(int w, int h);
void DeleteScene();

float basicPosition[4 * 2] = {
    1.0f, 1.0f,
    -1.0f, 1.0f,
    1.0f, -1.0f,
    -1.0f, -1.0f
};

int basicIndices[4 * 2] = {
    0, 1,
    0, 2,
    1, 3,
    2, 3
};

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitContextFlags(GLUT_DOUBLE | GLUT_RGB);
    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    glutInitWindowSize(800, 600);

    glutCreateWindow("2D Game");

    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK)
    {
        std::cout << "!GLEW_OK\n";

        return -1;
    }

    if(!GLEW_VERSION_4_3)
    {
        std::cout << "Wrong version OpenGL\n";

        return -2;
    }

    glGetError();

    InitScene();

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);

    glutMainLoop();

    DeleteScene();

    return 0;
}

void InitScene()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    program = glCreateProgram();
    glAttachShader(program, LoadShader(GL_VERTEX_SHADER, "data/shaders/basic_vs.glsl"));
    glAttachShader(program, LoadShader(GL_FRAGMENT_SHADER, "data/shaders/basic_fs.glsl"));
    LinkValidateProgram(program);

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(basicPosition), basicPosition, GL_STATIC_DRAW);

    unsigned int posLoc = glGetAttribLocation(program, "inPos");

    glVertexAttribPointer(posLoc, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(posLoc);

    glGenBuffers(1, &basicIndicesBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, basicIndicesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(basicIndices), basicIndices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void DisplayScene()
{
    glm::mat4x4 viMat = glm::mat4x4(1.0);

    viMat = glm::lookAt(glm::vec3(0.0f, 0.0f, (zFar + zNear) / 2.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4x4 modMat = glm::mat4x4(1.0);

    modMat = glm::scale(modMat, glm::vec3(scale, scale, scale));

    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(vertexArray);
    glUseProgram(program);

    glm::mat4x4 projViModMat = projMat * viMat * modMat;
    glUniformMatrix4fv(glGetUniformLocation(program, "projViModMat"), 1, GL_FALSE, glm::value_ptr(projViModMat));

    glDrawElements(GL_TRIANGLES, 4 * 3, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);
    glUseProgram(0);
}

void Display()
{
    DisplayScene();

    int error = glGetError();

    switch(error)
    {
    case GL_INVALID_ENUM:
        std::cout << "enum\n";
        exit(2);
    case GL_INVALID_VALUE:
        std::cout << "value\n";
        exit(2);
    case GL_INVALID_INDEX:
        std::cout << "index\n";
        exit(2);
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        std::cout << "frOp\n";
        exit(2);
    case GL_STACK_OVERFLOW:
        std::cout << "over\n";
        exit(2);
    case GL_STACK_UNDERFLOW:
        std::cout << "under\n";
        exit(2);
    case GL_OUT_OF_MEMORY:
        std::cout << "outOfMem\n";
        exit(2);
    case GL_CONTEXT_LOST:
        std::cout << "lost\n";
        exit(2);
    case GL_NO_ERROR:
        break;
    }

    glutSwapBuffers();
}

void Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

void DeleteScene()
{
    glDeleteVertexArrays(1, &vertexArray);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &basicIndicesBuffer);
    glDeleteProgram(program);
}