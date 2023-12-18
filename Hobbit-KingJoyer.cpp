#include <fstream>
#include <iostream>
#include <string>
#include <memoryapi.h>
#include <windows.h>
#include <tlhelp32.h>
#define DEV_BUTTON 1
#define Volume_BUTTON 3
#define Render_BUTTON 2
using namespace std;
int Volume_hobbit(){
	DWORD pid = 0;     //переменная айди процесса
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(snapshot, &pe32)) {
        while (Process32Next(snapshot, &pe32)) { //ищет айди процесса
            if (strcmp(pe32.szExeFile, "Meridian.exe") == 0) {
                pid = pe32.th32ProcessID; //переменная айди процесса
                break;
            }
        }
    }
    CloseHandle(snapshot);
    DWORD Prava = PROCESS_ALL_ACCESS; //это права доступа
	HANDLE Process = OpenProcess(Prava, FALSE, pid); //числовое значение - это айди процесса в диспетчере задач 
	LPVOID Address = (LPVOID)0x00777B04; //это адрес в чит енжине
    BYTE value;  //переменная значения байта по адресу
    if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //Чтение значения байта
        CloseHandle(Process);
        return 1;
    }
    BYTE Znachenie = !value; 
    SIZE_T dwSize = sizeof(Znachenie); 
    BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &Znachenie, dwSize, NULL); //замена байта на значение
	if(bWriteSuccess ) return 0;
}
int DEV_hobbit() 
{ 
	DWORD pid = 0;     //переменная айди процесса
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(snapshot, &pe32)) {
        while (Process32Next(snapshot, &pe32)) { //ищет айди процесса
            if (strcmp(pe32.szExeFile, "Meridian.exe") == 0) {
                pid = pe32.th32ProcessID; //переменная айди процесса
                break;
            }
        }
    }
    CloseHandle(snapshot);
    DWORD Prava = PROCESS_ALL_ACCESS; //это права доступа
	HANDLE Process = OpenProcess(Prava, FALSE, pid); //числовое значение - это айди процесса в диспетчере задач 
	LPVOID Address = (LPVOID)0x007600E9; //это адрес в чит енжине
    BYTE value;  //переменная значения байта по адресу
    if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //Чтение значения байта
        CloseHandle(Process);
        return 1;
    }
    BYTE Znachenie = !value; 
    SIZE_T dwSize = sizeof(Znachenie); 
    BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &Znachenie, dwSize, NULL); //замена байта на значение
	if(bWriteSuccess ) return 0;
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    	
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_COMMAND:{
        	if(LOWORD(wParam)==DEV_BUTTON){
			MessageBox(hwnd, "Button pressed", NULL, MB_OK); //надо удалить
			DEV_hobbit(); //функция включения режима разработчика
			return 0;
			}
			else if(LOWORD(wParam)==Volume_BUTTON){
			MessageBox(hwnd, "Button pressed", NULL, MB_OK); //надо удалить
			Volume_hobbit(); //функция включения режима разработчика
			return 0;
			}
			else if(LOWORD(wParam)==Render_BUTTON){
			HWND hwndButton1 = CreateWindow( 
   		 	"BUTTON",  // Predefined class; Unicode assumed 
    		"Волумы",      // текст кнопки
    		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    		10,         // x позиция
    		160,         // y позиция 
    		200,        // ширина кнопки
    		50,        // длина кнопки
    		hwnd,     // родительское окно
    		(HMENU)Volume_BUTTON,      // айди кнопки
    		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
   		 	NULL);
   		 	                // ???????? ?????????? ??????
            HWND hButton = GetDlgItem(hwnd, Render_BUTTON);
                // ???????? ??????
            ShowWindow(hButton, SW_HIDE);
			}}
        default:

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    
}
int main() {
	setlocale(LC_ALL, "RUSSIAN"); //Русский язык
	   // регистрируем класс окна
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(0);
    wc.lpszClassName = "MyWindowClass";
    RegisterClass(&wc);

    // создаем окно
    HWND hwnd = CreateWindowEx(0, 
	"MyWindowClass", 
	"пример окна", 
	WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
	800, 
	600, 
	0,  
	0, 
	GetModuleHandle(0), 0);
    HWND hwndButton2 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Рендер",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    10,         // x позиция
    60,         // y позиция 
    200,        // ширина кнопки
    50,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)Render_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed.
	//создаем кнопку
	HWND hwndButton = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Режим разработчика",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    10,         // x позиция
    10,         // y позиция 
    200,        // ширина кнопки
    50,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)DEV_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
      // Pointer not needed.
    // отображение окна
    ShowWindow(hwnd, SW_SHOWDEFAULT);
	
    // запускаем цикл окна
    MSG msg = {};
    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
