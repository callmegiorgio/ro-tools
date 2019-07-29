#ifndef RO_WINDOW_MOUSEBUTTONEVENT_HPP
#define RO_WINDOW_MOUSEBUTTONEVENT_HPP

#include "ModifierKeys.hpp"
#include "MouseButton.hpp"

class MouseButtonEvent {
public:
	enum Action {
		Pressed,
		Released
	};

    explicit MouseButtonEvent(Action action, int x, int y, MouseButton button, ModifierKeys modifiers = ModifierKeys::None)
		: action_{action}
		, x_{x}
		, y_{y}
		, button_{button}
		, modifiers_{modifiers} {}
		
	Action action() const { return action_; }
	int x() const { return x_; }
	int y() const { return y_; }
	MouseButton button() const { return button_; }
	ModifierKeys modifiers() const { return modifiers_; }
	bool shift() const { return modifiers_ & ModifierKeys::Shift; }
	bool ctrl() const { return modifiers_ & ModifierKeys::Ctrl; }
	bool alt() const { return modifiers_ & ModifierKeys::Alt; }
	bool super() const { return modifiers_ & ModifierKeys::Super; }
	bool capsLock() const { return modifiers_ & ModifierKeys::CapsLock; }
	bool numLock() const { return modifiers_ & ModifierKeys::NumLock; }

private:
	Action action_;
	int x_;
	int y_;
	MouseButton button_;
	ModifierKeys modifiers_;
};

#endif // RO_WINDOW_MOUSEBUTTONEVENT_HPP