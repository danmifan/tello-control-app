#include "event.h"

eventpp::EventDispatcher<std::string, void(const Event&)> gevent_dispatcher;
