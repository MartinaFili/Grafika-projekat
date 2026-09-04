//
// Created by maki on 9/2/26.
//

#include "../include/MainController.hpp"

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
    camera->Pitch = -20.0f;
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
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.2f, -5.0f));
    model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.3f));
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

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.5f, -5.0f));
    model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f));
    shader->set_mat4("model", model);

    texture->bind(engine::graphics::OpenGL::texture_unit(0));
    shader->set_int("floor_texture", 0);

    engine::graphics::OpenGL::draw_floor_quad();
}

void MainController::update_camera() {
    auto gui_controller = engine::core::Controller::get<GUIController>();
    if (gui_controller->is_enabled()) { return; }
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto graphichs = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphichs->camera();
    float dt = platform->dt();
    if (platform->key(engine::platform::KeyId::KEY_W).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt); }
    if (platform->key(engine::platform::KeyId::KEY_S).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt); }
    if (platform->key(engine::platform::KeyId::KEY_A).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt); }
    if (platform->key(engine::platform::KeyId::KEY_D).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt); }
    if (platform->key(engine::platform::KeyId::KEY_Q).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::UP, dt); }
    if (platform->key(engine::platform::KeyId::KEY_E).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::DOWN, dt); }
}

void MainController::update() { update_camera(); }

void MainController::begin_draw() { engine::graphics::OpenGL::clear_buffers(); }

void MainController::draw() {
    draw_floor();
    draw_rooftop();
}

void MainController::end_draw() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}
}// app