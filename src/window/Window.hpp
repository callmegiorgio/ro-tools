#ifndef RO_WINDOW_WINDOW_HPP
#define RO_WINDOW_WINDOW_HPP

// STL
#include <memory>
#include <string_view>

// Apollo
#include "KeyEvent.hpp"
#include "MouseButtonEvent.hpp"
#include "MouseMotionEvent.hpp"
#include "MouseWheelEvent.hpp"

class Window {
public:
    Window();
    virtual ~Window();

	bool show(int width, int height, std::string_view title, bool resizable = false, bool fullscreen = false);
	void move(int x, int y);
	void resize(int width, int height);
    void close();
	void loop();

	int x() const { return mouse_x_; }
	int y() const { return mouse_y_; }
    int width() const { return width_; }
    int height() const { return height_; }

protected:
	virtual void update(double dt) {}
	virtual void draw() {}
	virtual void onClose() {}
	virtual void onMove(int x, int y) {}
	virtual void onResize(int width, int height) {}
    virtual void onKeyEvent(KeyEvent evt) {}
    virtual void onMouseButtonEvent(MouseButtonEvent evt) {}
    virtual void onMouseMotionEvent(MouseMotionEvent evt) {}
    virtual void onMouseWheelEvent(MouseWheelEvent evt) {}
	virtual void onMouseEnter() {}
	virtual void onMouseLeave() {}

private:
	struct Impl;
	friend struct Impl;

	std::unique_ptr<Impl> impl_;
	int mouse_x_;
	int mouse_y_;
	int width_;
	int height_;
	double last_time_;
};

#endif // RO_WINDOW_WINDOW_HPP