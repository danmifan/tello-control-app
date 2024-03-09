#include "views/flight_controls_view.h"

#include <imgui.h>

#include "global.h"

char buffer[256];

FlightControlView::FlightControlView() {}

void FlightControlView::update() {
  ImGui::Begin("Commands");

  if (ImGui::Button("EnableSDK")) {
    gbutton_event_dispatcher.dispatch("EnableSDK");
  }

  if (ImGui::Button("Takeoff")) {
    gbutton_event_dispatcher.dispatch("Takeoff");
  }

  ImGui::SameLine();

  if (ImGui::Button("Land")) {
    gbutton_event_dispatcher.dispatch("Land");
  }

  ImGui::SameLine();

  if (ImGui::Button("Emergency")) {
    gbutton_event_dispatcher.dispatch("Emergency");
  }

  if (ImGui::Button("Hover")) {
    gbutton_event_dispatcher.dispatch("IP_Hover");
  }

  if (ImGui::Button("Battery?")) {
    gbutton_event_dispatcher.dispatch("Battery");
  }

  if (ImGui::Button("Stream ON")) {
    gbutton_event_dispatcher.dispatch("Streamon");
  }

  ImGui::SameLine();

  if (ImGui::Button("Stream OFF")) {
    gbutton_event_dispatcher.dispatch("Streamoff");
  }

  ImGui::InputText("Custom", buffer, sizeof(buffer));

  ImGui::SameLine();

  if (ImGui::Button("Validate")) {
    std::string str(buffer);
    gbutton_input_event_dispatcher.dispatch("CustomCommand", str);
  }

  ImGui::End();
}