#pragma once
#include <glm/glm.hpp>
#include "../andgui/include/widgets/Texts.hpp"

class CustomWidget : public nd::Widget {
public:
    CustomWidget() : nd::Widget() {}
    CustomWidget* clone() override { return new CustomWidget(); }
    
    bool set_spec(std::string key, std::string raw_value) override;
    void build() override;
    void draw(sf::RenderWindow& window) override;
    
    void set_font_size(unsigned int size) { __font_size = size; }

    void set_mat_values(glm::mat4 mat);

private:
    nd::Text 
        __text_ix, __text_jx, __text_kx,
        __text_iy, __text_jy, __text_ky,
        __text_iz, __text_jz, __text_kz;
    unsigned int __font_size = 20;
};