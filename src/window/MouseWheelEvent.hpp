#ifndef RO_WINDOW_MOUSEWHEELEVENT_HPP
#define RO_WINDOW_MOUSEWHEELEVENT_HPP

class MouseWheelEvent {
public:
	explicit MouseWheelEvent(int x, int y) : x_{x}, y_{y} {}

	int x() const { return x_; }
	int y() const { return y_; }

private:
	int x_, y_;
};

#endif // RO_WINDOW_MOUSEWHEELEVENT_HPP