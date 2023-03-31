#ifndef ABSTRACT_VIEW_H
#define ABSTRACT_VIEW_H

class AView {
 public:
  virtual ~AView() = default;
  virtual void init() {}
  virtual void update() = 0;
  virtual void shutdown() {}
  bool isInit() { return init_; }

 private:
  bool init_ = false;
};

#endif  // ABSTRACT_VIEW_H