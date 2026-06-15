#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <map>
#include <string>

// Simple vectors
struct Vec2 {
    float x, y;
    Vec2() : x(0.0f), y(0.0f) {}
    Vec2(float x, float y) : x(x), y(y) {}
};

struct Vec2i {
    int x, y;
    Vec2i() : x(0), y(0) {}
    Vec2i(int x, int y) : x(x), y(y) {}
    bool operator==(const Vec2i &other) const {
        return x == other.x && y == other.y;
    }
};

struct Vec3 {
    float r, g, b;
    Vec3() : r(0.0f), g(0.0f), b(0.0f) {}
    Vec3(float r, float g, float b) : r(r), g(g), b(b) {}
};

// Game constants
const int GRID_WIDTH = 20;
const int GRID_HEIGHT = 20;
const float UPDATE_INTERVAL = 0.15f; //seconds
const float CELL_WIDTH = 2.0f / GRID_WIDTH;
const float CELL_HEIGHT = 2.0f / GRID_HEIGHT;

// Game state
enum class Direction {
    Up,
    Down,
    Left,
    Right,
    None
};

Direction snakeDir = Direction::None;
std::vector<Vec2i> snake = {
    Vec2i(5, 10),
    Vec2i(4, 10),
    Vec2i(3, 10)
};

Vec2i fruit;
int score = 0;
bool gameOver = false;
bool gameStarted = false;
float timeSinceLastUpdate = 0.0f;
float snakeSpeed = UPDATE_INTERVAL;

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

// Bitmap font - each character is 5x5 pixels
const int FONT_WIDTH = 5;
const int FONT_HEIGHT =5;
const int FONT_SPACING = 1;

// Character definitions (0 = empty, 1 = filled)
std::map<char, std::vector<int>> fontMap = {
    {' ', {0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0}},
    {'A', {0,1,1,0,0, 1,0,0,1,0, 1,1,1,1,0, 1,0,0,1,0, 1,0,0,1,0}},
    {'B', {1,1,1,0,0, 1,0,0,1,0, 1,1,1,0,0, 1,0,0,1,0, 1,1,1,0,0}},
    {'C', {0,1,1,1,0, 1,0,0,0,0, 1,0,0,0,0, 1,0,0,0,0, 0,1,1,1,0}},
    {'D', {1,1,1,0,0, 1,0,0,1,0, 1,0,0,1,0, 1,0,0,1,0, 1,1,1,0,0}},
    {'E', {1,1,1,1,0, 1,0,0,0,0, 1,1,1,0,0, 1,0,0,0,0, 1,1,1,1,0}},
    {'F', {1,1,1,1,0, 1,0,0,0,0, 1,1,1,0,0, 1,0,0,0,0, 1,0,0,0,0}},
    {'G', {0,1,1,1,0, 1,0,0,0,0, 1,0,1,1,0, 1,0,0,1,0, 0,1,1,1,0}},
    {'H', {1,0,0,1,0, 1,0,0,1,0, 1,1,1,1,0, 1,0,0,1,0, 1,0,0,1,0}},
    {'I', {1,1,1,0,0, 0,1,0,0,0, 0,1,0,0,0, 0,1,0,0,0, 1,1,1,0,0}},
    {'J', {0,0,1,1,0, 0,0,0,1,0, 0,0,0,1,0, 1,0,0,1,0, 0,1,1,0,0}},
    {'K', {1,0,0,1,0, 1,0,1,0,0, 1,1,0,0,0, 1,0,1,0,0, 1,0,0,1,0}},
    {'L', {1,0,0,0,0, 1,0,0,0,0, 1,0,0,0,0, 1,0,0,0,0, 1,1,1,1,0}},
    {'M', {1,0,0,0,1, 1,1,0,1,1, 1,0,1,0,1, 1,0,0,0,1, 1,0,0,0,1}},
    {'N', {1,0,0,0,1, 1,1,0,0,1, 1,0,1,0,1, 1,0,0,1,1, 1,0,0,0,1}},
    {'O', {0,1,1,0,0, 1,0,0,1,0, 1,0,0,1,0, 1,0,0,1,0, 0,1,1,0,0}},
    {'P', {1,1,1,0,0, 1,0,0,1,0, 1,1,1,0,0, 1,0,0,0,0, 1,0,0,0,0}},
    {'Q', {0,1,1,0,0, 1,0,0,1,0, 1,0,0,1,0, 1,0,1,0,0, 0,1,0,1,0}},
    {'R', {1,1,1,0,0, 1,0,0,1,0, 1,1,1,0,0, 1,0,1,0,0, 1,0,0,1,0}},
    {'S', {0,1,1,1,0, 1,0,0,0,0, 0,1,1,0,0, 0,0,0,1,0, 1,1,1,0,0}},
    {'T', {1,1,1,1,1, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0}},
    {'U', {1,0,0,1,0, 1,0,0,1,0, 1,0,0,1,0, 1,0,0,1,0, 0,1,1,0,0}},
    {'V', {1,0,0,0,1, 1,0,0,0,1, 0,1,0,1,0, 0,1,0,1,0, 0,0,1,0,0}},
    {'W', {1,0,0,0,1, 1,0,0,0,1, 1,0,1,0,1, 1,0,1,0,1, 0,1,0,1,0}},
    {'X', {1,0,0,0,1, 0,1,0,1,0, 0,0,1,0,0, 0,1,0,1,0, 1,0,0,0,1}},
    {'Y', {1,0,0,0,1, 0,1,0,1,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0}},
    {'Z', {1,1,1,1,1, 0,0,0,1,0, 0,0,1,0,0, 0,1,0,0,0, 1,1,1,1,1}},
    {'0', {0,1,1,0,0, 1,0,0,1,0, 1,0,0,1,0, 1,0,0,1,0, 0,1,1,0,0}},
    {'1', {0,0,1,0,0, 0,1,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,1,1,1,0}},
    {'2', {0,1,1,0,0, 1,0,0,1,0, 0,0,1,0,0, 0,1,0,0,0, 1,1,1,1,0}},
    {'3', {1,1,1,0,0, 0,0,0,1,0, 0,1,1,0,0, 0,0,0,1,0, 1,1,1,0,0}},
    {'4', {0,0,1,1,0, 0,1,0,1,0, 1,0,0,1,0, 1,1,1,1,1, 0,0,0,1,0}},
    {'5', {1,1,1,1,0, 1,0,0,0,0, 1,1,1,0,0, 0,0,0,1,0, 1,1,1,0,0}},
    {'6', {0,1,1,0,0, 1,0,0,0,0, 1,1,1,0,0, 1,0,0,1,0, 0,1,1,0,0}},
    {'7', {1,1,1,1,0, 0,0,0,1,0, 0,0,1,0,0, 0,1,0,0,0, 1,0,0,0,0}},
    {'8', {0,1,1,0,0, 1,0,0,1,0, 0,1,1,0,0, 1,0,0,1,0, 0,1,1,0,0}},
    {'9', {0,1,1,0,0, 1,0,0,1,0, 0,1,1,1,0, 0,0,0,1,0, 0,1,1,0,0}},
    {':', {0,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0}},
    {'-', {0,0,0,0,0, 0,0,0,0,0, 1,1,1,1,0, 0,0,0,0,0, 0,0,0,0,0}},
    {'.', {0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,0}}
};

