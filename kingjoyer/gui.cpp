#include "gui.h"
#include "byte_functions.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"

#include <iostream>
using namespace std;
#include "string"


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


bool fps60 = false;
bool invulBilbo = false;
struct Point {
	float x = 0;
	float y = 0;
	float z = 0;
	LPDWORD ukazatel = 0x00;
};
Point savedPoint;
float x, y, z;
LPDWORD ukazatel;

bool debug = false;
int lang = 0; // 0 - RUS , 1 - ENG

void gui::Render() noexcept
{


	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	ImGui::Begin(
		"The KingJoyer v1.0 by King174rus",
		&isRunning,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);

	if  (ImGui::Button("Change Language")) 
	{
		lang = !lang;
	}
	ImGui::Text("This is some useful text.");
	

		if (ImGui::Checkbox((const char*)u8"Режим Разработчик",&developerMode)) {
		change_1Byte_hobbit((LPVOID)0x007600E9, 0x01, 0x00); //функция включения режима разработчика
	}
		if (ImGui::Checkbox((const char*)u8"Волумы", &renderVolumes)) {
		change_1Byte_hobbit((LPVOID)0x00777B04, 0x01, 0x00); //функция рендера волумов
	}
		if (ImGui::Checkbox((const char*)u8"Триггеры Загрузки", &renderLoadTriggers)) {
		change_1Byte_hobbit((LPVOID)0x00777B18, 0x01, 0x00); //функция рендера загрузочных триггеров
	}
		if (ImGui::Checkbox((const char*)u8"Триггеры", &renderTriggers)) {
		change_1Byte_hobbit((LPVOID)0x00777B1C, 0x01, 0x00); //функция рендера триггеров
	}
		if (ImGui::Checkbox((const char*)u8"Вода", &renderWater)) {
		change_1Byte_hobbit((LPVOID)0x00777B10, 0x01, 0x00); //функция рендера воды
	}
		if (ImGui::Checkbox((const char*)u8"Паутина", &renderWeb)) {
		change_1Byte_hobbit((LPVOID)0x00777B90, 0x01, 0x00); //функция рендера паутины
	}
		if (ImGui::Checkbox((const char*)u8"Веревка", &renderRopes)) {
		change_1Byte_hobbit((LPVOID)0x00777B24, 0x01, 0x00); //функция рендера веревок
	}
		if (ImGui::Checkbox((const char*)u8"Листа", &renderLeaves)) {
		change_1Byte_hobbit((LPVOID)0x00777B80, 0x01, 0x00); //функция рендер кластеров листвы деревьев
	}
		if (ImGui::Checkbox((const char*)u8"Сундуки", &renderChests)) {
		change_1Byte_hobbit((LPVOID)0x00777AF0, 0x01, 0x00); //функция рендер сундуков
	}
		if (ImGui::Checkbox((const char*)u8"Рычаги", &renderLevers)) {
		change_1Byte_hobbit((LPVOID)0x00777AEC, 0x01, 0x00); //функция рендер рычагов
	}
		if (ImGui::Checkbox((const char*)u8"Бильбо", &renderBilbo)) {
		change_1Byte_hobbit((LPVOID)0x00777AA0, 0x01, 0x00); //функция рендер Бильбо
	}
		if (ImGui::Checkbox((const char*)u8"Свет", &renderLights)) {
		change_1Byte_hobbit((LPVOID)0x00777AA4, 0x01, 0x00); //функция рендер света
	}
		if (ImGui::Checkbox((const char*)u8"Эффекты", &renderEffects)) {
		change_1Byte_hobbit((LPVOID)0x00777B88, 0x01, 0x00); //функция рендер эффектов
	}
		if (ImGui::Checkbox((const char*)u8"Падающий путь", &breakway)) {
		change_1Byte_hobbit((LPVOID)0x00777B0C, 0x01, 0x00); //функция рендера падающего пути
	}
		if (ImGui::Checkbox((const char*)u8"Болдер ран", &boulderRun)) {
		change_1Byte_hobbit((LPVOID)0x00777AFC, 0x01, 0x00); //функция рендер болдер рана
	}
		if (ImGui::Checkbox((const char*)u8"60 фпс", &fps60)) {
		change_2Byte_hobbit((LPVOID)0x006EFBDA, 0x4180, 0x4204); //функция FPS
	}
		if (ImGui::Checkbox((const char*)u8"Скайбокс", &renderSkybox)) {
		change_1Byte_hobbit((LPVOID)0x00777B5C, 0x01, 0x00); //функция рендера скайбокса
	}
		if (ImGui::Checkbox((const char*)u8"Пьедестал сохранения", &renderSavePedestal)) {
		change_1Byte_hobbit((LPVOID)0x00777AF8, 0x01, 0x00); //функция рендера сохранялок
	}
		if (ImGui::Checkbox((const char*)u8"Пуш бокс", &renderPushBoxes)) {
		change_1Byte_hobbit((LPVOID)0x00777AF4, 0x01, 0x00); //функция рендера пушбоксов
	}
		if (ImGui::Button((const char*)u8"Камни")) {
			plusA_float_hobbit((LPVOID)0x0075BDB4, 1); //функция прибавки на 1 камней
	}
		const char* questItems[] = { "AAAA", "BBBB", "CCCC", "DDDD" };
		static int questItem = -1;
		ImGui::Combo("Combo", &questItem, questItems, IM_ARRAYSIZE(questItems));
		ImGui::SameLine();
		if (ImGui::Button((const char*)u8"Выдать квестовый предмет")) {
		plusA_float_hobbit((LPBYTE)0x0075BE98 + questItem * 4, 1); //функция выдачи квестового предмета
	}
		if (ImGui::Button((const char*)u8"Удалить квестовый предмет")) {
		change_2Byte_hobbit((LPBYTE)0x0075BE9A + questItem * 4, 0x00, 0x00); //функция выдачи квестового предмета
	}
		if (ImGui::Button((const char*)u8"1 доп хп")) {
		plusA_float_hobbit((LPVOID)0x0075BDC4, 1); //функция прибавки на 1 доп хп
	}
		if (ImGui::Button((const char*)u8"10 доп хп")) {
		plusA_float_hobbit((LPVOID)0x0075BDC4, 10); //функция прибавки на 10 доп хп
	}
		if (ImGui::Button((const char*)u8"1 макс хп")) {
		plusA_float_hobbit((LPVOID)0x0075BE14, 1); //функция прибавки на 1 макс хп
	}
		if (ImGui::Button((const char*)u8"10 макс хп")) {
		plusA_float_hobbit((LPVOID)0x0075BE14, 10); //функция прибавки на 10 макс хп
	}
		if (ImGui::Button((const char*)u8"Приблизить")) {
		plusA_float_hobbit((LPVOID)0x00772BF0, -0.1); //функция приближения камеры на 0.1
	}
		if (ImGui::Button((const char*)u8"Отдалить")) {
		plusA_float_hobbit((LPVOID)0x00772BF0, +0.1); //функция отдаления камеры на 0.1
	}
		if (ImGui::Checkbox((const char*)u8"Бессмертие", &invulBilbo)) {
		change_1Byte_hobbit((LPVOID)0x0075FBF4, 0x01, 0x00); //функция бессмертия
	}
		static int speed = 300;
		ImGui::InputInt((const char*)u8"Скорость Бильбо", &speed);
		ImGui::SameLine();
		if (ImGui::Button((const char*)u8"Применить")) {
		change_float_hobbit((LPVOID)0x0075B850, speed); //функция изменения скорости Бильбо
	}
		if (ImGui::Button((const char*)u8"Установить Точку Телепортации")) {
		savedPoint.ukazatel = ukazatel_hobbit((LPVOID)0x0075BA3C);
		ukazatel = savedPoint.ukazatel;
		savedPoint.x = save_float_hobbit(ukazatel + 5);
		savedPoint.y = save_float_hobbit(ukazatel + 6);
		savedPoint.z = save_float_hobbit(ukazatel + 7);//функция установки точки телепортации
	}
		if (ImGui::Button((const char*)u8"Телепортироваться")) {
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
		const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD" };
		static int item = -1;
		ImGui::Combo("Combo", &item, items, IM_ARRAYSIZE(items));

		ImGui::SameLine();
		if (ImGui::Button((const char*)u8"Выдать предмет")) {
		plusA_float_hobbit((LPBYTE)0x0075BDB0 + item * 4, 1); //функция выдачи предмета
	}
		if (ImGui::Button((const char*)u8"Удалить предмет")) {
		change_2Byte_hobbit((LPBYTE)0x0075BDB2 + item * 4, 0x00, 0x00); //функция удаления предмета
	}
		if (ImGui::Button((const char*)u8"Показать предмет")) {
		change_1Byte_hobbit((LPBYTE)0x007212BC + item * 4, 0x01, 0x01); //функция показа предмета
	}
		if (ImGui::Button((const char*)u8"Спрятать предмет")) {
		change_2Byte_hobbit((LPBYTE)0x007212BC + item * 4, 0x00, 0x00); //функция убирания предмета
	}


	

	ImGui::End();
}
