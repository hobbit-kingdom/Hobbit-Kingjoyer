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
bool polyCache = false;
bool bilboPos = false;
bool cutsceneInfo = false;
bool objInfo = false;
bool maxobjInfo = false;
bool objInView = false;
bool trianglesInView = false;


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

Point currentBilboPos;


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
	if (ImGui::Button(!lang ? "Change Language" : (const char*)u8"�������� ����"))
	{
		lang = !lang;
	}
	ImGui::Separator();
	ImGui::Text("");

	if (ImGui::Checkbox(lang ? "Developer mode" : (const char*)u8"����� ������������", &developerMode)) {
		change_1Byte_hobbit((LPVOID)0x007600E9, 0x01, 0x00); //������� ��������� ������ ������������
	}
	if (ImGui::Checkbox(lang ? "60 FPS" : (const char*)u8"60 ���", &fps60)) {
		change_2Byte_hobbit((LPVOID)0x006EFBDA, 0x4180, 0x4204); //������� FPS
	}


	if (ImGui::CollapsingHeader(lang ? "Renders" : (const char*)u8"�������"))
	{
		ImGui::Indent();

		ImGui::Text(lang ? "Renders" : (const char*)u8"�������");
		ImGui::Separator();

		if (ImGui::BeginTable("split", 3))
		{
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Volumes" : (const char*)u8"������", &renderVolumes)) {
				change_1Byte_hobbit((LPVOID)0x00777B04, 0x01, 0x00); //������� ������� �������
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "PolyCache" : (const char*)u8"��������", &polyCache)) {
				change_1Byte_hobbit((LPVOID)0x00778078, 0x01, 0x00); //������� ������� �������
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Load Triggers" : (const char*)u8"�������� ��������", &renderLoadTriggers)) {
				change_1Byte_hobbit((LPVOID)0x00777B18, 0x01, 0x00); //������� ������� ����������� ���������
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Triggers" : (const char*)u8"��������", &renderTriggers)) {
				change_1Byte_hobbit((LPVOID)0x00777B1C, 0x01, 0x00); //������� ������� ���������
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Water" : (const char*)u8"����", &renderWater)) {
				change_1Byte_hobbit((LPVOID)0x00777B10, 0x01, 0x00); //������� ������� ����
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Web" : (const char*)u8"�������", &renderWeb)) {
				change_1Byte_hobbit((LPVOID)0x00777B90, 0x01, 0x00); //������� ������� �������
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Ropes" : (const char*)u8"�������", &renderRopes)) {
				change_1Byte_hobbit((LPVOID)0x00777B24, 0x01, 0x00); //������� ������� �������
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Leaves" : (const char*)u8"������", &renderLeaves)) {
				change_1Byte_hobbit((LPVOID)0x00777B80, 0x01, 0x00); //������� ������ ��������� ������ ��������
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Chests" : (const char*)u8"�������", &renderChests)) {
				change_1Byte_hobbit((LPVOID)0x00777AF0, 0x01, 0x00); //������� ������ ��������
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Levers" : (const char*)u8"������", &renderLevers)) {
				change_1Byte_hobbit((LPVOID)0x00777AEC, 0x01, 0x00); //������� ������ �������
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Bilbo" : (const char*)u8"������", &renderBilbo)) {
				change_1Byte_hobbit((LPVOID)0x00777AA0, 0x01, 0x00); //������� ������ ������
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Lights" : (const char*)u8"����", &renderLights)) {
				change_1Byte_hobbit((LPVOID)0x00777AA4, 0x01, 0x00); //������� ������ �����
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Effects" : (const char*)u8"�������", &renderEffects)) {
				change_1Byte_hobbit((LPVOID)0x00777B88, 0x01, 0x00); //������� ������ ��������
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Breakway" : (const char*)u8"�������� ����", &breakway)) {
				change_1Byte_hobbit((LPVOID)0x00777B0C, 0x01, 0x00); //������� ������� ��������� ����
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Falling boulders" : (const char*)u8"�������� ���������", &boulderRun)) {
				change_1Byte_hobbit((LPVOID)0x00777AFC, 0x01, 0x00); //������� ������ ������ ����
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Skybox" : (const char*)u8"����", &renderSkybox)) {
				change_1Byte_hobbit((LPVOID)0x00777B5C, 0x01, 0x00); //������� ������� ���������
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Save Pedestal" : (const char*)u8"��������� ����������", &renderSavePedestal)) {
				change_1Byte_hobbit((LPVOID)0x00777AF8, 0x01, 0x00); //������� ������� ����������
			}
			ImGui::TableNextColumn(); if (ImGui::Checkbox(lang ? "Push boxes" : (const char*)u8"����������� �������", &renderPushBoxes)) {
				change_1Byte_hobbit((LPVOID)0x00777AF4, 0x01, 0x00); //������� ������� ���������
			}
			ImGui::EndTable();
		}

		ImGui::Unindent();

	}

	if (ImGui::CollapsingHeader(lang ? "Cheats" : (const char*)u8"����"))
	{
		ImGui::Indent();

		ImGui::Text(lang ? "Teleportation" : (const char*)u8"������������");
		ImGui::Separator();

		if (ImGui::Button(lang ? "Set Teleportation Waypoint" : (const char*)u8"���������� ����� ������������")) {
			savedPoint.ukazatel = ukazatel_hobbit((LPVOID)0x0075BA3C);
			ukazatel = savedPoint.ukazatel;
			savedPoint.x = save_float_hobbit(ukazatel + 5);
			savedPoint.y = save_float_hobbit(ukazatel + 6);
			savedPoint.z = save_float_hobbit(ukazatel + 7);//������� ��������� ����� ������������
		}
		ImGui::SameLine();
		ImGui::Text("");
		ImGui::SameLine();
		if (ImGui::Button(lang ? "Teleport!" : (const char*)u8"�����������������!")) {
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
		ImGui::Text(lang ? "Cheats" : (const char*)u8"����");
		ImGui::Separator();

		if (ImGui::Checkbox(lang ? "Invulnerability" : (const char*)u8"����������", &invulBilbo)) {
			change_1Byte_hobbit((LPVOID)0x0075FBF4, 0x01, 0x00); //������� ����������
		}
		if (ImGui::Button(lang ? "Add 1 stone" : (const char*)u8"������ 1 ������")) {
			plusA_float_hobbit((LPVOID)0x0075BDB4, 1); //������� �������� �� 1 ������
		}
		if (ImGui::Button(lang ? "Add 5 stones" : (const char*)u8"������ 5 �������")) {
			plusA_float_hobbit((LPVOID)0x0075BDB4, 5); //������� �������� �� 5 ������
		}
		if (ImGui::Button(lang ? "Add 1 extra HP" : (const char*)u8"1 ��� ��")) {
			plusA_float_hobbit((LPVOID)0x0075BDC4, 1); //������� �������� �� 1 ��� ��
		}
		if (ImGui::Button(lang ? "Add 10 extra HP" : (const char*)u8"10 ��� ��")) {
			plusA_float_hobbit((LPVOID)0x0075BDC4, 10); //������� �������� �� 10 ��� ��
		}
		if (ImGui::Button(lang ? "Add 1 Max HP" : (const char*)u8"1 ���� ��")) {
			plusA_float_hobbit((LPVOID)0x0075BE14, 1); //������� �������� �� 1 ���� ��
		}
		if (ImGui::Button(lang ? "Add 10 Max HP" : (const char*)u8"10 ���� ��")) {
			plusA_float_hobbit((LPVOID)0x0075BE14, 10); //������� �������� �� 10 ���� ��
		}

		ImGui::Text("");
		ImGui::Text(lang ? "Speed" : (const char*)u8"��������");
		ImGui::Separator();

		ImGui::Text(lang ? "Bilbos' speed" : (const char*)u8"�������� ������");
		static int speed = 300;
		ImGui::InputInt("", &speed, 50);

		if (ImGui::Button(lang ? "Apply speed" : (const char*)u8"��������� ��������")) {
			change_float_hobbit((LPVOID)0x0075B850, speed); //������� ��������� �������� ������
		}

		ImGui::Text("");
		ImGui::Text(lang ? "Jump" : (const char*)u8"������");
		ImGui::Separator();

		ImGui::Text(lang ? "Jump power (lower = better)" : (const char*)u8"���� ������ (������ = �����)");
		static int jumpPower = 3000;
		ImGui::InputInt("", &jumpPower, 500);

		if (ImGui::Button(lang ? "Apply power" : (const char*)u8"��������� ������")) {
			change_float_hobbit((LPVOID)0x0075B888, jumpPower);
		}
		ImGui::Text("");
		ImGui::Text(lang ? "Speed in jump" : (const char*)u8"�������� � ������");

		static int speedInJump = 350;
		ImGui::InputInt("", &speedInJump, 100);

		if (ImGui::Button(lang ? "Apply speed in jump" : (const char*)u8"��������� �������� � ������")) {
			change_float_hobbit((LPVOID)0x0075B868, speedInJump);
		}


		ImGui::Unindent();
	}

	if (ImGui::CollapsingHeader(lang ? "Camera" : (const char*)u8"������"))
	{
		ImGui::Indent();
		ImGui::Text(lang ? "Camera" : (const char*)u8"������");
		ImGui::Separator();

		if (ImGui::Button(lang ? "Decrease FOV" : (const char*)u8"��������� FOV")) {
			plusA_float_hobbit((LPVOID)0x00772BF0, -0.1); //������� ����������� ������ �� 0.1
		}
		if (ImGui::Button(lang ? "Increase FOV" : (const char*)u8"��������� FOV")) {
			plusA_float_hobbit((LPVOID)0x00772BF0, +0.1); //������� ��������� ������ �� 0.1
		}

		static int cameraDistance = 100;
		ImGui::InputInt(lang ? "Camera Distance" : (const char*)u8"��������� ������", &cameraDistance, 50);

		if (ImGui::Button(lang ? "Apply Distance" : (const char*)u8"��������� ����������")) {
			change_float_hobbit((LPVOID)0x00772A70, cameraDistance);
			change_float_hobbit((LPVOID)0x00772B38, cameraDistance);
		}

		static int maxCameraDistance = 300;
		ImGui::InputInt(lang ? "Max Camera Distance" : (const char*)u8"������������ ��������� ������", &maxCameraDistance, 50);

		if (ImGui::Button(lang ? "Apply Max Distance" : (const char*)u8"��������� ����. ����������")) {
			change_float_hobbit((LPVOID)0x00772B3C, maxCameraDistance);
		}

		ImGui::Unindent();
	}

	if (ImGui::CollapsingHeader(lang ? "Statistics" : (const char*)u8"����������"))
	{
		ImGui::Indent();

		ImGui::Text(lang ? "Statistics" : (const char*)u8"����������");
		ImGui::Separator();
		if (ImGui::Checkbox(lang ? "Bilbo Positon" : (const char*)u8"������� ������", &bilboPos)) {
			change_1Byte_hobbit((LPVOID)0x0075FBD4, 0x01, 0x00);
		}

		if (ImGui::Checkbox(lang ? "Cinema Debug" : (const char*)u8"���������� � ���������", &cutsceneInfo)) {
			change_1Byte_hobbit((LPVOID)0x0075FBF8, 0x01, 0x00);
		}

		if (ImGui::Checkbox(lang ? "Objects stats" : (const char*)u8"���������� �� ��������", &objInfo)) {
			change_1Byte_hobbit((LPVOID)0x0075FBC4, 0x01, 0x00);
		}

		if (ImGui::Checkbox(lang ? "Big Objects stats" : (const char*)u8"����� ���������� �� ��������", &maxobjInfo)) {
			change_1Byte_hobbit((LPVOID)0x00778054, 0x01, 0x00);
		}

		if (ImGui::Checkbox(lang ? "Objects in view" : (const char*)u8"������� � ���� ���������", &objInView)) {
			change_1Byte_hobbit((LPVOID)0x00778070, 0x01, 0x00);
		}

		if (ImGui::Checkbox(lang ? "Triangles in view" : (const char*)u8"������������ � ���� ���������", &trianglesInView)) {
			change_1Byte_hobbit((LPVOID)0x00778058, 0x01, 0x00);
		}

		//laggy a bit, should save xPointer and then read
		LPDWORD xPointer = ukazatel_hobbit((LPVOID)0x0075BA3C);
		float xPos = read_float_value((LPVOID)(xPointer + 497));
		float yPos = read_float_value((LPVOID)(xPointer + 498));
		float zPos = read_float_value((LPVOID)(xPointer + 499));

		ImGui::Text("X: %g", xPos);
		ImGui::Text("Y: %g", yPos);
		ImGui::Text("Z: %g", zPos);

		ImGui::Unindent();
	}

	if (ImGui::CollapsingHeader(lang ? "Quest Items" : (const char*)u8"��������� ��������"))
	{
		ImGui::Indent();
		const char* questItems[] = { (const char*)u8"���� ������" ,(const char*)u8"������ ������-�������" ,(const char*)u8"��������" ,(const char*)u8"����� ����" ,(const char*)u8"������� ����" ,(const char*)u8"������ ����" ,(const char*)u8"���������� �����",
		(const char*)u8"������� �����",
		(const char*)u8"����",
		(const char*)u8"��������",
		(const char*)u8"�����",
		(const char*)u8"�������� ���������",
		(const char*)u8"���������� ���������",
		(const char*)u8"������� ���������",
		(const char*)u8"������ ���������",
		(const char*)u8"�������� ���������",
		(const char*)u8"�������� ����������",
		(const char*)u8"���������� ����������",
		(const char*)u8"������� ����������",
		(const char*)u8"������ ����������",
		(const char*)u8"�������� ����������",
		(const char*)u8"�������� ��������",
		(const char*)u8"���������� ��������",
		(const char*)u8"������� ��������",
		(const char*)u8"������ ��������",
		(const char*)u8"�������� ��������",
		(const char*)u8"�������� ���������",
		(const char*)u8"���������� ���������",
		(const char*)u8"������� ���������",
		(const char*)u8"������ ���������",
		(const char*)u8"�������� ���������",
		(const char*)u8"�������� �����",
		(const char*)u8"���������� �����",
		(const char*)u8"������� �����",
		(const char*)u8"������ �����",
		(const char*)u8"�������� �����",
		(const char*)u8"�����",
		(const char*)u8"���� ����",
		(const char*)u8"���� ���",
		(const char*)u8"���� ����" ,(const char*)u8"���� �� ������" ,(const char*)u8"���� �� ���������� �����" ,(const char*)u8"���������� ������� �1" ,(const char*)u8"���������� ������� �2" ,(const char*)u8"����� ����" ,(const char*)u8"������ ����" ,(const char*)u8"������� ���� �����",(const char*)u8"�������� �������" ,(const char*)u8"����� ������" ,(const char*)u8"����� ������ �������",
		(const char*)u8"������� �����" ,(const char*)u8"�����" ,(const char*)u8"����������� ��� ������" ,(const char*)u8"���� �� �����" ,(const char*)u8"������" ,(const char*)u8"�������" ,(const char*)u8"������" ,(const char*)u8"�����" ,(const char*)u8"����" ,(const char*)u8"�����" ,(const char*)u8"����� �������" ,(const char*)u8"�����" ,(const char*)u8"������" ,(const char*)u8"��������" ,(const char*)u8"�������" ,(const char*)u8"������" ,(const char*)u8"������" ,(const char*)u8"1 ����-��������" ,(const char*)u8"2 ����-��������" ,(const char*)u8"3 ����-��������" ,(const char*)u8"4 ����-��������" ,(const char*)u8"���������� ����� ���������" ,(const char*)u8"1 ���� �� ����������" ,(const char*)u8"2 ���� �� ����������" ,(const char*)u8"3 ���� �� ����������" ,(const char*)u8"4 ���� �� ����������" ,(const char*)u8"�������� ��������" ,(const char*)u8"���-�����" ,(const char*)u8"���������" ,(const char*)u8"���������" ,(const char*)u8"��������� �����" ,(const char*)u8"��������� ������" ,(const char*)u8"���� �� ������������" ,(const char*)u8"����� ��� �����" ,(const char*)u8"1 ����� ��� �����" ,(const char*)u8"2 ����� ��� �����" ,(const char*)u8"���� �� ������������" ,(const char*)u8"1 ���� �� �������� ����" ,(const char*)u8"2 ���� �� �������� ����" ,(const char*)u8"���������" ,(const char*)u8"���� �� ���������� ������" ,(const char*)u8"������ ������" ,(const char*)u8"������� ������ �������" ,(const char*)u8"������ �������" ,(const char*)u8"����� �������" ,(const char*)u8"���������� �������" ,(const char*)u8"������� �������" ,(const char*)u8"������ �������" ,(const char*)u8"�������� ���������" ,(const char*)u8"���������" ,(const char*)u8"���� �������" ,(const char*)u8"�������� ��� ��������" ,(const char*)u8"���� �������" };


		ImGui::Text(lang ? "Quest Items" : (const char*)u8"��������� ��������");
		ImGui::Separator();

		static int questItem = -1;
		ImGui::Combo("", &questItem, questItems, IM_ARRAYSIZE(questItems));

		if (ImGui::Button(lang ? "Give quest item" : (const char*)u8"������ ��������� �������")) {
			plusA_float_hobbit((LPBYTE)0x0075BE98 + questItem * 4, 1); //������� ������ ���������� ��������
		}
		if (ImGui::Button(lang ? "Delete quest item" : (const char*)u8"������� ��������� �������")) {
			change_2Byte_hobbit((LPBYTE)0x0075BE9A + questItem * 4, 0x00, 0x00); //������� ������ ���������� ��������
		}
		ImGui::Unindent();
	}

	if (ImGui::CollapsingHeader(lang ? "Items" : (const char*)u8"��������"))
	{
		ImGui::Indent();
		const char* items[] = { (const char*)u8"������"
			,(const char*)u8"������"
			,(const char*)u8"Ihavehadstones"
			,(const char*)u8"��������"
			,(const char*)u8"GreaterHealth"
			,(const char*)u8"������������"
			 ,(const char*)u8"CPTOTAL"
			,(const char*)u8"LevelingCP"
			,(const char*)u8"����� ��������"
			,(const char*)u8"������� ��������"
			,(const char*)u8"������� ��������"
			,(const char*)u8"�������� ��������"
			 ,(const char*)u8"���������� ��������"
			,(const char*)u8"����� ��������"
			,(const char*)u8"Coinbag"
			,(const char*)u8"Coinbag5"
			,(const char*)u8"Coinbag10"
			,(const char*)u8"Coinbag25"
			 ,(const char*)u8"Coinbag50"
			,(const char*)u8"Coinbag100"
			,(const char*)u8"����� ��� ������"
			,(const char*)u8"����� ��� �����"
			,(const char*)u8"�������"
			,(const char*)u8"CLevelCP"
			,(const char*)u8"ChestCount"
			,(const char*)u8"BilboLevel"
			,(const char*)u8"BilbosPoisoned"
			,(const char*)u8"BilboPoisonTimer"
			,(const char*)u8"Bilbo_Staff_Swing"
			,(const char*)u8"Bilbo_Staff_Swing2"
			 ,(const char*)u8"Bilbo_Staff_Swing3"
			,(const char*)u8"Bilbo_Staff_Jump"
			,(const char*)u8"Bilbo_Staff_Jump2"
			,(const char*)u8"Bilbo_Staff_Jump3"
			,(const char*)u8"Bilbo_Staff_AE_Jump"
			,(const char*)u8"Bilbo_Staff_AE_Jump2"
			 ,(const char*)u8"Bilbo_Staff_AE_Jump3"
			,(const char*)u8"Bilbo_Sting_Swing"
			,(const char*)u8"Bilbo_Sting_Swing2"
			,(const char*)u8"Bilbo_Sting_Swing3"
			,(const char*)u8"Bilbo_Sting_Jump"
			,(const char*)u8"Bilbo_Sting_Jump2"
			 ,(const char*)u8"Bilbo_Sting_Jump3"
			,(const char*)u8"Bilbo_Stone_Throw"
			,(const char*)u8"Bilbo_Stone_Throw2"
			,(const char*)u8"Bilbo_Stone_Throw3"
			,(const char*)u8"����� ������"
			,(const char*)u8"�������� �����"
			,(const char*)u8"�����������"
			,(const char*)u8"����� ����"
			,(const char*)u8"����"
			 ,(const char*)u8"������"
			,(const char*)u8"�����"
			,(const char*)u8"�������� �����"
			,(const char*)u8"�������� �����"
			,(const char*)u8"������������ �����"
			,(const char*)u8"�������� �����",
			(const char*)u8"���������� �����" };
		static int item = -1;

		ImGui::Text(lang ? "Items" : (const char*)u8"��������");
		ImGui::Separator();

		ImGui::Combo("", &item, items, IM_ARRAYSIZE(items));

		if (ImGui::Button(lang ? "Give item" : (const char*)u8"������ �������")) {
			plusA_float_hobbit((LPBYTE)0x0075BDB0 + item * 4, 1); //������� ������ ��������
		}
		if (ImGui::Button(lang ? "Remove item" : (const char*)u8"������� �������")) {
			change_2Byte_hobbit((LPBYTE)0x0075BDB2 + item * 4, 0x00, 0x00); //������� �������� ��������
		}
		if (ImGui::Button(lang ? "Show item" : (const char*)u8"�������� �������")) {
			change_1Byte_hobbit((LPBYTE)0x007212BC + item * 4, 0x01, 0x01); //������� ������ ��������
		}
		if (ImGui::Button(lang ? "Hide item" : (const char*)u8"�������� �������")) {
			change_2Byte_hobbit((LPBYTE)0x007212BC + item * 4, 0x00, 0x00); //������� �������� ��������
		}
		ImGui::Unindent();
	}

	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("");

	ImGui::Text(lang ? "Our links" : (const char*)u8"���� ������");
	ImGui::Separator();

	if (ImGui::Button("king174rus")) {
		ShellExecute(NULL, "open", "https://www.youtube.com/c/@king174rus", 0, 0, SW_SHOWNORMAL);
	}
	ImGui::SameLine(); ImGui::Text(" "); ImGui::SameLine();
	if (ImGui::Button("Mr_Kliff")) {
		ShellExecute(NULL, "open", "https://youtube.com/c/@YKliffa", 0, 0, SW_SHOWNORMAL);
	}
	ImGui::SameLine(); ImGui::Text(" "); ImGui::SameLine();
	if (ImGui::Button(lang ? "Hobbit Technical Discord" : (const char*)u8"����������� ����� ������� � ��������")) {
		ShellExecute(NULL, "open", "https://discord.gg/4eM6cZdYG8", 0, 0, SW_SHOWNORMAL);
	}

	ImGui::End();
}
