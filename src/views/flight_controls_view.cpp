#include "views/flight_controls_view.h"

#include <imgui.h>

#include "event.h"

char buffer[256];

FlightControlView::FlightControlView() {
  gevent_dispatcher.appendListener(
      "RCCommands", eventpp::argumentAdapter<void(const RCEvent&)>([&](const RCEvent& e) {
        rc_commands_ = {e.y, e.x, e.minus_z, e.yaw};
      }));

  gevent_dispatcher.appendListener(
      "JoystickEnabled",
      eventpp::argumentAdapter<void(const EnableButtonEvent&)>(
          [&](const EnableButtonEvent& e) { joystick_control_enabled_ = e.enable; }));
}

void FlightControlView::update() {
  ImGui::Begin("Commands");

  if (ImGui::Button("EnableSDK")) {
    gevent_dispatcher.dispatch("EnableSDK", {});
  }

  if (ImGui::Button("Takeoff")) {
    gevent_dispatcher.dispatch("Takeoff", {});
  }

  ImGui::SameLine();

  if (ImGui::Button("Land")) {
    gevent_dispatcher.dispatch("Land", {});
  }

  ImGui::SameLine();

  if (ImGui::Button("Emergency")) {
    gevent_dispatcher.dispatch("Emergency", {});
  }

  if (ImGui::Button("Hover")) {
    gevent_dispatcher.dispatch("IP_Hover", {});
  }

  if (ImGui::Button("Battery?")) {
    gevent_dispatcher.dispatch("Battery", {});
  }

  if (ImGui::Button("Stream ON")) {
    gevent_dispatcher.dispatch("Streamon", {});
  }

  ImGui::SameLine();

  if (ImGui::Button("Stream OFF")) {
    gevent_dispatcher.dispatch("Streamoff", {});
  }

  ImGui::InputText("Custom", buffer, sizeof(buffer));

  ImGui::SameLine();

  if (ImGui::Button("Validate")) {
    ButtonInputEvent input_event(buffer);
    gevent_dispatcher.dispatch("CustomCommand", input_event);
  }

  std::string joystick_control_str = "Joystick controls disabled";
  if (joystick_control_enabled_) {
    joystick_control_str = "Joystick controls enabled";
  }

  ImGui::Text("%s", joystick_control_str.c_str());

  ImGui::Text("RC : %i %i %i %i", rc_commands_.x, rc_commands_.y, rc_commands_.z, rc_commands_.w);

  ImGui::End();
}