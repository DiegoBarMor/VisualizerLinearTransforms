#pragma once
#include "../andgui/include/Manager.hpp"
#include "CustomWidgets.hpp"

class ManagerImpl : public nd::Manager {
public:
    ManagerImpl(sf::RenderWindow& window) : nd::Manager(window) {}

    void draw_impl();

protected:
    void _on_init() override;
    void _on_create() override;
};
