#pragma once

#define EVENT_CLASS_TYPE(type)  static EventType getStaticType() { return EventType::type; } \
                                virtual EventType getEventType() const override { return getStaticType(); } \
                                virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual std::uint8_t getCategoryFlags() const override { return static_cast<std::uint8_t>(category); }

#define BIND_EVENT_FUNCTION(fn) [this]<typename T>(T&& x) { return fn(std::forward<T>(x)); }