#ifndef RO_WINDOW_MOUSEMOTIONEVENT_HPP
#define RO_WINDOW_MOUSEMOTIONEVENT_HPP

class MouseMotionEvent {
public:
	explicit MouseMotionEvent(int x, int y, int dx, int dy)
		: x_{x}, y_{y}, dx_{dx}, dy_{dy} {}

	int x() const { return x_; }
	int y() const { return y_; }
	int dx() const { return dx_; }
	int dy() const { return dy_; }

private:
	int x_, y_;
	int dx_, dy_;
};

#endif // RO_WINDOW_MOUSEMOTIONEVENT_HPP