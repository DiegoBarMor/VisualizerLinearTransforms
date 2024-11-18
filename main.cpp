#include <SFML/Graphics.hpp>

#include "include/ManagerImpl.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Visualizer Linear Transformations");
    window.setFramerateLimit(60);

    ManagerImpl gui = ManagerImpl(window);
    gui.setup("examples/basics.ndg");

    while (window.isOpen()) {
        gui.manage_events();
        window.clear(sf::Color::Black);
        gui.draw();
        window.display();
    }
    return 0;
}
