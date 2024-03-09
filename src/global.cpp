#include "global.h"

eventpp::EventDispatcher<std::string, void()> gbutton_event_dispatcher;

eventpp::EventDispatcher<std::string, void(const std::string& )> gbutton_input_event_dispatcher;

eventpp::EventDispatcher<int, void(const RCEvent& event)> grc_event_dispatcher;



