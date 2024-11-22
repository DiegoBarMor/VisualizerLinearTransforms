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

    sf::RenderWindow window_status(sf::VideoMode(700, 450), "Cube Status", sf::Style::Close);
    sf::RenderWindow window_input(sf::VideoMode(700, 450), "Matrix Input", sf::Style::Close);
    sf::RenderWindow window_panel(sf::VideoMode(600, 300), "Control Panel", sf::Style::Close);
    sf::Window window_cube(sf::VideoMode(600, 600), "OpenGL Cube", sf::Style::Close, settings);
    
    window_status.setPosition(sf::Vector2i(0, 0));
    window_input.setPosition(sf::Vector2i(0, 490));
    window_panel.setPosition(sf::Vector2i(700, 640));
    window_cube.setPosition(sf::Vector2i(700, 0));

    window_status.setFramerateLimit(120);
    window_input.setFramerateLimit(120);
    window_panel.setFramerateLimit(120);
    window_cube.setFramerateLimit(120);

    ManagerImpl gui_status = ManagerImpl(window_status);
    gui_status.setup("assets/cube_status.ndg");

    ManagerImpl gui_input = ManagerImpl(window_input);
    gui_input.setup("assets/matrix_input.ndg");

    ManagerImpl gui_panel = ManagerImpl(window_panel);
    gui_panel.setup("assets/control_panel.ndg");

    OpenGLCube cube = OpenGLCube(window_cube);

    ////// Link callbacks ///////////////////////////////////////////////////////
    nd::Widget* rbn_aut = gui_panel.get_widget("rbn_aut");
    nd::Widget* rbn_man = gui_panel.get_widget("rbn_man");
    nd::Widget* rbn_mat = gui_panel.get_widget("rbn_mat");
    nd::Widget* btn_reset = gui_panel.get_widget("btt_reset");
    CubeStatus* visualize_mat = (CubeStatus*)gui_status.get_widget("visualize_mat");

    if (visualize_mat == nullptr) {
        std::cerr << "Widget 'visualize_mat' not found. Aborting." << std::endl;
        return 1;
    }

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
        cube.set_mode(OpenGLCube::CubeMode::MATRIX); 
        return true;
    });
    if (btn_reset) btn_reset->link_on_mouse_release([&cube](sf::Event event){ 
        cube.reset_model();
        cube.get_window().requestFocus();
        return true;
    });

    ////// Main loop ////////////////////////////////////////////////////////////
    while (window_status.isOpen() && window_input.isOpen() && window_panel.isOpen() && cube.is_running()) {
        gui_status.manage_events();
        gui_input.manage_events();
        gui_panel.manage_events();
        cube.handle_events();

        visualize_mat->set_mat_values(cube.get_model());
       
        gui_status.draw_impl();
        gui_input.draw_impl();
        gui_panel.draw_impl();
        window_cube.setActive();
        cube.update();
        window_cube.display();
    }
    return 0;
}
