#include "gui.h"
#include<iostream>
#include <thread>
using namespace gui;
int main (  //__stdcall wWinMain
	HINSTANCE instance,
	HINSTANCE previousInstance,
	PWSTR arguments,
	int commandShow )
{
	setlocale(LC_ALL, "RUSSIAN");

	// create gui
	CreateHWindow("Cheat Menu");
	CreateDevice();
	CreateImGui();

	while (gui::isRunning)
	{
		gui::BeginRender();
		gui::Render();
		gui::EndRender();

		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	// destroy gui
	gui::DestroyImGui();
	gui::DestroyDevice();
	gui::DestroyHWindow();

	return EXIT_SUCCESS;
}
