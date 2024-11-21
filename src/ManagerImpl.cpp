#include "../include/ManagerImpl.hpp"

void ManagerImpl::_on_init() {
    // pre-creation operations, custom prototypes must be added here before parsing the GUI file
    nd::Widget::add_prototype("CUSTOM_WIDGET", new CustomWidget());
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

