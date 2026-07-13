#include <iostream>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // to ease the calculation of the view and projection matrices
#include <glm/gtc/type_ptr.hpp> // for converting a matrix object into a float array for usage in OpenGL

#include "include/AppVLT.hpp"
#include "include/OpenGLCube.hpp"

int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antiAliasingLevel = 2; // Optional
    settings.majorVersion = 3;
    settings.minorVersion = 2;
    settings.attributeFlags = sf::ContextSettings::Core;

    sf::RenderWindow window_status(sf::VideoMode({700, 450}), "Cube Status", sf::State::Windowed);
    sf::RenderWindow window_input(sf::VideoMode({700, 450}), "Matrix Input", sf::State::Windowed);
    sf::RenderWindow window_panel(sf::VideoMode({600, 300}), "Control Panel", sf::State::Windowed);
    sf::Window window_cube(sf::VideoMode({600, 600}), "OpenGL Cube", sf::State::Windowed, settings);

    window_status.setPosition(sf::Vector2i(0, 0));
    window_input.setPosition(sf::Vector2i(0, 490));
    window_panel.setPosition(sf::Vector2i(700, 640));
    window_cube.setPosition(sf::Vector2i(700, 0));

    window_status.setFramerateLimit(120);
    window_input.setFramerateLimit(120);
    window_panel.setFramerateLimit(120);
    window_cube.setFramerateLimit(120);

    AppVLT gui_status = AppVLT(window_status);
    gui_status.setup("assets/cube_status.ndg");

    AppVLT gui_input = AppVLT(window_input);
    gui_input.setup("assets/matrix_input.ndg");

    AppVLT gui_panel = AppVLT(window_panel);
    gui_panel.setup("assets/control_panel.ndg");

    OpenGLCube cube = OpenGLCube(window_cube);

    ////// Link callbacks ///////////////////////////////////////////////////////
    nd::Widget* rbn_aut = gui_panel.get_widget("rbn_aut");
    nd::Widget* rbn_key = gui_panel.get_widget("rbn_key");
    nd::Widget* rbn_mat = gui_panel.get_widget("rbn_mat");
    nd::RadioButton* btt_reset_model = (nd::RadioButton*) gui_panel.get_widget("btt_reset_model");
    nd::RadioButton* btt_reset_zoom = (nd::RadioButton*) gui_panel.get_widget("btt_reset_zoom");
    nd::RadioButton* btt_go = (nd::RadioButton*) gui_panel.get_widget("btt_go");
    nd::RadioButton* btt_clear = (nd::RadioButton*) gui_panel.get_widget("btt_clear");
    CubeStatus* visualize_mat = (CubeStatus*)gui_status.get_widget("visualize_mat");
    MatrixInput* input_mat = (MatrixInput*)gui_input.get_widget("input_mat");

    if (
        rbn_aut == nullptr || rbn_key == nullptr || rbn_mat == nullptr ||
        btt_reset_model == nullptr || btt_reset_zoom == nullptr ||
        btt_go == nullptr || btt_clear == nullptr
    ) {
        std::cerr << "A control panel widget was not found. Aborting." << std::endl;
        return 1;
    }
    if (visualize_mat == nullptr || input_mat == nullptr) {
        std::cerr << "A matrix widget not found. Aborting." << std::endl;
        return 1;
    }

    rbn_aut->link_on_mouse_release([&cube,btt_go,btt_clear](const std::optional<sf::Event> event){
        cube.set_mode(OpenGLCube::CubeMode::AUTO);
        btt_go->set_enabled(false);
        btt_clear->set_enabled(false);
        return true;
    });
    rbn_key->link_on_mouse_release([&cube,btt_go,btt_clear](const std::optional<sf::Event> event){
        cube.set_mode(OpenGLCube::CubeMode::MANUAL);
        cube.get_window().requestFocus();
        btt_go->set_enabled(false);
        btt_clear->set_enabled(false);
        return true;
    });
    rbn_mat->link_on_mouse_release([&cube,btt_go,btt_clear](const std::optional<sf::Event> event){
        cube.set_mode(OpenGLCube::CubeMode::MATRIX);
        btt_go->set_enabled(true);
        btt_clear->set_enabled(true);
        return true;
    });

    btt_reset_model->link_on_mouse_release([&cube](const std::optional<sf::Event> event){
        cube.reset_model();
        cube.get_window().requestFocus();
        return true;
    });
    btt_reset_zoom->link_on_mouse_release([&cube](const std::optional<sf::Event> event){
        cube.reset_zoom();
        cube.get_window().requestFocus();
        return true;
    });
    btt_go->link_on_mouse_release([&cube,&input_mat](const std::optional<sf::Event> event){
        cube.start_animation(input_mat->get_mat_values());
        cube.get_window().requestFocus();
        return true;
    });
    btt_clear->link_on_mouse_release([&input_mat](const std::optional<sf::Event> event){
        input_mat->clear();
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
