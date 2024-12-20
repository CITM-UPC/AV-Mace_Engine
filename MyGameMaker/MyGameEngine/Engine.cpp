#include "Engine.h"

#include "MyWindow.h"
#include "Input.h"
#include "Renderer.h"
#include "Scene.h"
#include "Debug.h"

//devil
#include <IL/il.h>
#include <IL/ilu.h>

void Engine::Awake()
{
    LOG(LogType::LOG_INFO, "Welcome to AV_Mace Engine!");
    window = new MyWindow();
    input = new Input();
	renderer = new Renderer();
	scene = new Scene("Scene");

    LOG(LogType::LOG_CHANGE_ENV, "------------- Application Init -------------");
    window->Awake();
    input->Awake();

    LOG(LogType::LOG_INFO, "# Initializing DevIL...");
    ilInit();
    iluInit();

    int devilVersion = ilGetInteger(IL_VERSION_NUM);
    int iluVersion = ilGetInteger(ILU_VERSION_NUM);

    LOG(LogType::LOG_APPINFO, "DevIL version: %s", std::to_string(devilVersion).c_str());
    LOG(LogType::LOG_APPINFO, "ILU version: %s", std::to_string(iluVersion).c_str());
}

void Engine::Start()
{
	window->Start();
    input->Start();
	renderer->Start();
	scene->Start();
    window->setRenderSize(window->renderX(), window->renderY(), window->renderWidth(), window->renderHeight());
}

bool Engine::PreUpdate()
{
    input->PreUpdate();
    return true;
}

void Engine::Update(double& dt)
{
    this->dt = dt;
    window->setRenderWidth(window->width() - ((WINDOW_WIDTH * 0.25) + (WINDOW_WIDTH * 0.15)));
    window->setRenderHeight(window->height() - 200);
	scene->Update(dt);
}

void Engine::PostUpdate()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //draw floor grid
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(&scene->camera()->GetComponent<Camera>()->view()[0][0]);


    //draw things
    drawFloorGrid(16, 0.25);
    scene->Draw(scene->root());
    glViewport(window->renderX(), window->renderY(), window->renderWidth(), window->renderHeight());
    scene->camera()->GetComponent<Camera>()->aspect() = static_cast<double>(window->renderWidth()) / window->renderHeight();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(&scene->camera()->GetComponent<Camera>()->projection()[0][0]);
}

void Engine::CleanUp()
{
    scene->CleanUp();
    window->close();
    input->CleanUp();
    delete scene;
    delete window;
    delete input;
}


std::vector<LogInfo> Engine::GetLogs()
{
    return logs;
}

void Engine::AddLog(LogType type, const char* entry)
{
    std::string toAdd = entry;
    LogInfo info = { type, toAdd };

    logs.push_back(info);
}

void Engine::CleanLogs()
{
    logs.clear();
}

Engine::~Engine()
{

}

void Engine::SwapBuffers()
{
    window->swapBuffers();
}
