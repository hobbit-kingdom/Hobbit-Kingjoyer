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
			if (_wcsicmp(pe32.szExeFile, L"Meridian.exe") == 0) {
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

DWORD GetProcessID(const wchar_t* processName) {
	DWORD processID = 0;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 processEntry;
	processEntry.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(snapshot, &processEntry)) {
		do {
			if (!_wcsicmp(processEntry.szExeFile, processName)) {
				processID = processEntry.th32ProcessID;
				break;
			}
		} while (Process32Next(snapshot, &processEntry));
	}
	CloseHandle(snapshot);
	return processID;
}

uintptr_t GetModuleBaseAddress(DWORD pid, const wchar_t* moduleName) {
	uintptr_t baseAddress = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
	if (hSnapshot != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 moduleEntry;
		moduleEntry.dwSize = sizeof(moduleEntry);
		if (Module32First(hSnapshot, &moduleEntry)) {
			do {
				if (_wcsicmp(moduleEntry.szModule, moduleName) == 0) {
					baseAddress = (uintptr_t)moduleEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnapshot, &moduleEntry));
		}
		CloseHandle(hSnapshot);
	}
	return baseAddress;
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
int save_2Byte_hobbit(LPVOID Address, WORD Znachenie)
{
	HANDLE Process;
	Process = read_process_hobbit();
	WORD value;  //переменна€ значени€ байта по адресу
	if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //„тение значени€ байта
		CloseHandle(Process);
		return 1;
	}
	DWORD oldProtect;

	SIZE_T dwSize = sizeof(Znachenie);
	VirtualProtectEx(Process, Address, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect);
	BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &Znachenie, dwSize, NULL);
	VirtualProtectEx(Process, Address, dwSize, oldProtect, &oldProtect);
	if (bWriteSuccess) return 0;

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

int save_1Byte_hobbit(LPVOID Address, BYTE Znachenie)
{
	HANDLE Process;
	Process = read_process_hobbit();
	BYTE value;  //переменна€ значени€ байта по адресу
	if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //„тение значени€ байта
		CloseHandle(Process);
		return 1;
	}
	DWORD oldProtect;

	SIZE_T dwSize = sizeof(Znachenie);
	VirtualProtectEx(Process, Address, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect);
	BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &Znachenie, dwSize, NULL);
	VirtualProtectEx(Process, Address, dwSize, oldProtect, &oldProtect);
	if (bWriteSuccess) return 0;
}
