#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <string>

// Simple vectors
struct Vec2 {
    float x, y;
    Vec2() : x(0.0f), y(0.0f) {}
    Vec2(float x, float y) : x(x), y(y) {}
};

struct Vec3 {
    float r, g, b;
    Vec3() : r(0.0f), g(0.0f), b(0.0f) {}
    Vec3(float r, float g, float b) : r(r), g(g), b(b) {}
};

// Object data
struct Object {
    Vec2 position;
    Vec2 basePosition;
    Vec3 color;
    float size;
    float speed;
    float phase;

    Object() :
        position(Vec2()),
        basePosition(Vec2()),
        color(Vec3()),
        size(0.1f),
        speed(1.0f),
        phase(0.0f) {}

    Object(Vec2 position, Vec3 color, float size, float speed, float phase) :
        position(position),
        basePosition(position),
        color(color),
        size(size),
        speed(speed),
        phase(phase) {}
};

// Game constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const float PLAYER_SPEED = 1.2f;

// Game state
Object player;
std::vector<Object> objects;
float totalTime = 0.0f;

// Shader sources
std::string vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;

    uniform vec2 uOffset;
    uniform vec2 uScale;

    void main() {
        vec2 position = (aPos * uScale) + uOffset;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)";

std::string fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    uniform vec3 uColor;

    void main() {
        FragColor = vec4(uColor, 1.0);
    }
)";

// OpenGL Objects
GLuint shaderProgram;
GLuint VAO, VBO;
GLuint uOffsetLoc, uScaleLoc, uColorLoc;

// Function declarations
bool checkGLError(const std::string &message);
bool createShaders();
void initGame();
void updateGame(GLFWwindow *window, float deltaTime);
void renderGame(GLFWwindow *window);
void drawObject(const Object &object);
void drawBackground();
void drawBorder();
void cleanup();

int main() {
    //initialise glfw
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    //configure glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    //create window
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Animated Objects Demo", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    //initialise glew
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    //clear old glew error
    glGetError();

    //create shaders
    if (!createShaders()) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    //setup quad VAO
    const float vertices[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
        -0.5f,  0.5f,
         0.5f,  0.5f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    if (!checkGLError("OpenGL setup")) {
        cleanup();
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    //init game
    initGame();

    //game loop
    auto lastTime = std::chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(window)) {
        //calc delta time
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        //process inputs
        glfwPollEvents();

        //update gamestate
        updateGame(window, deltaTime);

        //render game
        renderGame(window);
    }

    //cleanup
    cleanup();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

bool createShaders() {
    // compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderCStr = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderCStr, nullptr);
    glCompileShader(vertexShader);

    //check vertex shader compilation
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "ERROR:VERTEX_SHADER_COMPILATION_FAILED " << infoLog << std::endl;
        glDeleteShader(vertexShader);
        return false;
    }

    // compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderCStr = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderCStr, nullptr);
    glCompileShader(fragmentShader);

    //check fragment shader compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "ERROR:FRAGMENT_SHADER_COMPILATION_FAILED " << infoLog << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }

    //create shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //check shader program linking
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR:PROGRAM_LINKING_FAILED " << infoLog << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(shaderProgram);
        return false;
    }

    //cleanup shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //get uniform locations
    uOffsetLoc = glGetUniformLocation(shaderProgram, "uOffset");
    uScaleLoc = glGetUniformLocation(shaderProgram, "uScale");
    uColorLoc = glGetUniformLocation(shaderProgram, "uColor");

    return checkGLError("Shader creation");
}

