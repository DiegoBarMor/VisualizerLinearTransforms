#pragma once
#include "../almond/App.hpp"
#include "CustomWidgets.hpp"

class AppVLT : public nd::App {
public:
    AppVLT(sf::RenderWindow& window) : nd::App(window) {}

    void draw_impl();

protected:
    void _on_init() override;
    void _on_create() override;
};
