#include "gui.h"
#include "byte_functions.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"

#include "iostream"
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
	
	const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD" };
	static int item = -1;
	ImGui::Combo("Combo", &item, items, IM_ARRAYSIZE(items));

	
	ImGui::SameLine();

	if (ImGui::Button("Button1"))
	{
		std::cout << item << " ";
		
	}

	{
		
		if (ImGui::Checkbox( lang ? "Developer Mode" : (const char*)u8"ÊÈÍÃ ëîõ", &developerMode))
		{
			if ( debug ) std::cout << developerMode << " ";

			change_1Byte_hobbit((LPVOID)0x007600E9, 0x01, 0x00);
		}

		if (ImGui::Checkbox("Developer mode", &renderVolumes))
		{
			if (debug) std::cout << renderVolumes << " ";

			change_1Byte_hobbit((LPVOID)0x007600E9, 0x01, 0x00);
		}
		if (ImGui::Checkbox("Developer mode", &renderLoadTriggers))
		{
			if (debug) std::cout << renderLoadTriggers << " ";

			change_1Byte_hobbit((LPVOID)0x007600E9, 0x01, 0x00);
		}
		if (ImGui::Checkbox("Developer mode", &renderTriggers))
		{
			if (debug) std::cout << renderTriggers << " ";

			change_1Byte_hobbit((LPVOID)0x007600E9, 0x01, 0x00);
		}
		if (ImGui::Checkbox("Developer mode", &renderWater))
		{
			if (debug) std::cout << renderWater << " ";

			change_1Byte_hobbit((LPVOID)0x007600E9, 0x01, 0x00);
		}
		if (ImGui::Checkbox("Developer mode", &renderWeb))
		{
			if (debug) std::cout << renderWeb << " ";

			change_1Byte_hobbit((LPVOID)0x007600E9, 0x01, 0x00);
		}
		if (ImGui::Checkbox("Developer mode", &renderRopes))
		{
			if (debug) std::cout << renderRopes << " ";

			change_1Byte_hobbit((LPVOID)0x007600E9, 0x01, 0x00);
		}
		if (ImGui::Checkbox("Developer mode", &renderLeaves))
		{
			if (debug) std::cout << renderLeaves << " ";

			change_1Byte_hobbit((LPVOID)0x007600E9, 0x01, 0x00);
		}
		if (ImGui::Checkbox("Developer mode", &renderChests))
		{
			if (debug) std::cout << renderChests << " ";

			change_1Byte_hobbit((LPVOID)0x007600E9, 0x01, 0x00);
		}
		if (ImGui::Checkbox("Developer mode", &renderLevers))
		{
			if (debug) std::cout << renderLevers << " ";

			change_1Byte_hobbit((LPVOID)0x007600E9, 0x01, 0x00);
		}
		if (ImGui::Checkbox("Developer mode", &renderBilbo))
		{
			if (debug) std::cout << renderBilbo << " ";

			change_1Byte_hobbit((LPVOID)0x007600E9, 0x01, 0x00);
		}
		if (ImGui::Checkbox("Developer mode", &renderLights))
		{
			if (debug) std::cout << renderLights << " ";

			change_1Byte_hobbit((LPVOID)0x007600E9, 0x01, 0x00);
		}
		if (ImGui::Checkbox("Developer mode", &renderEffects))
		{
			if (debug) std::cout << renderEffects << " ";

			change_1Byte_hobbit((LPVOID)0x007600E9, 0x01, 0x00);
		}
		if (ImGui::Checkbox("Developer mode", &renderSkybox))
		{
			if (debug) std::cout << renderSkybox << " ";

			change_1Byte_hobbit((LPVOID)0x007600E9, 0x01, 0x00);
		}
		if (ImGui::Checkbox("Developer mode", &renderSavePedestal))
		{
			if (debug) std::cout << renderSavePedestal << " ";

			change_1Byte_hobbit((LPVOID)0x007600E9, 0x01, 0x00);
		}
		if (ImGui::Checkbox("Developer mode", &renderPushBoxes))
		{
			if (debug) std::cout << renderPushBoxes << " ";

			change_1Byte_hobbit((LPVOID)0x007600E9, 0x01, 0x00);
		}
		if (ImGui::Checkbox("Developer mode", &breakway))
		{
			if (debug) std::cout << breakway << " ";

			change_1Byte_hobbit((LPVOID)0x007600E9, 0x01, 0x00);
		}
		if (ImGui::Checkbox("Developer mode", &boulderRun))
		{
			if (debug) std::cout << boulderRun << " ";

			change_1Byte_hobbit((LPVOID)0x007600E9, 0x01, 0x00);
		}
		
	}

	

	ImGui::End();
}
