#include "views/flight_controls_view.h"

#include <imgui.h>

char buffer[256];

FlightControlView::FlightControlView(FlightControl* fc, VideoStreaming* vs, ImageProcessing* ip)
    : fc_(fc), vs_(vs), ip_(ip) {}

void FlightControlView::update() {
  ImGui::Begin("Commands");

  if (ImGui::Button("EnableSDK")) {
    fc_->enableSDK();
  }

  if (ImGui::Button("Takeoff")) {
    fc_->takeoff();
  }

  ImGui::SameLine();

  if (ImGui::Button("Land")) {
    fc_->land();
  }

  ImGui::SameLine();

  if (ImGui::Button("Emergency")) {
    fc_->emergencyStop();
  }

  if (ImGui::Button("Hover")) {
    ip_->hover();
  }

  if (ImGui::Button("Battery?")) {
    fc_->getBattery();
  }

  if (ImGui::Button("Stream ON")) {
    vs_->start();
    fc_->streamon();
  }

  ImGui::SameLine();

  if (ImGui::Button("Stream OFF")) {
    vs_->stop();
    fc_->streamoff();
  }

  ImGui::InputText("Custom", buffer, sizeof(buffer));

  ImGui::SameLine();

  if (ImGui::Button("Validate")) {
    std::string str(buffer);
    fc_->customCommand(str);
  }

  ImGui::End();
}