//
// Created by maki on 9/2/26.
//

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include <engine/core/Controller.hpp>
#include <engine/resources/Shader.hpp>

namespace app {

class MainController : public engine::core::Controller {
    void initialize() override;

    bool loop() override;

    void draw_rooftop();

    void draw_floor();

    void draw_saucer();

    void update_camera();

    void update_saucer(float dt);

    void update() override;

    void begin_draw() override;

    void draw_skybox();

    void draw() override;

    void end_draw() override;

    void set_light_uniforms(engine::resources::Shader *shader);

    bool m_saucer_descending = false;
    float m_saucer_y = 1.05f;
    float m_saucer_target_y = 0.2f;
    float m_saucer_progress = 0.0f;

public:
    std::string_view name() const override { return "app::MainController"; }
};

}// app

#endif //MAINCONTROLLER_HPP
