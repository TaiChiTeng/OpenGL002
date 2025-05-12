#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <learnopengl/shader_s.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 720;
const unsigned int SCR_HEIGHT = 1280;

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "DrawBg&Star", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader shaderProgram("2.4.shader.vs", "2.5.shader.fs");
    // 在main函数中初始化后添加
    Shader starShader("star.vs", "star.fs");
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // 位置              // 颜色
         1.0f,  1.0f, 0.0f, 0.188f, 0.208f, 0.231f,  // 右上
         1.0f, -1.0f, 0.0f, 0.416f, 0.518f, 0.569f,  // 右下
        -1.0f, -1.0f, 0.0f, 0.416f, 0.518f, 0.569f,  // 左下
        -1.0f,  1.0f, 0.0f, 0.188f, 0.208f, 0.231f   // 左上
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // 星星
    const int numStars = 100;
    struct Star {
        glm::vec3 position;
        float size;
        // float phase;    // 初始相位（0~2π）
        float frequency;// 闪烁频率（Hz）
    };
    
    Star stars[numStars];
    std::srand(static_cast<unsigned>(time(nullptr)));
    float aspect = (float)SCR_WIDTH / (float)SCR_HEIGHT;

    const float maxHorizontal = 1.0f;
    const float extraSpread = 0.2f;
    
    for (int i = 0; i < numStars; ++i) {
        // stars[i].position.x = ((rand() % 2000) / 1000.0f - 1.0f) * aspect;
        stars[i].position.x = ((rand() % 4000) / 2000.0f - 1.0f) * (maxHorizontal + extraSpread);
        stars[i].position.y = (rand() % 2000) / 1000.0f - 1.0f;
        stars[i].position.z = -0.1f;
        stars[i].size = (rand() % 100) / 100.0f * 2.5f + 1.5f; // 1.5-4.
        // stars[i].phase = static_cast<float>(rand() % 1000) / 1000.0f * 6.2832f; // 0~2π
        stars[i].frequency = (rand() % 100) / 100.0f * 1.0f + 0.5f; // 0.5~1.5 Hz
    }

    unsigned int starVAO, starVBO;
    glGenVertexArrays(1, &starVAO);
    glGenBuffers(1, &starVBO);
    glBindVertexArray(starVAO);
    glBindBuffer(GL_ARRAY_BUFFER, starVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(stars), stars, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Star), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Star), (void*)offsetof(Star, size));
    glEnableVertexAttribArray(1);
    // 绑定starVAO时添加以下属性
    // glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Star), (void*)offsetof(Star, phase));
    // glEnableVertexAttribArray(2);
    // glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Star), (void*)offsetof(Star, frequency));
    // glEnableVertexAttribArray(3);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Star), (void*)offsetof(Star, frequency));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // 在main函数中声明时间变量
    float lastFrameTime = 0.0f;
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // 计算时间差值
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        // input
        // -----
        processInput(window);

        // render
        glClearColor(0.718f, 0.318f, 0.114f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        starShader.use();
        starShader.setFloat("time", currentTime); // 传递时间
        glBindVertexArray(starVAO);
        glDrawArrays(GL_POINTS, 0, numStars);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &starVAO);
    glDeleteBuffers(1, &starVBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}