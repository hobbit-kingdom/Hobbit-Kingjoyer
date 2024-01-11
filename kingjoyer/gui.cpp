#include "gui.h"
#include "byte_functions.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"

#include <iostream>
#include "string"
#include <chrono>

using namespace std::chrono;
using namespace std;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

long __stdcall WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
		return true;

	switch (message)
	{
	case WM_SIZE: {
		if (gui::device && wideParameter != SIZE_MINIMIZED)
		{
			gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
			gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
			gui::ResetDevice();
		}
	}return 0;

	case WM_SYSCOMMAND: {
		if ((wideParameter & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
	}break;

	case WM_DESTROY: {
		PostQuitMessage(0);
	}return 0;

	case WM_LBUTTONDOWN: {
		gui::position = MAKEPOINTS(longParameter); // set click points
	}return 0;

	case WM_MOUSEMOVE: {
		if (wideParameter == MK_LBUTTON)
		{
			const auto points = MAKEPOINTS(longParameter);
			auto rect = ::RECT{ };

			GetWindowRect(gui::window, &rect);

			rect.left += points.x - gui::position.x;
			rect.top += points.y - gui::position.y;

			if (gui::position.x >= 0 &&
				gui::position.x <= gui::WIDTH &&
				gui::position.y >= 0 && gui::position.y <= 19)
				SetWindowPos(
					gui::window,
					HWND_TOPMOST,
					rect.left,
					rect.top,
					0, 0,
					SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
				);
		}

	}return 0;

	}

	return DefWindowProc(window, message, wideParameter, longParameter);
}

void gui::CreateHWindow(const char* windowName) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = "class001";
	windowClass.hIconSm = 0;

	RegisterClassEx(&windowClass);

	window = CreateWindowEx(
		0,
		"class001",
		windowName,
		WS_POPUP,
		100,
		100,
		WIDTH,
		HEIGHT,
		0,
		0,
		windowClass.hInstance,
		0
	);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void gui::DestroyHWindow() noexcept
{
	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool gui::CreateDevice() noexcept
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d)
		return false;

	ZeroMemory(&presentParameters, sizeof(presentParameters));

	presentParameters.Windowed = TRUE;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParameters.EnableAutoDepthStencil = TRUE;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&presentParameters,
		&device) < 0)
		return false;

	return true;
}

void gui::ResetDevice() noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = device->Reset(&presentParameters);

	if (result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();
}

void gui::DestroyDevice() noexcept
{
	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (d3d)
	{
		d3d->Release();
		d3d = nullptr;
	}
}

void gui::CreateImGui() noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 16.5f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	io.IniFilename = NULL;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
}

void gui::DestroyImGui() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void gui::BeginRender() noexcept
{
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);

		if (message.message == WM_QUIT)
		{
			isRunning = !isRunning;
			return;
		}
	}

	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void gui::EndRender() noexcept
{
	ImGui::EndFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	const auto result = device->Present(0, 0, 0, 0);

	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();
}

bool developerMode = false;


bool renderVolumes = false;
bool renderLoadTriggers = false;
bool renderTriggers = false;
bool renderWater = false;
bool renderWeb = false;
bool renderRopes = false;
bool renderLeaves = false;
bool renderChests = false;
bool renderLevers = false;
bool renderBilbo = false;
bool renderLights = false;
bool renderEffects = false;
bool renderSkybox = false;
bool renderSavePedestal = false;
bool renderPushBoxes = false;
bool breakway = false;
bool boulderRun = false;
bool polyCache = false;
bool bilboPos = false;
bool cutsceneInfo = false;
bool objInfo = false;
bool maxobjInfo = false;
bool objInView = false;
bool trianglesInView = false;



bool fps60 = false;
bool invulBilbo = false;
bool stamina = false;

bool poison_chance = false;
struct Point {
	float x = 0;
	float y = 0;
	float z = 0;
	LPDWORD ukazatel = 0x00;
	LPDWORD ukazatel_stamina = 0x00;
};
Point savedPoint;
float x, y, z;
LPDWORD ukazatel;
LPDWORD ukazatel_stamina;

bool debug = false;
int lang = 0; // 0 - RUS , 1 - ENG

Point currentBilboPos;

auto start = high_resolution_clock::now();

LPDWORD xPointer = 0x00;
float xPos = 0;
float yPos = 0;
float zPos = 0;


float numberOfAttacks = 0;
float numberOfJumps = 0;
float distanceTraveled = 0;
float damageTakenFromPoison = 0;
float timeSpentHiding = 0;
float numberOfStonesThrown = 0;
float missedJumps = 0;
float numberOfPoleJumps = 0;
float damageTaken = 0;
float vigorHealthUsed = 0;
float swingsFromMineCart = 0;
float ridesInMinecart = 0;
float spSpentInVendor = 0;
float healthPotionsBought = 0;
float jumpsAlmostMissed = 0;
float distanceInMineCart = 0;
float enemiesKilled = 0;
float deathsDueToSting = 0;
float deathDueToStuff = 0;
float deathsDueToStones = 0;
float missedCourageFromKills = 0;
float totalSpMissed = 0;
float totalCouragePointsMissed = 0;
float totalChestsMissed = 0;
float totalQuestsMissed = 0;
float amountOfBlocks = 0;

