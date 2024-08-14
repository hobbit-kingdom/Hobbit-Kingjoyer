#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <memoryapi.h>
#include <tlhelp32.h>

HANDLE read_process_hobbit()
{
	DWORD pid = 0;     //переменна€ айди процесса
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(snapshot, &pe32)) {
		while (Process32Next(snapshot, &pe32)) { //ищет айди процесса
			if (strcmp(pe32.szExeFile, "Meridian.exe") == 0) {
				pid = pe32.th32ProcessID; //переменна€ айди процесса
				break;
			}
		}
	}
	CloseHandle(snapshot);
	DWORD Prava = PROCESS_ALL_ACCESS; //это права доступа
	HANDLE Process = OpenProcess(Prava, FALSE, pid); //числовое значение - это айди процесса в диспетчере задач
	return Process;
}
LPDWORD ukazatel_hobbit(LPVOID Address) {
	HANDLE Process;
	Process = read_process_hobbit();
	LPDWORD value, ukazatel = 0x00;  //переменна€ значени€ байта по адресу
	if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //„тение значени€ байта
		CloseHandle(Process);
		return ukazatel;
	}

	ukazatel = value;
	return ukazatel;
}


int save_float_hobbit(LPVOID Address)
{
	HANDLE Process;
	Process = read_process_hobbit();
	float value, x;  //переменна€ значени€ байта по адресу
	if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //„тение значени€ байта
		CloseHandle(Process);
		return 1;
	}
	x = value;
	return x;
}

float read_float_value(LPVOID Address)
{
	HANDLE Process;
	Process = read_process_hobbit();
	float value;  //переменна€ значени€ байта по адресу
	if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //„тение значени€ байта
		CloseHandle(Process);
		return 1.0;
	}
	return value;
}

int change_float_hobbit(LPVOID Address, float a)
{
	HANDLE Process;
	Process = read_process_hobbit();
	float value;  //переменна€ значени€ байта по адресу
	if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //„тение значени€ байта
		CloseHandle(Process);
		return 1;
	}
	DWORD oldProtect;
	float Znachenie;
	Znachenie = a;
	SIZE_T dwSize = sizeof(Znachenie);
	VirtualProtectEx(Process, Address, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect);
	BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &Znachenie, dwSize, NULL);
	VirtualProtectEx(Process, Address, dwSize, oldProtect, &oldProtect);
	if (bWriteSuccess) return 0;
}
int plusA_float_hobbit(LPVOID Address, float a)
{
	HANDLE Process;
	Process = read_process_hobbit();
	float value;  //переменна€ значени€ байта по адресу
	if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //„тение значени€ байта
		CloseHandle(Process);
		return 1;
	}
	DWORD oldProtect;
	float Znachenie;
	Znachenie = value;
	Znachenie += a;
	SIZE_T dwSize = sizeof(Znachenie);
	VirtualProtectEx(Process, Address, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect);
	BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &Znachenie, dwSize, NULL);
	VirtualProtectEx(Process, Address, dwSize, oldProtect, &oldProtect);
	if (bWriteSuccess) return 0;
}
int change_2Byte_hobbit(LPVOID Address, WORD Znachenie, WORD Iznachalnoe)
{
	HANDLE Process;
	Process = read_process_hobbit();
	WORD value;  //переменна€ значени€ байта по адресу
	if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //„тение значени€ байта
		CloseHandle(Process);
		return 1;
	}
	DWORD oldProtect;
	if (value == Znachenie) {
		SIZE_T dwSize = sizeof(Iznachalnoe);
		VirtualProtectEx(Process, Address, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect);
		BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &Iznachalnoe, dwSize, NULL);
		VirtualProtectEx(Process, Address, dwSize, oldProtect, &oldProtect);
		if (bWriteSuccess) return 0;
	}
	else {
		SIZE_T dwSize = sizeof(Znachenie);
		VirtualProtectEx(Process, Address, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect);
		BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &Znachenie, dwSize, NULL);
		VirtualProtectEx(Process, Address, dwSize, oldProtect, &oldProtect);
		if (bWriteSuccess) return 0;
	}
}
int change_1Byte_hobbit(LPVOID Address, BYTE Znachenie, BYTE Iznachalnoe)
{
	HANDLE Process;
	Process = read_process_hobbit();
	BYTE value;  //переменна€ значени€ байта по адресу
	if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //„тение значени€ байта
		CloseHandle(Process);
		return 1;
	}
	DWORD oldProtect;
	if (value == Znachenie) {
		SIZE_T dwSize = sizeof(Iznachalnoe);
		VirtualProtectEx(Process, Address, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect);
		BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &Iznachalnoe, dwSize, NULL);
		VirtualProtectEx(Process, Address, dwSize, oldProtect, &oldProtect);
		if (bWriteSuccess) return 0;
	}
	else {
		SIZE_T dwSize = sizeof(Znachenie);
		VirtualProtectEx(Process, Address, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect);
		BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &Znachenie, dwSize, NULL);
		VirtualProtectEx(Process, Address, dwSize, oldProtect, &oldProtect);
		if (bWriteSuccess) return 0;
	}
}
int change_4Byte_hobbit(LPVOID Address, DWORD Znachenie, DWORD Iznachalnoe)
{
	HANDLE Process;
	Process = read_process_hobbit();
	DWORD value;  //переменна€ значени€ байта по адресу
	if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //„тение значени€ байта
		CloseHandle(Process);
		return 1;
	}
	DWORD oldProtect;
	if (value == Znachenie) {
		SIZE_T dwSize = sizeof(Iznachalnoe);
		VirtualProtectEx(Process, Address, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect);
		BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &Iznachalnoe, dwSize, NULL);
		VirtualProtectEx(Process, Address, dwSize, oldProtect, &oldProtect);
		if (bWriteSuccess) return 0;
	}
	else {
		SIZE_T dwSize = sizeof(Znachenie);
		VirtualProtectEx(Process, Address, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect);
		BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &Znachenie, dwSize, NULL);
		VirtualProtectEx(Process, Address, dwSize, oldProtect, &oldProtect);
		if (bWriteSuccess) return 0;
	}
}
int plusA_int_hobbit(LPVOID Address, int a)
{
	HANDLE Process;
	Process = read_process_hobbit();
	int value;  //переменна€ значени€ байта по адресу
	if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //„тение значени€ байта
		CloseHandle(Process);
		return 1;
	}
	DWORD oldProtect;
	int Znachenie;
	Znachenie = value;
	Znachenie += a;
	SIZE_T dwSize = sizeof(Znachenie);
	VirtualProtectEx(Process, Address, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect);
	BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &Znachenie, dwSize, NULL);
	VirtualProtectEx(Process, Address, dwSize, oldProtect, &oldProtect);
	if (bWriteSuccess) return 0;
}
int read_int_value(LPVOID Address)
{
	HANDLE Process;
	Process = read_process_hobbit();
	int value;  //переменна€ значени€ байта по адресу
	if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //„тение значени€ байта
		CloseHandle(Process);
		return 1.0;
	}
	return value;
}
