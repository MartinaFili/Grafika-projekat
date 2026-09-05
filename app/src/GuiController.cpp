//
// Created by maki on 9/4/26.
//

#include <GuiController.hpp>
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
    graphics->begin_gui();

    ImGui::SetNextWindowSize(ImVec2(400, 80), ImGuiCond_FirstUseEver);
    ImGui::Begin("Moonlight settings");
    ImGui::SliderFloat("Intensity", &m_dir_light_intensity, 0.0f, 1.5f);
    ImGui::End();

    graphics->end_gui();
}
}// app