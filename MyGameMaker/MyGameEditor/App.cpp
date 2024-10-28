#include "App.h"
//#include "Window.h"
//#include "Input.h"
//#include "Render.h"
//#include "Textures.h"
//#include "Audio.h"
//#include "Scene.h"
//#include "Map.h"
//#include "GuiManager.h"
//#include "DialogueTree.h"
//#include "QuestManager.h"
//#include "ModuleParticles.h"
//
//#include "Defs.h"
//#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	////Measure the amount of ms that takes to execute the App constructor and LOG the result
	//Timer timer = Timer();
	//startupTime = Timer();
	//frameTime = PerfTimer();
	//lastSecFrameTime = PerfTimer();

	//frames = 0;

	////Add the EntityManager Module to App

	//win = new Window();
	//input = new Input();
	//render = new Render();
	//tex = new Textures();
	//audio = new Audio();
	//sceneManager = new SceneManager();
	//map = new Map();
	//entityManager = new EntityManager();
	//guiManager = new GuiManager();
	//dialogueTree = new DialogueTree();
	//questManager = new QuestManager();
	//moduleParticles = new ModuleParticles();


	//// Ordered for awake / Start / Update
	//// Reverse order of CleanUp
	//AddModule(win);
	//AddModule(input);
	//AddModule(tex);
	//AddModule(audio);
	//AddModule(map);
	//AddModule(sceneManager);
	//AddModule(entityManager);
	//AddModule(moduleParticles);
	//AddModule(dialogueTree);
	//AddModule(questManager);
	//AddModule(guiManager);

	//// Render last to swap buffer
	//AddModule(render);

	//LOG("Timer App Constructor: %f", timer.ReadMSec());
}

// Destructor
App::~App()
{
	// Release modules
	/*ListItem<Module*>* item = modules.end;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();*/
}

void App::AddModule(Module* module)
{
	/*module->Init();
	modules.Add(module);*/
}

// Called before render is available
bool App::Awake()
{
	////Measure the amount of ms that takes to execute the Awake and LOG the result
	//Timer timer = Timer();

	//bool ret = LoadConfig();

	//if (ret == true)
	//{
	//	//Read the title from the config file and set the windows title 
	//	// substitute "Video Game Template" string from the value of the title in the config file
	//	// also read maxFrameDuration 
	//	gameTitle.Create(configFile.child("config").child("app").child("title").child_value());
	//	win->SetTitle(gameTitle.GetString());
	//	maxFrameDuration = configFile.child("config").child("app").child("maxFrameDuration").attribute("value").as_int();

	//	ListItem<Module*>* item;
	//	item = modules.start;

	//	while (item != NULL && ret == true)
	//	{
	//		//Add a new argument to the Awake method to receive a pointer to an xml node.
	//		// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
	//		// that can be used to read all variables for that module.
	//		// Send nullptr if the node does not exist in config.xml

	//		ret = item->data->Awake(configFile.child("config").child(item->data->name.GetString()));
	//		item = item->next;
	//	}
	//}

	//LOG("Timer App Awake(): %f", timer.ReadMSec());

	//return ret;
	return true;
}

// Called before the first frame
bool App::Start()
{
	////Measure the amount of ms that takes to execute the App Start() and LOG the result
	//Timer timer = Timer();

	//bool ret = true;
	//ListItem<Module*>* item;
	//item = modules.start;

	//while (item != NULL && ret == true)
	//{
	//	ret = item->data->Start();
	//	item = item->next;
	//}

	//LOG("Timer App Start(): %f", timer.ReadMSec());

	//return ret;
	return true;
}

// Called each loop iteration
bool App::Update()
{
	/*bool ret = true;
	PrepareUpdate();

	if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = DoUpdate();

	if (ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;*/
	return true;
}

// Load config from XML file
bool App::LoadConfig()
{
	//bool ret = true;

	////Load config.xml file using load_file() method from the xml_document class
	//// If the result is ok get the main node of the XML
	//// else, log the error
	//// check https://pugixml.org/docs/quickstart.html#loading

	//pugi::xml_parse_result result = configFile.load_file("config.xml");
	//if (result)
	//{
	//	LOG("config.xml parsed without errors");
	//}
	//else
	//{
	//	LOG("Error loading config.xml: %s", result.description());
	//}

	//return ret;

	return true;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	//frameTime.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	//// This is a good place to call Load / Save functions

	////Cap the framerate of the gameloop
	////Measure accurately the amount of time SDL_Delay() actually waits compared to what was expected

	//double currentDt = frameTime.ReadMs();
	//if (maxFrameDuration > 0 && currentDt < maxFrameDuration) {
	//	uint32 delay = (uint32)(maxFrameDuration - currentDt);

	//	PerfTimer delayTimer = PerfTimer();
	//	SDL_Delay(delay);
	//	//LOG("We waited for %I32u ms and got back in %f ms",delay,delayTimer.ReadMs());
	//}

	//// Amount of frames since startup
	//frameCount++;

	//// Amount of time since game start (use a low resolution timer)
	//secondsSinceStartup = startupTime.ReadSec();

	//// Amount of ms took the last update (dt)
	//dt = (float)frameTime.ReadMs();

	//// Amount of frames during the last second
	//lastSecFrameCount++;

	//// Average FPS for the whole game life
	//if (lastSecFrameTime.ReadMs() > 1000) {
	//	lastSecFrameTime.Start();
	//	averageFps = (averageFps + lastSecFrameCount) / 2;
	//	framesPerSecond = lastSecFrameCount;
	//	lastSecFrameCount = 0;
	//}

	////Button to see the debug
	//if (input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	//{
	//	if (debug)
	//		debug = false;
	//	else
	//		debug = true;
	//}

	//// Shows the time measurements in the window title
	//// check sprintf formats here https://cplusplus.com/reference/cstdio/printf/
	//static char title[256];
	//sprintf_s(title, 256, "%s: Av.FPS: %.2f Last sec frames: %i Last dt: %.3f Time since startup: %I32u Frame Count: %I64u ",
	//	gameTitle.GetString(), averageFps, framesPerSecond, dt, secondsSinceStartup, frameCount);

	//app->win->SetTitle(title);

	//if (loadRequest) {
	//	loadRequest = false;
	//	LoadFromFile();
	//}

	//if (saveRequest) {
	//	saveRequest = false;
	//	SaveFromFile();
	//}
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;

	for (const auto& module : modules)
	{
		if (module->active == false)
			continue;

		if (module->PreUpdate() == false)
			ret = false;
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;

	for (const auto& module : modules)
	{
		if (module->active == false)
			continue;

		if (module->Update(dt) == false)
			ret = false;
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;

	for (const auto& module : modules)
	{
		if (module->active == false)
			continue;

		if (module->PostUpdate() == false)
			ret = false;
	}


	return ret;
}

// Called before quitting
bool App::CleanUp()
{

	bool ret = true;

	for (auto item = modules.rbegin(); item != modules.rend(); ++item)
	{
		Module* module = *item;
		ret = module->CleanUp();
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return gameTitle.c_str();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.c_str();
}