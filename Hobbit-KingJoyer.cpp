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
    LPDWORD value,ukazatel;  //переменная значения байта по адресу
    if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //Чтение значения байта
        CloseHandle(Process);
        return ukazatel;
    }
    ukazatel=value;

	return ukazatel;
}
int save_float_hobbit(LPVOID Address)
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
    float value,x,y,z;  //переменная значения байта по адресу
    if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //Чтение значения байта
        CloseHandle(Process);
        return 1;
    }
    x=value;
	return x;
}
int change_float_hobbit(LPVOID Address, float a)
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
	//LPVOID Address = (LPVOID)0x007600E9; //это адрес в чит енжине
    float value;  //переменная значения байта по адресу
    if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //Чтение значения байта
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
	//LPVOID Address = (LPVOID)0x007600E9; //это адрес в чит енжине
    float value;  //переменная значения байта по адресу
    if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //Чтение значения байта
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
	//LPVOID Address = (LPVOID)0x007600E9; //это адрес в чит енжине
    WORD value;  //переменная значения байта по адресу
    if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //Чтение значения байта
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
	//LPVOID Address = (LPVOID)0x007600E9; //это адрес в чит енжине
    BYTE value;  //переменная значения байта по адресу
    if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //Чтение значения байта
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
			change_1Byte_hobbit((LPVOID)0x007600E9, 0x01, 0x00); //функция включения режима разработчика
			return 0;
			}
			else if(LOWORD(wParam)==Volume_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777B04, 0x01, 0x00); //функция рендера волумов
			return 0;
			}
			else if(LOWORD(wParam)==LoadTrigger_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777B18, 0x01, 0x00); //функция рендера загрузочных триггеров
			return 0;
			}
			else if(LOWORD(wParam)==Trigger_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777B1C, 0x01, 0x00); //функция рендера триггеров
			return 0;			
			}
			else if(LOWORD(wParam)==Water_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777B10, 0x01, 0x00); //функция рендера воды
			return 0;			
			}
			else if(LOWORD(wParam)==WebWall_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777B90, 0x01, 0x00); //функция рендера паутины
			return 0;			
			}
			else if(LOWORD(wParam)==Rope_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777B24, 0x01, 0x00); //функция рендера веревок
			return 0;			
			}
			else if(LOWORD(wParam)==TreeLeafClusters_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777B80, 0x01, 0x00); //функция рендер кластеров листвы деревьев
			return 0;			
			}
			else if(LOWORD(wParam)==TreasureChests_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777AF0, 0x01, 0x00); //функция рендер сундуков
			return 0;			
			}	
			else if(LOWORD(wParam)==Switches_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777AEC, 0x01, 0x00); //функция рендер рычагов
			return 0;			
			}	
			else if(LOWORD(wParam)==Bilbo_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777AA0, 0x01, 0x00); //функция рендер Бильбо
			return 0;			
			}
			else if(LOWORD(wParam)==Lights_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777AA4, 0x01, 0x00); //функция рендер света
			return 0;			
			}	
			else if(LOWORD(wParam)==FXObjects_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777B88, 0x01, 0x00); //функция рендер эффектов
			return 0;			
			}	
			else if(LOWORD(wParam)==Breakaway_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777B0C, 0x01, 0x00); //функция рендер света
			return 0;			
			}	
			else if(LOWORD(wParam)==BoulderRun_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777AFC, 0x01, 0x00); //функция рендер эффектов
			return 0;			
			}
			else if(LOWORD(wParam)==FPS_BUTTON){
			change_2Byte_hobbit((LPVOID)0x006EFBDA,0x4180,0x4204); //функция FPS
			return 0;			
			}
			else if(LOWORD(wParam)==Skybox_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777B5C,0x01,0x00); //функция рендера скайбокса
			return 0;			
			}
			else if(LOWORD(wParam)==SavePedestal_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777AF8,0x01,0x00); //функция рендера сохранялок
			return 0;			
			}
			else if(LOWORD(wParam)==PushBox_BUTTON){
			change_1Byte_hobbit((LPVOID)0x00777AF4,0x01,0x00); //функция рендера пушбоксов
			return 0;			
			}					
			else if(LOWORD(wParam)==Stones_BUTTON){
			plusA_float_hobbit((LPVOID)0x0075BDB4,1); //функция прибавки на 1 камней
			return 0;			
			}	
			else if(LOWORD(wParam)==GiveQuestItem_BUTTON){
			HWND hwndComboBox = GetDlgItem(hwnd, QuestItem); // получаем дескриптор выподающего списка
			int index = SendMessage(hwndComboBox, CB_GETCURSEL, 0, 0); // получаем индекс элемента
			int length = SendMessage(hwndComboBox, CB_GETLBTEXTLEN, index, 0); // получаем длинну элемента
			char* buffer = new char[length + 1]; // выделяем память под буффер
			SendMessage(hwndComboBox, CB_GETLBTEXT, index, (LPARAM)buffer); // получаем текст из выпадающего списка
			plusA_float_hobbit((LPBYTE)0x0075BE98+index*4,1); //функция выдачи квестового предмета
			delete[] buffer; // Удаление буффера
			}
			else if(LOWORD(wParam)==DeleteQuestItem_BUTTON){
			HWND hwndComboBox = GetDlgItem(hwnd, QuestItem); // получаем дескриптор выподающего списка
			int index = SendMessage(hwndComboBox, CB_GETCURSEL, 0, 0); // получаем индекс элемента
			int length = SendMessage(hwndComboBox, CB_GETLBTEXTLEN, index, 0); // получаем длинну элемента
			char* buffer = new char[length + 1]; // выделяем память под буффер
			SendMessage(hwndComboBox, CB_GETLBTEXT, index, (LPARAM)buffer); // получаем текст из выпадающего списка
			change_2Byte_hobbit((LPBYTE)0x0075BE9A+index*4,0x00,0x00); //функция выдачи квестового предмета
			delete[] buffer; // Удаление буффера
			return 0;			
			}				
			else if(LOWORD(wParam)==Vigor1_BUTTON){
			plusA_float_hobbit((LPVOID)0x0075BDC4,1); //функция прибавки на 1 доп хп
			return 0;			
			}		
			else if(LOWORD(wParam)==Vigor10_BUTTON){
			plusA_float_hobbit((LPVOID)0x0075BDC4,10); //функция прибавки на 10 доп хп
			return 0;			
			}
			else if(LOWORD(wParam)==Max1HP_BUTTON){
			plusA_float_hobbit((LPVOID)0x0075BE14,1); //функция прибавки на 1 макс хп
			return 0;			
			}		
			else if(LOWORD(wParam)==Max10HP_BUTTON){
			plusA_float_hobbit((LPVOID)0x0075BE14,10); //функция прибавки на 10 макс хп
			return 0;			
			}	
			else if(LOWORD(wParam)==minusFOV01_BUTTON){
			plusA_float_hobbit((LPVOID)0x00772BF0,-0.1); //функция приближения камеры на 0.1
			return 0;			
			}		
			else if(LOWORD(wParam)==plusFOV01_BUTTON){
			plusA_float_hobbit((LPVOID)0x00772BF0,+0.1); //функция отдаления камеры на 0.1
			return 0;			
			}		
			else if(LOWORD(wParam)==InvulBilbo_BUTTON){
			change_1Byte_hobbit((LPVOID)0x0075FBF4,0x01,0x00); //функция бессмертия
			return 0;			
			}	
			else if(LOWORD(wParam)==SpeedBilbo_BUTTON){
			HWND hwndTextbox = GetDlgItem(hwnd, SpeedBilbo);
			char buffer[256];
			GetWindowText(hwndTextbox, buffer, 256);
			int speed = atoi(buffer);
			change_float_hobbit((LPVOID)0x0075B850,speed); //функция изменения скорости Бильбо
			return 0;			
			}	
			else if(LOWORD(wParam)==WayPoint_BUTTON){
			savedPoint.ukazatel=ukazatel_hobbit((LPVOID)0x0075BA3C);
			ukazatel=savedPoint.ukazatel;
			savedPoint.x=save_float_hobbit(ukazatel+5);
			savedPoint.y=save_float_hobbit(ukazatel+6);
			savedPoint.z=save_float_hobbit(ukazatel+7);//функция изменения скорости Бильбо

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
	800, 
	0,  
	0, 
	GetModuleHandle(0), 0);
	//создаем кнопку
	HWND hwndButton1 = CreateWindow( 
   	"BUTTON",  // Predefined class; Unicode assumed 
    "Волумы",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    10,         // x позиция
    50,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)Volume_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
   	NULL);
   	
	//создаем кнопку
	HWND hwndButton = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Режим разработчика",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    10,         // x позиция
    10,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)DEV_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed

	HWND hwndButton2 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Триггеры загрузки",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    10,         // x позиция
    90,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)LoadTrigger_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
    
    HWND hwndButton3 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Триггеры",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    10,         // x позиция
    130,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)Trigger_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
    
    HWND hwndButton4 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Вода",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    10,         // x позиция
    170,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)Water_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
    
    HWND hwndButton5 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Паутина",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    10,         // x позиция
    210,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)WebWall_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
    
    HWND hwndButton6 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Веревка",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    10,         // x позиция
    250,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)Rope_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
    
    HWND hwndButton7 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Листья деревьев",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    10,         // x позиция
    290,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)TreeLeafClusters_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
    
    HWND hwndButton8 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Сундуки",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    10,         // x позиция
    330,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)TreasureChests_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
                
    HWND hwndButton9 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Рычаги",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    10,         // x позиция
    370,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)Switches_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
      
    HWND hwndButtonA = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Бильбо",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    10,         // x позиция
    410,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)Bilbo_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
  
    HWND hwndButtonB = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Свет",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    10,         // x позиция
    450,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)Lights_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed

    HWND hwndButtonC = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "эффекты",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    10,         // x позиция
    490,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)FXObjects_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed

    HWND hwndButtonD = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "ломающийся путь",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    10,         // x позиция
    530,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)Breakaway_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed

    HWND hwndButtonE = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Болдер ран",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    10,         // x позиция
    570,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)BoulderRun_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed

    HWND hwndButtonF = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "60FPS",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    85,         // x позиция
    10,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)FPS_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed 

    HWND hwndButton10 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Камни",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    85,         // x позиция
    50,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)Stones_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed 

    HWND hwndButton11 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Скайбокс",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    10,         // x позиция
    610,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)Skybox_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed

    HWND hwndButton12 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Пьедестал Сохранения",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    10,         // x позиция
    650,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)SavePedestal_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
	                 
    HWND hwndButton13 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Пуш бокс",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    10,         // x позиция
    690,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)PushBox_BUTTON,      // айди кнопки
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

    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Ключ тролля");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Камень Короля-Чародея");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Шиполист");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Синяя урна");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Красная урна");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Желтая урна");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Незаженный факел");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Горящий факел");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Мехи");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Перстень");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Масло");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Железная звездочка");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Серебряная звездочка");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Золотая звездочка");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Медная звездочка");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Стальная звездочка");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Железная шестеренка");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Серебряная шестеренка");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Золотая шестеренка");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Медная шестеренка");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Стальная шестеренка");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Железная шестерня");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Серебряная шестерня");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Золотая шестерня");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Медная шестерня");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Стальная шестерня");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Железный толкатель");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Серебряный толкатель");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Золотой толкатель");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Медный толкатель");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Стальной толкатель");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Железная шпага");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Серебряная шпага");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Золотая шпага");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Медная шпага");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Стальная шпага");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Дрова");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Ключ Грит");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Ключ Дим");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Ключ Мугг");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Ключ от тюрьмы");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Ключ от последнего моста");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Шестеренка Гандолы №1");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Шестеренка Гандолы №2");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Рычаг Варт");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Камень Варт");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Золотая чаша Трора");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Ожерелье Гириона");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Рубин Гроина");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Копье короля Дортина");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Золотое блюдо");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Кексы");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Ингридиенты для кексов");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Ключ от сарая");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Яблоко");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Молоток");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Гвозди");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Посох");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Яйцо");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Ягоды");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Мешок пшеницы");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Сахар");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Специи");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Колбаски");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Пшеница");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Одеяло");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Иголка");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"1 ключ-кристалл");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"2 ключ-кристалл");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"3 ключ-кристалл");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"4 ключ-кристалл");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Эльфийское зелье исцеления");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"1 ключ от Подземелья");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"2 ключ от Подземелья");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"3 ключ от Подземелья");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"4 ключ от Подземелья");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Пещерный кристалл");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Сон-трава");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Луннолист");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Паутинник");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Паутинное зелье");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Приводной ремень");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Цепь из сокровищницы");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Форма для ключа");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"1 форма для ключа");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"2 форма для ключа");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Ключ от сокровищницы");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"1 ключ от Тронного зала");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"2 ключ от Тронного зала");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Аркенстон");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Ключ от городского склада");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Черная стрела");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Золотой кинжал Мэллока");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Черная бутылка");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Синяя бутылка");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Фиолетовая бутылка");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Красная бутылка");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Желтая бутылка");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Послание Гендольфа");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Лекарство");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Ключ троллей");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Рукоятка для лестницы");
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)"Ключ Реннара");
    HWND hwndButton14 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Выдать квестовый предмет",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    85,         // x позиция
    90,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)GiveQuestItem_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed

    HWND hwndButton15 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Удалить квестовый предмет",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    85,         // x позиция
    130,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)DeleteQuestItem_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
    
    HWND hwndButton16 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "+1 дополнительное хп ",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    85,         // x позиция
    170,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)Vigor1_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
		  
    HWND hwndButton17 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "+10 дополнительное хп ",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    85,         // x позиция
    210,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)Vigor10_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
    
    HWND hwndButton18 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "+1 максимального хп ",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    85,         // x позиция
    250,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)Max1HP_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
    
    HWND hwndButton19 = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "+10 максимального хп ",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    85,         // x позиция
    290,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)Max10HP_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
    
    HWND hwndButton1A = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Отдалить камеру ",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    85,         // x позиция
    330,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)plusFOV01_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
    
    HWND hwndButton1B = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Приблизить камеру",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    85,         // x позиция
    370,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)minusFOV01_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
	
    HWND hwndButton1C = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Бессмертие",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    85,         // x позиция
    410,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)InvulBilbo_BUTTON,      // айди кнопки
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
    // отображение окна
    ShowWindow(hwnd, SW_SHOWDEFAULT);
	
    HWND hwndButton1D = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Изменить скорость Бильбо",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    85,         // x позиция
    450,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)SpeedBilbo_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
		
    HWND hwndButton1E = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Установить точку телепортации",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    85,         // x позиция
    490,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)WayPoint_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
		
    HWND hwndButton1F = CreateWindow( 
    "BUTTON",  // Predefined class; Unicode assumed 
    "Телепортироваться",      // текст кнопки
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // стиль
    85,         // x позиция
    530,         // y позиция 
    75,        // ширина кнопки
    40,        // длина кнопки
    hwnd,     // родительское окно
    (HMENU)Teleport_BUTTON,      // айди кнопки
    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
    NULL);      // Pointer not needed
		
    // запускаем цикл окна
    MSG msg = {};
    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
