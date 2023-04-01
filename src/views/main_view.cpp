#include "views/main_view.h"

#include <imgui.h>

#include "logger.h"

MainView::MainView(int image_width, int image_height)
    : image_width_(image_width), image_height_(image_height) {
  event_ = new Event;
}

void MainView::showDroneVideoFeed() {
  glBindTexture(GL_TEXTURE_2D, textures_[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width_, image_height_, 0, GL_RGB, GL_UNSIGNED_BYTE,
               image_);

  // glBindTexture(GL_TEXTURE_2D, textures_[1]);
  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width_, image_height_, 0, GL_RGB,
  // GL_UNSIGNED_BYTE,
  //              face_image_);

  glBindTexture(GL_TEXTURE_2D, textures_[2]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width_, image_height_, 0, GL_RGB, GL_UNSIGNED_BYTE,
               imgproc_image_);

  ImGui::Begin("Images");
  ImGui::BeginTabBar("test");
  if (ImGui::BeginTabItem("Video feed")) {
    ImGui::Image((void*)(intptr_t)textures_[0], ImVec2(image_width_, image_height_));
    ImGui::EndTabItem();
  }
  // if (ImGui::BeginTabItem("Face detect")) {
  //   ImGui::Image((void*)(intptr_t)textures_[1], ImVec2(image_width_, image_height_));
  //   ImGui::EndTabItem();
  // }
  if (ImGui::BeginTabItem("ImgProc")) {
    ImGui::Image((void*)(intptr_t)textures_[2], ImVec2(image_width_, image_height_));

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
  ImGui::EndTabBar();
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
    if (ImGui::IsMousePosValid()) {
      ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
    } else {
      ImGui::Text("Mouse Position: <invalid>");
    }

    for (const auto& time : thread_time_) {
      ImGui::Text("%s :  %i ms", time.first.c_str(), time.second);
    }
  }
  ImGui::End();
}

void MainView::update() {
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

  showDroneVideoFeed();

  showConsole();

  showOverlay();
}

void MainView::setImage(unsigned char* image) { image_ = image; }

void MainView::setImgProcImage(unsigned char* image) { imgproc_image_ = image; }

void MainView::setTextures(GLuint* textures) { textures_ = textures; }

Event* MainView::getEvent() { return event_; }