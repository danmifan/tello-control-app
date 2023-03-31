#include "window/window.h"

#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GL/gl.h>
#include <chrono>
#include <thread>

MyWindow::MyWindow(int width, int height, int framerate)
    : width_(width), height_(height), framerate_(framerate) {}

void glfw_error_callback(int error, const char *description) {
  std::cout << "GLFW error : " << error << " : " << description << std::endl;
}

int MyWindow::init() {
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) {
    return -1;
  }

  window_ = glfwCreateWindow(width_, height_, "Window", NULL, NULL);

  if (!window_) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window_);
  glfwSwapInterval(1);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window_, true);
  ImGui_ImplOpenGL3_Init("#version 130");

  // Create texture
  glGenTextures(3, image_textures_);

  // glActiveTexture(GL_TEXTURE0);

  glBindTexture(GL_TEXTURE_2D, image_textures_[0]);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

  // glActiveTexture(GL_TEXTURE1);

  glBindTexture(GL_TEXTURE_2D, image_textures_[1]);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

  glBindTexture(GL_TEXTURE_2D, image_textures_[2]);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

  return 1;
}

void MyWindow::update() {
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  while (!glfwWindowShouldClose(window_)) {
    auto t1 = std::chrono::high_resolution_clock::now();
    glfwPollEvents();

    // Render loop

    // ImGUI new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // ImGui::ShowDemoWindow();

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
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

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0, 0));

    // Views
    for (const auto &view : views_) {
      view->update();
    }

    ImGui::End();

    ImGui::Render();

    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    glViewport(0, 0, width, height);

    // Render loop end

    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window_);

    auto t2 = std::chrono::high_resolution_clock::now();

    int ifps = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    int target_ifps = (1.0f / framerate_) * 1000;

    int diff_ifps = target_ifps - ifps;

    if (diff_ifps > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(diff_ifps));
    }
  }
}

void MyWindow::shutdown() {
  for (const auto &view : views_) {
    view->shutdown();
  }
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();

  glfwDestroyWindow(window_);
  glfwTerminate();
  ImGui::DestroyContext();

  glDeleteTextures(3, image_textures_);
}

void MyWindow::addView(AView *view) { views_.push_back(view); }

unsigned char *MyWindow::getImage() { return image_data_; }

GLuint *MyWindow::getTextures() { return image_textures_; }
