#pragma once

#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <memoryapi.h>
#include <tlhelp32.h>

int change_1Byte_hobbit(LPVOID Address, BYTE Znachenie, BYTE Iznachalnoe) noexcept;
int save_1Byte_hobbit(LPVOID Address, BYTE Znachenie) noexcept;
int change_2Byte_hobbit(LPVOID Address, WORD Znachenie, WORD Iznachalnoe) noexcept;
int change_4Byte_hobbit(LPVOID Address, DWORD Znachenie, DWORD Iznachalnoe) noexcept;
int plusA_int_hobbit(LPVOID Address, int a) noexcept;
int read_int_value(LPVOID Address) noexcept;
int plusA_float_hobbit(LPVOID Address, float a) noexcept;
int change_float_hobbit(LPVOID Address, float a) noexcept;
int save_float_hobbit(LPVOID Address) noexcept;
float read_float_value(LPVOID Address) noexcept;
LPDWORD ukazatel_hobbit(LPVOID Address) noexcept;
HANDLE read_process_hobbit() noexcept;
uintptr_t GetModuleBaseAddress(DWORD pid, const wchar_t* moduleName) noexcept;
DWORD GetProcessID(const wchar_t* processName) noexcept;