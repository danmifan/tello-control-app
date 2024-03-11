#include "views/main_view.h"

#include <imgui.h>
#include <implot.h>
#include <sstream>

#include "event.h"
#include "logger.h"

#include "global.h"

MainView::MainView(int image_width, int image_height)
    : image_width_(image_width), image_height_(image_height) {
  gevent_dispatcher.appendListener(
      "ArucoDetectorStatus",
      eventpp::argumentAdapter<void(const ArucoDetectorStatusEvent&)>(
          [&](const ArucoDetectorStatusEvent& e) { aruco_status_ = e.markers; }));

  gevent_dispatcher.appendListener(
      "ThreadTime", eventpp::argumentAdapter<void(const ThreadTimeEvent&)>(
                        [&](const ThreadTimeEvent& e) { thread_time_[e.name] = e.value; }));
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

  if (ImGui::BeginTabItem("ImgProc")) {
    ImGui::Image((void*)(intptr_t)textures_[2], ImVec2(image_width_, image_height_));

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
      ImVec2 mousePositionAbsolute = ImGui::GetMousePos();
      ImVec2 screenPositionAbsolute = ImGui::GetItemRectMin();
      ImVec2 mousePositionRelative = ImVec2(mousePositionAbsolute.x - screenPositionAbsolute.x,
                                            mousePositionAbsolute.y - screenPositionAbsolute.y);

      MouseEvent mouse_event(mousePositionRelative.x, mousePositionRelative.y);
      gevent_dispatcher.dispatch("TrackerNewTarget", mouse_event);
    }
    ImGui::EndTabItem();
  }

  if (ImGui::BeginTabItem("Video feed")) {
    ImGui::Image((void*)(intptr_t)textures_[0], ImVec2(image_width_, image_height_));
    ImGui::EndTabItem();
  }
  // if (ImGui::BeginTabItem("Face detect")) {
  //   ImGui::Image((void*)(intptr_t)textures_[1], ImVec2(image_width_, image_height_));
  //   ImGui::EndTabItem();
  // }

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
    ImGui::Text("Threads time");
    ImGui::Separator();
    if (ImGui::IsMousePosValid()) {
      ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
    } else {
      ImGui::Text("Mouse Position: <invalid>");
    }

    int dt = ImGui::GetIO().DeltaTime * 1000;
    ImGui::Text("%s :  %i ms", "GUI", dt);
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
    ImPlot::ShowDemoWindow();
  }

  showDroneVideoFeed();

  showConsole();

  showOverlay();

  std::string aruco_button_string;
  if (aruco_detector_enabled_) {
    aruco_button_string = "Disable";
  } else {
    aruco_button_string = "Enable";
  }

  ImGui::Begin("Aruco detector");
  if (ImGui::Button(aruco_button_string.c_str())) {
    aruco_detector_enabled_ = !aruco_detector_enabled_;
    EnableButtonEvent enable_event(aruco_detector_enabled_);
    gevent_dispatcher.dispatch("ArucoDetector", enable_event);
  }

  ImGui::Text("Markers detected : %i", (int)aruco_status_.size());

  if (ImGui::BeginTable("ArucoTable", 3)) {
    ImGui::TableSetupColumn("ID");
    ImGui::TableSetupColumn("eulers");
    ImGui::TableSetupColumn("tvec");
    ImGui::TableHeadersRow();

    for (int i = 0; i < aruco_status_.size(); i++) {
      ImGui::TableNextRow();

      ArucoMarker marker = aruco_status_.at(i);

      ImGui::TableSetColumnIndex(0);
      ImGui::Text("%i", marker.id);

      ImGui::TableSetColumnIndex(1);
      ImGui::Text("%f %f %f", marker.euler.x, marker.euler.y, marker.euler.z);

      ImGui::TableSetColumnIndex(2);
      ImGui::Text("%f %f %f", marker.tvec.x, marker.tvec.y, marker.tvec.z);
    }

    ImGui::EndTable();
  }

  ImGui::End();

  std::string face_button_string;
  if (face_detector_enabled_) {
    face_button_string = "Disable";
  } else {
    face_button_string = "Enable";
  }

  ImGui::Begin("Face detector");
  if (ImGui::Button(face_button_string.c_str())) {
    face_detector_enabled_ = !face_detector_enabled_;
    EnableButtonEvent enable_event(face_detector_enabled_);
    gevent_dispatcher.dispatch("FaceDetector", enable_event);
  }
  ImGui::End();

  // ImGui::Begin("Plot");
  // static float test[] = {0};
  // // static float test2[] = {360.0f};

  // if (ImPlot::BeginPlot("My Plot")) {
  //   ImPlot::PlotLine("PID", pid_values_.data(), pid_values_.size());
  //   ImPlot::PlotInfLines("Test", test, 1, ImPlotInfLinesFlags_Horizontal);
  //   ImPlot::EndPlot();
  // }
  // ImGui::End();

  ImGui::Begin("CV");
  if (ImGui::Button("Screenshot")) {
    gevent_dispatcher.dispatch("Screenshot", {});
  }
  ImGui::End();

  ImGui::Begin("Plot");
  if (ImPlot::BeginPlot("RC commands")) {
    ImPlot::SetupAxes(NULL, "RC", 0, ImPlotAxisFlags_LockMax | ImPlotAxisFlags_LockMin);
    ImPlot::SetupAxesLimits(0, 1000, -100, 100);
    ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, 10000);
    ImPlot::PlotLine("y_cmd", y_cmds.data(), y_cmds.size());
    ImPlot::PlotLine("z_cmd", z_cmds.data(), z_cmds.size());
    ImPlot::EndPlot();
  }
  ImGui::End();
}

void MainView::setImage(unsigned char* image) { image_ = image; }

void MainView::setImgProcImage(unsigned char* image) { imgproc_image_ = image; }

void MainView::setTextures(GLuint* textures) { textures_ = textures; }
