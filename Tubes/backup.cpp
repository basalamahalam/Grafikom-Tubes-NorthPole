#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
#include "camera.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Snow Island", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("7.4.camera.vs", "7.4.camera.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float sea[] = {
        //1	
        3.8928, -1.5631, 4.1666,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        3.2325, -1.6227, 3.8686,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        2.3931, -1.5877, 4.1423,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //2	
        -1.9596, -1.5185, 3.6208,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -2.7757, -1.6322, 4.0689,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.6144, -1.6593, 4.0918,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //3	
        -2.7757, -1.6322, 4.0689,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -3.1722, -1.5739, 3.8099,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -3.4751, -1.5404, 4.0938,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //4	
        0.9830, -1.5460, 4.1329,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.4254, -1.5966, 3.5410,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.1491, -1.6737, 4.1110,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //5	
        -0.5537, -1.5675, 4.1065,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.0342, -1.5795, 3.4062,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.6144, -1.6593, 4.0918,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //6	
        3.2325, -1.6227, 3.8686,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        2.4705, -1.5800, 3.4935,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        2.3931, -1.5877, 4.1423,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //7	
        2.3931, -1.5877, 4.1423,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        2.4705, -1.5800, 3.4935,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        1.4522, -1.6436, 4.1367,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //8	
        -1.9596, -1.5185, 3.6208,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -2.4543, -1.6161, 3.1524,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -2.7918, -1.7294, 3.6537,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //9	
        1.5934, -1.5088, 2.6057,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        1.3192, -1.5880, 2.8881,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        1.4522, -1.6436, 4.1367,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //10	
        1.4522, -1.6436, 4.1367,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        1.3192, -1.5880, 2.8881,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.9830, -1.5460, 4.1329,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //11	
        -0.2768, -1.5616, 3.1632,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.0342, -1.5795, 3.4062,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.5537, -1.5675, 4.1065,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //12	
        0.9830, -1.5460, 4.1329,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.6124, -1.5865, 2.7016,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.4254, -1.5966, 3.5410,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //13	
        0.4254, -1.5966, 3.5410,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.2100, -1.5085, 3.0235,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.2768, -1.5616, 3.1632,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //14	
        -2.5806, -1.5938, 2.1452,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -3.1342, -1.5079, 2.7732,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -2.4543, -1.6161, 3.1524,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //15	
        1.5934, -1.5088, 2.6057,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.6124, -1.5865, 2.7016,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        1.3192, -1.5880, 2.8881,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //16	
        0.5084, -1.5634, 2.0440,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.1997, -1.6276, 2.7099,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.6124, -1.5865, 2.7016,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //17	
        -1.3133, -1.6198, 2.0896,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -2.0926, -1.6046, 2.5245,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.4398, -1.4885, 2.9909,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //18	
        -0.1745, -1.4651, 1.9066,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.5376, -1.5329, 1.7078,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.8575, -1.5915, 2.4092,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //19	
        -2.0926, -1.6046, 2.5245,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -2.2758, -1.5071, 1.6458,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -2.5806, -1.5938, 2.1452,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //20	
        0.7551, -1.4626, 1.5392,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.1745, -1.4651, 1.9066,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.5084, -1.5634, 2.0440,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //21	
        -0.5376, -1.5329, 1.7078,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.6616, -1.4656, 1.3405,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.3133, -1.6198, 2.0896,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //22	
        0.7551, -1.4626, 1.5392,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.1415, -1.3972, 1.0339,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.1745, -1.4651, 1.9066,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //23	
        -1.0908, -1.6234, 1.1896,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.4508, -1.5027, 0.5196,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.6616, -1.4656, 1.3405,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //24	
        -1.9596, -1.5185, 3.6208,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -2.7918, -1.7294, 3.6537,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -2.7757, -1.6322, 4.0689,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //25	
        -2.7757, -1.6322, 4.0689,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -2.7918, -1.7294, 3.6537,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -3.1722, -1.5739, 3.8099,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //26	
        0.1491, -1.6737, 4.1110,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.2768, -1.5616, 3.1632,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.5537, -1.5675, 4.1065,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //27	
        0.4254, -1.5966, 3.5410,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.2768, -1.5616, 3.1632,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.1491, -1.6737, 4.1110,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //28	
        -1.0342, -1.5795, 3.4062,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.9596, -1.5185, 3.6208,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.6144, -1.6593, 4.0918,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //29	
        2.4705, -1.5800, 3.4935,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        1.5934, -1.5088, 2.6057,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        1.4522, -1.6436, 4.1367,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //30	
        -1.9596, -1.5185, 3.6208,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.0342, -1.5795, 3.4062,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.4398, -1.4885, 2.9909,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //31	
        -2.7918, -1.7294, 3.6537,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -3.1342, -1.5079, 2.7732,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -3.1722, -1.5739, 3.8099,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //32	
        -1.4398, -1.4885, 2.9909,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -2.4543, -1.6161, 3.1524,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.9596, -1.5185, 3.6208,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //33	
        -2.4543, -1.6161, 3.1524,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -3.1342, -1.5079, 2.7732,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -2.7918, -1.7294, 3.6537,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //34	
        0.9830, -1.5460, 4.1329,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        1.3192, -1.5880, 2.8881,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.6124, -1.5865, 2.7016,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //35	
        0.4254, -1.5966, 3.5410,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.6124, -1.5865, 2.7016,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.2100, -1.5085, 3.0235,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //36	
        -1.0342, -1.5795, 3.4062,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.2768, -1.5616, 3.1632,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.4398, -1.4885, 2.9909,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //37	
        -0.2768, -1.5616, 3.1632,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.1997, -1.6276, 2.7099,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.8575, -1.5915, 2.4092,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //38	
        -0.2768, -1.5616, 3.1632,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.8575, -1.5915, 2.4092,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.4398, -1.4885, 2.9909,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //39	
        -2.4543, -1.6161, 3.1524,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.4398, -1.4885, 2.9909,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -2.0926, -1.6046, 2.5245,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //40	
        -2.4543, -1.6161, 3.1524,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -2.0926, -1.6046, 2.5245,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -2.5806, -1.5938, 2.1452,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //41	
        -0.1997, -1.6276, 2.7099,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.1745, -1.4651, 1.9066,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.8575, -1.5915, 2.4092,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //42	
        1.5934, -1.5088, 2.6057,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.5084, -1.5634, 2.0440,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.6124, -1.5865, 2.7016,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //43	
        -0.8575, -1.5915, 2.4092,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.3133, -1.6198, 2.0896,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.4398, -1.4885, 2.9909,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //44	
        0.5084, -1.5634, 2.0440,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.1745, -1.4651, 1.9066,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.1997, -1.6276, 2.7099,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //45	
        -0.5376, -1.5329, 1.7078,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.3133, -1.6198, 2.0896,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.8575, -1.5915, 2.4092,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //46	
        -0.5376, -1.5329, 1.7078,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.0908, -1.6234, 1.1896,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.6616, -1.4656, 1.3405,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //47	
        -0.1745, -1.4651, 1.9066,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.1415, -1.3972, 1.0339,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.5376, -1.5329, 1.7078,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //48	
        -0.5376, -1.5329, 1.7078,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.3632, -1.4535, 0.7709,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.0908, -1.6234, 1.1896,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //49	
        -0.5376, -1.5329, 1.7078,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.1415, -1.3972, 1.0339,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.3632, -1.4535, 0.7709,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //50	
        -1.0908, -1.6234, 1.1896,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.3632, -1.4535, 0.7709,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.4508, -1.5027, 0.5196,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //51	
        5.1035, -1.3178, -3.8316,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        4.5092, -1.2846, -3.1736,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        5.0955, -1.3334, -2.7404,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //52	
        5.1035, -1.3178, -3.8316,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        4.6182, -1.2526, -3.9526,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        4.5092, -1.2846, -3.1736,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //53	
        4.6182, -1.2526, -3.9526,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        3.8554, -1.3690, -3.3583,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        4.5092, -1.2846, -3.1736,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //54	
        3.9254, -1.3157, -4.2135,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        3.2970, -1.2915, -4.5416,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        3.0297, -1.3727, -4.0503,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //55	
        5.1136, -1.2610, -4.6326,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        4.6182, -1.2526, -3.9526,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        5.1035, -1.3178, -3.8316,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //56	
        5.1136, -1.2610, -4.6326,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        3.9254, -1.3157, -4.2135,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        4.6182, -1.2526, -3.9526,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //57	
        3.9220, -1.2351, -4.7166,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        3.2970, -1.2915, -4.5416,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        3.9254, -1.3157, -4.2135,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //58	
        3.0297, -1.3727, -4.0503,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        2.0426, -1.3952, -5.0813,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        2.2413, -1.3463, -4.3404,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //59	
        -3.4560, -1.3320, -5.1459,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -4.1029, -1.3943, -4.2109,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -3.6535, -1.3491, -4.3168,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //60	
        5.1136, -1.2610, -4.6326,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        3.9220, -1.2351, -4.7166,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        3.9254, -1.3157, -4.2135,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //61	
        3.2970, -1.2915, -4.5416,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        2.6918, -1.2894, -5.0736,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        3.0297, -1.3727, -4.0503,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //62	
        3.9220, -1.2351, -4.7166,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        3.4758, -1.2323, -5.0610,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        3.2970, -1.2915, -4.5416,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //63	
        -2.2656, -1.2376, -4.9806,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -2.2487, -0.4908, -4.5372,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.3875, -1.2637, -4.8202,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //64	
        1.4774, -1.2722, -4.7740,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.6750, -1.2885, -5.0927,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.8535, -1.3185, -4.7418,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //65	
        -1.4840, -1.2549, -5.1310,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.3875, -1.2637, -4.8202,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.6572, -1.3150, -5.1104,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //66	
        -1.3875, -1.2637, -4.8202,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.4840, -1.2549, -5.1310,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -2.2656, -1.2376, -4.9806,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //67	
        -2.2656, -1.2376, -4.9806,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -2.4214, -1.2598, -5.1365,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -3.4560, -1.3320, -5.1459,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //68	
        3.8554, -1.3690, -3.3583,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        4.6182, -1.2526, -3.9526,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        3.9254, -1.3157, -4.2135,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //69	
        3.5033, -1.2543, -3.3415,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        3.8554, -1.3690, -3.3583,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        3.0297, -1.3727, -4.0503,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //70	
        3.8554, -1.3690, -3.3583,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        3.9254, -1.3157, -4.2135,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        3.0297, -1.3727, -4.0503,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //71	
        3.5033, -1.2543, -3.3415,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        3.0297, -1.3727, -4.0503,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        3.0219, -1.2182, -3.6648,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //72	
        3.0219, -1.2182, -3.6648,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        3.0297, -1.3727, -4.0503,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        2.2413, -1.3463, -4.3404,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //73	
        -3.0994, -1.2246, -4.6555,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -3.4560, -1.3320, -5.1459,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -3.6535, -1.3491, -4.3168,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //74	
        3.0297, -1.3727, -4.0503,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        2.6918, -1.2894, -5.0736,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        2.0426, -1.3952, -5.0813,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //75	
        2.2413, -1.3463, -4.3404,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        2.0426, -1.3952, -5.0813,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        1.4774, -1.2722, -4.7740,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //76	
        -3.4560, -1.3320, -5.1459,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -4.1021, -1.2638, -5.1534,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -4.1029, -1.3943, -4.2109,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //77	
        5.1166, -1.2956, -5.0390,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        4.7534, -1.3833, -5.0524,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        5.1136, -1.2610, -4.6326,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //78	
        0.8535, -1.3185, -4.7418,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.6572, -1.3150, -5.1104,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.5334, -1.2224, -4.7547,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //79	
        -0.5334, -1.2224, -4.7547,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.6572, -1.3150, -5.1104,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.3875, -1.2637, -4.8202,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //80	
        -3.0994, -1.2246, -4.6555,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -2.2656, -1.2376, -4.9806,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -3.4560, -1.3320, -5.1459,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //81	
        3.2970, -1.2915, -4.5416,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        3.4758, -1.2323, -5.0610,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        2.6918, -1.2894, -5.0736,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //82	
        1.4774, -1.2722, -4.7740,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        2.0426, -1.3952, -5.0813,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.6750, -1.2885, -5.0927,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //83	
        0.6750, -1.2885, -5.0927,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.6572, -1.3150, -5.1104,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.8535, -1.3185, -4.7418,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //84	
        -1.4840, -1.2549, -5.1310,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -2.4214, -1.2598, -5.1365,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -2.2656, -1.2376, -4.9806,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //85	
        -0.2768, -1.5616, 3.1632,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.2100, -1.5085, 3.0235,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.6124, -1.5865, 2.7016,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //86	
        -0.2768, -1.5616, 3.1632,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        0.6124, -1.5865, 2.7016,    0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -0.1997, -1.6276, 2.7099,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //87	
        3.4758, -1.2323, -5.0610,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        3.9220, -1.2351, -4.7166,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        5.1136, -1.2610, -4.6326,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	

        //88	
        3.4758, -1.2323, -5.0610,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        5.1136, -1.2610, -4.6326,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        4.7534, -1.3833, -5.0524,   0.38f, 0.43f, 0.43f,    0.0f, 0.0f,
     };
    // world space positions of our cubes
    glm::vec3 sea_Position[] = {
        glm::vec3(0.0f,  0.0f, 0.0f)
    };

    float rightStone[] = {
        //1	
        -1.3165, -1.3790, -0.1158,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,	
        -1.2250, -1.3736, -0.1592,	0.38f, 0.43f, 0.43f,    0.0f, 0.0f,
        -1.2479, -1.4518, -0.0987,  0.38f, 0.43f, 0.43f,    0.0f, 0.0f,

        //2	
        -1.2250, -1.3736, -0.1592,	0.33f, 0.37f, 0.36f,    0.0f, 0.0f,
        -1.3165, -1.3790, -0.1158,	0.33f, 0.37f, 0.36f,    0.0f, 0.0f,
        -1.2676, -1.2902, -0.1201,  0.33f, 0.37f, 0.36f,    0.0f, 0.0f,

        //3	
        -1.3165, -1.3790, -0.1158,  0.48f, 0.51f, 0.52f,    0.0f, 0.0f,	
        -1.2479, -1.4518, -0.0987,	0.48f, 0.51f, 0.52f,    0.0f, 0.0f,
        -1.3047, -1.4168, -0.0223,  0.48f, 0.51f, 0.52f,    0.0f, 0.0f,

        //4	
        -1.3165, -1.3790, -0.1158,  0.55f, 0.59f, 0.62f,    0.0f, 0.0f,	
        -1.3047, -1.4168, -0.0223,	0.55f, 0.59f, 0.62f,    0.0f, 0.0f,
        -1.3169, -1.3169, -0.0355,  0.55f, 0.59f, 0.62f,    0.0f, 0.0f,

        //5	
        -1.3165, -1.3790, -0.1158,	0.62f, 0.66f, 0.69f,    0.0f, 0.0f,
        -1.3169, -1.3169, -0.0355,	0.62f, 0.66f, 0.69f,    0.0f, 0.0f,
        -1.2676, -1.2902, -0.1201,  0.62f, 0.66f, 0.69f,    0.0f, 0.0f,

        //6	
        -1.2250, -1.3736, -0.1592,	0.62f, 0.63f, 0.6f,    0.0f, 0.0f,
        -1.2676, -1.2902, -0.1201,	0.62f, 0.63f, 0.6f,    0.0f, 0.0f,
        -1.1687, -1.3081, -0.1058,  0.62f, 0.63f, 0.6f,    0.0f, 0.0f,

        //7	
        -1.2479, -1.4518, -0.0987,	0.33f, 0.34f, 0.31f,    0.0f, 0.0f,
        -1.2250, -1.3736, -0.1592,	0.33f, 0.34f, 0.31f,    0.0f, 0.0f,
        -1.1566, -1.4080, -0.0926,  0.33f, 0.34f, 0.31f,    0.0f, 0.0f,

        //8	
        -1.3047, -1.4168, -0.0223,  0.28f, 0.72f, 0.72f,    0.0f, 0.0f,	
        -1.2479, -1.4518, -0.0987,	0.28f, 0.72f, 0.72f,    0.0f, 0.0f,
        -1.2059, -1.4347, -0.0080,  0.28f, 0.72f, 0.72f,    0.0f, 0.0f,

        //9	
        -1.3169, -1.3169, -0.0355,  0.28f, 0.72f, 0.72f,    0.0f, 0.0f,	
        -1.3047, -1.4168, -0.0223,	0.28f, 0.72f, 0.72f,    0.0f, 0.0f,
        -1.2485, -1.3513, 0.0311,   0.28f, 0.72f, 0.72f,    0.0f, 0.0f,

        //10	
        -1.2676, -1.2902, -0.1201,  0.28f, 0.72f, 0.72f,    0.0f, 0.0f,	
        -1.3169, -1.3169, -0.0355,	0.28f, 0.72f, 0.72f,    0.0f, 0.0f,
        -1.2256, -1.2731, -0.0294,  0.28f, 0.72f, 0.72f,    0.0f, 0.0f,

        //11	
        -1.2250, -1.3736, -0.1592,  0.28f, 0.72f, 0.72f,    0.0f, 0.0f,	
        -1.1687, -1.3081, -0.1058,	0.28f, 0.72f, 0.72f,    0.0f, 0.0f,
        -1.1566, -1.4080, -0.0926,  0.28f, 0.72f, 0.72f,    0.0f, 0.0f,

        //12	
        -1.2479, -1.4518, -0.0987,  0.28f, 0.72f, 0.72f,    0.0f, 0.0f,	
        -1.1566, -1.4080, -0.0926,	0.28f, 0.72f, 0.72f,    0.0f, 0.0f,
        -1.2059, -1.4347, -0.0080,  0.28f, 0.72f, 0.72f,    0.0f, 0.0f,

        //13	
        -1.3047, -1.4168, -0.0223,  0.28f, 0.72f, 0.72f,    0.0f, 0.0f,	
        -1.2059, -1.4347, -0.0080,	0.28f, 0.72f, 0.72f,    0.0f, 0.0f,
        -1.2485, -1.3513, 0.0311,   0.28f, 0.72f, 0.72f,    0.0f, 0.0f,

        //14	
        -1.3169, -1.3169, -0.0355,  0.28f, 0.72f, 0.72f,    0.0f, 0.0f,	
        -1.2485, -1.3513, 0.0311,	0.28f, 0.72f, 0.72f,    0.0f, 0.0f,
        -1.2256, -1.2731, -0.0294,  0.28f, 0.72f, 0.72f,    0.0f, 0.0f,

        //15	
        -1.2676, -1.2902, -0.1201,  0.28f, 0.72f, 0.72f,    0.0f, 0.0f,	
        -1.2256, -1.2731, -0.0294,	0.28f, 0.72f, 0.72f,    0.0f, 0.0f,
        -1.1687, -1.3081, -0.1058,  0.28f, 0.72f, 0.72f,    0.0f, 0.0f,

        //16	
        -1.1566, -1.4080, -0.0926,  0.28f, 0.72f, 0.72f,    0.0f, 0.0f,	
        -1.1687, -1.3081, -0.1058,	0.28f, 0.72f, 0.72f,    0.0f, 0.0f,
        -1.1570, -1.3459, -0.0123,  0.28f, 0.72f, 0.72f,    0.0f, 0.0f,

        //17	
        -1.2059, -1.4347, -0.0080,  0.28f, 0.72f, 0.72f,    0.0f, 0.0f,	
        -1.1566, -1.4080, -0.0926,	0.28f, 0.72f, 0.72f,    0.0f, 0.0f,
        -1.1570, -1.3459, -0.0123,  0.28f, 0.72f, 0.72f,    0.0f, 0.0f,

        //18	
        -1.2485, -1.3513, 0.0311,   0.28f, 0.72f, 0.72f,    0.0f, 0.0f,	
        -1.2059, -1.4347, -0.0080,	0.28f, 0.72f, 0.72f,    0.0f, 0.0f,
        -1.1570, -1.3459, -0.0123,  0.28f, 0.72f, 0.72f,    0.0f, 0.0f,

        //19	
        -1.2256, -1.2731, -0.0294,  0.28f, 0.72f, 0.72f,    0.0f, 0.0f,	
        -1.2485, -1.3513, 0.0311,	0.28f, 0.72f, 0.72f,    0.0f, 0.0f,
        -1.1570, -1.3459, -0.0123,  0.28f, 0.72f, 0.72f,    0.0f, 0.0f,

        //20	
        -1.1687, -1.3081, -0.1058,  0.28f, 0.72f, 0.72f,    0.0f, 0.0f,	
        -1.2256, -1.2731, -0.0294,	0.28f, 0.72f, 0.72f,    0.0f, 0.0f,
        -1.1570, -1.3459, -0.0123,	0.28f, 0.72f, 0.72f,    0.0f, 0.0f,
    };

    glm::vec3 rightStone_Position[] = {
        glm::vec3(0.0f, 0.0f, 0.0f)
    }

    unsigned int VBO[2], VAO[2], EBO[2];
    glGenVertexArrays(1, &VAO[0]);
    glGenBuffers(1, &VBO[0]);
    glGenBuffers(1, &EBO[0]);

    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sea), sea, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    // atribut warna
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);





    glGenVertexArrays(1, &VAO[1]);
    glGenBuffers(1, &VBO[1]);
    glGenBuffers(1, &EBO[1]);

    glBindVertexArray(VAO[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rightStone), rightStone, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    // atribut warna
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);




    // load and create a texture
    // -------------------------
    unsigned int texture1;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load("white.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture1", 0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // activate shader
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        // render boxes
        glBindVertexArray(VAO[0]);
        for (unsigned int i = 0; i < 1; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, sea_Position[i]);
            float angle = 20.0f * 0;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 30000);
        }

        glBindVertexArray(VAO[1]);
        for (unsigned int i = 0; i < 1; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, rightStone_Position[i]);
            float angle = 20.0f * 0;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 30000);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO[0]);
    glDeleteBuffers(1, &VBO[0]);
    glDeleteBuffers(1, &EBO[0]);

    glDeleteVertexArrays(1, &VAO[1]);
    glDeleteBuffers(1, &VBO[1]);
    glDeleteBuffers(1, &EBO[1]);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

