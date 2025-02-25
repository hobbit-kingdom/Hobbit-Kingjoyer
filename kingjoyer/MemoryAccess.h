#pragma once
#include <vector>
#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <memoryapi.h>
#include <tlhelp32.h>
#include <vector>

#include <cstring> // for std::memcpy
#include <type_traits> // for std::enable_if and std::is_arithmetic
#include <mutex>


using PROCESS = HANDLE;

enum
{
	memQueryFlags_ = 0,
	memQueryFlags_None = 0,
	memQueryFlags_Read = 0b0001,
	memQueryFlags_Write = 0b0010,
	memQueryFlags_Execute = 0b0100,

};
struct OppenedQuery
{
	PROCESS queriedProcess = 0;
	char* baseQueriedPtr = (char*)0x0;
	bool oppened()
	{
		return queriedProcess != 0;
	}
};


class MemoryAccess
{
private:
	static std::mutex guardWriteData;
	static std::mutex guardProcess;
	void setExecutableName(const std::string& newName);
	HANDLE process;

public:
	bool checkProcess()
	{
		std::lock_guard<std::mutex> guard(guardProcess);
		if (!process) {
			std::cout << "PROCESS IS NOT SET" << std::endl;
			return 0;
		}
		return 1;
	}

	HANDLE setProcess(const char* processName);
	HANDLE setProcess(std::string processName) { setProcess(processName.c_str()); }

	HANDLE getProcess()
	{
		return process;
	}

	uint32_t readData(LPVOID Address)
	{
		if (!checkProcess()) return 0;
		uint32_t data = 0;
		if (!ReadProcessMemory(process, Address, &data, sizeof(data), NULL)) { // Reading the data from memory

			data = 0;
		}
		return uint32_t(data);
	}
	std::vector<uint8_t> readData(LPVOID Address, size_t size)
	{
		if (!checkProcess()) return std::vector<uint8_t>();
		std::vector<uint8_t> data(size);
		SIZE_T bytesRead;
		if (!ReadProcessMemory(process, Address, data.data(), size, &bytesRead)) { // Reading the data from memory
			data.clear();
		}
		return data;
	}

	uint32_t readData(uint32_t Address)
	{
		return readData(LPVOID(Address));
	}
	std::vector<uint8_t> readData(uint32_t Address, size_t size)
	{
		return readData(LPVOID(Address), size);
	}


	// templates
	template<typename T>
	T writeData(LPVOID Address, T data)
	{
		std::lock_guard<std::mutex> guard(guardWriteData);
		if (!checkProcess()) return 0;
		T temporary = data;
		if (!ReadProcessMemory(process, Address, &temporary, sizeof(temporary), NULL)) { // Reading the value from memory
			return T(); // Return default value of type T if reading fails
		}
		DWORD oldProtect;

		SIZE_T dwSize = sizeof(data);
		VirtualProtectEx(process, Address, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect);
		BOOL bWriteSuccess = WriteProcessMemory(process, Address, &data, dwSize, NULL);
		VirtualProtectEx(process, Address, dwSize, oldProtect, &oldProtect);
		if (bWriteSuccess) return T(); // Return default value of type T if writing fails
		return 0;
	}
	void writeData(LPVOID Address, std::vector<uint8_t> data)
	{
		std::lock_guard<std::mutex> guard(guardWriteData);
		if (!checkProcess()) return;
		SIZE_T dwSize = data.size();
		DWORD oldProtect;
		VirtualProtectEx(process, Address, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect);
		BOOL bWriteSuccess = WriteProcessMemory(process, Address, data.data(), dwSize, NULL);
		VirtualProtectEx(process, Address, dwSize, oldProtect, &oldProtect);
		if (!bWriteSuccess) {
			// Handle error
		}
	}
	void writeData(LPVOID Address, float data)
	{
		writeData(Address, floatToUInt32(data));
	}
	void writeData(uint32_t Address, uint32_t data)
	{
		writeData(LPVOID(Address), data);
	}

	// converts uint32_t to float
	static float uint32ToFloat(uint32_t value) {
		float result;
		std::memcpy(&result, &value, sizeof(result));
		return result;
	}
	// converts float to uint32_t
	static uint32_t floatToUInt32(float value) {
		uint32_t result;
		std::memcpy(&result, &value, sizeof(result));
		return result;
	}

	// additional funcitons Find by Pattern
	std::vector<void*> findBytePatternInProcessMemory(void* pattern, size_t patternLen);
	std::vector<uint32_t> findBytePatternInProcessMemory(const std::vector<uint32_t>& pattern);
	std::vector<uint32_t> findBytePatternInProcessMemory(uint32_t pattern) 
	{
		std::vector<uint32_t> tempPattern;
		tempPattern.push_back(pattern);
		return findBytePatternInProcessMemory(tempPattern);
	};

	bool getNextQuery(OppenedQuery& query, void*& low, void*& hi, int& flags);
	OppenedQuery initVirtualQuery(PROCESS process);
};