// Function declarations
void spawnFruit();
void initGame();
void resetGame();
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void drawCell(const Vec2i &position, const Vec3 &color);
void drawChar(char c, float x, float y, float scale, const Vec3 &color);
void drawText(const std::string &text, float x, float y, float scale, const Vec3 &color);
void renderGame(GLFWwindow *window);
void updateGame(float deltaTime);
void drawBorder();
void drawSnake();
void drawScore();
void drawGameOver();
void drawStartScreen();

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
    GLFWwindow *window = glfwCreateWindow(800, 800, "Snake Game", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    //initialise glew
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // compile shaders
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
        std::cerr << "ERROR:VERTEX_SHADER_COMPILATION_FAILED" << infoLog << std::endl;
        glfwTerminate();
        return -1;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderCStr = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderCStr, nullptr);
    glCompileShader(fragmentShader);

    //check fragment shader compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "ERROR:FRAGMENT_SHADER_COMPILATION_FAILED" << infoLog << std::endl;
        glfwTerminate();
        return -1;
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
        std::cerr << "ERROR:PROGRAM_LINKING_FAILED" << infoLog << std::endl;
        glfwTerminate();
        return -1;
    }

    //cleanup shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //get uniform locations
    uOffsetLoc = glGetUniformLocation(shaderProgram, "uOffset");
    uScaleLoc = glGetUniformLocation(shaderProgram, "uScale");
    uColorLoc = glGetUniformLocation(shaderProgram, "uColor");

    //setup quad VAO
    const float vertices[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        -0.5f, 0.5f,
        0.5f, 0.5f
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
        updateGame(deltaTime);

        //render game
        renderGame(window);
    }

    //cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void initGame() {
    resetGame();
    spawnFruit();
}

void resetGame() {
    snake = {
        Vec2i(5, 10), Vec2i(4, 10), Vec2i(3, 10)
    };
    snakeDir = Direction::None;
    gameOver = false;
    gameStarted = false;
    score = 0;
    timeSinceLastUpdate = 0.0f;
    snakeSpeed = UPDATE_INTERVAL;
}

