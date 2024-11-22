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

    sf::RenderWindow window_gui(sf::VideoMode(800, 600), "Cube Status", sf::Style::Close);
    sf::Window window_cube(sf::VideoMode(600, 600), "OpenGL Cube", sf::Style::Close, settings);
    window_gui.setPosition(sf::Vector2i(0, 0));
    window_cube.setPosition(sf::Vector2i(800, 0));

    window_gui.setFramerateLimit(120);
    window_cube.setFramerateLimit(120);

    ManagerImpl gui = ManagerImpl(window_gui);
    gui.setup("assets/cube_status.ndg");

    OpenGLCube cube = OpenGLCube(window_cube);

    ////// Link callbacks ///////////////////////////////////////////////////////
    nd::Widget* rbn_aut = gui.get_widget("rbn_aut");
    nd::Widget* rbn_man = gui.get_widget("rbn_man");
    nd::Widget* rbn_mat = gui.get_widget("rbn_mat");
    nd::Widget* btn_reset = gui.get_widget("btt_reset");
    CustomWidget* visualize_mat = (CustomWidget*)gui.get_widget("visualize_mat");

    if (rbn_aut) rbn_aut->link_on_mouse_release([&cube](sf::Event event){ 
        cube.set_mode(OpenGLCube::CubeMode::AUTO); 
        return true;
    });
    if (rbn_man) rbn_man->link_on_mouse_release([&cube](sf::Event event){ 
        cube.set_mode(OpenGLCube::CubeMode::MANUAL); 
        cube.get_window().requestFocus();
        return true;
    });
    if (rbn_mat) rbn_mat->link_on_mouse_release([&cube](sf::Event event){ 
        cube.set_mode(OpenGLCube::CubeMode::MATRICES); 
        return true;
    });
    if (btn_reset) btn_reset->link_on_mouse_release([&cube](sf::Event event){ 
        cube.reset();
        cube.get_window().requestFocus();
        return true;
    });

    ////// Main loop ////////////////////////////////////////////////////////////
    while (cube.is_running() && window_gui.isOpen()) {
        gui.manage_events();
        cube.handle_events();
        if (visualize_mat) {
            visualize_mat->set_mat_values(cube.get_model());
        }

        window_gui.setActive();
        window_gui.clear(sf::Color::Black);
        gui.draw();
        window_gui.display();

        window_cube.setActive();
        cube.update();
        window_cube.display();
    }
    return 0;
}
