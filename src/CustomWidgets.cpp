#include "../include/CustomWidgets.hpp"

bool CustomWidget::set_spec(std::string key, std::string raw_value) {
    if (key == "FS" || key == "FONT_SIZE") {
        __font_size = std::stoi(raw_value);
        return true;
    }
    return nd::Widget::set_spec(key, raw_value);
}

void CustomWidget::build() {
    nd::Widget::build();

    sf::Vector2f size_third = _size / 3.0f;

    sf::Vector2f pos_ix = _pos;
    sf::Vector2f pos_jy = pos_ix + size_third;
    sf::Vector2f pos_kz = pos_jy + size_third;

    sf::Vector2f pos_jx(pos_jy.x, pos_ix.y);
    sf::Vector2f pos_kx(pos_kz.x, pos_ix.y);
    sf::Vector2f pos_iy(pos_ix.x, pos_jy.y);
    sf::Vector2f pos_ky(pos_kz.x, pos_jy.y);
    sf::Vector2f pos_iz(pos_ix.x, pos_kz.y);
    sf::Vector2f pos_jz(pos_jy.x, pos_kz.y);

    __text_ix.set_text_str("ix"); __text_jx.set_text_str("jx"); __text_kx.set_text_str("kx");
    __text_iy.set_text_str("iy"); __text_jy.set_text_str("jy"); __text_ky.set_text_str("ky");
    __text_iz.set_text_str("iz"); __text_jz.set_text_str("jz"); __text_kz.set_text_str("kz");

    __text_ix.set_pos(pos_ix); __text_jx.set_pos(pos_jx); __text_kx.set_pos(pos_kx);
    __text_iy.set_pos(pos_iy); __text_jy.set_pos(pos_jy); __text_ky.set_pos(pos_ky);
    __text_iz.set_pos(pos_iz); __text_jz.set_pos(pos_jz); __text_kz.set_pos(pos_kz);

    __text_ix.set_size(size_third); __text_jx.set_size(size_third); __text_kx.set_size(size_third);
    __text_iy.set_size(size_third); __text_jy.set_size(size_third); __text_ky.set_size(size_third);
    __text_iz.set_size(size_third); __text_jz.set_size(size_third); __text_kz.set_size(size_third);
    
    __text_ix.set_font_size(__font_size); __text_jx.set_font_size(__font_size); __text_kx.set_font_size(__font_size);
    __text_iy.set_font_size(__font_size); __text_jy.set_font_size(__font_size); __text_ky.set_font_size(__font_size);
    __text_iz.set_font_size(__font_size); __text_jz.set_font_size(__font_size); __text_kz.set_font_size(__font_size);

    __text_ix.set_bg_color(sf::Color(0x330000FF)); __text_jx.set_bg_color(sf::Color(0x003300FF)); __text_kx.set_bg_color(sf::Color(0x000033FF));
    __text_iy.set_bg_color(sf::Color(0x660000FF)); __text_jy.set_bg_color(sf::Color(0x006600FF)); __text_ky.set_bg_color(sf::Color(0x000066FF));
    __text_iz.set_bg_color(sf::Color(0x990000FF)); __text_jz.set_bg_color(sf::Color(0x009900FF)); __text_kz.set_bg_color(sf::Color(0x000099FF));

    __text_ix.build(); __text_jx.build(); __text_kx.build();
    __text_iy.build(); __text_jy.build(); __text_ky.build();
    __text_iz.build(); __text_jz.build(); __text_kz.build();
}

void CustomWidget::draw(sf::RenderWindow& window) {
    nd::Widget::draw(window);
    __text_ix.draw(window); __text_jx.draw(window); __text_kx.draw(window);
    __text_iy.draw(window); __text_jy.draw(window); __text_ky.draw(window);
    __text_iz.draw(window); __text_jz.draw(window); __text_kz.draw(window);

}