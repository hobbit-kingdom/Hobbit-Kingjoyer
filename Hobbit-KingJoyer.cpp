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
	DWORD pid = 0;     //���������� ���� ��������
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(snapshot, &pe32)) {
        while (Process32Next(snapshot, &pe32)) { //���� ���� ��������
            if (strcmp(pe32.szExeFile, "Meridian.exe") == 0) {
                pid = pe32.th32ProcessID; //���������� ���� ��������
                break;
            }
        }
    }
    CloseHandle(snapshot);
    DWORD Prava = PROCESS_ALL_ACCESS; //��� ����� �������
	HANDLE Process = OpenProcess(Prava, FALSE, pid); //�������� �������� - ��� ���� �������� � ���������� ����� 
	LPVOID Address = (LPVOID)0x00777B04; //��� ����� � ��� ������
    BYTE value;  //���������� �������� ����� �� ������
    if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //������ �������� �����
        CloseHandle(Process);
        return 1;
    }
    BYTE Znachenie = !value; 
    SIZE_T dwSize = sizeof(Znachenie); 
    BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &Znachenie, dwSize, NULL); //������ ����� �� ��������
	if(bWriteSuccess ) return 0;
}
int DEV_hobbit() 
{ 
	DWORD pid = 0;     //���������� ���� ��������
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(snapshot, &pe32)) {
        while (Process32Next(snapshot, &pe32)) { //���� ���� ��������
            if (strcmp(pe32.szExeFile, "Meridian.exe") == 0) {
                pid = pe32.th32ProcessID; //���������� ���� ��������
                break;
            }
        }
    }
    CloseHandle(snapshot);
    DWORD Prava = PROCESS_ALL_ACCESS; //��� ����� �������
	HANDLE Process = OpenProcess(Prava, FALSE, pid); //�������� �������� - ��� ���� �������� � ���������� ����� 
	LPVOID Address = (LPVOID)0x007600E9; //��� ����� � ��� ������
    BYTE value;  //���������� �������� ����� �� ������
    if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //������ �������� �����
        CloseHandle(Process);
        return 1;
    }
    BYTE Znachenie = !value; 
    SIZE_T dwSize = sizeof(Znachenie); 
    BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &Znachenie, dwSize, NULL); //������ ����� �� ��������
	if(bWriteSuccess ) return 0;
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    	
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_COMMAND:{
        	if(LOWORD(wParam)==DEV_BUTTON){
			MessageBox(hwnd, "Button pressed", NULL, MB_OK); //���� �������
			DEV_hobbit(); //������� ��������� ������ ������������
			return 0;
			}
			else if(LOWORD(wParam)==Volume_BUTTON){
			MessageBox(hwnd, "Button pressed", NULL, MB_OK); //���� �������
			Volume_hobbit(); //������� ��������� ������ ������������
			return 0;
			}
			else if(LOWORD(wParam)==Render_BUTTON){
			HWND hwndButton1 = CreateWindow( 
   		 	"BUTTON",  // Predefined class; Unicode assumed 
    		"������",      // ����� ������
    		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    		10,         // x �������
    		160,         // y ������� 
    		200,        // ������ ������
    		50,        // ����� ������
    		hwnd,     // ������������ ����
    		(HMENU)Volume_BUTTON,      // ���� ������
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
	setlocale(LC_ALL, "RUSSIAN"); //������� ����
	   // ������������ ����� ����
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(0);
    wc.lpszClassName = "MyWindowClass";
    RegisterClass(&wc);

    // ������� ����
    HWND hwnd = CreateWindowEx(0, 
	"MyWindowClass", 
	"������ ����", 
	WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
	800, 
	600, 
	0,  
	0, 
	GetModuleHandle(0), 0);
    HWND hwndButton2 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "������",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    10,         // x �������
    60,         // y ������� 
    200,        // ������ ������
    50,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)Render_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed.
	//������� ������
	HWND hwndButton = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "����� ������������",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    10,         // x �������
    10,         // y ������� 
    200,        // ������ ������
    50,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)DEV_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
      // Pointer not needed.
    // ����������� ����
    ShowWindow(hwnd, SW_SHOWDEFAULT);
	
    // ��������� ���� ����
    MSG msg = {};
    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
