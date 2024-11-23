#include "../include/ManagerImpl.hpp"

void ManagerImpl::_on_init() {
    nd::Widget::add_prototype("CUBE_STATUS", new CubeStatus());
    nd::Widget::add_prototype("MATRIX_INPUT", new MatrixInput());
}

void ManagerImpl::_on_create() {
    nd::Widget* root = get_widget("root");
    if (root == nullptr) {
        std::cerr << "Root widget not found" << std::endl;
        return;
    }
    
    root->link_on_window_closed([this](sf::Event event) {
        this->get_window().close();
        return true;
    });
    root->link_on_key_press([this](sf::Event event) {
        if (event.key.code == sf::Keyboard::Escape) {
            this->get_window().close();
            return true;
        }
        return false;
    });
}

void ManagerImpl::draw_impl() {
    sf::RenderWindow &window = get_window();
    window.setActive();
    window.clear(sf::Color::Black);
    this->draw();
    window.display();
}