int HUD_HP = 98615574-22+6;
void gui::Render() noexcept
{
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	ImGui::Begin(
		"The KingJoyer v1.0 by king174rus and Mr_Kliff",
		&isRunning,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);

	ImGui::Text("                                                                               The Hobbit KingJoyer                  ");
	ImGui::Text("");

	if (ImGui::Button(!lang ? "Change Language" : (const char*)u8"Поменять язык")) lang = !lang;

	ImGui::Separator();
	ImGui::Text("");

	if (ImGui::Checkbox(lang ? "Developer mode" : (const char*)u8"Режим Разработчика", &developerMode)) {
		change_1Byte_hobbit((LPVOID)0x007600E9, 0x01, 0x00); //функция включения режима разработчика
	}
	if (ImGui::Checkbox(lang ? "60 FPS" : (const char*)u8"60 фпс", &fps60)) {
		change_2Byte_hobbit((LPVOID)0x006EFBDA, 0x4180, 0x4204); //функция FPS
	}


	if (ImGui::CollapsingHeader(lang ? "Renders" : (const char*)u8"Рендеры"))
	{
		ImGui::Indent();

		ImGui::Text(lang ? "Renders" : (const char*)u8"Рендеры");
		ImGui::Separator();

		if (ImGui::BeginTable("split", 3))
		{
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Volumes" : (const char*)u8"Волумы", &renderVolumes)) {
				change_1Byte_hobbit((LPVOID)0x00777B04, 0x01, 0x00); //функция рендера волумов
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "PolyCache" : (const char*)u8"Полигоны", &polyCache)) {
				change_1Byte_hobbit((LPVOID)0x00778078, 0x01, 0x00); //функция рендера волумов
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Load Triggers" : (const char*)u8"Триггеры Загрузки", &renderLoadTriggers)) {
				change_1Byte_hobbit((LPVOID)0x00777B18, 0x01, 0x00); //функция рендера загрузочных триггеров
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Triggers" : (const char*)u8"Триггеры", &renderTriggers)) {
				change_1Byte_hobbit((LPVOID)0x00777B1C, 0x01, 0x00); //функция рендера триггеров
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Water" : (const char*)u8"Вода", &renderWater)) {
				change_1Byte_hobbit((LPVOID)0x00777B10, 0x01, 0x00); //функция рендера воды
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Web" : (const char*)u8"Паутина", &renderWeb)) {
				change_1Byte_hobbit((LPVOID)0x00777B90, 0x01, 0x00); //функция рендера паутины
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Ropes" : (const char*)u8"Веревка", &renderRopes)) {
				change_1Byte_hobbit((LPVOID)0x00777B24, 0x01, 0x00); //функция рендера веревок
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Leaves" : (const char*)u8"Листва", &renderLeaves)) {
				change_1Byte_hobbit((LPVOID)0x00777B80, 0x01, 0x00); //функция рендер кластеров листвы деревьев
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Chests" : (const char*)u8"Сундуки", &renderChests)) {
				change_1Byte_hobbit((LPVOID)0x00777AF0, 0x01, 0x00); //функция рендер сундуков
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Levers" : (const char*)u8"Рычаги", &renderLevers)) {
				change_1Byte_hobbit((LPVOID)0x00777AEC, 0x01, 0x00); //функция рендер рычагов
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Bilbo" : (const char*)u8"Бильбо", &renderBilbo)) {
				change_1Byte_hobbit((LPVOID)0x00777AA0, 0x01, 0x00); //функция рендер Бильбо
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Lights" : (const char*)u8"Свет", &renderLights)) {
				change_1Byte_hobbit((LPVOID)0x00777AA4, 0x01, 0x00); //функция рендер света
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Effects" : (const char*)u8"Эффекты", &renderEffects)) {
				change_1Byte_hobbit((LPVOID)0x00777B88, 0x01, 0x00); //функция рендер эффектов
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Breakway" : (const char*)u8"Падающий путь", &breakway)) {
				change_1Byte_hobbit((LPVOID)0x00777B0C, 0x01, 0x00); //функция рендера падающего пути
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Falling boulders" : (const char*)u8"Падающие булыжники", &boulderRun)) {
				change_1Byte_hobbit((LPVOID)0x00777AFC, 0x01, 0x00); //функция рендер болдер рана
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Skybox" : (const char*)u8"Небо", &renderSkybox)) {
				change_1Byte_hobbit((LPVOID)0x00777B5C, 0x01, 0x00); //функция рендера скайбокса
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Save Pedestal" : (const char*)u8"Пьедестал сохранения", &renderSavePedestal)) {
				change_1Byte_hobbit((LPVOID)0x00777AF8, 0x01, 0x00); //функция рендера сохранялок
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Push boxes" : (const char*)u8"Двигающиеся коробки", &renderPushBoxes)) {
				change_1Byte_hobbit((LPVOID)0x00777AF4, 0x01, 0x00); //функция рендера пушбоксов
			}
			ImGui::EndTable();
		}

		ImGui::Unindent();

	}

	if (ImGui::CollapsingHeader(lang ? "Cheats" : (const char*)u8"Читы"))
	{
		ImGui::Indent();

		ImGui::Text(lang ? "Teleportation" : (const char*)u8"Телепортация");
		ImGui::Separator();

		if (ImGui::Button(lang ? "Set Teleportation Waypoint" : (const char*)u8"Установить Точку Телепортации")) {
			savedPoint.ukazatel = ukazatel_hobbit((LPVOID)0x0075BA3C);
			ukazatel = savedPoint.ukazatel;
			savedPoint.x = save_float_hobbit(ukazatel + 5);
			savedPoint.y = save_float_hobbit(ukazatel + 6);
			savedPoint.z = save_float_hobbit(ukazatel + 7);//функция установки точки телепортации
		}
		ImGui::SameLine();
		ImGui::Text("");
		ImGui::SameLine();
		if (ImGui::Button(lang ? "Teleport!" : (const char*)u8"Телепортироваться!")) {
			x = savedPoint.x;
			y = savedPoint.y;
			z = savedPoint.z;
			ukazatel = savedPoint.ukazatel;
			if (x) {
				change_float_hobbit(ukazatel + 5, x);
				change_float_hobbit(ukazatel + 281, x);
				change_float_hobbit(ukazatel + 6, y);
				change_float_hobbit(ukazatel + 282, y);
				change_float_hobbit(ukazatel + 7, z);
				change_float_hobbit(ukazatel + 283, z);
			}
		}

		ImGui::Text("X: %g", savedPoint.x); ImGui::SameLine();
		ImGui::Text("Y: %g", savedPoint.y); ImGui::SameLine();
		ImGui::Text("Z: %g", savedPoint.z);

		ImGui::Text("");
		ImGui::Text(lang ? "Cheats" : (const char*)u8"Читы");
		ImGui::Separator();

		if (ImGui::Checkbox(lang ? "Full stamina" : (const char*)u8"Бесконечная выносливость", &stamina)) {
			savedPoint.ukazatel_stamina = ukazatel_hobbit((LPDWORD)0x0075BA3C);
			ukazatel_stamina = savedPoint.ukazatel_stamina; //функция беконечной стамины
		}
		if (ImGui::Checkbox(lang ? "Invulnerability" : (const char*)u8"Бессмертие", &invulBilbo)) {
			change_1Byte_hobbit((LPVOID)0x0075FBF4, 0x01, 0x00); //функция бессмертия
		}
		if (ImGui::Button(lang ? "Add 1 stone" : (const char*)u8"Выдать 1 камень")) {
			plusA_float_hobbit((LPVOID)0x0075BDB4, 1); //функция прибавки на 1 камней
		}
		if (ImGui::Button(lang ? "Add 5 stones" : (const char*)u8"Выдать 5 каменей")) {
			plusA_float_hobbit((LPVOID)0x0075BDB4, 5); //функция прибавки на 5 камней
		}
		if (ImGui::Button(lang ? "Add 1 extra HP" : (const char*)u8"1 доп хп")) {
			plusA_float_hobbit((LPVOID)0x0075BDC4, 1); //функция прибавки на 1 доп хп
		}
		if (ImGui::Button(lang ? "Add 10 extra HP" : (const char*)u8"10 доп хп")) {
			plusA_float_hobbit((LPVOID)0x0075BDC4, 10); //функция прибавки на 10 доп хп
		}
		if (ImGui::Button(lang ? "Add 1 Max HP" : (const char*)u8"1 макс хп")) {
			plusA_float_hobbit((LPVOID)0x0075BE14, 1); //функция прибавки на 1 макс хп
		}
		if (ImGui::Button(lang ? "Add 10 Max HP" : (const char*)u8"10 макс хп")) {
			plusA_float_hobbit((LPVOID)0x0075BE14, 10); //функция прибавки на 10 макс хп
		}

		ImGui::Text("");
		ImGui::Text(lang ? "Speed" : (const char*)u8"Скорость");
		ImGui::Separator();

		ImGui::Text(lang ? "Bilbos' speed" : (const char*)u8"Скорость Бильбо");
		static int speed = 300;
		ImGui::InputInt("", &speed, 50);

		if (ImGui::Button(lang ? "Apply speed" : (const char*)u8"Применить скорость")) {
			change_float_hobbit((LPVOID)0x0075B850, speed); //функция изменения скорости Бильбо
		}

		ImGui::Text("");
		ImGui::Text(lang ? "Jump" : (const char*)u8"Прыжок");
		ImGui::Separator();

		ImGui::Text(lang ? "Jump power (lower = better)" : (const char*)u8"Сила прыжка (меньше = лучше)");
		static int jumpPower = 3000;
		ImGui::InputInt(" ", &jumpPower, 500);

		if (ImGui::Button(lang ? "Apply power" : (const char*)u8"Применить прыжок")) {
			change_float_hobbit((LPVOID)0x0075B888, jumpPower);
		}
		ImGui::Text("");
		ImGui::Text(lang ? "Speed in jump" : (const char*)u8"Скорость в прыжке");

		static int speedInJump = 350;
		ImGui::InputInt("  ", &speedInJump, 100);

		if (ImGui::Button(lang ? "Apply speed in jump" : (const char*)u8"Применить скорсоть в прыжке")) {
			change_float_hobbit((LPVOID)0x0075B868, speedInJump);
		}


		ImGui::Unindent();
	}

	if (ImGui::CollapsingHeader(lang ? "Camera" : (const char*)u8"Камера"))
	{
		ImGui::Indent();
		ImGui::Text(lang ? "Camera" : (const char*)u8"Камера");
		ImGui::Separator();

		if (ImGui::Button(lang ? "Decrease FOV" : (const char*)u8"Уменьшить FOV")) {
			plusA_float_hobbit((LPVOID)0x00772BF0, -0.1); //функция приближения камеры на 0.1
		}
		if (ImGui::Button(lang ? "Increase FOV" : (const char*)u8"Увеличить FOV")) {
			plusA_float_hobbit((LPVOID)0x00772BF0, +0.1); //функция отдаления камеры на 0.1
		}

		static int cameraDistance = 100;
		ImGui::InputInt(lang ? "Camera Distance" : (const char*)u8"Растояние камеры", &cameraDistance, 50);

		if (ImGui::Button(lang ? "Apply Distance" : (const char*)u8"Применить расстояние")) {
			change_float_hobbit((LPVOID)0x00772A70, cameraDistance);
			change_float_hobbit((LPVOID)0x00772B38, cameraDistance);
		}

		static int maxCameraDistance = 300;
		ImGui::InputInt(lang ? "Max Camera Distance" : (const char*)u8"Максимальное Растояние камеры", &maxCameraDistance, 50);

		if (ImGui::Button(lang ? "Apply Max Distance" : (const char*)u8"Применить макс. расстояние")) {
			change_float_hobbit((LPVOID)0x00772B3C, maxCameraDistance);
		}

		ImGui::Unindent();
	}

	if (ImGui::CollapsingHeader(lang ? "Statistics" : (const char*)u8"Статистика"))
	{
		ImGui::Indent();

		ImGui::Text(lang ? "Statistics" : (const char*)u8"Статистика");
		ImGui::Separator();
		if (ImGui::Checkbox(lang ? "Bilbo Positon" : (const char*)u8"Позиция Бильбо", &bilboPos)) {
			change_1Byte_hobbit((LPVOID)0x0075FBD4, 0x01, 0x00);
		}

		if (ImGui::Checkbox(lang ? "Cinema Debug" : (const char*)u8"Информация в катсценах", &cutsceneInfo)) {
			change_1Byte_hobbit((LPVOID)0x0075FBF8, 0x01, 0x00);
		}

		if (ImGui::Checkbox(lang ? "Objects stats" : (const char*)u8"Информация об объектах", &objInfo)) {
			change_1Byte_hobbit((LPVOID)0x0075FBC4, 0x01, 0x00);
		}

		if (ImGui::Checkbox(lang ? "Big Objects stats" : (const char*)u8"Много информации об объектах", &maxobjInfo)) {
			change_1Byte_hobbit((LPVOID)0x00778054, 0x01, 0x00);
		}

		if (ImGui::Checkbox(lang ? "Objects in view" : (const char*)u8"Объекты в зоне видимости", &objInView)) {
			change_1Byte_hobbit((LPVOID)0x00778070, 0x01, 0x00);
		}

		if (ImGui::Checkbox(lang ? "Triangles in view" : (const char*)u8"Треугольники в зоне видимости", &trianglesInView)) {
			change_1Byte_hobbit((LPVOID)0x00778058, 0x01, 0x00);
		}

		auto tim = duration_cast<seconds>(high_resolution_clock::now() - start).count();

		if (tim > 2)
		{
			start = high_resolution_clock::now();

			numberOfAttacks = read_float_value((LPVOID)(0x0075C034));
			numberOfJumps = read_float_value((LPVOID)(0x0075C034 + 4));
			distanceTraveled = read_float_value((LPVOID)(0x0075C034 + 8));
			damageTakenFromPoison = read_float_value((LPVOID)(0x0075C034 + 12));
			timeSpentHiding = read_float_value((LPVOID)(0x0075C034 + 16));
			numberOfStonesThrown = read_float_value((LPVOID)(0x0075C034 + 20));
			missedJumps = read_float_value((LPVOID)(0x0075C034 + 24));
			numberOfPoleJumps = read_float_value((LPVOID)(0x0075C034 + 28));
			damageTaken = read_float_value((LPVOID)(0x0075C034 + 32));
			vigorHealthUsed = read_float_value((LPVOID)(0x0075C034 + 36));
			swingsFromMineCart = read_float_value((LPVOID)(0x0075C034 + 40));
			ridesInMinecart = read_float_value((LPVOID)(0x0075C034 + 44));
			spSpentInVendor = read_float_value((LPVOID)(0x0075C034 + 48));
			healthPotionsBought = read_float_value((LPVOID)(0x0075C034 + 52));
			jumpsAlmostMissed = read_float_value((LPVOID)(0x0075C034 + 56));
			distanceInMineCart = read_float_value((LPVOID)(0x0075C034 + 60));
			enemiesKilled = read_float_value((LPVOID)(0x0075C034 + 64));
			deathsDueToSting = read_float_value((LPVOID)(0x0075C034 + 68));
			deathDueToStuff = read_float_value((LPVOID)(0x0075C034 + 72));
			deathsDueToStones = read_float_value((LPVOID)(0x0075C034 + 76));
			missedCourageFromKills = read_float_value((LPVOID)(0x0075C034 + 80));
			totalSpMissed = read_float_value((LPVOID)(0x0075C034 + 84));
			totalCouragePointsMissed = read_float_value((LPVOID)(0x0075C034 + 88));
			totalChestsMissed = read_float_value((LPVOID)(0x0075C034 + 92));
			totalQuestsMissed = read_float_value((LPVOID)(0x0075C034 + 96));
			amountOfBlocks = read_float_value((LPVOID)(0x0075C034 + 100));

			xPointer = ukazatel_hobbit((LPVOID)0x0075BA3C);
			xPos = read_float_value((LPVOID)(xPointer + 497));
			yPos = read_float_value((LPVOID)(xPointer + 498));
			zPos = read_float_value((LPVOID)(xPointer + 499));
		}

		ImGui::Text("X: %g", xPos); ImGui::SameLine();
		ImGui::Text("Y: %g", yPos); ImGui::SameLine();
		ImGui::Text("Z: %g", zPos);

		ImGui::Text(lang ? "Number Of Attacks: %g" : (const char*)u8"Количество Ударов: %g", numberOfAttacks);
		ImGui::Text(lang ? "Number Of Jumps: %g" : (const char*)u8"Количество Прыжков: %g", numberOfJumps);
		ImGui::Text(lang ? "Distance Traveled: %g" : (const char*)u8"Пройденное Расстояние: %g", distanceTraveled);
		ImGui::Text(lang ? "Missed Jumps: %g" : (const char*)u8"Неудачные Прыжки: %g", missedJumps);
		ImGui::Text(lang ? "Damage Taken: %g" : (const char*)u8"Полученный Урон: %g", damageTaken);
		ImGui::Text(lang ? "Number Of Pole Jumps: %g" : (const char*)u8"Количество Прыжков С Посохом: %g", numberOfPoleJumps);
		ImGui::Text(lang ? "Jumps Almost Missed: %g" : (const char*)u8"Количество Рискованных Прыжков: %g", jumpsAlmostMissed);
		ImGui::Text(lang ? "Deaths Due to Sting: %g" : (const char*)u8"Убито Жалом: %g", deathsDueToSting);
		ImGui::Text(lang ? "Deaths Due to Staff: %g" : (const char*)u8"Убито Посохом: %g", deathDueToStuff);
		ImGui::Text(lang ? "Deaths Due to Stones: %g" : (const char*)u8"Убито Камнями: %g", deathsDueToStones);
		ImGui::Text(lang ? "Damage Taken From Poison: %g" : (const char*)u8"Урон От Яда: %g", damageTakenFromPoison);
		ImGui::Text(lang ? "Time Spent Hiding: %g" : (const char*)u8"Время Спрятавшись: %g", timeSpentHiding);
		ImGui::Text(lang ? "Number Of Stones Thrown: %g" : (const char*)u8"Количество Брошенных Камней: %g", numberOfStonesThrown);
		ImGui::Text(lang ? "Vigor Health Used: %g" : (const char*)u8"Использованные Лечебные Зелья: %g", vigorHealthUsed);
		ImGui::Text(lang ? "Swings from Mine Cart: %g" : (const char*)u8"Количество Ударов С Вагонетки: %g", swingsFromMineCart);
		ImGui::Text(lang ? "Rides In Mine Cart: %g" : (const char*)u8"Количество Поездок В Вагонетке: %g", ridesInMinecart);
		ImGui::Text(lang ? "SP Spent in Vendor: %g" : (const char*)u8"Монет Потраченно: %g", spSpentInVendor);
		ImGui::Text(lang ? "Health Potions Purchased: %g" : (const char*)u8"Зелий Куплено: %g", healthPotionsBought);
		ImGui::Text(lang ? "Distance in Mine Cart: %g" : (const char*)u8"Расстояние На Вагонетке: %g", distanceInMineCart);
		ImGui::Text(lang ? "Enemies Killed: %g" : (const char*)u8"Врагов Убито: %g", enemiesKilled);
		ImGui::Text(lang ? "Missed Courage from Kills: %g" : (const char*)u8"Кристаллов Потеряно: %g", missedCourageFromKills);
		ImGui::Text(lang ? "Total SP Missed: %g" : (const char*)u8"Пропущено Монет: %g", totalSpMissed);
		ImGui::Text(lang ? "Total Courage Missed: %g" : (const char*)u8"Пропущено Кристаллов: %g", totalCouragePointsMissed);
		ImGui::Text(lang ? "Total Chests Missed: %g" : (const char*)u8"Пропущено Сундуков: %g", totalChestsMissed);
		ImGui::Text(lang ? "Total Quests Missed: %g" : (const char*)u8"Пропущено Квестов: %g", totalQuestsMissed);
		ImGui::Text(lang ? "Blocks: %g" : (const char*)u8"Количество Блоков: %g", amountOfBlocks);

		const char* Stats[] = { lang ? "Number Of Attacks" : (const char*)u8"Количество Ударов",
			lang ? "Number Of Jumps" : (const char*)u8"Количество Прыжков",
		lang ? "Distance Traveled" : (const char*)u8"Пройденное Расстояние",
		lang ? "Damage Taken From Poison" : (const char*)u8"Урон От Яда",
		lang ? "Time Spent Hiding" : (const char*)u8"Время Спрятавшись",
		lang ? "Number Of Stones Thrown" : (const char*)u8"Количество Брошенных Камней",
		lang ? "Missed Jumps" : (const char*)u8"Неудачные Прыжки",
		lang ? "Number Of Pole Jumps" : (const char*)u8"Количество Прыжков С Посохом",
		lang ? "Damage Taken" : (const char*)u8"Полученный Урон",
		lang ? "Vigor Health Used" : (const char*)u8"Использованные Лечебные Зелья",
		lang ? "Swings from Mine Cart" : (const char*)u8"Количество Ударов С Вагонетки",
		lang ? "Rides In Mine Cart" : (const char*)u8"Количество Поездок В Вагонетке",
		lang ? "SP Spent in Vendor" : (const char*)u8"Монет Потраченно",
		lang ? "Health Potions Purchased" : (const char*)u8"Зелий Куплено",
		lang ? "Jumps Almost Missed" : (const char*)u8"Количество Рискованных Прыжков",
		lang ? "Distance in Mine Cart" : (const char*)u8"Расстояние На Вагонетке",
		lang ? "Enemies Killed" : (const char*)u8"Врагов Убито",
		lang ? "Deaths Due to Sting" : (const char*)u8"Убито Жалом",
		lang ? "Deaths Due to Staff" : (const char*)u8"Убито Посохом",
		lang ? "Deaths Due to Stones" : (const char*)u8"Убито Камнями",
		lang ? "Missed Courage from Kills" : (const char*)u8"Кристаллов Потеряно",
		lang ? "Total SP Missed" : (const char*)u8"Пропущено Монет",
		lang ? "Total Courage Missed" : (const char*)u8"Пропущено Кристаллов",
		lang ? "Total Chests Missed" : (const char*)u8"Пропущено Сундуков",
		lang ? "Total Quests Missed" : (const char*)u8"Пропущено Квестов",
		lang ? "Blocks" : (const char*)u8"Количество Блоков" };

		static int NumberStat = -1;
		ImGui::Combo(lang ? "Select Statistics" : (const char*)u8"Выбрать Статистику", &NumberStat, Stats, IM_ARRAYSIZE(Stats));
		static int Stat = 0;
		ImGui::InputInt(" ", &Stat);
		if (ImGui::Button(lang ? "Change Statistics" : (const char*)u8"Изменить Статистику")) {
			change_float_hobbit((LPDWORD)0x0075C034 + NumberStat, Stat);
		}

		ImGui::Unindent();
	}

	if (ImGui::CollapsingHeader(lang ? "Quest Items" : (const char*)u8"Квестовые предметы"))
	{
		ImGui::Indent();
		const char* questItems[] = { lang ? "Troll key(OHUH)" : (const char*)u8"Ключ тролля",
		lang ? "Witch King Crystal" : (const char*)u8"Камень Короля-Чародея",
		lang ? "Burberry plant" : (const char*)u8"Шиполист",
		lang ? "Blue urn" : (const char*)u8"Синяя урна",
		lang ? "Red urn" : (const char*)u8"Красная урна",
		lang ? "Yellow urn" : (const char*)u8"Желтая урна",
		lang ? "Unlit torch" : (const char*)u8"Незаженный факел",
		lang ? "Lit torch" : (const char*)u8"Горящий факел",
		lang ? "Bellows" : (const char*)u8"Мехи",
		lang ? "Signet ring" : (const char*)u8"Перстень",
		lang ? "Can of oil" : (const char*)u8"Масло",
		lang ? "Small iron gear" : (const char*)u8"Железная звездочка",
		lang ? "Small silver gear" : (const char*)u8"Серебряная звездочка",
		lang ? "Small golden gear" : (const char*)u8"Золотая звездочка",
		lang ? "Small copper gear" : (const char*)u8"Медная звездочка",
		lang ? "Small metallic" : (const char*)u8"Стальная звездочка",
		lang ? "Iron gear" : (const char*)u8"Железная шестеренка",
		lang ? "Silver gear" : (const char*)u8"Серебряная шестеренка",
		lang ? "Golden gear" : (const char*)u8"Золотая шестеренка",
		lang ? "Copper gear" : (const char*)u8"Медная шестеренка",
		lang ? "Metallic gear" : (const char*)u8"Стальная шестеренка",
		lang ? "Large iron gear" : (const char*)u8"Железная шестерня",
		lang ? "Large silver gear" : (const char*)u8"Серебряная шестерня",
		lang ? "Large golden gear" : (const char*)u8"Золотая шестерня",
		lang ? "Large copper gear" : (const char*)u8"Медная шестерня",
		lang ? "Large metallic gear" : (const char*)u8"Стальная шестерня",
		lang ? "Iron shank head" : (const char*)u8"Железный толкатель",
		lang ? "Silver shank head" : (const char*)u8"Серебряный толкатель",
		lang ? "Golden shank head" : (const char*)u8"Золотой толкатель",
		lang ? "Copper shank head" : (const char*)u8"Медный толкатель",
		lang ? "Metallic shank head" : (const char*)u8"Стальной толкатель",
		lang ? "Iron shank arm" : (const char*)u8"Железная шпага",
		lang ? "Silver shank arm" : (const char*)u8"Серебряная шпага",
		lang ? "Golden shank arm" : (const char*)u8"Золотая шпага",
		lang ? "Copper shank arm" : (const char*)u8"Медная шпага",
		lang ? "Metallic shank arm" : (const char*)u8"Стальная шпага",
		lang ? "Firewood" : (const char*)u8"Дрова",
		lang ? "Grit-lift key" : (const char*)u8"Ключ Грит",
		lang ? "Dim-lift key" : (const char*)u8"Ключ Дим",
		lang ? "Mugg-lift key" : (const char*)u8"Ключ Мугг",
		lang ? "Jail Exit Key" : (const char*)u8"Ключ от тюрьмы",
		lang ? "Final Bridge Key" : (const char*)u8"Ключ от последнего моста",
		lang ? "Gandola gear 1" : (const char*)u8"Шестеренка Гандолы №1",
		lang ? "Gandola gear 2" : (const char*)u8"Шестеренка Гандолы №2",
		lang ? "Wart-lift lever" : (const char*)u8"Рычаг Варт",
		lang ? "Wart-stone" : (const char*)u8"Камень Варт",
		lang ? "Thror`s golden cup" : (const char*)u8"Золотая чаша Трора",
		lang ? "Necklace of Girion" : (const char*)u8"Ожерелье Гириона",
		lang ? "Groin`s ruby" : (const char*)u8"Рубин Гроина",
		lang ? "King Bladorthin`s spears" : (const char*)u8"Копье короля Дортина",
		lang ? "Golden serving dish" : (const char*)u8"Золотое блюдо",
		lang ? "Tea-cakes" : (const char*)u8"Кексы",
		lang ? "Tea-cakes ingredients" : (const char*)u8"Ингридиенты для кексов",
		lang ? "Shed key" : (const char*)u8"Ключ от сарая",
		lang ? "Apple" : (const char*)u8"Яблоко",
		lang ? "Hammer" : (const char*)u8"Молоток",
		lang ? "Nails" : (const char*)u8"Гвозди",
		lang ? "Walking stick" : (const char*)u8"Посох",
		lang ? "Egg" : (const char*)u8"Яйцо",
		lang ? "Berries" : (const char*)u8"Ягоды",
		lang ? "Sack of Wheat" : (const char*)u8"Мешок пшеницы",
		lang ? "Sugar" : (const char*)u8"Сахар",
		lang ? "Spice" : (const char*)u8"Специи",
		lang ? "Sausage" : (const char*)u8"Колбаски",
		lang ? "Wheat" : (const char*)u8"Пшеница",
		lang ? "Quilt" : (const char*)u8"Одеяло",
		lang ? "Quilting needle" : (const char*)u8"Иголка",
		lang ? "Elvish opening crystal 1" : (const char*)u8"1 ключ-кристалл",
		lang ? "Elvish opening crystal 2" : (const char*)u8"2 ключ-кристалл",
		lang ? "Elvish opening crystal 3" : (const char*)u8"3 ключ-кристалл",
		lang ? "Elvish opening crystal 4" : (const char*)u8"4 ключ-кристалл",
		lang ? "Elvish healing potion" : (const char*)u8"Эльфийское зелье исцеления",
		lang ? "Deep cellar key 1" : (const char*)u8"1 ключ от Подземелья",
		lang ? "Deep cellar key 2" : (const char*)u8"2 ключ от Подземелья",
		lang ? "Deep cellar key 3" : (const char*)u8"3 ключ от Подземелья",
		lang ? "Deep cellar key 4" : (const char*)u8"4 ключ от Подземелья",
		lang ? "Cavern crystal" : (const char*)u8"Пещерный кристалл",
		lang ? "Wilowweed" : (const char*)u8"Сон-трава",
		lang ? "Moonleaf" : (const char*)u8"Луннолист",
		lang ? "Spidersbane" : (const char*)u8"Паутинник",
		lang ? "Web potion" : (const char*)u8"Паутинное зелье",
		lang ? "Gear belt" : (const char*)u8"Приводной ремень",
		lang ? "Treasury Chain" : (const char*)u8"Цепь из сокровищницы",
		lang ? "Treasury key mold" : (const char*)u8"Форма для ключа",
		lang ? "Throne key mold 1" : (const char*)u8"1 форма для ключа",
		lang ? "Throne key mold 2" : (const char*)u8"2 форма для ключа",
		lang ? "Treasury key" : (const char*)u8"Ключ от сокровищницы",
		lang ? "Throne key 1" : (const char*)u8"1 ключ от Тронного зала",
		lang ? "Throne key 2" : (const char*)u8"2 ключ от Тронного зала",
		lang ? "Arkenstone" : (const char*)u8"Аркенстон",
		lang ? "City warehouse key" : (const char*)u8"Ключ от городского склада",
		lang ? "Black arrow" : (const char*)u8"Черная стрела",
		lang ? "Malloc`s golden dagger" : (const char*)u8"Золотой кинжал Мэллока",
		lang ? "Black wine bottle" : (const char*)u8"Черная бутылка",
		lang ? "Blue wine bottle" : (const char*)u8"Синяя бутылка",
		lang ? "Purple wine bottle" : (const char*)u8"Фиолетовая бутылка",
		lang ? "Red wine bottle" : (const char*)u8"Красная бутылка",
		lang ? "Yellow wine bottle" : (const char*)u8"Желтая бутылка",
		lang ? "Gandalf`s Message" : (const char*)u8"Послание Гендольфа",
		lang ? "Healing Draught" : (const char*)u8"Лекарство",
		lang ? "Troll key(TH)" : (const char*)u8"Ключ троллей",
		lang ? "Ladder switch lever" : (const char*)u8"Рукоятка для лестницы",
		lang ? "Rennar`s key" : (const char*)u8"Ключ Реннара"};


		ImGui::Text(lang ? "Quest Items" : (const char*)u8"Квестовые предметы");
		ImGui::Separator();

		static int questItem = -1;
		ImGui::Combo("   ", &questItem, questItems, IM_ARRAYSIZE(questItems));

		if (ImGui::Button(lang ? "Give quest item" : (const char*)u8"Выдать квестовый предмет")) {
			plusA_float_hobbit((LPBYTE)0x0075BE98 + questItem * 4, 1); //функция выдачи квестового предмета
		}
		if (ImGui::Button(lang ? "Delete quest item" : (const char*)u8"Удалить квестовый предмет")) {
			change_2Byte_hobbit((LPBYTE)0x0075BE9A + questItem * 4, 0x00, 0x00); //функция выдачи квестового предмета
		}
		ImGui::Unindent();
	}

	if (ImGui::CollapsingHeader(lang ? "Items" : (const char*)u8"Предметы"))
	{
		ImGui::Indent();
		const char* items[] = { lang ? "Coin" : (const char*)u8"Монета",
			lang ? "Stones" : (const char*)u8"Камень",
			(const char*)u8"Ihavehadstones",
			lang ? "Health" : (const char*)u8"Здоровье",
			(const char*)u8"GreaterHealth",
			lang ? "Vigor Health" : (const char*)u8"Выносливость",
			(const char*)u8"CPTOTAL",
			(const char*)u8"LevelingCP",
			lang ? "Courage" : (const char*)u8"Синий кристалл",
			lang ? "Courage1" : (const char*)u8"Голубой кристалл",
			lang ? "Courage5" : (const char*)u8"Красный кристалл",
			lang ? "Courage10" : (const char*)u8"Зеленный кристалл",
			lang ? "Courage25" : (const char*)u8"Фиолетовый кристалл",
			lang ? "Courage50" : (const char*)u8"Белый кристалл",
			(const char*)u8"Coinbag",
			(const char*)u8"Coinbag5",
			(const char*)u8"Coinbag10",
			(const char*)u8"Coinbag25",
			(const char*)u8"Coinbag50",
			(const char*)u8"Coinbag100",
			lang ? "Rock Bag" : (const char*)u8"Сумка для камней",
			lang ? "Med Bag" : (const char*)u8"Сумка для зелей",
			lang ? "Skeleton key" : (const char*)u8"Отмычка",
			(const char*)u8"CLevelCP",
			(const char*)u8"ChestCount",
			(const char*)u8"BilboLevel",
			(const char*)u8"BilbosPoisoned",
			(const char*)u8"BilboPoisonTimer",
			(const char*)u8"Bilbo_Staff_Swing",
			(const char*)u8"Bilbo_Staff_Swing2",
			(const char*)u8"Bilbo_Staff_Swing3",
			(const char*)u8"Bilbo_Staff_Jump",
			(const char*)u8"Bilbo_Staff_Jump2",
			(const char*)u8"Bilbo_Staff_Jump3",
			(const char*)u8"Bilbo_Staff_AE_Jump",
			(const char*)u8"Bilbo_Staff_AE_Jump2",
			(const char*)u8"Bilbo_Staff_AE_Jump3",
			(const char*)u8"Bilbo_Sting_Swing",
			(const char*)u8"Bilbo_Sting_Swing2",
			(const char*)u8"Bilbo_Sting_Swing3",
			(const char*)u8"Bilbo_Sting_Jump",
			(const char*)u8"Bilbo_Sting_Jump2",
			(const char*)u8"Bilbo_Sting_Jump3",
			(const char*)u8"Bilbo_Stone_Throw",
			(const char*)u8"Bilbo_Stone_Throw2",
			(const char*)u8"Bilbo_Stone_Throw3",
			lang ? "Level Potion" : (const char*)u8"Зелье уровня",
			lang ? "Health Potion" : (const char*)u8"Лечебное зелье",
			lang ? "Antidote" : (const char*)u8"Противоядие",
			lang ? "Water of Vigor" : (const char*)u8"Живая вода",
			lang ? "Sword Sting" : (const char*)u8"Жало",
			lang ? "Ring" : (const char*)u8"Кольцо",
			lang ? "Staff" : (const char*)u8"Посох",
			lang ? "Explosive Stone" : (const char*)u8"Взрывные камни",
			lang ? "Fire Stone" : (const char*)u8"Огненные камни",
			lang ? "Freeze Stone" : (const char*)u8"Замороженные камни",
			lang ? "Poison Stone" : (const char*)u8"Ядовитые камни",
			lang ? "Magic Stone" : (const char*)u8"Магические камни"};
		static int item = -1;

		ImGui::Text(lang ? "Items" : (const char*)u8"Предметы");
		ImGui::Separator();

		ImGui::Combo("", &item, items, IM_ARRAYSIZE(items));

		if (ImGui::Button(lang ? "Give item" : (const char*)u8"Выдать предмет")) {
			plusA_float_hobbit((LPBYTE)0x0075BDB0 + item * 4, 1); //функция выдачи предмета
		}
		if (ImGui::Button(lang ? "Remove item" : (const char*)u8"Удалить предмет")) {
			change_2Byte_hobbit((LPBYTE)0x0075BDB2 + item * 4, 0x00, 0x00); //функция удаления предмета
		}
		if (ImGui::Button(lang ? "Show item" : (const char*)u8"Показать предмет")) {
			change_1Byte_hobbit((LPBYTE)0x007212BC + item * 4, 0x01, 0x01); //функция показа предмета
		}
		if (ImGui::Button(lang ? "Hide item" : (const char*)u8"Спрятать предмет")) {
			change_2Byte_hobbit((LPBYTE)0x007212BC + item * 4, 0x00, 0x00); //функция убирания предмета
		}
		ImGui::Unindent();
	}
	if (ImGui::CollapsingHeader(lang ? "Special option" : (const char*)u8"Специальные опции"))
	{
		ImGui::Indent();
		if (ImGui::Checkbox(lang ? "Can be poisoned" : (const char*)u8"Возможно ли отравиться", &poison_chance)) {
			change_1Byte_hobbit((LPVOID)0x0042132C, 0x00, 0x01); //функция возможности отравиться у Бильбо
		}
		ImGui::Text(lang ? "Change HUD HP (max 22)" : (const char*)u8"Изменение HUD ХП (макс 22)");
		if (ImGui::Button("<")) {
			HUD_HP = read_int_value((LPBYTE)0x004F5BB8);
			if (HUD_HP == 98615552) {
				change_1Byte_hobbit((LPBYTE)0x004F5BB8, 0x16, 0x00);
			}
			else plusA_int_hobbit((LPBYTE)0x004F5BB8, -1); //функция изменения худа хп
		}
		ImGui::SameLine();
		if (ImGui::Button(">")) {
			HUD_HP = read_int_value((LPBYTE)0x004F5BB8);
			if (HUD_HP == 98615574) {
				change_1Byte_hobbit((LPBYTE)0x004F5BB8, 0x00, 0x00);
			}
			else plusA_int_hobbit((LPBYTE)0x004F5BB8, 1); //функция изменения худа хп
		}
		HUD_HP = read_int_value((LPBYTE)0x004F5BB8);
		ImGui::Text((const char*)to_string(HUD_HP-98615552).c_str());
		ImGui::Unindent();
		
	}
	if (stamina == true)
		change_float_hobbit(ukazatel_stamina + 641, 10);
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("");


	ImGui::Text(lang ? "Our links" : (const char*)u8"Наши ссылки");
	ImGui::Separator();

	if (ImGui::Button("king174rus")) {
		ShellExecute(NULL, "open", "https://www.youtube.com/c/@king174rus", 0, 0, SW_SHOWNORMAL);
	}
	ImGui::SameLine(); ImGui::Text(" "); ImGui::SameLine();
	if (ImGui::Button("Mr_Kliff")) {
		ShellExecute(NULL, "open", "https://youtube.com/c/@YKliffa", 0, 0, SW_SHOWNORMAL);
	}
	ImGui::SameLine(); ImGui::Text(" "); ImGui::SameLine();
	if (ImGui::Button(lang ? "Hobbit Technical Discord" : (const char*)u8"Технический канал Хоббита в Дискорде")) {
		ShellExecute(NULL, "open", "https://discord.gg/hvzB3maxQ3", 0, 0, SW_SHOWNORMAL);
	}

	ImGui::End();
}
