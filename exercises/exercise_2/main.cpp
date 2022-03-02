#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader.h>
#include <random>
#include <iostream>
#include <vector>
#include <math.h>
#include <string>

// structure to hold the info necessary to render an object
struct SceneObject {
    unsigned int VAO;           // vertex array object handle
    unsigned int vertexCount;   // number of vertices in the object
    float r, g, b;              // for object color
    float x, y;                 // for position offset
};

// declaration of the function you will implement in exercise 2.1
SceneObject instantiateCone(float r, float g, float b, float offsetX, float offsetY);
// mouse, keyboard and screen reshape glfw callbacks
void button_input_callback(GLFWwindow* window, int button, int action, int mods);
void key_input_callback(GLFWwindow* window, int button, int other,int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

// global variables we will use to store our objects, shaders, and active shader
std::vector<SceneObject> sceneObjects;
std::vector<Shader> shaderPrograms;
Shader* activeShader;

std::random_device rd; // obtain a random number from hardware
std::mt19937 gen(rd()); // seed the generator
std::uniform_int_distribution<> distr(0, 100);

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Exercise 2 - Voronoi Diagram", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // setup frame buffer size callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // setup input callbacks
    glfwSetMouseButtonCallback(window, button_input_callback); // NEW!
    glfwSetKeyCallback(window, key_input_callback); // NEW!

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // NEW!
    // build and compile the shader programs
    shaderPrograms.push_back(Shader("shaders/shader.vert", "shaders/color.frag"));
    shaderPrograms.push_back(Shader("shaders/shader.vert", "shaders/distance.frag"));
    shaderPrograms.push_back(Shader("shaders/shader.vert", "shaders/distance_color.frag"));
    activeShader = &shaderPrograms[0];

    // NEW!
    // set up the z-buffer
    glDepthRange(1,-1); // make the NDC a right handed coordinate system, with the camera pointing towards -z
    glEnable(GL_DEPTH_TEST); // turn on z-buffer depth test
    glDepthFunc(GL_LESS); // draws fragments that are closer to the screen in NDC

    // TODO exercise 2.6
    // enable blending
    // choose the right blending factors to produce additive blending
    // glBlendFunc(?, ?);

    //sceneObjects.push_back(instantiateCone(1.0f, 1.0f, 1.0f, 0.0f, 0.0f));
    

    // render loop
    while (!glfwWindowShouldClose(window)) {


        // background color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // notice that now we are clearing two buffers, the color and the z-buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render the cones
        glUseProgram(activeShader->ID);

        // TODO exercise 2.3
        // Iterate through the scene objects, for each object:
        // - bind the VAO; set the uniform variables; and draw.
        int colorPosition = glGetUniformLocation(activeShader->ID, "color");
        for (int i = 0; i < sceneObjects.size(); i++)
        {
            glBindVertexArray(sceneObjects[i].VAO);
            glUniform3f(colorPosition, sceneObjects[i].r, sceneObjects[i].g, sceneObjects[i].b);
            glDrawElements(GL_TRIANGLES, sceneObjects[i].vertexCount, GL_UNSIGNED_INT, 0);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}


// creates a cone triangle mesh, uploads it to openGL and returns the VAO associated to the mesh
SceneObject instantiateCone(float r, float g, float b, float offsetX, float offsetY){
    // TODO exercise 2.1
    // (exercises 1.7 and 1.8 can help you with implementing this function)

    // Create an instance of a SceneObject,
    SceneObject sceneObject{};

    // you will need to store offsetX, offsetY, r, g and b in the object.
    sceneObject.r = r;
    sceneObject.g = g;
    sceneObject.b = r;
    sceneObject.x = offsetX;
    sceneObject.y = offsetY;

    // Build the geometry into an std::vector<float> or float array.
    int sections = 20;
    float angle = 0;
    float angleIncrease = 360.0f / sections;
    float radius = sqrt(2 * 2 + 2 * 2);

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    vertices.insert(vertices.end(), { 0 + offsetX, 0 - offsetY, 1.0f});

 
    for (int i = 1; i <= sections; i++)
    {
        angle += angleIncrease;
        float x = cos(glm::radians(angle)) * radius;
        float y = sin(glm::radians(angle)) * radius;

        //std::cout << "x: " + std::to_string(x) + " y: " + std::to_string(y) << std::endl;

        vertices.insert(vertices.end(), { x + offsetX, y - offsetY, -1});
        indices.insert(indices.end(), { (unsigned int)i, 0,  i == sections ? 1 : (unsigned int)i + 1 });
        unsigned int ui = (unsigned int)i;
        //std::cout << std::to_string(ui) + ", 0, " +  std::to_string(i == sections ? 1 : (unsigned int)i + 1) << std::endl;
    }
    

    // Store the number of vertices in the mesh in the scene object.
    sceneObject.vertexCount =  sections * 3;
    // Declare and generate a VAO and VBO (and an EBO if you decide the work with indices).
    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind and set the VAO and VBO (and optionally a EBO) in the correct order.
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLfloat), &indices[0], GL_STATIC_DRAW);

    // Set the position attribute pointers in the shader.
    //int posLocation = glGetAttribLocation(0, "aPos");
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Store the VAO handle in the scene object.
    sceneObject.VAO = VAO;

    // 'return' the scene object for the cone instance you just created.
    return sceneObject;
}

// glfw: called whenever a mouse button is pressed
void button_input_callback(GLFWwindow* window, int button, int action, int mods){
    // TODO exercise 2.2
    // (exercises 1.9 and 2.2 can help you with implementing this function)

    // Test button press, see documentation at:
    //     https://www.glfw.org/docs/latest/input_guide.html#input_mouse_button
    // CODE HERE
    // If a left mouse button press was detected, call instantiateCone:
    // - Push the return value to the back of the global 'vector<SceneObject> sceneObjects'.
    // - The click position should be transformed from screen coordinates to normalized device coordinates,
    //   to obtain the offset values that describe the position of the object in the screen plane.
    // - A random value in the range [0, 1] should be used for the r, g and b variables.
    
    if (button == 0 && action == GLFW_PRESS)
    {
        float r = distr(gen) / 100.0f;
        float g = distr(gen) / 100.0f;
        float b = distr(gen) / 100.0f;

        double x, y;
        glfwGetCursorPos(window, &x, &y);
        x = (float)x / (float)SCR_WIDTH * 2.0f - 1.0f;
        y = (float)y / (float)SCR_HEIGHT * 2.0f- 1.0f;
        sceneObjects.push_back(instantiateCone(r, g, b, x, y));
    }
}

// glfw: called whenever a keyboard key is pressed
void key_input_callback(GLFWwindow* window, int button, int other,int action, int mods){
    // TODO exercise 2.4

    // Set the activeShader variable by detecting when the keys 1, 2 and 3 were pressed;
    // see documentation at https://www.glfw.org/docs/latest/input_guide.html#input_keyboard
    // Key 1 sets the activeShader to &shaderPrograms[0];
    //   and so on.
    // CODE HERE

    if(button ==  GLFW_KEY_1)
        activeShader = &shaderPrograms[0];
    if(button ==  GLFW_KEY_2)
        activeShader = &shaderPrograms[1];
    if(button ==  GLFW_KEY_3)
        activeShader = &shaderPrograms[2];
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}