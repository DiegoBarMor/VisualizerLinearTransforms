#include "../include/OpenGLCube.hpp"

OpenGLCube::OpenGLCube(sf::Window &window) : window(window) {
    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);

    init_vertex_input();
    init_shaders();
    init_texture();
    init_transformations();
}

OpenGLCube::~OpenGLCube() {
    glDeleteTextures(1, &texture);

    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

}

void OpenGLCube::init_vertex_input() {
    // VERTEX ARRAY OBJECTS
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // VERTEX BUFFER OBJECTS
    // The next step is to upload this vertex data to the graphics card.
    glGenBuffers(1, &vbo); // This is done by creating a Vertex Buffer Object (VBO)
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // To upload the actual data to it you first have to make it the active object
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Now that it's active we can copy the vertex data to it.
    /*
        GL_STATIC_DRAW : The vertex data will be uploaded onceand drawn many times (e.g.the world).
        GL_DYNAMIC_DRAW : The vertex data will be created once, changed from time to time, but drawn many times more than that.
        GL_STREAM_DRAW : The vertex data will be uploaded onceand drawn once.
    */

    // ELEMENT BUFFER OBJECTS
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
}

void OpenGLCube::init_shaders() {
    // COMPILING SHADERS
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // creating a shader itself starts with creating a shader object and loading data into it.
    glShaderSource(vertexShader, 1, &vertexSource, NULL); // Unlike VBOs, you can simply pass a reference to shader functions instead of making it active
    glCompileShader(vertexShader); // All that's left is compiling the shader into code that can be executed by the graphics card now

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // COMBINING SHADERS INTO A PROGRAM
    shaderProgram = glCreateProgram(); // Up until now the vertex and fragment shaders have been two separate objects.
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

    posAttrib = glGetAttribLocation(shaderProgram, "position"); // OpenGL still doesn't know how the attributes are formatted and ordered. With the reference
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0); // to the input, you can specify how the data for that input is retrieved from the array
    glEnableVertexAttribArray(posAttrib); // Last, but not least, the vertex attribute array needs to be enabled.

    colAttrib = glGetAttribLocation(shaderProgram, "color");
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(colAttrib);
}

void OpenGLCube::init_texture() {
    texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(texAttrib);

    texture = GLuint();
    glGenTextures(1, &texture);

    image.loadFromFile("cats.png");
    glActiveTexture(GL_TEXTURE0); // specifies which texture unit a texture object is bound to when glBindTexture is called
    glBindTexture(GL_TEXTURE_2D, texture);
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
}

void OpenGLCube::init_transformations() {
    uniTrans = glGetUniformLocation(shaderProgram, "model");
    uniView = glGetUniformLocation(shaderProgram, "view");
    uniProj = glGetUniformLocation(shaderProgram, "proj");
}

void OpenGLCube::handle_events() {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            running = false; break;
            
        case sf::Event::KeyPressed:
            switch (event.key.code) {
            case sf::Keyboard::Escape:
                running = false; break;
            case sf::Keyboard::Numpad2:
                dy = -delta_angle; break;
            case sf::Keyboard::Numpad4:
                dz = -delta_angle; break; 
            case sf::Keyboard::Numpad6:
                dz = delta_angle; break;
            case sf::Keyboard::Numpad7:
                dx = -delta_angle; break;
            case sf::Keyboard::Numpad8:
                dy = delta_angle; break;
            case sf::Keyboard::Numpad9:
                dx = delta_angle; break;
            } break;
            
        case sf::Event::KeyReleased:
            switch (event.key.code) {
            case sf::Keyboard::Numpad2:
                dy = 0.0f; break;
            case sf::Keyboard::Numpad4:
                dz = 0.0f; break;
            case sf::Keyboard::Numpad6:
                dz = 0.0f; break;
            case sf::Keyboard::Numpad7:
                dx = 0.0f; break;
            case sf::Keyboard::Numpad8:
                dy = 0.0f; break;
            case sf::Keyboard::Numpad9:
                dx = 0.0f; break;
            } break;
        }
    }
}

void OpenGLCube::update() {
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
}
