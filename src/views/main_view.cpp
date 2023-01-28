#include "views/main_view.h"

#include <imgui.h>
#include "global.h"

void MainView::update() {
  DroneState& state = status_.front();

  // ImGui::Begin("Drone");
  // ImGui::Text("Roll : %f Pitch : %f Yaw : %f", state.attitude.x, state.attitude.y,
  //             state.attitude.z);
  // ImGui::End();
}