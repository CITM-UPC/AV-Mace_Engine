#include "PanelMenu.h"
#include <imgui.h>

#include "MyGUI.h"

#include "MyGameEngine/Engine.h"
#include "MyGameEngine/Scene.h"
#include "MyGameEngine/GameObject.h"
#include "MyGameEngine/Mesh.h"
#include "MyGameEngine/Log.h"

#include "PanelConsole.h"
#include "PanelInspector.h"
#include "PanelHierarchy.h"
#include "PanelConfiguration.h"
#include "PanelScene.h"

PanelMenu::PanelMenu(std::string name) : Panel(name, 0, 0)
{
	SwitchState();
}

PanelMenu::~PanelMenu() {}

bool PanelMenu::Draw()
{
    if (ImGui::BeginMainMenuBar()) 
    {
        // File menu
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene", "Ctrl+N", nullptr, false)) {
                // Action for "New" item
            }
            if (ImGui::MenuItem("Open Scene", "Ctrl+O", nullptr, false)) {
                // Action for "Open" item
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Save", "Ctrl+S", nullptr, false)) {
                // Action for "Save" item
            }  
            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S", nullptr, false)) {
                // Action for "Save" item
            }
            ImGui::Separator();
            if (ImGui::MenuItem("New Project", "Ctrl+Shift+N", nullptr, false)) {
                // Action for "Exit" item
            }
            if (ImGui::MenuItem("Open Project", "Ctrl+Shift+O", nullptr, false)) {
                // Action for "Exit" item
            }
            if (ImGui::MenuItem("Save Project", nullptr, nullptr, false)) {
                // Action for "Exit" item
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Alt+F4")) {
				Engine::Instance().quit = true;
            }
            ImGui::EndMenu();
        }

        // Edit menu
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z", nullptr, false)) {
                // Action for "Undo" item
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y", nullptr, false)) {
                // Action for "Redo" item
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Select All", "Ctrl+A", nullptr, false)) {
                // Action for "Undo" item
            }
            if (ImGui::MenuItem("Deselect All", "Shift+D", nullptr, false)) {
                // Action for "Undo" item
            }
            if (ImGui::MenuItem("Select Children", "Shift+C", nullptr, false)) {
                // Action for "Undo" item
            }
            if (ImGui::MenuItem("Invert Selection", "Ctrl+I", nullptr, false)) {
                // Action for "Undo" item
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Duplicate", "Ctrl+D", nullptr, false)) {
                // Action for "Undo" item
            }
            if (ImGui::MenuItem("Delete", "Supr", nullptr, MyGUI::Instance().hierarchy().selectedGameObject())) {
				MyGUI::Instance().hierarchy().selectedGameObject()->Delete();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Play", "Ctrl+P", nullptr)) {
                LOG(LogType::LOG_INFO, "Play button pressed");
            }
            if (ImGui::MenuItem("Pause", "Ctrl+Shift+P", nullptr)) {
                LOG(LogType::LOG_INFO, "Pause button pressed");
            }
            if (ImGui::MenuItem("Step", "Ctrl+Alt+P", nullptr)) {
                LOG(LogType::LOG_INFO, "Step button pressed");
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Settings...")) {
				MyGUI::Instance().configuration().SwitchState();
            }
            ImGui::EndMenu();
        }

        // Window menu
        if (ImGui::BeginMenu("Window")) {
            
            if (ImGui::BeginMenu("Veiwport", false))
            {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Menus"))
            {
                if (ImGui::MenuItem("Toolbar", nullptr, nullptr, false)) {
                    // Action for "Undo" item
                }
                if (ImGui::MenuItem("Hierarchy", nullptr, MyGUI::Instance().hierarchy().GetState())) {
					MyGUI::Instance().hierarchy().SwitchState();
                }
                if (ImGui::MenuItem("Project", nullptr, nullptr, false)) {
                    // Action for "Undo" item
                }
                if (ImGui::MenuItem("Console", nullptr, MyGUI::Instance().console().GetState())) {
                    MyGUI::Instance().console().SwitchState();
                }
				if (ImGui::MenuItem("Inspector", nullptr, MyGUI::Instance().inspector().GetState())) {
					MyGUI::Instance().inspector().SwitchState();
				}
                if (ImGui::MenuItem("Scene", nullptr, MyGUI::Instance().scene().GetState())) {
					MyGUI::Instance().scene().SwitchState();
                }
                if (ImGui::MenuItem("Game", nullptr, nullptr, false)) {
                    // Action for "Undo" item
                }
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::BeginMenu("DevDebug"))
            {
				if (ImGui::MenuItem("Show UI Demo")) {
					// Action for "Undo" item
				}
				if (ImGui::MenuItem("Generate Game", nullptr, nullptr, false)) {
					// Action for "Undo" item
				}
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        // GameObjects menu
        if (ImGui::BeginMenu("GameObjects")) 
        {
            if (ImGui::BeginMenu("Primitives..."))
            {
                if (ImGui::MenuItem("Create Empty", nullptr, nullptr)) {
					Engine::Instance().scene->CreateGameObject();
                }
                if (ImGui::MenuItem("Create Empty x10", nullptr, nullptr)) {
                    for (size_t i = 0; i < 10; i++) {
                        Engine::Instance().scene->CreateGameObject();
                    }
                }
                if (ImGui::MenuItem("Plane", nullptr, nullptr)) {
					Engine::Instance().scene->CreatePlane();
                }
                if (ImGui::MenuItem("Cube", nullptr, nullptr)) {
					Engine::Instance().scene->CreateCube();
                }
				if (ImGui::MenuItem("Sphere", nullptr, nullptr)) {
					Engine::Instance().scene->CreateSphere();
				}
                if (ImGui::MenuItem("Hemisphere", nullptr, nullptr, false)) {
                    // Action for "Undo" item
                }
				if (ImGui::MenuItem("Cylinder", nullptr, nullptr)) {
					Engine::Instance().scene->CreateCylinder();
				}
                if (ImGui::MenuItem("Cone", nullptr, nullptr)) {
					Engine::Instance().scene->CreateCone();
                }
                if (ImGui::MenuItem("Torus", nullptr, nullptr)) {
					Engine::Instance().scene->CreateTorus();
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("3D Objects..."))
            {
                if (ImGui::MenuItem("Create Camera", nullptr, nullptr)) {
                    Engine::Instance().scene->CreateCamera();
                }
                ImGui::EndMenu();
            }
			
            if (ImGui::BeginMenu("Draw Mode")) {

                if (ImGui::MenuItem("Mesh", nullptr, currentDrawMode == DrawMode::MESH)) {
                    currentDrawMode = DrawMode::MESH;
                }
                if (ImGui::MenuItem("Wireframe", nullptr, currentDrawMode == DrawMode::WIREFRAME)) {
                    currentDrawMode = DrawMode::WIREFRAME;
                }
                if (ImGui::MenuItem("Vertexs", nullptr, currentDrawMode == DrawMode::VERTEX)) {
                    currentDrawMode = DrawMode::VERTEX;
                }
                ImGui::Separator();
                if (ImGui::Checkbox("Vertex Normals", &showPerTriangleNormals)) {
                    for (auto& child : Engine::Instance().scene->root()->children())
					{
						showVertexNormals(child, showPerTriangleNormals);
					}
                }
                if (ImGui::Checkbox("Face Normals", &showPerFaceNormals)) {
                    for (auto& child : Engine::Instance().scene->root()->children())
                    {
						showFaceNormals(child, showPerFaceNormals);
                    }
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        // Help menu
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::Button("About AV-Mace...")) {
                system("start chrome.exe https://github.com/CITM-UPC/AV-Mace_Engine");
            }
			ImGui::Separator();
			if (ImGui::MenuItem("Documentation")) {
				// Action for "Documentation" item
                system("start chrome.exe https://github.com/CITM-UPC/AV-Mace_Engine/blob/main/README.md");
			}
            if (ImGui::MenuItem("Download latest version")) {
                //link to release when uploaded
                system("start chrome.exe https://github.com/CITM-UPC/AV-Mace_Engine/releases");
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Report bug / Suggest Feature")) {
                // Action for "Documentation" item
            }
            if (ImGui::MenuItem("Give us Feedback!")) {
                // Action for "Documentation" item
            }   
            if (ImGui::MenuItem("Contact us!")) {
                // Action for "Documentation" item
            }
            ImGui::EndMenu();
        }

        // Run Menu
		if (ImGui::BeginMenu("Run")) {
			if (ImGui::MenuItem("Play", "Ctrl+P", nullptr)) {
                LOG(LogType::LOG_INFO, "Play button pressed");
			}
			if (ImGui::MenuItem("Pause", "Ctrl+Shift+P", nullptr)) {
                LOG(LogType::LOG_INFO, "Pause button pressed");
			}
			if (ImGui::MenuItem("Step", "Ctrl+Alt+P", nullptr)) {
				LOG(LogType::LOG_INFO, "Step button pressed");
			}
			ImGui::EndMenu();
		}

        ImGui::EndMainMenuBar();
    }

	return true;
}

void PanelMenu::showVertexNormals(auto& child, bool showVertex)
{
	if (child->HasComponent<Mesh>()) {
		child->GetComponent<Mesh>()->setDebugNormals(showVertex);
	}

	if (!child->children().empty()) {
        for (auto& grandchild : child->children()) {
            showVertexNormals(grandchild, showVertex);
        }
	}
}

void PanelMenu::showFaceNormals(auto& child, bool showFace)
{
    if (child->HasComponent<Mesh>()) {
        child->GetComponent<Mesh>()->setDebugFaceNormals(showFace);
    }

    if (!child->children().empty()) {
        for (auto& grandchild : child->children()) {
            showFaceNormals(grandchild, showFace);
        }
    }
}