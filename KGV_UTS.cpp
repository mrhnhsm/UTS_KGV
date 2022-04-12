#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#define _USE_MATH_DEFINES
#include <iostream>
#include "Util.h"

using namespace std;

// 6 * n + 1
float vertices[49] = {};
float currentSize = 0.5;
GLint scaleLoc;

float radian(float deg) {
    return deg*M_PI/180;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_W && action == GLFW_PRESS) currentSize += 0.1;
    if (key == GLFW_KEY_S && action == GLFW_PRESS) currentSize -= 0.1;

    glUniform1f(scaleLoc, currentSize);
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "KGV UTS", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, keyCallback);

    GLenum err = glewInit();

    float deg = 360/8;
    float rad = radian(deg);
    float r = 0.25;

    vertices[0] = vertices[1] = vertices[2] = vertices[8] = 0;
    vertices[6] = -1*sin(radian(deg))*r;
    vertices[7] = -1*cos(radian(deg))*r;

    for (int i=12; i<49; i+=6)
    {
        vertices[i] = vertices[6]*cos(rad) - vertices[7]*sin(rad);
        vertices[i+1] = vertices[6]*sin(rad) + vertices[7]*cos(rad);
        vertices[i+2] = vertices[i+4] = vertices[i+5] = 0;
        vertices[i+3] = 1;
        rad += radian(deg);
    }

    for (int i=3; i<49; i+=6)
    {
        vertices[i] = rand()%256/256;
        vertices[i+1] = rand()%256/256;
        vertices[i+2] = rand()%256/256;
    }

    unsigned int indexArr[] = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        0, 5, 6,
        0, 6, 7,
        0, 7, 8,
        0, 8, 1
    };

    unsigned int vertexBuffer = 0;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 54, vertices, GL_STATIC_DRAW);
    unsigned int indexBuffer = 0;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 24, indexArr, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    string vertexString = readFile("vertex.vert");
    string fragmentString = readFile("fragment.frag");

    const char* vertexChar = vertexString.c_str();
    const char* fragmentChar = fragmentString.c_str();
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexChar, NULL);
    glShaderSource(fragmentShader, 1, &fragmentChar, NULL);
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    scaleLoc = glGetUniformLocation(program, "vSize");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        srand(glfwGetTime());
        for (int i = 3; i < 6 * 8 + 1; i += 6)
        {
            vertices[i] = rand() % 256 / 256.0;
            vertices[i + 1] = rand() % 256 / 256.0;
            vertices[i + 2] = rand() % 256 / 256.0;
        }

        int key = glfwGetKey(window, GLFW_KEY_E);
        if (key == GLFW_PRESS)
        {
            srand(time(NULL));
            for (int i = 3; i < 6 * 8 + 1; i += 6)
            {
                vertices[i] = rand() % 256 / 256.0;
                vertices[i + 1] = rand() % 256 / 256.0;
                vertices[i + 2] = rand() % 256 / 256.0;
            }
        }

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 54, vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glUniform1f(scaleLoc, currentSize);

        // Draw using IndexBuffer
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