void spawnFruit() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distX(0, GRID_WIDTH - 1);
    std::uniform_int_distribution<> distY(0, GRID_HEIGHT - 1);

    while (true) {
        Vec2i newFruit(distX(gen), distY(gen));

        //ensure fruit doesnt spawn on snake
        bool validPos = true;
        for (const auto& segment : snake) {
            if (segment == newFruit) {
                validPos = false;
                break;
            }
        }

        if (validPos) {
            fruit = newFruit;
            break;
        }
    }
}

void updateGame(float deltaTime) {
    if (gameStarted && !gameOver) {
        //update game timer
        timeSinceLastUpdate += deltaTime;

        //game update
        if (timeSinceLastUpdate >= snakeSpeed) {
            timeSinceLastUpdate = 0.0f;

            //move snake
            Vec2i newHead = snake.front();

            switch (snakeDir) {
                case Direction::Left:
                    newHead.x--;
                    break;
                case Direction::Right:
                    newHead.x++;
                    break;
                case Direction::Up:
                    newHead.y++;
                    break;
                case Direction::Down:
                    newHead.y--;
                    break;
                case Direction::None:
                    return; //do nothing
            }

            //check collision
            //wall collision
            if (newHead.x < 0 || newHead.x >= GRID_WIDTH || newHead.y < 0 || newHead.y >= GRID_HEIGHT) {
                gameOver = true;
                return;
            }

            //self collision
            for (const auto& segment : snake) {
                if (segment == newHead) {
                    gameOver = true;
                    return;
                }
            }

            //add new head
            snake.insert(snake.begin(), newHead);

            //check fruit collision
            if (newHead == fruit) {
                score += 10;
                spawnFruit();

                //increase speed every 5 fruits
                if (score % 50 == 0 && snakeSpeed > 0.05f) {
                    snakeSpeed -= 0.01f;
                }
            } else {
                //remove tail
                snake.pop_back();
            }
        }
    }
}

void renderGame(GLFWwindow *window) {
    //set background color
    glClearColor(0.08f, 0.1f, 0.12f, 1.0f); //dark blueish gray background
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    //draw game border
    drawBorder();

    if (!gameStarted) {
        drawStartScreen();
    } else if (gameOver) {
        drawGameOver();
    } else {
        drawSnake();
        drawScore();
    }

    glBindVertexArray(0);

    //swap buffers
    glfwSwapBuffers(window);
}

void drawCell(const Vec2i &position, const Vec3 &color) {
    //calculate pos in NDC [-1, 1]
    Vec2 offset(
        -1.0 + position.x * CELL_WIDTH + CELL_WIDTH * 0.5f,
        -1.0 + position.y * CELL_HEIGHT + CELL_HEIGHT * 0.5f
    );

    //apply scaling to fit cell
    Vec2 scale(CELL_WIDTH * 0.9f, CELL_HEIGHT * 0.9f);  //slightly smaller for grid effect

    //draw cell
    glUniform3f(uColorLoc, color.r, color.g, color.b);
    glUniform2f(uOffsetLoc, offset.x, offset.y);
    glUniform2f(uScaleLoc, scale.x, scale.y);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void drawStartScreen() {
    //draw a snake and a fruit
    drawSnake();

    //draw title
    drawText("Snake Game", 0.0f, 0.3f, 0.025f, Vec3(0.2f, 0.8f, 0.3f));

    //draw instructions
    drawText("USE ARROW KEYS TO MOVE", 0.0f, 0.0f, 0.012f, Vec3(0.9f, 0.9f, 0.9f));
    drawText("EAT THE RED FRUIT TO GROW", 0.0f, -0.1f, 0.012f, Vec3(0.9f, 0.9f, 0.9f));
    drawText("AVOID WALLS AND YOURSELF", 0.0f, -0.2f, 0.012f, Vec3(0.9f, 0.9f, 0.9f));
    drawText("PRESS ANY KEY TO START", 0.0f, -0.4f, 0.012f, Vec3(0.8f, 0.8f, 0.2f));
}

void drawGameOver() {
    // Draw semi-transparent overlay
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0 ; y < GRID_HEIGHT; y++) {
            drawCell(Vec2i(x, y), Vec3(0.2f, .1f, 0.1f));
        }
    }

    // Draw game over text
    drawText("GAME OVER", 0.0f, 0.1f, 0.03f, Vec3(1.0f, 0.3f, 0.3f));
    drawText("SCORE: " + std::to_string(score), 0.0f, -0.05f, 0.02f, Vec3(1.0f, 1.0f, 1.0f));
    drawText("PRESS R TO RESTART", 0.0f, -0.2f, 0.015f, Vec3(0.8f, 0.8f, 0.8f));
}

