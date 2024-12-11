#include "PanelScene.h"
#include <imgui.h>
#include "MyGameEngine/Engine.h"
#include "MyGameEngine/MyWindow.h"
#include "MyGameEngine/Scene.h"

PanelScene::PanelScene(std::string name) : Panel(name, WINDOW_WIDTH * 0.6, WINDOW_HEIGHT - 219)
{
	//SwitchState();
}

PanelScene::~PanelScene() {}

bool PanelScene::Draw()
{
    ImGui::SetNextWindowSize(ImVec2(width, Engine::Instance().window->height() - 219));
    ImGui::SetNextWindowPos(ImVec2(WINDOW_WIDTH * 0.15, 19));

    ImGui::Begin("Scene", &showWindow, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    //RenderScene();
    ImGui::End();

    if (!showWindow) SwitchState();
    return true;
}

void PanelScene::RenderScene()
{
	LOG(LogType::LOG_INFO, "Rendering Scene...");

    // Optionally handle mouse inputs if you need interaction in the scene:
    if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered()) {
        // Handle click logic for scene interaction, e.g., selecting objects
        //Engine::Instance().scene->SelectObjectAtMouse();
    }
}
