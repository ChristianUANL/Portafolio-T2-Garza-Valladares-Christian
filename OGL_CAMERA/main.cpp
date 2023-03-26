#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <engine/shader_m.h>
#include <engine/camera.h>
#include <iostream>

using namespace std;
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float roll = 0.0f;
float lastZ = 1.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// timing
float deltaTime = 0.0f;
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
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
    Shader ourShader("camera.vs", "camera.fs");


    float textCords[6][12]{
        {
            //FACE 1
            0.0f, 0.34f,
            0.25f, 0.34f,
            0.25f, 0.66f,
            0.25f, 0.66f,
            0.0f, 0.66f,
            0.0f, 0.34f
        },

        {
            //FACE 2
            0.25f, 0.34f,
            0.5f, 0.34f,
            0.5f, 0.66f,
            0.5f, 0.66f,
            0.25f, 0.66f,
            0.25f, 0.34f
        },

        {
            //FACE 3
            0.25f, 0.66f,
            0.5f, 0.66f,
            0.5f, 1.0f,
            0.5f, 1.0f,
            0.25f, 1.0f,
            0.25f, 0.66f
        },
        {
            //FACE 4
            0.25f, 0.0f,
            0.5f, 0.0f,
            0.5f, 0.34f,
            0.5f, 0.34f,
            0.25f, 0.34f,
            0.25f, 0.0f
        },

        {
            //FACE 5
            0.5f, 0.34f,
            0.75f, 0.34f,
            0.75f, 0.66f,
            0.75f, 0.66f,
            0.5f, 0.66f,
            0.5f, 0.34f,
        },

        {
            //FACE 6
            0.75f, 0.34f,
            1.0f, 0.34f,
            1.0f, 0.66f,
            1.0f, 0.66f,
            0.75f, 0.66f,
            0.75f, 0.34f
            }
    };
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        //FACE 1
        -0.5f, -0.5f, -0.5f, textCords[0][0],textCords[0][1],
        0.5f, -0.5f, -0.5f, textCords[0][2],textCords[0][3],
        0.5f, 0.5f, -0.5f, textCords[0][4],textCords[0][5],
        0.5f, 0.5f, -0.5f, textCords[0][6],textCords[0][7],
        -0.5f, 0.5f, -0.5f, textCords[0][8],textCords[0][9],
        -0.5f, -0.5f, -0.5f, textCords[0][10],textCords[0][11],

        //FACE 2
        -0.5f, -0.5f, 0.5f, textCords[1][0],textCords[1][1],
        0.5f, -0.5f, 0.5f, textCords[1][2],textCords[1][3],
        0.5f, 0.5f, 0.5f, textCords[1][4],textCords[1][5],
        0.5f, 0.5f, 0.5f, textCords[1][6],textCords[1][7],
        -0.5f, 0.5f, 0.5f, textCords[1][8],textCords[1][9],
        -0.5f, -0.5f, 0.5f, textCords[1][10],textCords[1][11],

        //FACE 3
        -0.5f, 0.5f, 0.5f, textCords[2][0],textCords[2][1],
        -0.5f, 0.5f, -0.5f, textCords[2][2],textCords[2][3],
        -0.5f, -0.5f, -0.5f, textCords[2][4],textCords[2][5],
        -0.5f, -0.5f, -0.5f, textCords[2][6],textCords[2][7],
        -0.5f, -0.5f, 0.5f, textCords[2][8],textCords[2][9],
        -0.5f, 0.5f, 0.5f, textCords[2][10],textCords[2][11],

        //FACE 4
        0.5f, 0.5f, 0.5f, textCords[3][0],textCords[3][1],
        0.5f, 0.5f, -0.5f, textCords[3][2],textCords[3][3],
        0.5f, -0.5f, -0.5f, textCords[3][4],textCords[3][5],
        0.5f, -0.5f, -0.5f, textCords[3][6],textCords[3][7],
        0.5f, -0.5f, 0.5f, textCords[3][8],textCords[3][9],
        0.5f, 0.5f, 0.5f, textCords[3][10],textCords[3][11],

        //FACE 5
        -0.5f, -0.5f, -0.5f, textCords[4][0],textCords[4][1],
        0.5f, -0.5f, -0.5f, textCords[4][2],textCords[4][3],
        0.5f, -0.5f, 0.5f, textCords[4][4],textCords[4][5],
        0.5f, -0.5f, 0.5f, textCords[4][6],textCords[4][7],
        -0.5f, -0.5f, 0.5f, textCords[4][8],textCords[4][9],
        -0.5f, -0.5f, -0.5f, textCords[4][10],textCords[4][11],

        //FACE 6
        -0.5f, 0.5f, -0.5f, textCords[5][0],textCords[5][1],
        0.5f, 0.5f, -0.5f, textCords[5][2],textCords[5][3],
        0.5f, 0.5f, 0.5f, textCords[5][4],textCords[5][5],
        0.5f, 0.5f, 0.5f, textCords[5][6],textCords[5][7],
        -0.5f, 0.5f, 0.5f, textCords[5][8],textCords[5][9],
        -0.5f, 0.5f, -0.5f, textCords[5][10],textCords[5][11]
    };

    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f)};
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // load and create a texture
    // -------------------------
    unsigned int texture1, texture2;
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
    unsigned char *data = stbi_load("c:\\gl\\gl\\resources\\textures\\cubo_rubick.jpg", &width, &height, &nrChannels, 0);
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
    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("c:\\gl\\gl\\resources\\textures\\awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
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
    ourShader.setInt("texture2", 1);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // activate shader
        ourShader.use();
        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader.setMat4("view", view);

        // render boxes
        glBindVertexArray(VAO);
      
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 0.0f));


            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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

    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        roll -= lastZ;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        roll += lastZ;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
         pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));//cos(glm::radians(roll)
    front.y = sin(glm::radians(pitch));//sin(glm::radians(roll))
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}
