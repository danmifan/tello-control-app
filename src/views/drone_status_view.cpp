#include "views/drone_status_view.h"

#include <imgui.h>

#include "data.h"

void DroneStatusView::update() {
  if (!status_.empty()) {
    current_state_ = status_.back();
  }

  Vec4i battery_color;

  if (current_state_.bat >= 0 && current_state_.bat <= 25) {
    battery_color = {226, 15, 15, 204};
  } else if (current_state_.bat > 25 && current_state_.bat <= 50) {
    battery_color = {226, 107, 15, 204};
  } else if (current_state_.bat > 50 && current_state_.bat <= 75) {
    battery_color = {255, 218, 0, 204};
  } else if (current_state_.bat > 75 && current_state_.bat <= 100) {
    battery_color = {76, 227, 14, 204};
  }

  ImGui::Begin("Drone info");
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
  ImGui::PushStyleColor(
      ImGuiCol_Text, IM_COL32(battery_color.x, battery_color.y, battery_color.z, battery_color.w));
  ImGui::Text("Battery : %i", current_state_.bat);
  ImGui::PopStyleColor(1);
  ImGui::Text("TOF : %i", current_state_.tof);
  ImGui::Text("Time : %i", current_state_.time);

  ImGui::End();
}