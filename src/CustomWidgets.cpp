#include "../include/CustomWidgets.hpp"

////////////////////////////////////////////////////////////////////////////////
bool MatrixGadget::set_spec(std::string key, std::string raw_value) {
    if (key == "FS" || key == "FONT_SIZE") {
        _font_size = std::stoi(raw_value);
        return true;
    }
    return nd::Widget::set_spec(key, raw_value);
}

void MatrixGadget::build() {
    nd::Widget::build();
    sf::Vector2f size_third = _size / 3.0f;
    sf::Vector2f pos_ix = _pos;
    sf::Vector2f pos_jy = pos_ix + size_third;
    sf::Vector2f pos_kz = pos_jy + size_third;

    _cells[0]->set_pos(pos_ix);
    _cells[1]->set_pos(sf::Vector2f(pos_jy.x, pos_ix.y));
    _cells[2]->set_pos(sf::Vector2f(pos_kz.x, pos_ix.y));
    _cells[3]->set_pos(sf::Vector2f(pos_ix.x, pos_jy.y));
    _cells[4]->set_pos(pos_jy);
    _cells[5]->set_pos(sf::Vector2f(pos_kz.x, pos_jy.y));
    _cells[6]->set_pos(sf::Vector2f(pos_ix.x, pos_kz.y));
    _cells[7]->set_pos(sf::Vector2f(pos_jy.x, pos_kz.y));
    _cells[8]->set_pos(pos_kz);

    _cells[0]->set_bg_color(sf::Color(0x310000FF)); _cells[1]->set_bg_color(sf::Color(0x003100FF)); _cells[2]->set_bg_color(sf::Color(0x000031FF));
    _cells[3]->set_bg_color(sf::Color(0x390000FF)); _cells[4]->set_bg_color(sf::Color(0x003900FF)); _cells[5]->set_bg_color(sf::Color(0x000039FF));
    _cells[6]->set_bg_color(sf::Color(0x310000FF)); _cells[7]->set_bg_color(sf::Color(0x003100FF)); _cells[8]->set_bg_color(sf::Color(0x000031FF));

    for (auto cell : _cells) {
        cell->set_size(size_third);
        cell->set_font_size(_font_size);
        cell->build();
    }
}

void MatrixGadget::draw(sf::RenderWindow& window) {
    nd::Widget::draw(window);
    for (auto cell : _cells)
        cell->draw(window);
}

////////////////////////////////////////////////////////////////////////////////

CubeStatus::CubeStatus() : MatrixGadget() {
    for (int i = 0; i < 9; i++) _cells[i] = new nd::Text();
}

void CubeStatus::set_mat_values(glm::mat4 mat) {
    _cells[0]->set_text_str(std::to_string(mat[0][0])); _cells[1]->set_text_str(std::to_string(mat[1][0])); _cells[2]->set_text_str(std::to_string(mat[2][0]));
    _cells[3]->set_text_str(std::to_string(mat[0][1])); _cells[4]->set_text_str(std::to_string(mat[1][1])); _cells[5]->set_text_str(std::to_string(mat[2][1]));
    _cells[6]->set_text_str(std::to_string(mat[0][2])); _cells[7]->set_text_str(std::to_string(mat[1][2])); _cells[8]->set_text_str(std::to_string(mat[2][2]));
    for (auto cell : _cells) cell->build();
}

void CubeStatus::build() {
    _cells[0]->set_font_color(sf::Color(0xFF0000FF)); _cells[1]->set_font_color(sf::Color(0xFF0000FF)); _cells[2]->set_font_color(sf::Color(0xFF0000FF));
    _cells[3]->set_font_color(sf::Color(0x00FF00FF)); _cells[4]->set_font_color(sf::Color(0x00FF00FF)); _cells[5]->set_font_color(sf::Color(0x00FF00FF));
    _cells[6]->set_font_color(sf::Color(0x0000FFFF)); _cells[7]->set_font_color(sf::Color(0x0000FFFF)); _cells[8]->set_font_color(sf::Color(0x0000FFFF));
    MatrixGadget::build();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MatrixInput::MatrixInput() : MatrixGadget() {
    for (int i = 0; i < 9; i++) _cells[i] = new nd::TextInput();
}

void MatrixInput::build() {
    nd::TextInput* text_ix = (nd::TextInput*)_cells[0]; nd::TextInput* text_jx = (nd::TextInput*)_cells[1]; nd::TextInput* text_kx = (nd::TextInput*)_cells[2];
    nd::TextInput* text_iy = (nd::TextInput*)_cells[3]; nd::TextInput* text_jy = (nd::TextInput*)_cells[4]; nd::TextInput* text_ky = (nd::TextInput*)_cells[5];
    nd::TextInput* text_iz = (nd::TextInput*)_cells[6]; nd::TextInput* text_jz = (nd::TextInput*)_cells[7]; nd::TextInput* text_kz = (nd::TextInput*)_cells[8];

    text_ix->set_hint_str("1.0"); text_jx->set_hint_str("0.0"); text_kx->set_hint_str("0.0");
    text_iy->set_hint_str("0.0"); text_jy->set_hint_str("1.0"); text_ky->set_hint_str("0.0");
    text_iz->set_hint_str("0.0"); text_jz->set_hint_str("0.0"); text_kz->set_hint_str("1.0");

    text_ix->set_font_color(sf::Color(0x770000FF)); text_jx->set_font_color(sf::Color(0x770000FF)); text_kx->set_font_color(sf::Color(0x770000FF));
    text_iy->set_font_color(sf::Color(0x007700FF)); text_jy->set_font_color(sf::Color(0x007700FF)); text_ky->set_font_color(sf::Color(0x007700FF));
    text_iz->set_font_color(sf::Color(0x000077FF)); text_jz->set_font_color(sf::Color(0x000077FF)); text_kz->set_font_color(sf::Color(0x000077FF));

    MatrixGadget::build();
}

bool MatrixInput::handle_event(const std::optional<sf::Event> event) {
    for (auto cell : _cells) {
        if (cell->handle_event(event)) return true;
    }
    return MatrixGadget::handle_event(event);
}

glm::mat4 MatrixInput::get_mat_values() {
    glm::mat3 matrix;
    std::string value;
    nd::TextInput* cell;
    for (int i = 0; i < 9; i++) {
        cell = (nd::TextInput*)_cells[i];
        value = cell->get_text_str();
        if (value.empty()) value = cell->get_hint_str();
        matrix[i % 3][i / 3] = nd::parse_float_string(value);
    }
    return glm::mat4(matrix);
}

void MatrixInput::clear() {
    for (auto cell : _cells) {
        cell->set_text_str("");
        cell->build();
    }
}

////////////////////////////////////////////////////////////////////////////////
