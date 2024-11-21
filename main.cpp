#include <iostream>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // to ease the calculation of the view and projection matrices
#include <glm/gtc/type_ptr.hpp> // for converting a matrix object into a float array for usage in OpenGL

#include "include/ManagerImpl.hpp"
#include "include/OpenGLCube.hpp"

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

    OpenGLCube cube = OpenGLCube(window_cube);

    ////////////////////////////////////////////////////////////////////////////
    while (cube.is_running()) {
        window_gui.setActive();
        gui.manage_events();
        window_gui.clear(sf::Color::Black);
        gui.draw();
        window_gui.display();

        window_cube.setActive();
        cube.handle_events();
        cube.update();
        window_cube.display();
    }
    return 0;
}
