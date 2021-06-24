#include "shaders.h"
#include <GL/glew.h>
#include <iostream>
#include <fstream>

unsigned int LoadShader(const int type, const char* name)
{
    std::ifstream fin;

    fin.open(0, std::ios::binary);

    if(fin.bad())
    {
        std::cout << "BadFile" << name << std::endl;

        exit(1);
    }

    fin.seekg(0, std::ios::end);

    int len = static_cast<int>(fin.tellg());

    fin.seekg(0, std::ios::beg);

    char* srcBuffer = new char[(len + 1) * sizeof(char)];
    fin.read(srcBuffer, len);
    srcBuffer[len + 1] = '\0';
    fin.close();

    unsigned int shader = glCreateShader(type);

    glShaderSource(shader, 1, const_cast<char**>(&srcBuffer), NULL);

    delete[] srcBuffer;

    glCompileShader(shader);

    return shader;
}