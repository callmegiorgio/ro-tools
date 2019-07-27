#ifndef RO_WINDOW_MODIFIERKEYS_HPP
#define RO_WINDOW_MODIFIERKEYS_HPP

enum class ModifierKeys {
	None = 0x000,
	Shift = 0x001,
	Ctrl = 0x002,
	Alt = 0x004,
	Super = 0x008,
	CapsLock = 0x010,
	NumLock = 0x020
};

inline int operator&(ModifierKeys lhs, ModifierKeys rhs) { return static_cast<int>(lhs) & static_cast<int>(rhs); }
inline int operator|(ModifierKeys lhs, ModifierKeys rhs) { return static_cast<int>(lhs) | static_cast<int>(rhs); }
inline int operator+(ModifierKeys lhs, ModifierKeys rhs) { return static_cast<int>(lhs) + static_cast<int>(rhs); }

#endif // RO_WINDOW_MODIFIERKEYS_HPP