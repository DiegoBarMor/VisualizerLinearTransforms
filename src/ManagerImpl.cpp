#include "../include/AppVLT.hpp"

void AppVLT::_on_init() {
    nd::Widget::add_prototype("CUBE_STATUS", new CubeStatus());
    nd::Widget::add_prototype("MATRIX_INPUT", new MatrixInput());
}

void AppVLT::_on_create() {
    nd::Widget* root = get_widget("root");
    if (root == nullptr) {
        std::cerr << "Root widget not found" << std::endl;
        return;
    }

    root->link_on_window_closed([this](const std::optional<sf::Event> event) {
        this->get_window().close();
        return true;
    });
    root->link_on_key_press([this](const std::optional<sf::Event> event) {
        const auto* keyPress = event->getIf<sf::Event::KeyPressed>();
        if (keyPress && keyPress->code == sf::Keyboard::Key::Escape) {
            this->get_window().close();
            return true;
        }
        return false;
    });
}

void AppVLT::draw_impl() {
    sf::RenderWindow &window = get_window();
    window.setActive();
    window.clear(sf::Color::Black);
    this->draw();
    window.display();
}
