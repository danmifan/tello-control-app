#include "window/window.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

MyWindow::MyWindow(int width, int height) : width_(width), height_(height) {}

void glfw_error_callback(int error, const char *description) {
  std::cout << "GLFW error : " << error << " : " << description << std::endl;
}

int MyWindow::init() {
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) {
    return -1;
  }

  window_ = glfwCreateWindow(800, 600, "Window", NULL, NULL);

  if (!window_) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window_);
  glfwSwapInterval(1);

  // bool err = gladLoadGL() == 0;
  // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
  //   std::cout << "Failed to initialize OpenGL Loader" << std::endl;
  // }

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window_, true);
  ImGui_ImplOpenGL3_Init("#version 130");

  return 1;
}

void MyWindow::update() {
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  while (!glfwWindowShouldClose(window_)) {
    glfwPollEvents();

    // Render loop

    // ImGUI new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // GUI
    ImGui::Begin("zob");
    ImGui::Text("%s", "Test");
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
  }
}

void MyWindow::shutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();

  glfwDestroyWindow(window_);
  glfwTerminate();
  ImGui::DestroyContext();
}