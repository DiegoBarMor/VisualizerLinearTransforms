#include <iostream>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // to ease the calculation of the view and projection matrices
#include <glm/gtc/type_ptr.hpp> // for converting a matrix object into a float array for usage in OpenGL

#include "include/ManagerImpl.hpp"

int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 2; // Optional
    settings.majorVersion = 3;
    settings.minorVersion = 2;
    settings.attributeFlags = sf::ContextSettings::Core;

    sf::RenderWindow window_gui(sf::VideoMode(800, 600), "GUI", sf::Style::Close);
    sf::Window window_cube(sf::VideoMode(800, 600), "OpenGL", sf::Style::Close, settings);

    ManagerImpl gui = ManagerImpl(window_gui);
    gui.setup("gui.ndg");

    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    /********** VERTEX INPUT **********/
    float vertices[] = { // better square (glDrawElements)
        //  X     Y     Z      R     G     B     S     T    Position - Color - Texcoords
            -.5f, -.5f, -.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // black    0
             .5f, -.5f, -.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // blue     0
             .5f,  .5f, -.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, // cyan     0
            -.5f,  .5f, -.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // green    0
            -.5f, -.5f,  .5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // red      0
             .5f, -.5f,  .5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, // magenta  0
             .5f,  .5f,  .5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // white    0
            -.5f,  .5f,  .5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // yellow   0
            -.5f,  .5f,  .5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // yellow   1
            -.5f,  .5f, -.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // green    1
            -.5f, -.5f, -.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // black    1
             .5f,  .5f,  .5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // white    1
             .5f, -.5f, -.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // blue     1
             .5f, -.5f,  .5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // magenta  1
             .5f, -.5f, -.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // blue     1
            -.5f,  .5f,  .5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // yellow   1
    };

    GLuint elements[] = {
        0,  1,  2,    2,  3,  0,
        4,  5,  6,    6,  7,  4,
        8,  9,  10,   10, 4,  8,
        11, 2,  12,   12, 13, 11,
        10, 14, 5,    5,  4,  10,
        3,  2,  11,   11, 15, 3,
    };

    // VERTEX ARRAY OBJECTS
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // VERTEX BUFFER OBJECTS
    GLuint vbo; // The next step is to upload this vertex data to the graphics card.
    glGenBuffers(1, &vbo); // This is done by creating a Vertex Buffer Object (VBO)
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // To upload the actual data to it you first have to make it the active object
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Now that it's active we can copy the vertex data to it.
    /*
        GL_STATIC_DRAW : The vertex data will be uploaded onceand drawn many times (e.g.the world).
        GL_DYNAMIC_DRAW : The vertex data will be created once, changed from time to time, but drawn many times more than that.
        GL_STREAM_DRAW : The vertex data will be uploaded onceand drawn once.
    */

    // ELEMENT BUFFER OBJECTS
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    /********** SHADERS **********/

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

        uniform sampler2D texKitten;

        void main()
        {
            outColor = vec4(Color, 1.0);
            outColor = texture(texKitten, Texcoord) * vec4(Color, 1.0);
        }
    )glsl";

    // COMPILING SHADERS
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // creating a shader itself starts with creating a shader object and loading data into it.
    glShaderSource(vertexShader, 1, &vertexSource, NULL); // Unlike VBOs, you can simply pass a reference to shader functions instead of making it active
    glCompileShader(vertexShader); // All that's left is compiling the shader into code that can be executed by the graphics card now

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // COMBINING SHADERS INTO A PROGRAM
    GLuint shaderProgram = glCreateProgram(); // Up until now the vertex and fragment shaders have been two separate objects.
    glAttachShader(shaderProgram, vertexShader); // While they've been programmed to work together, they aren't actually connected yet.
    glAttachShader(shaderProgram, fragmentShader); // This connection is made by creating a program out of these two shaders.

    /*
    Since a fragment shader is allowed to write to multiple framebuffers, you need to explicitly specify
    which output is written to which framebuffer. This needs to happen before linking the program. However,
    since this is 0 by default and there's only one output right now, the following line of code is not necessary:
    // glBindFragDataLocation(shaderProgram, 0, "outColor");
    */

    glLinkProgram(shaderProgram); // After attaching both the fragment and vertex shaders, the connection is made by linking the program.
    glUseProgram(shaderProgram); // To actually start using the shaders in the program, you just have to call.
    // Just like a vertex buffer, only one program can be active at a time.

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position"); // OpenGL still doesn't know how the attributes are formatted and ordered. With the reference
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0); // to the input, you can specify how the data for that input is retrieved from the array
    glEnableVertexAttribArray(posAttrib); // Last, but not least, the vertex attribute array needs to be enabled.

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(colAttrib);

    /********** TEXTURES **********/
    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(texAttrib);

    GLuint textures[1];
    glGenTextures(1, textures);
    sf::Image image;

    image.loadFromFile("cats.png");
    glActiveTexture(GL_TEXTURE0); // specifies which texture unit a texture object is bound to when glBindTexture is called
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glUniform1i(glGetUniformLocation(shaderProgram, "texKitten"), 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    /********** TRANSFORMATIONS **********/
    glm::mat4 identity = glm::mat4(1.0f);
    glm::vec3 xPivot = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 yPivot = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 zPivot = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;

    GLint uniTrans = glGetUniformLocation(shaderProgram, "model");
    GLint uniView = glGetUniformLocation(shaderProgram, "view");
    GLint uniProj = glGetUniformLocation(shaderProgram, "proj");

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // MAIN LOOP
    sf::Clock clock;
    bool running = true;
    float dx = 0.0f, dy = 0.0f, dz = 0.0f;
    float anglex = 0.0f, angley = 0.0f, anglez = 0.0f;
    float delta_angle = 0.001f;

    while (running) {
        window_gui.setActive();
        gui.manage_events();
        window_gui.clear(sf::Color::Black);
        gui.draw();
        window_gui.display();

        window_cube.setActive();
        sf::Event windowEvent;
        while (window_cube.pollEvent(windowEvent)) {
            switch (windowEvent.type) {
            case sf::Event::Closed:
                running = false;
                break;
            case sf::Event::KeyPressed:
                switch (windowEvent.key.code) {
                case sf::Keyboard::Escape:
                    running = false;
                    break;
                case sf::Keyboard::Numpad2:
                    dy = -delta_angle;
                    break;
                case sf::Keyboard::Numpad4:
                    dz = -delta_angle;
                    break;
                case sf::Keyboard::Numpad6:
                    dz = delta_angle;
                    break;
                case sf::Keyboard::Numpad7:
                    dx = -delta_angle;
                    break;
                case sf::Keyboard::Numpad8:
                    dy = delta_angle;
                    break;
                case sf::Keyboard::Numpad9:
                    dx = delta_angle;
                    break;
                }
                break;
            case sf::Event::KeyReleased:
                switch (windowEvent.key.code) {
                case sf::Keyboard::Numpad2:
                    dy = 0.0f;
                    break;
                case sf::Keyboard::Numpad4:
                    dz = 0.0f;
                    break;
                case sf::Keyboard::Numpad6:
                    dz = 0.0f;
                    break;
                case sf::Keyboard::Numpad7:
                    dx = 0.0f;
                    break;
                case sf::Keyboard::Numpad8:
                    dy = 0.0f;
                    break;
                case sf::Keyboard::Numpad9:
                    dx = 0.0f;
                    break;
                }
                break;
            }
        }
        
        anglex += dx;
        angley += dy;
        anglez += dz;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // to fix display of the cube

        float time = clock.getElapsedTime().asSeconds();

        // MODEL TRANSFORMATION
        model = glm::rotate(identity, anglex, xPivot);
        model = glm::rotate(model, angley, yPivot);
        model = glm::rotate(model, anglez, zPivot);
        //model = glm::rotate(identity, time * glm::radians(45.0f), zPivot);
        glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(model));

        // VIEW TRANSFORMATION
        view = glm::lookAt(
            glm::vec3(1.5f, 1.5f, 1.5f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            zPivot
        );
        glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

        // PERSPECTIVE PROJECTION TRANSFORMATION
        proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
        glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

        // DRAW AND DISPLAY
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        window_cube.display();

    }

    // CLEANUP
    glDeleteTextures(2, textures);

    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    return 0;
}
