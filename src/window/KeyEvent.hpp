#ifndef RO_WINDOW_KEYEVENT_HPP
#define RO_WINDOW_KEYEVENT_HPP

#include "Key.hpp"
#include "ModifierKeys.hpp"

class KeyEvent {
public:
	enum Action {
		Pressed,
		Released,
		Repeated,
	};

    explicit KeyEvent(Action action, int keycode, int scancode, ModifierKeys modifiers = ModifierKeys::None)
        : action_{action}
		, keycode_{keycode}
        , scancode_{scancode}
        , modifiers_{modifiers} {}

	Action action() const { return action_; }
	Key key() const { return static_cast<Key>(keycode_); }
    int keycode() const { return keycode_; }
	int scancode() const { return scancode_; }
	ModifierKeys modifiers() const { return modifiers_; }
	bool shift() const { return modifiers_ & ModifierKeys::Shift; }
	bool ctrl() const { return modifiers_ & ModifierKeys::Ctrl; }
	bool alt() const { return modifiers_ & ModifierKeys::Alt; }
	bool super() const { return modifiers_ & ModifierKeys::Super; }
	bool capsLock() const { return modifiers_ & ModifierKeys::CapsLock; }
	bool numLock() const { return modifiers_ & ModifierKeys::NumLock; }

private:
	Action action_;
    int keycode_;
    int scancode_;
	ModifierKeys modifiers_;
};

#endif // RO_WINDOW_KEYEVENT_HPP