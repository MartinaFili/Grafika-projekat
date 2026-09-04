//
// Created by maki on 9/4/26.
//

#include "../include/GuiController.hpp"
#include <engine/platform/PlatformController.hpp>

#include <imgui.h>
#include <engine/graphics/GraphicsController.hpp>

namespace app {
void GUIController::initialize() { set_enable(false); }

void GUIController::poll_events() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_R).state() == engine::platform::Key::State::JustPressed) { set_enable(!is_enabled()); }
}

void GUIController::draw() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();
    graphics->begin_gui();

    ImGui::Begin("Camera info");
    ImGui::Text("Camera position: (%f, %f, %f)", camera->Position.x, camera->Position.y, camera->Position.z);

    ImGui::End();
    graphics->end_gui();
}
}// app