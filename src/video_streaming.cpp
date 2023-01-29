#include "video_streaming.h"

#include <unistd.h>
#include <iostream>

VideoStreaming::~VideoStreaming() { close(video_socket_); }

void VideoStreaming::init() {
  video_socket_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (video_socket_ == -1) {
    std::cout << "Errour could not initialize VIDEO socket" << std::endl;
    return;
  }

  video_addr_.sin_family = AF_INET;
  video_addr_.sin_addr.s_addr = inet_addr("0.0.0.0");
  video_addr_.sin_port = htons(11111);
  if (bind(video_socket_, (sockaddr *)&video_addr_, sizeof(video_addr_)) == -1) {
    std::cout << "VIDEO socket bind failed" << std::endl;
  }

  source_addr_size_ = sizeof(video_addr_);
}