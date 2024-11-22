#include <iostream>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // to ease the calculation of the view and projection matrices
#include <glm/gtc/type_ptr.hpp>         // for converting a matrix object into a float array for usage in OpenGL

class OpenGLCube {
public:
    enum class CubeMode { AUTO, MANUAL, MATRIX };

    OpenGLCube(sf::Window &window);
    ~OpenGLCube();

    void handle_events();
    void update();

    bool is_running() { return running; }
    void set_mode(CubeMode mode) { this->mode = mode; }
    sf::Window &get_window() { return window; }
    glm::mat4 get_model() { return model; }
    void reset_model() { model = glm::mat4(1.0f); }

private:
    void init_vertex_input();
    void init_shaders();
    void init_texture();
    void init_transformations();

    sf::Window &window;

    float vertices[128] = { // better square (glDrawElements)
        //  X     Y     Z      R     G     B     S     T    Position - Color - Texcoords
            -.5f, -.5f, -.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // black    0
             .5f, -.5f, -.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // blue     0
             .5f,  .5f, -.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // cyan     0
            -.5f,  .5f, -.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // green    0
            -.5f, -.5f,  .5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // red      0
             .5f, -.5f,  .5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, // magenta  0
             .5f,  .5f,  .5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // white    0
            -.5f,  .5f,  .5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, // yellow   0
            -.5f,  .5f,  .5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, // yellow   1
            -.5f,  .5f, -.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // green    1
            -.5f, -.5f, -.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // black    1
             .5f,  .5f,  .5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // white    1
             .5f, -.5f, -.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // blue     1
             .5f, -.5f,  .5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // magenta  1
             .5f, -.5f, -.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // blue     1
            -.5f,  .5f,  .5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // yellow   1
    };

    GLuint elements[36] = {
        0,  1,  2,    2,  3,  0,
        4,  5,  6,    6,  7,  4,
        8,  9,  10,   10, 4,  8,
        11, 2,  12,   12, 13, 11,
        10, 14, 5,    5,  4,  10,
        3,  2,  11,   11, 15, 3,
    };

    // vertex array object, vertex buffer object, element buffer object
    GLuint vao, vbo, ebo;
    GLuint vertexShader, fragmentShader, shaderProgram, texture;
    GLint posAttrib, colAttrib, texAttrib;
    sf::Image image;

    glm::vec3 
        xPivot = glm::vec3(1.0f, 0.0f, 0.0f),
        yPivot = glm::vec3(0.0f, 1.0f, 0.0f),
        zPivot = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::mat4 model, view, proj;
    GLint uniTrans, uniView, uniProj;

    bool running = true;
    CubeMode mode = CubeMode::AUTO;
    float dx = 0.0f, dy = 0.0f, dz = 0.0f;
    float delta_angle = 0.01f;
    float ratio_perspective;
    unsigned int animation_frames = 240;
    unsigned int animation_counter = 0;
    
    // VERTEX SHADER. mandatory output: final vertex position in device coordinates and any data the fragment shader requires
    const char* vertexSource = R"glsl(
        #version 150 core

        in vec3 position;
        in vec3 color;
        in vec2 texcoord;

        out vec3 Color;
        out vec2 Texcoord;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 proj;

        void main()
        {
            Color = color;
            Texcoord = texcoord;
            gl_Position = proj * view * model * vec4(position, 1.0);
        }
    )glsl";

    // FRAGMENT SHADER. mandatory output: final color of a fragment
    const char* fragmentSource = R"glsl(
        #version 150 core

        in vec3 Color;
        in vec2 Texcoord;

        out vec4 outColor;

        uniform sampler2D texCats;

        void main()
        {
            outColor = texture(texCats, Texcoord) * vec4(Color, 1.0);
        }
    )glsl";
};
