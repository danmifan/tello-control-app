#ifndef ABSTRACT_WIDGET_H
#define ABSTRACT_WIDGET_H

class AbstractWidget {
 public:
  virtual void init() {}
  virtual void update() = 0;
  virtual void shutdown() {}
  bool isInit() { return init_; }

 private:
  bool init_ = false;
};

#endif  // ABSTRACT_WIDGET_H