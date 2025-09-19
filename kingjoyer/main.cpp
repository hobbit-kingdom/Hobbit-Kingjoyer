#include "gui.h"
#include<iostream>
#include <thread>
using namespace gui;

int __stdcall wWinMain(  //__stdcall wWinMain
//	int main(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	PWSTR arguments,
	int commandShow)
{
	setlocale(LC_ALL, "RUSSIAN");

	// create gui
	CreateHWindow(L"Cheat Menu");
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
