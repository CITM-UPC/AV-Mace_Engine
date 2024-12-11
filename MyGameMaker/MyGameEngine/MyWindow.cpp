#include "MyWindow.h"

#include <GL/glew.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_opengl.h>
#include <exception>
#include <stdexcept>

#include "Engine.h"

using namespace std;

MyWindow::MyWindow() : _window(nullptr), _ctx(nullptr), _framebuffer(0), _renderTexture(0), _depthBuffer(0) {}

MyWindow::~MyWindow()
{
    glDeleteFramebuffers(1, &_framebuffer);
    glDeleteTextures(1, &_renderTexture);
    glDeleteRenderbuffers(1, &_depthBuffer);
    close();
}

void MyWindow::Awake() {
    LOG(LogType::LOG_INFO, "# Setting Window Atributes...");
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
}

void MyWindow::Start()
{
    LOG(LogType::LOG_INFO, "# Creating Window...");

    if (isOpen()) return;
    _window = SDL_CreateWindow("AV-MACE ENGINE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!_window) throw exception(SDL_GetError());

    LOG(LogType::LOG_INFO, "# Creating OpenGL context...");

    _ctx = SDL_GL_CreateContext(_window);
    if (!_ctx) throw exception(SDL_GetError());
    if (SDL_GL_MakeCurrent(_window, _ctx) != 0) throw exception(SDL_GetError());
    if (SDL_GL_SetSwapInterval(1) != 0) throw exception(SDL_GetError());

    glewExperimental = GL_TRUE; // Ensure compatibility
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Failed to initialize GLEW!");
    }

    InitializeFramebuffer();
}

void MyWindow::close() {
    if (!isOpen()) return;

    LOG(LogType::LOG_INFO, "# Cleaning up Window...");

    glDeleteFramebuffers(1, &_framebuffer);
    glDeleteTextures(1, &_renderTexture);
    glDeleteRenderbuffers(1, &_depthBuffer);

    SDL_GL_DeleteContext(_ctx);
    _ctx = nullptr;

    SDL_DestroyWindow(static_cast<SDL_Window*>(_window));
    _window = nullptr;
}

void MyWindow::swapBuffers() const { SDL_GL_SwapWindow(static_cast<SDL_Window*>(_window)); }

void MyWindow::InitializeFramebuffer() {
    LOG(LogType::LOG_INFO, "# Initializing Framebuffer...");

    glGenFramebuffers(1, &_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

    // Crear la textura de renderizado
    glGenTextures(1, &_renderTexture);
    glBindTexture(GL_TEXTURE_2D, _renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _renderWidth, _renderHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _renderTexture, 0);

    // Crear el depth buffer
    glGenRenderbuffers(1, &_depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _renderWidth, _renderHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);

    // Comprobar si el framebuffer está completo
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw exception("Framebuffer is not complete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Desvincular el framebuffer
}

void MyWindow::setRenderSize(int x, int y, int width, int height)
{
    _renderWidth = width;
    _renderHeight = height;

    // Reasignar el tamaño del framebuffer
    glBindTexture(GL_TEXTURE_2D, _renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _renderWidth, _renderHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _renderWidth, _renderHeight);

    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw exception("Framebuffer resize failed!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Actualizar el viewport de OpenGL
    glViewport(x, y, _renderWidth, _renderHeight);
}
