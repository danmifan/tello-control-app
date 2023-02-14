#include "views/main_view.h"

#include <imgui.h>
#include "global.h"
#include <iostream>

char buffer[256];

void MainView::update() {
  if (!status_.empty()) {
    current_state_ = status_.back();
  }

  ImGui::Begin("Drone status");
  ImGui::Text("Roll : %i Pitch : %i Yaw : %i", current_state_.attitude.x, current_state_.attitude.y,
              current_state_.attitude.z);
  ImGui::Text("vx : %i vy : %i vz : %i", current_state_.velocity.x, current_state_.velocity.y,
              current_state_.velocity.z);
  ImGui::Text("ax : %f ay : %f az : %f", current_state_.acceleration.x,
              current_state_.acceleration.y, current_state_.acceleration.z);
  ImGui::Text("Lowest temperature : %i", current_state_.templ);
  ImGui::Text("Highest temperature : %i", current_state_.temph);
  ImGui::Text("Height : %i", current_state_.h);
  ImGui::Text("Baro : %f", current_state_.baro);
  ImGui::Text("Battery : %i", current_state_.bat);
  ImGui::Text("TOF : %i", current_state_.tof);
  ImGui::Text("Time : %i", current_state_.time);

  if (ImGui::Button("Takeoff")) {
    fc_->takeoff();
    std::cout << "Takeoff" << std::endl;
  }

  if (ImGui::Button("Landing")) {
    fc_->land();
    std::cout << "Landing" << std::endl;
  }

  if (ImGui::Button("Emergency")) {
    std::cout << "Emergency" << std::endl;
  }

  if (ImGui::Button("Battery?")) {
    fc_->getBattery();
  }

  ImGui::InputText("Custom", buffer, sizeof(buffer));

  if (ImGui::Button("Validate")) {
    std::string str(buffer);
    fc_->customCommand(str);
  }

  if (ImGui::Button("Stream ON")) {
    fc_->streamon();
    std::cout << "streamon" << std::endl;
  }

  if (ImGui::Button("Stream OFF")) {
    fc_->streamoff();
    std::cout << "streamoff" << std::endl;
  }

  if (ImGui::Button("EnableSDK")) {
    fc_->enableSDK();
    std::cout << "enable SDK" << std::endl;
  }

  ImGui::End();
}