//
// Created by maki on 9/2/26.
//

#include <MainController.hpp>

#include <GuiController.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/ResourcesController.hpp>
#include <spdlog/spdlog.h>

namespace app {

class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
public:
    void on_mouse_move(engine::platform::MousePosition position) override;
};

void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
    auto gui_controller = engine::core::Controller::get<GUIController>();
    if (!gui_controller->is_enabled()) {
        auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
        camera->rotate_camera(position.dx, position.dy);
    }
}

void MainController::initialize() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
    engine::graphics::OpenGL::enable_depth_testing();

    auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
    camera->Position = glm::vec3(0.0f, 1.5f, 0.0f);
    camera->Yaw = -160.0f;
    camera->Pitch = 20.0f;
    camera->rotate_camera(0.0f, 0.0f);

}

bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) { return false; }
    return true;
}

void MainController::draw_rooftop() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphichs = engine::core::Controller::get<engine::graphics::GraphicsController>();
    engine::resources::Model *rooftop = resources->model("rooftop");
    engine::resources::Shader *shader = resources->shader("rooftop");

    shader->use();
    shader->set_mat4("projection", graphichs->projection_matrix());
    shader->set_mat4("view", graphichs->camera()->view_matrix());
    set_light_uniforms(shader);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(3.6f, -1.5f, -4.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4f));
    shader->set_mat4("model", model);
    rooftop->draw(shader);

}

void MainController::draw_floor() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphichs = engine::core::Controller::get<engine::graphics::GraphicsController>();
    engine::resources::Shader *shader = resources->shader("floor");
    engine::resources::Texture *texture = resources->texture("rooftop_floor");

    shader->use();
    shader->set_mat4("projection", graphichs->projection_matrix());
    shader->set_mat4("view", graphichs->camera()->view_matrix());
    set_light_uniforms(shader);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(4.0f, -1.8f, -4.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.25f));
    shader->set_mat4("model", model);

    texture->bind(engine::graphics::OpenGL::texture_unit(0));
    shader->set_int("floor_texture", 0);

    engine::graphics::OpenGL::draw_floor_quad();
}

void MainController::draw_saucer() {
    if (!m_saucer_visible) { return; }
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphichs = engine::core::Controller::get<engine::graphics::GraphicsController>();
    engine::resources::Model *saucer = resources->model("saucer");
    engine::resources::Shader *shader = resources->shader("saucer");

    shader->use();
    shader->set_mat4("projection", graphichs->projection_matrix());
    shader->set_mat4("view", graphichs->camera()->view_matrix());

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(3.6f, m_saucer_y, -4.0f));
    model = glm::scale(model, glm::vec3(0.01f));
    shader->set_mat4("model", model);
    saucer->draw(shader);
}

void MainController::update_saucer(float dt) {
    if (m_waiting_to_descend) {
        m_wait_timer -= dt;
        if (m_wait_timer <= 0.0f) {
            m_waiting_to_descend = false;
            m_saucer_descending = true;
        }
        return;
    }
    if (m_saucer_descending) {
        float descend_speed = 0.5f;
        m_saucer_y -= descend_speed * dt;

        if (m_saucer_y <= m_saucer_target_y) {
            m_saucer_y = m_saucer_target_y;
            m_saucer_descending = false;

            m_waiting_to_vanish = true;
            m_vanish_timer = 2.0f;
        }

        float start_y = 1.05f;
        float total_distance = start_y - m_saucer_target_y;
        float traveled = start_y - m_saucer_y;
        m_saucer_progress = traveled / total_distance;
        return;
    }

    if (m_waiting_to_vanish) {
        m_vanish_timer -= dt;
        if (m_vanish_timer <= 0.0f) {
            m_waiting_to_vanish = false;
            m_saucer_visible = false;
        }
    }
}

void MainController::update() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_SPACE).state() == engine::platform::Key::State::JustPressed) {
        if (!m_saucer_descending && !m_waiting_to_descend && !m_waiting_to_vanish) {
            if (m_saucer_progress >= 1.0f || !m_saucer_visible) {
                m_saucer_y = 1.05f;
                m_saucer_progress = 0.0f;
                m_saucer_visible = true;
            }
            m_waiting_to_descend = true;
            m_wait_timer = 2.0f;
        }
    }
    update_saucer(platform->dt());
}

void MainController::begin_draw() { engine::graphics::OpenGL::clear_buffers(); }

void MainController::draw_skybox() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto skybox = resources->skybox("night_skybox");
    auto shader = resources->shader("skybox");
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    graphics->draw_skybox(shader, skybox);
}

void MainController::draw() {
    draw_floor();
    draw_rooftop();
    draw_saucer();
    draw_skybox();
}

void MainController::end_draw() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}

void MainController::set_light_uniforms(engine::resources::Shader *shader) {
    glm::vec3 saucer_pos = glm::vec3(3.6f, m_saucer_y, -4.0f);

    // ugao konusa
    float outer_deg = glm::mix(25.0f, 20.0f, m_saucer_progress);
    float inner_deg = glm::mix(18.0f, 15.0f, m_saucer_progress);

    // jacina
    float light_intensity = m_saucer_visible ? glm::mix(1.0f, 4.0f, m_saucer_progress) : 0.0f;

    shader->set_vec3("lightPos", saucer_pos);
    shader->set_vec3("lightDir", glm::vec3(0.0f, -1.0f, 0.0f));
    shader->set_vec3("lightColor", glm::vec3(1.0f, 1.0f, 0.9f));
    shader->set_float("cutOff", glm::cos(glm::radians(inner_deg)));
    shader->set_float("outerCutOff", glm::cos(glm::radians(outer_deg)));
    shader->set_float("intensity", light_intensity);

    // Directional light (moonlight)
    auto gui_controller = engine::core::Controller::get<GUIController>();
    shader->set_vec3("dirLightDirection", glm::normalize(glm::vec3(-0.3f, -1.0f, -0.2f)));
    shader->set_vec3("dirLightColor", glm::vec3(0.5f, 0.55f, 0.7f));
    shader->set_float("dirLightIntensity", gui_controller->dir_light_intensity());
}
}// app