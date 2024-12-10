#include "PanelHierarchy.h"

#include <imgui.h>
#include <memory>

#include "MyGameEngine/Engine.h"
#include "MyGameEngine/MyWindow.h"
#include "MyGameEngine/Scene.h"
#include "MyGameEngine/types.h"
#include "PanelInspector.h"
#include "MyGameEngine/Log.h"

PanelHierarchy::PanelHierarchy(std::string name) : Panel(name, WINDOW_WIDTH * 0.2, WINDOW_HEIGHT - 219)
{
	SwitchState();
}

PanelHierarchy::~PanelHierarchy() {}

bool PanelHierarchy::Draw()
{
   ImGui::SetNextWindowSize(ImVec2(width, Engine::Instance().window->height() - 219));
   ImGui::SetNextWindowPos(ImVec2(0, 19));

   ImGui::Begin("Hierarchy", &showWindow, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

   if (ImGui::IsMouseClicked(0) && !ImGui::IsAnyItemHovered() && ImGui::IsWindowHovered()) {  SetSelectedGameObject(nullptr);  }
   
   ImGui::BeginChild("HierarchyRoot", ImVec2(0, 0), true);

   for (const std::shared_ptr<GameObject>& gameObjectPtr : Engine::Instance().scene->root()->children()) {
	   DrawGameObjectTree(gameObjectPtr.get());
   }

   // Updated drop target handling
   if (ImGui::BeginDragDropTarget()) 
   {
       if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT_DRAG")) 
       {
           IM_ASSERT(payload->DataSize == sizeof(GameObject*));
           GameObject* draggedObject = *static_cast<GameObject**>(payload->Data);

           if (draggedObject) {
               GameObject* root = Engine::Instance().scene->root();
               // Compare pointers directly now that parent() returns a pointer
               if (draggedObject->parent() != root) { draggedObject->setParent(root); }
           }
       }
       ImGui::EndDragDropTarget();
   }

   ImGui::EndChild();

   // Right-click context menu
   if (ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered()) {
       ImGui::OpenPopup("CreateGameObjectPopup");
   }

   // Create GameObjects Popup
   if (ImGui::BeginPopup("CreateGameObjectPopup")) 
   {
       LOG(LogType::LOG_INFO, "Popup Called");
	   if (ImGui::MenuItem("Cut")) {

	   }
       if (ImGui::MenuItem("Copy")) {

	   }
       if (ImGui::MenuItem("Paste")) {
       }
	   if (ImGui::MenuItem("Paste As Child")) {
	   }
	   ImGui::Separator();
	   if (ImGui::MenuItem("Rename")) {
	   }
	   if (ImGui::MenuItem("Duplicate")) {
	   }
	   if (ImGui::MenuItem("Delete")) {
	   }
	   ImGui::Separator();
	   if (ImGui::MenuItem("Select Children")) {
	   }
	   ImGui::Separator();
       if (ImGui::MenuItem("Create Empty")) {
           Engine::Instance().scene->CreateGameObject();
       }
	   if (ImGui::BeginMenu("Primitives")) {
	   }
	   if (ImGui::BeginMenu("Material")) {
	   }
       ImGui::EndPopup();
   }

   ImGui::End();

   if (!showWindow) {
       SwitchState();
   }

   Engine::Instance().scene->selectedGameObject = selectedGameObject();

   return true;
}

void PanelHierarchy::DrawGameObjectTree(GameObject* gameObject)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	if (selectedGameObject() == gameObject) flags |= ImGuiTreeNodeFlags_Selected;

	bool isNodeOpen = ImGui::TreeNodeEx(gameObject->name().c_str(), flags);
	if (ImGui::IsItemClicked()) SetSelectedGameObject(gameObject);

	if (isNodeOpen) {
		for (const std::shared_ptr<GameObject>& childObjectPtr : gameObject->children()) {
			DrawGameObjectTree(childObjectPtr.get());
		}
		ImGui::TreePop();
	}
}
