#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include "../vendor/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb_image.h"

#include "shader_manager.h"
#include "dvd_simulation.h"

static const constexpr std::size_t VERTICIES_ARRAY_LEN   = 32;
static const constexpr std::size_t INDICIES_ARRAY_LEN    = 6;
static const constexpr GLfloat     DVD_LOGO_DEFAULT_SIZE = 0.16;

struct GLdata {
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
};

GLdata glInitPrimitiveDS() 
{
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    return GLdata{VBO, VAO, EBO};
}

void glReleasePrimitiveDS(GLdata opengl) {
    glDeleteVertexArrays(1, &opengl.VAO);
    glDeleteBuffers(1, &opengl.VBO);
    glDeleteBuffers(1, &opengl.EBO);
}

void glSetupBuffersAndAttributes(GLdata opengl, GLfloat *verticies, size_t vertSize, GLuint *indicies, size_t indxSize) {
    glBindVertexArray(opengl.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, opengl.VBO);

    glBufferData(GL_ARRAY_BUFFER, vertSize, verticies, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, opengl.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indxSize, indicies, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Coordinates attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void glfwBufferSizeChangedCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void glfwWindowProc(GLFWwindow *window) {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void inline glClearScreen() {
    glClearColor(0.05f, 0.05f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

GLuint glLoadTexture(const char* textureName) {
    GLuint samleTexture;
	glGenTextures(1, &samleTexture);
	glBindTexture(GL_TEXTURE_2D, samleTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLint width, height, nrChannels;
	unsigned char* data = stbi_load(textureName, &width, &height, &nrChannels, 0);
	if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		fprintf(stderr, "Failed loading texture %s", textureName);
	}
	
    stbi_image_free(data);	
    return samleTexture;
}

int main(int argc, char *argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Set the vertical synchronizaton
    glfwSwapInterval(1)

    stbi_set_flip_vertically_on_load(true);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "DVD Logo simulation", NULL, NULL);
    if (window == nullptr) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, glfwBufferSizeChangedCallback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        glfwTerminate();
    	return -1;
    }

    GLfloat textureVerticies[] = {
         DVD_LOGO_DEFAULT_SIZE,  DVD_LOGO_DEFAULT_SIZE, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         DVD_LOGO_DEFAULT_SIZE, -DVD_LOGO_DEFAULT_SIZE, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -DVD_LOGO_DEFAULT_SIZE, -DVD_LOGO_DEFAULT_SIZE, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -DVD_LOGO_DEFAULT_SIZE,  DVD_LOGO_DEFAULT_SIZE, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };

    GLuint textureIndicies[] = {
        0, 1, 3, 
        1, 2, 3  
    };

    GLdata opengl = glInitPrimitiveDS();
    glSetupBuffersAndAttributes(opengl, textureVerticies, sizeof(textureVerticies), textureIndicies, sizeof(textureIndicies));

    DVDSimulation dvdSimulation  = DVDSimulation({0.005f, 0.004f}, {0.0f, 0.0f});
    ShaderManager defaultShaders = ShaderManager("shaders/default.vert", "shaders/default.frag");
    GLuint dvdTexture            = glLoadTexture("data/dvd-logo.png");

    defaultShaders.execute();
    defaultShaders.setInteger("dvdTexture", 0);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    while(!glfwWindowShouldClose(window)) {
        dvdSimulation.step();

        glfwWindowProc(window);

        glClearScreen();
        glBindTexture(GL_TEXTURE_2D, dvdTexture);

        defaultShaders.execute();
        defaultShaders.setFloat("textureMoveOffsetX", dvdSimulation.getPosition().first);
        defaultShaders.setFloat("textureMoveOffsetY", dvdSimulation.getPosition().second);
        glBindVertexArray(opengl.VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents ();
    }

    glReleasePrimitiveDS(opengl);
    glfwTerminate();
    return 0;
}