#include "Window.hpp"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static void onError(int, const char* msg)
{
	std::cout << msg << std::endl;
}

struct Window::Impl {
	void setCallbacks(Window* instance);

	GLFWwindow* window = nullptr;
	int mouse_x = 0;
	int mouse_y = 0;
};

void Window::Impl::setCallbacks(Window* instance)
{
	glfwSetWindowUserPointer(window, instance);

	double x, y;
	glfwGetCursorPos(window, &x, &y);
	mouse_x += static_cast<int>(x);
	mouse_y += static_cast<int>(y);

	// Key callback
	glfwSetKeyCallback(window,
		[](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			static_cast<Window*>(glfwGetWindowUserPointer(window))->onKeyEvent(KeyEvent(
				action == GLFW_PRESS ? KeyEvent::Pressed : KeyEvent::Released,
				key,
				scancode,
				static_cast<ModifierKeys>(mods))
			);
		});

	// Mouse action callback
	glfwSetMouseButtonCallback(window,
		[](GLFWwindow* window, int button, int action, int mods)
		{
			MouseButton mouse_btn;

			switch (button)
			{
				case GLFW_MOUSE_BUTTON_LEFT: mouse_btn = MouseButton::Left; break;
				case GLFW_MOUSE_BUTTON_RIGHT: mouse_btn = MouseButton::Right; break;
				case GLFW_MOUSE_BUTTON_MIDDLE: mouse_btn = MouseButton::Middle; break;
				case GLFW_MOUSE_BUTTON_4: mouse_btn = MouseButton::X1; break;
				case GLFW_MOUSE_BUTTON_5: mouse_btn = MouseButton::X2; break;
			}

			double x, y;
			glfwGetCursorPos(window, &x, &y);

			static_cast<Window*>(glfwGetWindowUserPointer(window))->onMouseButtonEvent(MouseButtonEvent(
				action == GLFW_PRESS ? MouseButtonEvent::Pressed : MouseButtonEvent::Released,
				static_cast<int>(x),
				static_cast<int>(y),
				mouse_btn,
				static_cast<ModifierKeys>(mods))
			);
		});

	// Mouse move callback
	glfwSetCursorPosCallback(window,
		[](GLFWwindow* window, double xpos, double ypos)
		{
			auto instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
			int& mouse_x = instance->impl_->mouse_x;
			int& mouse_y = instance->impl_->mouse_y;

			int x = static_cast<int>(xpos);
			int y = static_cast<int>(ypos);

			instance->onMouseMotionEvent(MouseMotionEvent(x, y, x - mouse_x, y - mouse_y));
			mouse_x += x;
			mouse_y += y;
		});

	// Mouse scroll callback
	glfwSetScrollCallback(window,
		[](GLFWwindow* window, double xoffset, double yoffset)
		{
			static_cast<Window*>(glfwGetWindowUserPointer(window))->onMouseWheelEvent(
				MouseWheelEvent(static_cast<int>(xoffset), static_cast<int>(yoffset)));
		});

	// Framebuffer resize callback
	glfwSetFramebufferSizeCallback(window,
		[](GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
			auto instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
			
			instance->width_ = width;
			instance->height_ = height;
			instance->onResize(width, height);
		});

	// Window move callback
	glfwSetWindowPosCallback(window,
		[](GLFWwindow* window, int xpos, int ypos)
		{
			auto instance = static_cast<Window*>(glfwGetWindowUserPointer(window));

			instance->mouse_x_ += xpos;
			instance->mouse_y_ += ypos;
			instance->onMove(xpos, ypos);
		});
}

Window::Window() : impl_{std::make_unique<Impl>()}
{
	// TODO: should it throw?
    glfwInit();
}

Window::~Window()
{
    glfwTerminate();
}

bool Window::show(int width, int height, std::string_view title, bool resizable, bool fullscreen)
{
	glfwSetErrorCallback(onError);

	glfwWindowHint(GLFW_RESIZABLE, static_cast<int>(resizable));
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	impl_->window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

	if (!impl_->window)
		return false;

    glfwMakeContextCurrent(impl_->window);

	// Load OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		impl_->window = nullptr;
		return false;
	}

	width_ = width;
	height_ = height;
	impl_->setCallbacks(this);

    return true;
}

void Window::move(int x, int y)
{
	if (impl_->window) {
		mouse_x_ += x;
		mouse_y_ += y;
		glfwSetWindowPos(impl_->window, x, y);
	}
}

void Window::resize(int width, int height)
{
	if (impl_->window) {
		width_ = width;
		height_ = height;
		glfwSetWindowSize(impl_->window, width, height);
	}
}

void Window::close()
{
	if (impl_->window)
		glfwSetWindowShouldClose(impl_->window, true);
}

void Window::loop()
{
	if (!impl_->window)
		return;

	last_time_ = glfwGetTime();

	while (!glfwWindowShouldClose(impl_->window))
	{
		double current_time = glfwGetTime();		
		
		update(current_time - last_time_);
		draw();
		
		glfwSwapBuffers(impl_->window);
		glfwPollEvents();

		last_time_ = current_time;
	}

	onClose();
}