#include <fstream>
#include <iostream>
#include <string>
#include <memoryapi.h>
#include <windows.h>
#include <tlhelp32.h>
#define DEV_BUTTON 1
#define Volume_BUTTON 3
#define LoadTrigger_BUTTON 2
#define Trigger_BUTTON 4
#define Water_BUTTON 5
#define WebWall_BUTTON 6
#define Rope_BUTTON 7
#define TreeLeafClusters_BUTTON 8
#define TreasureChests_BUTTON 9
#define Switches_BUTTON 10
#define Bilbo_BUTTON 11
#define Lights_BUTTON 12
#define FXObjects_BUTTON 13
#define Breakaway_BUTTON 14
#define BoulderRun_BUTTON 15
#define FPS_BUTTON 16
#define Stones_BUTTON 17
#define Skybox_BUTTON 18
#define SavePedestal_BUTTON 19
#define PushBox_BUTTON 20
#define GiveQuestItem_BUTTON 21
#define QuestItem 22
#define DeleteQuestItem_BUTTON 23
#define Vigor1_BUTTON 24
#define Vigor10_BUTTON 25
#define Max1HP_BUTTON 26
#define Max10HP_BUTTON 27
#define minusFOV01_BUTTON 28
#define plusFOV01_BUTTON 29
#define InvulBilbo_BUTTON 30
#define SpeedBilbo 31
#define SpeedBilbo_BUTTON 32
#define WayPoint_BUTTON 33
#define Teleport_BUTTON 34
using namespace std;
LPDWORD ukazatel_hobbit(LPVOID Address){
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
    LPDWORD value,ukazatel;  //���������� �������� ����� �� ������
    if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //������ �������� �����
        CloseHandle(Process);
        return ukazatel;
    }
    ukazatel=value;

	return ukazatel;
}
int save_float_hobbit(LPVOID Address)
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
    float value,x,y,z;  //���������� �������� ����� �� ������
    if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //������ �������� �����
        CloseHandle(Process);
        return 1;
    }
    x=value;
	return x;
}
int change_float_hobbit(LPVOID Address, float a)
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
	//LPVOID Address = (LPVOID)0x007600E9; //��� ����� � ��� ������
    float value;  //���������� �������� ����� �� ������
    if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //������ �������� �����
        CloseHandle(Process);
        return 1;
    }
    DWORD oldProtect;
    float Znachenie;
    Znachenie=a;
    SIZE_T dwSize = sizeof(Znachenie);
    VirtualProtectEx(Process, Address, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect); 
    BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &Znachenie, dwSize, NULL);
    VirtualProtectEx(Process, Address, dwSize, oldProtect, &oldProtect);
	if(bWriteSuccess ) return 0;
}
int plusA_float_hobbit(LPVOID Address, float a)
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
	//LPVOID Address = (LPVOID)0x007600E9; //��� ����� � ��� ������
    float value;  //���������� �������� ����� �� ������
    if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //������ �������� �����
        CloseHandle(Process);
        return 1;
    }
    DWORD oldProtect;
    float Znachenie;
    Znachenie=value;
    Znachenie+=a;
    SIZE_T dwSize = sizeof(Znachenie);
    VirtualProtectEx(Process, Address, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect); 
    BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &Znachenie, dwSize, NULL);
    VirtualProtectEx(Process, Address, dwSize, oldProtect, &oldProtect);
	if(bWriteSuccess ) return 0;
}
int change_2Byte_hobbit(LPVOID Address, WORD Znachenie, WORD Iznachalnoe)
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
	//LPVOID Address = (LPVOID)0x007600E9; //��� ����� � ��� ������
    WORD value;  //���������� �������� ����� �� ������
    if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //������ �������� �����
        CloseHandle(Process);
        return 1;
    }
    DWORD oldProtect;
    if (value==Znachenie){
    SIZE_T dwSize = sizeof(Iznachalnoe);
    VirtualProtectEx(Process, Address, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect); 
    BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &Iznachalnoe, dwSize, NULL);
    VirtualProtectEx(Process, Address, dwSize, oldProtect, &oldProtect);
	if(bWriteSuccess ) return 0;
}
    else {
    SIZE_T dwSize = sizeof(Znachenie);
    VirtualProtectEx(Process, Address, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect); 
    BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &Znachenie, dwSize, NULL);
    VirtualProtectEx(Process, Address, dwSize, oldProtect, &oldProtect);
    if(bWriteSuccess ) return 0;
}}
int change_1Byte_hobbit(LPVOID Address, BYTE Znachenie, BYTE Iznachalnoe)
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
	//LPVOID Address = (LPVOID)0x007600E9; //��� ����� � ��� ������
    BYTE value;  //���������� �������� ����� �� ������
    if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //������ �������� �����
        CloseHandle(Process);
        return 1;
    }
    DWORD oldProtect;
    if (value==Znachenie){
    SIZE_T dwSize = sizeof(Iznachalnoe);
    VirtualProtectEx(Process, Address, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect); 
    BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &Iznachalnoe, dwSize, NULL);
    VirtualProtectEx(Process, Address, dwSize, oldProtect, &oldProtect);
	if(bWriteSuccess ) return 0;
}
    else {
    SIZE_T dwSize = sizeof(Znachenie);
    VirtualProtectEx(Process, Address, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect); 
    BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &Znachenie, dwSize, NULL);
    VirtualProtectEx(Process, Address, dwSize, oldProtect, &oldProtect);
    if(bWriteSuccess ) return 0;
}}
struct Point {
    float x;
    float y;
    float z;
    LPDWORD ukazatel;
};
Point savedPoint;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    	float x,y,z;
    	LPDWORD ukazatel;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_COMMAND:{
        	if(LOWORD(wParam)==DEV_BUTTON){
			change_1Byte_hobbit((LPVOID)0x007600E9, 0x01, 0x00); //������� ��������� ������ ������������
			return 0;
			}
			else if(LOWORD(wParam)==Volume_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777B04, 0x01, 0x00); //������� ������� �������
			return 0;
			}
			else if(LOWORD(wParam)==LoadTrigger_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777B18, 0x01, 0x00); //������� ������� ����������� ���������
			return 0;
			}
			else if(LOWORD(wParam)==Trigger_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777B1C, 0x01, 0x00); //������� ������� ���������
			return 0;			
			}
			else if(LOWORD(wParam)==Water_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777B10, 0x01, 0x00); //������� ������� ����
			return 0;			
			}
			else if(LOWORD(wParam)==WebWall_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777B90, 0x01, 0x00); //������� ������� �������
			return 0;			
			}
			else if(LOWORD(wParam)==Rope_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777B24, 0x01, 0x00); //������� ������� �������
			return 0;			
			}
			else if(LOWORD(wParam)==TreeLeafClusters_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777B80, 0x01, 0x00); //������� ������ ��������� ������ ��������
			return 0;			
			}
			else if(LOWORD(wParam)==TreasureChests_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777AF0, 0x01, 0x00); //������� ������ ��������
			return 0;			
			}	
			else if(LOWORD(wParam)==Switches_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777AEC, 0x01, 0x00); //������� ������ �������
			return 0;			
			}	
			else if(LOWORD(wParam)==Bilbo_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777AA0, 0x01, 0x00); //������� ������ ������
			return 0;			
			}
			else if(LOWORD(wParam)==Lights_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777AA4, 0x01, 0x00); //������� ������ �����
			return 0;			
			}	
			else if(LOWORD(wParam)==FXObjects_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777B88, 0x01, 0x00); //������� ������ ��������
			return 0;			
			}	
			else if(LOWORD(wParam)==Breakaway_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777B0C, 0x01, 0x00); //������� ������ �����
			return 0;			
			}	
			else if(LOWORD(wParam)==BoulderRun_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777AFC, 0x01, 0x00); //������� ������ ��������
			return 0;			
			}
			else if(LOWORD(wParam)==FPS_BUTTON){
			change_2Byte_hobbit((LPVOID)0x006EFBDA,0x4180,0x4204); //������� FPS
			return 0;			
			}
			else if(LOWORD(wParam)==Skybox_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777B5C,0x01,0x00); //������� ������� ���������
			return 0;			
			}
			else if(LOWORD(wParam)==SavePedestal_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777AF8,0x01,0x00); //������� ������� ����������
			return 0;			
			}
			else if(LOWORD(wParam)==PushBox_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777AF4,0x01,0x00); //������� ������� ���������
			return 0;			
			}					
			else if(LOWORD(wParam)==Stones_BUTTON){
			plusA_float_hobbit((LPVOID)0x0075BDB4,1); //������� �������� �� 1 ������
			return 0;			
			}	
			else if(LOWORD(wParam)==GiveQuestItem_BUTTON){
			HWND hwndComboBox = GetDlgItem(hwnd, QuestItem); // �������� ���������� ����������� ������
			int index = SendMessage(hwndComboBox, CB_GETCURSEL, 0, 0); // �������� ������ ��������
			int length = SendMessage(hwndComboBox, CB_GETLBTEXTLEN, index, 0); // �������� ������ ��������
			char* buffer = new char[length + 1]; // �������� ������ ��� ������
			SendMessage(hwndComboBox, CB_GETLBTEXT, index, (LPARAM)buffer); // �������� ����� �� ����������� ������
			plusA_float_hobbit((LPBYTE)0x0075BE98+index*4,1); //������� ������ ���������� ��������
			delete[] buffer; // �������� �������
			}
			else if(LOWORD(wParam)==DeleteQuestItem_BUTTON){
			HWND hwndComboBox = GetDlgItem(hwnd, QuestItem); // �������� ���������� ����������� ������
			int index = SendMessage(hwndComboBox, CB_GETCURSEL, 0, 0); // �������� ������ ��������
			int length = SendMessage(hwndComboBox, CB_GETLBTEXTLEN, index, 0); // �������� ������ ��������
			char* buffer = new char[length + 1]; // �������� ������ ��� ������
			SendMessage(hwndComboBox, CB_GETLBTEXT, index, (LPARAM)buffer); // �������� ����� �� ����������� ������
			change_2Byte_hobbit((LPBYTE)0x0075BE9A+index*4,0x00,0x00); //������� ������ ���������� ��������
			delete[] buffer; // �������� �������
			return 0;			
			}				
			else if(LOWORD(wParam)==Vigor1_BUTTON){
			plusA_float_hobbit((LPVOID)0x0075BDC4,1); //������� �������� �� 1 ��� ��
			return 0;			
			}		
			else if(LOWORD(wParam)==Vigor10_BUTTON){
			plusA_float_hobbit((LPVOID)0x0075BDC4,10); //������� �������� �� 10 ��� ��
			return 0;			
			}
			else if(LOWORD(wParam)==Max1HP_BUTTON){
			plusA_float_hobbit((LPVOID)0x0075BE14,1); //������� �������� �� 1 ���� ��
			return 0;			
			}		
			else if(LOWORD(wParam)==Max10HP_BUTTON){
			plusA_float_hobbit((LPVOID)0x0075BE14,10); //������� �������� �� 10 ���� ��
			return 0;			
			}	
			else if(LOWORD(wParam)==minusFOV01_BUTTON){
			plusA_float_hobbit((LPVOID)0x00772BF0,-0.1); //������� ����������� ������ �� 0.1
			return 0;			
			}		
			else if(LOWORD(wParam)==plusFOV01_BUTTON){
			plusA_float_hobbit((LPVOID)0x00772BF0,+0.1); //������� ��������� ������ �� 0.1
			return 0;			
			}		
			else if(LOWORD(wParam)==InvulBilbo_BUTTON){
			change_1Byte_hobbit((LPVOID)0x0075FBF4,0x01,0x00); //������� ����������
			return 0;			
			}	
			else if(LOWORD(wParam)==SpeedBilbo_BUTTON){
			HWND hwndTextbox = GetDlgItem(hwnd, SpeedBilbo);
			char buffer[256];
			GetWindowText(hwndTextbox, buffer, 256);
			int speed = atoi(buffer);
			change_float_hobbit((LPVOID)0x0075B850,speed); //������� ��������� �������� ������
			return 0;			
			}	
			else if(LOWORD(wParam)==WayPoint_BUTTON){
			savedPoint.ukazatel=ukazatel_hobbit((LPVOID)0x0075BA3C);
			ukazatel=savedPoint.ukazatel;
			savedPoint.x=save_float_hobbit(ukazatel+5);
			savedPoint.y=save_float_hobbit(ukazatel+6);
			savedPoint.z=save_float_hobbit(ukazatel+7);//������� ��������� �������� ������

			return x,y,z;		
			}	
			else if(LOWORD(wParam)==Teleport_BUTTON){
			x=savedPoint.x;
			y=savedPoint.y;
			z=savedPoint.z;
			ukazatel=savedPoint.ukazatel;
			if (x){
				change_float_hobbit(ukazatel+5,x);
				change_float_hobbit(ukazatel+281,x);
				change_float_hobbit(ukazatel+6,y);
				change_float_hobbit(ukazatel+282,y);
				change_float_hobbit(ukazatel+7,z);
				change_float_hobbit(ukazatel+283,z);
			}
			return 0;			
			}																						
		}
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
	800, 
	0,  
	0, 
	GetModuleHandle(0), 0);
	//������� ������
	HWND hwndButton1 = CreateWindow( 
   	"BUTTON",  // Predefined class; Unicode assumed 
    "������",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    10,         // x �������
    50,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)Volume_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
   	NULL);
   	
	//������� ������
	HWND hwndButton = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "����� ������������",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    10,         // x �������
    10,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)DEV_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed

	HWND hwndButton2 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "�������� ��������",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    10,         // x �������
    90,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)LoadTrigger_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
    
    HWND hwndButton3 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "��������",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    10,         // x �������
    130,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)Trigger_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
    
    HWND hwndButton4 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "����",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    10,         // x �������
    170,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)Water_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
    
    HWND hwndButton5 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "�������",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    10,         // x �������
    210,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)WebWall_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
    
    HWND hwndButton6 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "�������",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    10,         // x �������
    250,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)Rope_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
    
    HWND hwndButton7 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "������ ��������",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    10,         // x �������
    290,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)TreeLeafClusters_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
    
    HWND hwndButton8 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "�������",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    10,         // x �������
    330,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)TreasureChests_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
                
    HWND hwndButton9 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "������",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    10,         // x �������
    370,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)Switches_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
      
    HWND hwndButtonA = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "������",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    10,         // x �������
    410,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)Bilbo_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
  
    HWND hwndButtonB = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "����",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    10,         // x �������
    450,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)Lights_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed

    HWND hwndButtonC = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "�������",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    10,         // x �������
    490,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)FXObjects_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed

    HWND hwndButtonD = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "���������� ����",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    10,         // x �������
    530,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)Breakaway_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed

    HWND hwndButtonE = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "������ ���",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    10,         // x �������
    570,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)BoulderRun_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed

    HWND hwndButtonF = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "60FPS",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    85,         // x �������
    10,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)FPS_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed 

    HWND hwndButton10 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "�����",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    85,         // x �������
    50,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)Stones_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed 

    HWND hwndButton11 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "��������",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    10,         // x �������
    610,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)Skybox_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed

    HWND hwndButton12 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "��������� ����������",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    10,         // x �������
    650,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)SavePedestal_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
	                 
    HWND hwndButton13 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "��� ����",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    10,         // x �������
    690,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)PushBox_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
    
    HWND hComboBox = CreateWindowEx(0, 
	"COMBOBOX", 
	NULL, 
	CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 
	180, 
	110, 
	150, 
	500, 
	hwnd, 
	(HMENU)QuestItem, 
	(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
	NULL);

    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���� ������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������ ������-�������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"��������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"����� ����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������� ����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������ ����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���������� �����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������� �����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"��������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"�����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"�������� ���������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���������� ���������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������� ���������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������ ���������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"�������� ���������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"�������� ����������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���������� ����������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������� ����������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������ ����������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"�������� ����������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"�������� ��������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���������� ��������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������� ��������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������ ��������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"�������� ��������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"�������� ���������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���������� ���������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������� ���������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������ ���������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"�������� ���������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"�������� �����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���������� �����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������� �����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������ �����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"�������� �����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"�����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���� ����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���� ���");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���� ����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���� �� ������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���� �� ���������� �����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���������� ������� �1");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���������� ������� �2");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"����� ����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������ ����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������� ���� �����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"�������� �������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"����� ������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"����� ������ �������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������� �����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"�����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"����������� ��� ������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���� �� �����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"�������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"�����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"�����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"����� �������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"�����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"��������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"�������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"1 ����-��������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"2 ����-��������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"3 ����-��������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"4 ����-��������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���������� ����� ���������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"1 ���� �� ����������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"2 ���� �� ����������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"3 ���� �� ����������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"4 ���� �� ����������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"�������� ��������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���-�����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"��������� �����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"��������� ������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���� �� ������������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"����� ��� �����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"1 ����� ��� �����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"2 ����� ��� �����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���� �� ������������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"1 ���� �� �������� ����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"2 ���� �� �������� ����");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���� �� ���������� ������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������ ������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������� ������ �������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������ �������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"����� �������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���������� �������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������� �������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"������ �������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"�������� ���������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���� �������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"�������� ��� ��������");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"���� �������");
    HWND hwndButton14 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "������ ��������� �������",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    85,         // x �������
    90,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)GiveQuestItem_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed

    HWND hwndButton15 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "������� ��������� �������",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    85,         // x �������
    130,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)DeleteQuestItem_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
    
    HWND hwndButton16 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "+1 �������������� �� ",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    85,         // x �������
    170,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)Vigor1_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
		  
    HWND hwndButton17 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "+10 �������������� �� ",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    85,         // x �������
    210,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)Vigor10_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
    
    HWND hwndButton18 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "+1 ������������� �� ",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    85,         // x �������
    250,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)Max1HP_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
    
    HWND hwndButton19 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "+10 ������������� �� ",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    85,         // x �������
    290,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)Max10HP_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
    
    HWND hwndButton1A = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "�������� ������ ",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    85,         // x �������
    330,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)plusFOV01_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
    
    HWND hwndButton1B = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "���������� ������",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    85,         // x �������
    370,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)minusFOV01_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
	
    HWND hwndButton1C = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "����������",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    85,         // x �������
    410,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)InvulBilbo_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
			  		         
    HWND hTextbox=CreateWindow(TEXT("EDIT"), TEXT(""),
    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
    170, 
	460, 
	100, 
	20,
    hwnd, 
	(HMENU)SpeedBilbo, 
	GetModuleHandle(NULL), NULL);						                        
    // ����������� ����
    ShowWindow(hwnd, SW_SHOWDEFAULT);
	
    HWND hwndButton1D = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "�������� �������� ������",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    85,         // x �������
    450,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)SpeedBilbo_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
		
    HWND hwndButton1E = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "���������� ����� ������������",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    85,         // x �������
    490,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)WayPoint_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
		
    HWND hwndButton1F = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "�����������������",      // ����� ������
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // �����
    85,         // x �������
    530,         // y ������� 
    75,        // ������ ������
    40,        // ����� ������
    hwnd,     // ������������ ����
    (HMENU)Teleport_BUTTON,      // ���� ������
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
		
    // ��������� ���� ����
    MSG msg = {};
    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
