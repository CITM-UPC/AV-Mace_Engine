#include "PanelHierarchy.h"

#include <imgui.h>
#include <memory>

#include "MyGameEngine/Engine.h"
#include "MyGameEngine/MyWindow.h"
#include "MyGameEngine/Scene.h"
#include "MyGameEngine/types.h"
#include "PanelInspector.h"
#include "MyGameEngine/Log.h"

PanelHierarchy::PanelHierarchy(std::string name) : Panel(name, WINDOW_WIDTH * 0.15, WINDOW_HEIGHT - 219)
{
	SwitchState();
}

PanelHierarchy::~PanelHierarchy() {}

bool PanelHierarchy::Draw()
{
   ImGui::SetNextWindowSize(ImVec2(width, Engine::Instance().window->height() - 219));
   ImGui::SetNextWindowPos(ImVec2(0, 19));

   ImGui::Begin("Hierarchy", &showWindow, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

   if (ImGui::IsMouseClicked(0) && !ImGui::IsAnyItemHovered() && ImGui::IsWindowHovered()) SetSelectedGameObject(nullptr);
   for (auto& gameObjectPtr : Engine::Instance().scene->root()->children()) DrawGameObjectTree(gameObjectPtr.get());

   // Updated drop target handling
   if (ImGui::BeginDragDropTarget()) 
   {
       if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT_DRAG")) 
       {
           IM_ASSERT(payload->DataSize == sizeof(GameObject*));
           GameObject* draggedObject = *static_cast<GameObject**>(payload->Data);

           if (draggedObject) {
               GameObject* root = Engine::Instance().scene->root();
			   if (draggedObject->parent() != root) draggedObject->reparent(root);
           }
       }
       ImGui::EndDragDropTarget();
   }

   // Right-click context menu
   if (ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered()) {
       ImGui::OpenPopup("CreateGameObjectPopup");
   }

   // Create GameObjects Popup
   if (ImGui::BeginPopup("CreateGameObjectPopup")) 
   {
	   if (ImGui::MenuItem("Cut", nullptr, nullptr, _selectedGameObject != nullptr)) 
	   {
	   }
       if (ImGui::MenuItem("Copy", nullptr, nullptr, _selectedGameObject != nullptr)) 
	   {
	   }
       if (ImGui::MenuItem("Paste", nullptr, nullptr, _selectedGameObject != nullptr)) 
	   {
	   }
	   if (ImGui::MenuItem("Paste As Child", nullptr, nullptr, _selectedGameObject != nullptr)) 
	   {
	   }
	   ImGui::Separator();
	   if (ImGui::MenuItem("Rename", nullptr, nullptr, _selectedGameObject != nullptr)) 
	   {
	   }
	   if (ImGui::MenuItem("Duplicate", nullptr, nullptr, _selectedGameObject != nullptr)) 
	   {
		   Engine::Instance().scene->CreateGameObject(_selectedGameObject->name() + "_Copy");
	   }
	   if (ImGui::MenuItem("Delete", nullptr, nullptr, _selectedGameObject != nullptr)) 
	   {
		   _selectedGameObject->Delete();
		   _selectedGameObject = nullptr;
	   }
	   ImGui::Separator();
	   if (ImGui::MenuItem("Select Children", nullptr, nullptr, _selectedGameObject != nullptr)) 
	   {
		   for (auto& child : _selectedGameObject->children()); //child->setSelected(true);
	   }
	   ImGui::Separator();
       if (ImGui::MenuItem("Create Empty")) Engine::Instance().scene->CreateGameObject();
	   if (ImGui::BeginMenu("Primitives")) 
	   {
		   if (ImGui::MenuItem("Cube"))  Engine::Instance().scene->CreateCube();
		   if (ImGui::MenuItem("Plane"))  Engine::Instance().scene->CreatePlane();
		   if (ImGui::MenuItem("Sphere"))  Engine::Instance().scene->CreateSphere();
		   if (ImGui::MenuItem("Cylinder"))  Engine::Instance().scene->CreateCylinder();
		   if (ImGui::MenuItem("Cone"))  Engine::Instance().scene->CreateCone();
		   if (ImGui::MenuItem("Torus"))  Engine::Instance().scene->CreateTorus();
		   ImGui::EndMenu();
	   }
	   if (ImGui::BeginMenu("Material")) 
	   {
		   if (ImGui::MenuItem("Create Material")) {}
		   if (ImGui::MenuItem("Create Shader")) {}
		   ImGui::EndMenu();
	   }
       ImGui::EndPopup();
   }
   ImGui::End();

   if (!showWindow) SwitchState();
   Engine::Instance().scene->selectedGameObject = selectedGameObject();
   return true;
}

void PanelHierarchy::DrawGameObjectTree(GameObject* gameObject)
{
	if (!gameObject) return;

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow |
		ImGuiTreeNodeFlags_OpenOnDoubleClick |
		ImGuiTreeNodeFlags_SpanAvailWidth;

	if (selectedGameObject() == gameObject) flags |= ImGuiTreeNodeFlags_Selected;
	if (gameObject->children().empty())	flags |= ImGuiTreeNodeFlags_Leaf;

	// Add drag source identifier
	char uniqueLabel[256];
	const size_t labelSize = sizeof(uniqueLabel);
	int result = sprintf_s(uniqueLabel, labelSize, "%s##%p", gameObject->name().c_str(), static_cast<void*>(gameObject));
	if (result < 0) strcpy_s(uniqueLabel, labelSize, gameObject->name().c_str());
	bool isNodeOpen = ImGui::TreeNodeEx(gameObject->name().c_str(), flags);

	if (ImGui::IsItemClicked()) {
		SetSelectedGameObject(gameObject);
		Engine::Instance().scene->selectedGameObject = gameObject;
	}
	else SetSelectedGameObject(Engine::Instance().scene->selectedGameObject);

	// Begin drag source
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) 
	{
		if (gameObject) {
			ImGui::SetDragDropPayload("GAMEOBJECT_DRAG", &gameObject, sizeof(GameObject*));
			ImGui::Text("Moving %s", gameObject->name().c_str());
		}
		ImGui::EndDragDropSource();
	}

	// Handle drop target (for parenting to this object)
	if (ImGui::BeginDragDropTarget()) 
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT_DRAG")) {
			IM_ASSERT(payload->DataSize == sizeof(GameObject*));
			GameObject* draggedObject = *static_cast<GameObject**>(payload->Data);
			if (draggedObject && draggedObject != gameObject) draggedObject->reparent(gameObject);
		}
		ImGui::EndDragDropTarget();
	}

	if (isNodeOpen) {
		auto children = gameObject->children();
		for (const auto& child : children) if (child) DrawGameObjectTree(child.get());
		ImGui::TreePop();
	}
}
