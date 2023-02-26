#include "views/main_view.h"

#include <imgui.h>
#include "global.h"
#include <iostream>

#include "logger.h"

char buffer[256];

void MainView::showDroneVideoFeed() {
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width_, image_height_, 0, GL_RGB, GL_UNSIGNED_BYTE,
               image_);

  ImGui::Begin("Video feed");
  ImGui::Image((void*)(intptr_t)texture_, ImVec2(image_width_, image_height_));
  ImGui::End();
}

void MainView::showDroneStatus() {
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
  ImGui::Text("Battery : %i", current_state_.bat);
  ImGui::Text("TOF : %i", current_state_.tof);
  ImGui::Text("Time : %i", current_state_.time);

  ImGui::End();
}

void MainView::showCommands() {
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

  if (ImGui::Button("Stream OFF")) {
    vs_->stop();
    fc_->streamoff();
  }

  if (ImGui::Button("Stream ON")) {
    vs_->start();
    fc_->streamon();
  }

  ImGui::End();
}

void MainView::showConsole() {
  ImGui::Begin("Console");

  for (const auto& log : Log::get().getLogs()) {
    ImGui::Text("%s", log.c_str());
  }

  // Auto scroll to last line
  if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) ImGui::SetScrollHereY(1.0f);

  ImGui::End();
}

void MainView::update() {
  if (!status_.empty()) {
    current_state_ = status_.back();
  }

  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

  ImGui::Begin("Dockspace", NULL, window_flags);

  ImGui::PopStyleVar(2);

  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("Menu")) {
      if (ImGui::MenuItem("Demo")) {
        show_demo_ = true;
      }

      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  if (show_demo_) {
    ImGui::ShowDemoWindow();
  }

  ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
  ImGui::DockSpace(dockspace_id, ImVec2(0, 0));

  showDroneVideoFeed();

  showDroneStatus();

  showCommands();

  showConsole();

  ImGui::End();
}

void MainView::setImage(unsigned char* image) { image_ = image; }

void MainView::setTexture(GLuint texture) { texture_ = texture; }