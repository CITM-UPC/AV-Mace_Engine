#pragma once

#include <GL/glew.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>

#include "types.h"

class MyWindow {

	SDL_Window* _window = nullptr;
	void* _ctx = nullptr;

	unsigned short _width = WINDOW_WIDTH;
	unsigned short _height = WINDOW_HEIGHT;

	GLuint _framebuffer = 0; // Framebuffer for rendering
	GLuint _renderTexture = 0; // Texture attached to the framebuffer
	GLuint _depthBuffer = 0; // Depth buffer for the framebuffer

public:

	int width() const { return _width; }
	int height() const { return _height; }
	void setWidth(int &width) { _width = width; }
	void setHeight(int& height) { _height = height; }
	double aspect() const { return static_cast<double>(_width) / _height; }

	auto* windowPtr() const { return _window; }
	auto* contextPtr() const { return _ctx; }

	MyWindow();
	MyWindow(MyWindow&&) noexcept = delete;
	MyWindow(const MyWindow&) = delete;
	MyWindow& operator=(const MyWindow&) = delete;
	~MyWindow();

	void Awake();
	void Start();

	void open(const char* title, unsigned short width, unsigned short height);
	void close();
	bool isOpen() const { return _window != nullptr; }

	void swapBuffers() const;

	void ResizeWindow(int width, int height) {
		SDL_SetWindowSize(_window, width, height);
		_width = width;
		_height = height;
	}

	GLuint GetFramebufferTexture() const { return _renderTexture; }
	void InitializeFramebuffer();

};