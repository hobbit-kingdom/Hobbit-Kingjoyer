#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <memoryapi.h>
#include <tlhelp32.h>
#include <vector>
#include <mutex>

using namespace std;




HANDLE read_process_hobbit()
{
	DWORD pid = 0;     //переменная айди процесса
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(snapshot, &pe32)) {
		while (Process32Next(snapshot, &pe32)) { //ищет айди процесса
			if (_wcsicmp(pe32.szExeFile, L"Meridian.exe") == 0) {
				pid = pe32.th32ProcessID; //переменная айди процесса
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