void initGame() {
    //player object
    player = Object(
        Vec2(0.0f, 0.0f),
        Vec3(0.2f, 0.8f, 1.0f),
        0.12f,
        0.0f,
        0.0f
    );

    //at least 5 visible animated objects
    objects.clear();
    objects.push_back(Object(Vec2(-0.65f,  0.55f), Vec3(1.0f, 0.25f, 0.25f), 0.10f, 1.4f, 0.0f));
    objects.push_back(Object(Vec2( 0.65f,  0.55f), Vec3(0.3f, 1.0f, 0.35f), 0.10f, 1.8f, 1.0f));
    objects.push_back(Object(Vec2(-0.65f, -0.55f), Vec3(1.0f, 0.8f, 0.2f), 0.10f, 1.2f, 2.0f));
    objects.push_back(Object(Vec2( 0.65f, -0.55f), Vec3(0.8f, 0.3f, 1.0f), 0.10f, 1.6f, 3.0f));
    objects.push_back(Object(Vec2( 0.0f,  0.70f), Vec3(1.0f, 0.5f, 0.1f), 0.10f, 2.0f, 4.0f));
}

void updateGame(GLFWwindow *window, float deltaTime) {
    totalTime += deltaTime;

    //close window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    //input-driven player movement
    Vec2 movement(0.0f, 0.0f);

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        movement.x -= PLAYER_SPEED * deltaTime;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        movement.x += PLAYER_SPEED * deltaTime;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        movement.y += PLAYER_SPEED * deltaTime;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        movement.y -= PLAYER_SPEED * deltaTime;
    }

    player.position.x += movement.x;
    player.position.y += movement.y;

    //keep player inside screen
    if (player.position.x < -0.9f) player.position.x = -0.9f;
    if (player.position.x >  0.9f) player.position.x =  0.9f;
    if (player.position.y < -0.9f) player.position.y = -0.9f;
    if (player.position.y >  0.9f) player.position.y =  0.9f;

    //object animation updates every frame
    for (size_t i = 0; i < objects.size(); i++) {
        Object &object = objects[i];

        float waveX = std::cos(totalTime * object.speed + object.phase) * 0.12f;
        float waveY = std::sin(totalTime * object.speed + object.phase) * 0.12f;

        object.position.x = object.basePosition.x + waveX;
        object.position.y = object.basePosition.y + waveY;

        object.size = 0.09f + std::sin(totalTime * 2.0f + object.phase) * 0.025f;
    }
}

void renderGame(GLFWwindow *window) {
    //set background color
    glClearColor(0.08f, 0.1f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    //draw scene
    drawBackground();
    drawBorder();

    //draw animated objects
    for (const auto& object : objects) {
        drawObject(object);
    }

    //draw player last
    drawObject(player);

    glBindVertexArray(0);

    //swap buffers
    glfwSwapBuffers(window);
}

void drawObject(const Object &object) {
    //draw object
    glUniform3f(uColorLoc, object.color.r, object.color.g, object.color.b);
    glUniform2f(uOffsetLoc, object.position.x, object.position.y);
    glUniform2f(uScaleLoc, object.size, object.size);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void drawBackground() {
    //draw simple checker style background
    Object tile;
    tile.size = 0.08f;
    tile.color = Vec3(0.12f, 0.14f, 0.17f);

    for (int x = -10; x <= 10; x++) {
        for (int y = -10; y <= 10; y++) {
            if ((x + y) % 2 == 0) {
                tile.position = Vec2(x * 0.1f, y * 0.1f);
                drawObject(tile);
            }
        }
    }
}

void drawBorder() {
    //draw border around play area
    Object border;
    border.color = Vec3(0.3f, 0.3f, 0.5f);

    //top border
    border.size = 0.04f;
    for (int i = -10; i <= 10; i++) {
        border.position = Vec2(i * 0.1f, 0.98f);
        drawObject(border);
    }

    //bottom border
    for (int i = -10; i <= 10; i++) {
        border.position = Vec2(i * 0.1f, -0.98f);
        drawObject(border);
    }

    //left border
    for (int i = -10; i <= 10; i++) {
        border.position = Vec2(-0.98f, i * 0.1f);
        drawObject(border);
    }

    //right border
    for (int i = -10; i <= 10; i++) {
        border.position = Vec2(0.98f, i * 0.1f);
        drawObject(border);
    }
}

bool checkGLError(const std::string &message) {
    GLenum error = glGetError();

    if (error != GL_NO_ERROR) {
        std::cerr << "OPENGL ERROR at " << message << ": " << error << std::endl;
        return false;
    }

    return true;
}

void cleanup() {
    //cleanup opengl objects
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}