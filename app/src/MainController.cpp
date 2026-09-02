//
// Created by maki on 9/2/26.
//

#include "../include/MainController.hpp"

#include <engine/platform/PlatformController.hpp>
#include <engine/resources/ResourcesController.hpp>
#include <spdlog/spdlog.h>

namespace app {
void MainController::initialize() {}

bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) { return false; }
    return true;
}

void MainController::draw_rooftop() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    engine::resources::Model *model = resources->model("rooftop");
    engine::resources::Shader *shader = resources->shader("rooftop");
    model->draw(shader);

}

void MainController::draw() { draw_rooftop(); }
}// app