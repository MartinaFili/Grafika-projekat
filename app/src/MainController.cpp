//
// Created by maki on 9/2/26.
//

#include "../include/MainController.hpp"

#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/ResourcesController.hpp>
#include <spdlog/spdlog.h>

namespace app {
void MainController::initialize() { engine::graphics::OpenGL::enable_depth_testing(); }

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
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
    model = glm::scale(model, glm::vec3(0.3f));
    shader->set_mat4("model", model);
    rooftop->draw(shader);

}

void MainController::begin_draw() { engine::graphics::OpenGL::clear_buffers(); }

void MainController::draw() { draw_rooftop(); }

void MainController::end_draw() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}
}// app