void drawChar(char c, float x, float y, float scale, const Vec3 &color) {
    // Convert to uppercase
    c = std::toupper(c);

    // Find character in font map
    auto it = fontMap.find(c);
    if (it == fontMap.end()) {
        // Default to space if character not found
        it = fontMap.find(' ');
    }

    const std::vector<int>& bitmap = it->second;

    // Draw each pixel of the character
    float charWidth = FONT_WIDTH * scale;
    float charHeight = FONT_HEIGHT * scale;

    for (int i = 0; i < FONT_HEIGHT; i++) {
        for (int j = 0; j < FONT_WIDTH; j++) {
            if (bitmap[i * FONT_WIDTH + j]) {
                Vec2 offset(
                    x + j * scale - charWidth / 2.0f,
                    y - i * scale + charHeight / 2.0f
                );

                glUniform3f(uColorLoc, color.r, color.g, color.b);
                glUniform2f(uOffsetLoc, offset.x, offset.y);
                glUniform2f(uScaleLoc, scale, scale);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            }
        }
    }
}

void drawText(const std::string &text, float x, float y, float scale, const Vec3 &color) {
    float charWidth = FONT_WIDTH * scale;
    float spacing = FONT_SPACING * scale;
    float totalWidth = text.size() * (charWidth + spacing) - spacing;

    float startX = x - totalWidth / 2.0f;

    for (size_t i = 0; i < text.size(); i++) {
        drawChar(text[i], startX + i * (charWidth + spacing), y, scale, color);
    }
}

void drawSnake() {
    // Draw snake
    Vec3 headColor(0.0f, 0.95f, 0.3f); // Bright green for head
    Vec3 bodyColor(0.0f, 0.7f, 0.1f); // Darker green for body

    // Draw snake body
    for (size_t i = 1; i < snake.size(); i++) {
        // Create a gradient effect for the snake body
        float factor = static_cast<float>(i) / snake.size();
        Vec3 segmentColor(
            bodyColor.r * (1.0f - factor) + 0.1f * factor,
            bodyColor.g * (1.0f - factor) + 0.8f * factor,
            bodyColor.b * (1.0f - factor)
        );
        drawCell(snake[i], segmentColor);
    }

    // Draw snake head
    drawCell(snake[0], headColor);

    // Draw fruit
    drawCell(fruit, Vec3(1.0f, 0.3f, 0.3f)); // Red fruit
}

void drawScore() {
    //draw score at the top
    std::string scoreText = "Score: " + std::to_string(score);
    drawText(scoreText, 0.0f, 0.9f, 0.02f, Vec3(0.9f, 0.9f, 0.9f));
}

void drawBorder() {
    // Draw border around the game area
    Vec3 borderColor(0.3f, 0.3f, 0.5f);

    // Top border
    for (int x =- 1; x <= GRID_WIDTH; x++) {
        drawCell(Vec2i(x, GRID_HEIGHT), borderColor);
    }

    // Bottom border
    for (int x =- 1; x <= GRID_WIDTH; x++) {
        drawCell(Vec2i(x, -1), borderColor);
    }

    // Left border
    for (int y =- 1; y <= GRID_HEIGHT; y++) {
        drawCell(Vec2i(-1, y), borderColor);
    }

    // Right border
    for (int y =- 1; y <= GRID_HEIGHT; y++) {
        drawCell(Vec2i(GRID_WIDTH, y), borderColor);
    }

    // Draw grid lines
    Vec3 gridColor(0.15f, .17f, 0.2f);
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            if ((x + y) % 2 == 0) { drawCell(Vec2i(x, y), gridColor); }
        }
    }
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (!gameStarted && key != GLFW_KEY_R) {
            gameStarted = true;
            snakeDir = Direction::Right;
            return;
        }

        if (gameOver && key == GLFW_KEY_R) {
            resetGame();
            spawnFruit();
            return;
        }

        if (!gameOver && gameStarted) {
            switch (key) {
                case GLFW_KEY_UP: {
                    if (snakeDir != Direction::Down) snakeDir = Direction::Up;
                }
                break;
                case GLFW_KEY_DOWN: {
                    if (snakeDir != Direction::Up) snakeDir = Direction::Down;
                }
                break;
                case GLFW_KEY_LEFT: {
                    if (snakeDir != Direction::Right) snakeDir = Direction::Left;
                }
                break;
                case GLFW_KEY_RIGHT: {
                    if (snakeDir != Direction::Left) snakeDir = Direction::Right;
                }
                break;
                default:
                    break;
            }
        }
    }
}
