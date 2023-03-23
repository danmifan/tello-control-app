#include "views/main_view.h"

#include <imgui.h>
#include "global.h"
#include <iostream>

#include "logger.h"

char buffer[256];

MainView::MainView(FlightControl* fc, VideoStreaming* vs, int image_width, int image_height)
    : fc_(fc), vs_(vs), image_width_(image_width), image_height_(image_height) {
  event_ = new Event;
}

void MainView::showDroneVideoFeed() {
  glBindTexture(GL_TEXTURE_2D, textures_[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width_, image_height_, 0, GL_RGB, GL_UNSIGNED_BYTE,
               image_);

  glBindTexture(GL_TEXTURE_2D, textures_[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width_, image_height_, 0, GL_RGB, GL_UNSIGNED_BYTE,
               face_image_);

  glBindTexture(GL_TEXTURE_2D, textures_[2]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width_, image_height_, 0, GL_RGB, GL_UNSIGNED_BYTE,
               imgproc_image_);

  ImGui::Begin("Images");
  ImGui::BeginTabBar("test");
  if (ImGui::BeginTabItem("Video feed")) {
    ImGui::Image((void*)(intptr_t)textures_[0], ImVec2(image_width_, image_height_));

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
      ImVec2 mousePositionAbsolute = ImGui::GetMousePos();
      ImVec2 screenPositionAbsolute = ImGui::GetItemRectMin();
      ImVec2 mousePositionRelative = ImVec2(mousePositionAbsolute.x - screenPositionAbsolute.x,
                                            mousePositionAbsolute.y - screenPositionAbsolute.y);

      event_->data.x = mousePositionRelative.x;
      event_->data.y = mousePositionRelative.y;
      event_->active = true;
    }
    ImGui::EndTabItem();
  }
  if (ImGui::BeginTabItem("Face detect")) {
    ImGui::Image((void*)(intptr_t)textures_[1], ImVec2(image_width_, image_height_));
    ImGui::EndTabItem();
  }
  if (ImGui::BeginTabItem("ImgProc")) {
    ImGui::Image((void*)(intptr_t)textures_[2], ImVec2(image_width_, image_height_));
    ImGui::EndTabItem();
  }
  ImGui::EndTabBar();
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
  ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(battery_color_.x, battery_color_.y,
                                                battery_color_.z, battery_color_.w));
  ImGui::Text("Battery : %i", current_state_.bat);
  ImGui::PopStyleColor(1);
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

  ImGui::End();
}

void MainView::showConsole() {
  ImGui::Begin("Console");

#warning To do : get only last elements
  std::list<std::string> logs = Log::get().getLogs();

  for (const auto& log : logs) {
    if (log.find("[WARNING]") != std::string::npos) {
      ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
      ImGui::Text("%s", log.c_str());
      ImGui::PopStyleColor(1);
    } else if (log.find("[ERROR]") != std::string::npos) {
      ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
      ImGui::Text("%s", log.c_str());
      ImGui::PopStyleColor(1);
    } else {
      ImGui::Text("%s", log.c_str());
    }
  }

  // Auto scroll to last line
  if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) ImGui::SetScrollHereY(1.0f);

  ImGui::End();
}

void MainView::showOverlay() {
  ImGuiIO& io = ImGui::GetIO();
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
                                  ImGuiWindowFlags_AlwaysAutoResize |
                                  ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

  ImGui::SetNextWindowBgAlpha(0.35f);  // Transparent background
  if (ImGui::Begin("Example: Simple overlay", NULL, window_flags)) {
    ImGui::Text(
        "Simple overlay\n"
        "(right-click to change position)");
    ImGui::Separator();
    if (ImGui::IsMousePosValid())
      ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
    else
      ImGui::Text("Mouse Position: <invalid>");
  }
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
      ImGui::MenuItem("Demo", NULL, &show_demo_);

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

  showOverlay();

  if (current_state_.bat >= 0 && current_state_.bat <= 25) {
    battery_color_ = {226, 15, 15, 204};
  } else if (current_state_.bat > 25 && current_state_.bat <= 50) {
    battery_color_ = {226, 107, 15, 204};
  } else if (current_state_.bat > 50 && current_state_.bat <= 75) {
    battery_color_ = {255, 218, 0, 204};
  } else if (current_state_.bat > 75 && current_state_.bat <= 100) {
    battery_color_ = {76, 227, 14, 204};
  }

  ImGui::End();
}

void MainView::setImage(unsigned char* image) { image_ = image; }

void MainView::setFaceImage(unsigned char* image) { face_image_ = image; }

void MainView::setImgProcImage(unsigned char* image) { imgproc_image_ = image; }

void MainView::setTextures(GLuint* textures) { textures_ = textures; }

Event* MainView::getEvent() { return event_; }