#include "../include/CustomWidgets.hpp"

bool CustomWidget::set_spec(std::string key, std::string raw_value) {
    if (key == "C0" || key == "COLOR_0") {
        __color_0 = nd::parse_color_string(raw_value);
        return true;
    }
    if (key == "C1" || key == "COLOR_1") {
        __color_1 = nd::parse_color_string(raw_value);
        return true;
    }
    if (key == "C2" || key == "COLOR_2") {
        __color_2 = nd::parse_color_string(raw_value);
        return true;
    }
    if (key == "C3" || key == "COLOR_3") {
        __color_3 = nd::parse_color_string(raw_value);
        return true;
    }
    return nd::Widget::set_spec(key, raw_value);
}

void CustomWidget::build() {
    __vertices = sf::VertexArray(sf::Quads, 4);
    __vertices[0].position = _pos;
    __vertices[1].position = sf::Vector2f(_pos.x + _size.x, _pos.y);
    __vertices[2].position = _pos + _size;
    __vertices[3].position = sf::Vector2f(_pos.x, _pos.y + _size.y);
    __vertices[0].color = __color_0;
    __vertices[1].color = __color_1;
    __vertices[2].color = __color_2;
    __vertices[3].color = __color_3;
}

void CustomWidget::draw(sf::RenderWindow& window) {
    window.draw(__vertices);
}