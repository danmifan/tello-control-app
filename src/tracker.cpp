#include "tracker.h"

#include "logger.h"

#include "event.h"

Tracker::Tracker() {
  cv_tracker_ = cv::legacy::TrackerMedianFlow::create();

  gevent_dispatcher.appendListener(
      "TrackerNewTarget",
      eventpp::argumentAdapter<void(const MouseEvent &)>([&](const MouseEvent &e) {
        new_target_received_ = true;
        target_x = e.x;
        target_y = e.y;
        std::stringstream ss;
        ss << "New tracker target : " << e.x << " " << e.y;
        Log::get().info(ss.str());
      }));
}

bool Tracker::track(cv::Mat frame, TrackData &target) {
  if (new_target_received_) {
    int tl_x = target_x - rect_width_ / 2;
    int tl_y = target_y - rect_height_ / 2;

    cv::Rect2d rect(tl_x, tl_y, rect_width_, rect_height_);

    cv::rectangle(frame, rect, cv::Scalar(255, 255, 255, 255));

    // int delta_pos_x = -(960 / 2 - event_->data.x);
    // int delta_pos_y = -(720 / 2 - event_->data.y);
    new_target_received_ = false;
    // Log::get().info(std::to_string(delta_pos_.x) + " " +
    // std::to_string(delta_pos_.y);

    if (!init_) {
      cv_tracker_->init(frame, rect);
      init_ = true;
    } else {
      cv_tracker_->clear();

      cv_tracker_ = cv::legacy::TrackerMedianFlow::create();

      cv_tracker_->init(frame, rect);
    }
  }

  if (init_) {
    cv::Rect2d tracked_object;

    if (cv_tracker_->update(frame, tracked_object)) {
      cv::rectangle(frame, tracked_object, cv::Scalar(0, 255, 0, 255));

      cv::circle(frame, cv::Point(960 / 2, 720 / 2), 10, cv::Scalar(0, 0, 255, 255));

      int target_center_x = tracked_object.x + tracked_object.width / 2;
      int target_center_y = tracked_object.y + tracked_object.height / 2;

      cv::circle(frame, cv::Point(target_center_x, target_center_y), 10,
                 cv::Scalar(255, 255, 255, 255));

      cv::line(frame, cv::Point(960 / 2, 720 / 2), cv::Point(target_center_x, target_center_y),
               cv::Scalar(255, 255, 255, 255));

      // int dx = target_center_x - (960 / 2);
      // int dy = target_center_y - (720 / 2);

      Log::get().info("Width : " + std::to_string(tracked_object.width) +
                      " height : " + std::to_string(tracked_object.height));

      Vec2i position = {target_center_x, target_center_y};
      target = {position, tracked_object.width, tracked_object.height};
      return true;

      // Log::get().info("dx : " + std::to_string(dx) + " dy : " + std::to_string(dy));
    }
  }

  return false;
}