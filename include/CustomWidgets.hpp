#pragma once
#include "../andgui/include/widgets/Widget.hpp"

class CustomWidget : public nd::Widget {
public:
    CustomWidget() : nd::Widget() {}
    CustomWidget* clone() override { return new CustomWidget(); }

    bool set_spec(std::string key, std::string raw_value) override;
    void build() override;
    void draw(sf::RenderWindow& window) override;

private:
    sf::Color __color_0 = sf::Color::Transparent;
    sf::Color __color_1 = sf::Color::Transparent;
    sf::Color __color_2 = sf::Color::Transparent;
    sf::Color __color_3 = sf::Color::Transparent;
    sf::VertexArray __vertices;
};