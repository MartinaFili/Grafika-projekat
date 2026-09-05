//
// Created by maki on 9/4/26.
//

#ifndef GUICONTROLLER_HPP
#define GUICONTROLLER_HPP
#include <engine/core/Controller.hpp>

namespace app {

class GUIController : public engine::core::Controller {
public:
    std::string_view name() const override { return "app::GUIController"; }

    float dir_light_intensity() const { return m_dir_light_intensity; }

private:
    void initialize() override;

    void poll_events() override;

    void draw() override;

    float m_dir_light_intensity = 0.3f;
};

}// app

#endif //GUICONTROLLER_HPP
