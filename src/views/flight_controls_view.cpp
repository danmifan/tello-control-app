#include "views/flight_controls_view.h"

#include <imgui.h>

char buffer[256];

FlightControlView::FlightControlView(FlightControl* fc, VideoStreaming* vs) : fc_(fc), vs_(vs) {}

void FlightControlView::update() {
  ImGui::Begin("Commands");

  if (ImGui::Button("EnableSDK")) {
    fc_->enableSDK();
  }

  if (ImGui::Button("Takeoff")) {
    fc_->takeoff();
  }

  if (ImGui::Button("Landing")) {
    fc_->land();
  }

  if (ImGui::Button("Hover")) {
    fc_->hover();
  }

  if (ImGui::Button("Emergency")) {
    fc_->emergencyStop();
  }

  if (ImGui::Button("Battery?")) {
    fc_->getBattery();
  }

  ImGui::InputText("Custom", buffer, sizeof(buffer));

  if (ImGui::Button("Validate")) {
    std::string str(buffer);
    fc_->customCommand(str);
  }

  if (ImGui::Button("Stream OFF")) {
    vs_->stop();
    fc_->streamoff();
  }

  if (ImGui::Button("Stream ON")) {
    vs_->start();
    fc_->streamon();
  }

  // ImGui::Checkbox("Joystick", joystick_enabled_);

  ImGui::End();
}