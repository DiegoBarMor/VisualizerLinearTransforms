#pragma once
#include <glm/glm.hpp>
#include "../andgui/include/widgets/Texts.hpp"

////////////////////////////////////////////////////////////////////////////////

class MatrixGadget : public nd::Widget {
public:
    ~MatrixGadget() { for (auto cell : _cells) delete cell; }
    MatrixGadget* clone() override { return new MatrixGadget(); }
    
    bool set_spec(std::string key, std::string raw_value) override;
    void build() override;
    void draw(sf::RenderWindow& window) override;
    
    void set_font_size(unsigned int size) { _font_size = size; }


protected:
    MatrixGadget() : nd::Widget() {};
    nd::Text* _cells[9];
    unsigned int _font_size = 20;
};

////////////////////////////////////////////////////////////////////////////////

class CubeStatus : public MatrixGadget {
public:
    CubeStatus();
    CubeStatus* clone() override { return new CubeStatus(); }

    void build() override;

    void set_mat_values(glm::mat4 mat);
};

////////////////////////////////////////////////////////////////////////////////

class MatrixInput : public MatrixGadget {
public:
    MatrixInput();
    MatrixInput* clone() override { return new MatrixInput(); }

    void build() override;
    bool handle_event(sf::Event event) override;

    glm::mat4 get_mat_values() { }
    // glm::mat4 matrix = glm::mat4(1.0f);
};

////////////////////////////////////////////////////////////////////////////////
