#pragma once

#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <memoryapi.h>
#include <tlhelp32.h>
#include <vector>
#include <mutex>

using namespace std;




HANDLE read_process_hobbit() noexcept;
uintptr_t GetModuleBaseAddress(DWORD pid, const wchar_t* moduleName) noexcept;
DWORD GetProcessID(const wchar_t* processName) noexcept;

//template <typename T>
//vector<uint32_t> findAllGameObjByPattern(T pattern, uint32_t shift) noexcept;
//vector<uint8_t> readData(LPVOID address, size_t bytesSize) noexcept;


template <typename T>
static inline int change_value_hobbit(LPVOID Address, T Znachenie, T Iznachalnoe)
{
	HANDLE Process = read_process_hobbit();
	T value;
	BOOL bWriteSuccess;
	const size_t T_size = sizeof(T);

	if (!ReadProcessMemory(Process, Address, &value, T_size, NULL)) {
		CloseHandle(Process);
		return 1;
	}

	DWORD oldProtect;
	VirtualProtectEx(Process, Address, T_size, PAGE_EXECUTE_READWRITE, &oldProtect);

	if (value == Znachenie) {
		bWriteSuccess = WriteProcessMemory(Process, Address, &Iznachalnoe, T_size, NULL);
	}
	else {
		bWriteSuccess = WriteProcessMemory(Process, Address, &Znachenie, T_size, NULL);
	}

	VirtualProtectEx(Process, Address, T_size, oldProtect, &oldProtect);
	CloseHandle(Process);
	if (bWriteSuccess) return 0;
	return 1;
}


template <typename T>
static inline int change_value_hobbit(LPVOID Address, T Znachenie)
{
	return change_value_hobbit(Address, Znachenie, Znachenie);
}




template <typename T>
static inline int plusA_value_hobbit(LPVOID Address, T a)
{
	HANDLE Process;
	Process = read_process_hobbit();
	T value;  //переменная значения байта по адресу
	if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //Чтение значения байта
		CloseHandle(Process);
		return 1;
	}
	DWORD oldProtect;
	T Znachenie;
	value += a;
	SIZE_T dwSize = sizeof(value);
	VirtualProtectEx(Process, Address, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect);
	BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &value, dwSize, NULL);
	VirtualProtectEx(Process, Address, dwSize, oldProtect, &oldProtect);
	CloseHandle(Process);
	if (bWriteSuccess) return 0;
	return 1;
}




template <typename T>
T read_value_hobbit(LPVOID Address) {
	HANDLE Process = read_process_hobbit();
	T value;

	if (!ReadProcessMemory(Process, Address, &value, sizeof(T), NULL)) {
		CloseHandle(Process);
		return T();
	}

	CloseHandle(Process);
	return value;
}




static uint32_t findObjectAddressByGUID(uint32_t guid) {
	HANDLE Process = read_process_hobbit();
	

	// ЧИТАЕМ БАЗОВЫЙ АДРЕС ОДИН РАЗ!
	uint32_t objectStackAddress = read_value_hobbit<uint32_t>((LPVOID)0x0076F648);
	if (objectStackAddress == 0) {
		CloseHandle(Process);
		return 0;
	}

	const size_t stackSize = 0xEFEC;
	const size_t jumpSize = 0x14;

	// Loop through the stack memory
	for (size_t offset = stackSize; offset > 0; offset -= jumpSize) {
		uint32_t objectAddress = 0;
		uint32_t objectGUID = 0;

		// Используем уже прочитанный адрес
		LPVOID objectPtrPtr = LPVOID(objectStackAddress + offset);

		//read the pointer of an object
		if (ReadProcessMemory(Process, objectPtrPtr, &objectAddress, sizeof(objectAddress), NULL)) {
			LPVOID guidAddress = LPVOID(objectAddress + 0x8);
			//read the guid
			if (ReadProcessMemory(Process, guidAddress, &objectGUID, sizeof(objectGUID), NULL) && objectGUID == guid) {
				CloseHandle(Process);
				return objectAddress;
			}
		}
	}

	CloseHandle(Process);
	return 0;
}




template <typename T>
vector<uint32_t> findAllGameObjByPattern(T pattern, uint32_t shift) {
	HANDLE Process = read_process_hobbit();
	vector<uint32_t> gameObjs;

	if (Process == NULL) return gameObjs;

	// Читаем базовые адреса
	DWORD objectStackSize = read_value_hobbit<DWORD>((LPVOID)0x0076F660);
	DWORD objectStackAddress = read_value_hobbit<DWORD>((LPVOID)0x0076F648);

	if (objectStackAddress == 0 || objectStackSize == 0) {
		CloseHandle(Process);
		return gameObjs;
	}
	cout<<hex<<objectStackSize<<" "<<objectStackAddress;
	const uint32_t OBJECT_PTR_SIZE = 0x14; // Размер между объектами в стеке
	const uint32_t totalSize = objectStackSize * OBJECT_PTR_SIZE;

	// Буфер для чтения всего стека объектов
	vector<BYTE> stackBuffer(totalSize);
	
	// Читаем весь стек объектов
	if (ReadProcessMemory(Process, (LPVOID)objectStackAddress, stackBuffer.data(), totalSize, NULL)) {

		// Проходим по всем объектам в стеке
		for (uint32_t offset = 0; offset < totalSize; offset += OBJECT_PTR_SIZE) {
			// Извлекаем адрес объекта из буфера
			//uint32_t objAddrs = *reinterpret_cast<uint32_t*>(stackBuffer.data() + offset);
			uint32_t objAddrs = *(int*)(((char*)stackBuffer.data()) + offset);
			if (objAddrs != 0) {
				// Читаем паттерн объекта
			
				T objPattern;
				if (ReadProcessMemory(Process, (LPVOID)(objAddrs + shift), &objPattern, sizeof(T), NULL)) {
				
					if (objPattern == pattern) {
						gameObjs.push_back(objAddrs);
					}
				}
			}
		}
	}

	CloseHandle(Process);
	return gameObjs;
}
