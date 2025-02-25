#pragma once
#include "MemoryAccess.h"

class HobbitMemoryAccess
{
private:
	static uint32_t OBJECT_STACK_ADDRESS;
public:
	static MemoryAccess memoryAccess;

	static void setObjectStackAddress()
	{
		//0x0076F648: Object Stack Address in HobbitTM
		OBJECT_STACK_ADDRESS = memoryAccess.readData(0x0076F648);
	}
	static uint32_t findObjectAddressByGUID(uint32_t guid) {
		if (!memoryAccess.checkProcess()) return 0;

		const size_t stackSize = 0xEFEC;
		const size_t jumpSize = 0x14;
		// Loop through the stack memory
		for (size_t offset = stackSize; offset > 0; offset -= jumpSize) {
			uint32_t objectAddress = 0;
			uint32_t objectGUID = 0;
			LPVOID objectPtrPtr = LPVOID(OBJECT_STACK_ADDRESS + offset);

			//read the pointer of an object
			if (ReadProcessMemory(memoryAccess.getProcess(), objectPtrPtr, &objectAddress, sizeof(objectAddress), NULL)) {
				LPVOID guidAddress = LPVOID(objectAddress + 0x8);
				//read the guid
				if (ReadProcessMemory(memoryAccess.getProcess(), guidAddress, &objectGUID, sizeof(objectGUID), NULL) && objectGUID == guid) {
					return objectAddress;
				}
			}

		}

		return 0; // Return nullptr if dataToFind is not found in the stack range
	}

	static void setHobbitMemoryAccess()
	{
		memoryAccess.setProcess("Meridian.exe");
		setObjectStackAddress();		
	}

	static bool isLittleEndian() {
		int num = 1;
		if (*(char*)&num == 1)
		{
			return true;
		}
		return false;
	}
	static uint32_t getObjectStackAddress()
	{
		return OBJECT_STACK_ADDRESS;
	}

	static std::vector<uint32_t> findObjectsByData(void* data, size_t dataSize, uint32_t shift) {
		std::vector<uint32_t> foundObjects;
		if (!memoryAccess.checkProcess()) return foundObjects;

		const size_t stackSize = 0xEFEC;
		const size_t jumpSize = 0x14;

		for (size_t offset = stackSize; offset > 0; offset -= jumpSize) {
			uint32_t objectAddress = 0;
			LPVOID objectPtrPtr = LPVOID(OBJECT_STACK_ADDRESS + offset);

			if (ReadProcessMemory(memoryAccess.getProcess(), objectPtrPtr, &objectAddress, sizeof(objectAddress), NULL)) {
				LPVOID dataAddress = LPVOID(uintptr_t(objectAddress) + shift);
				uint8_t buffer[256]; // Assume max size of data is 256 bytes; adjust as needed
				if (dataSize <= sizeof(buffer) &&
					ReadProcessMemory(memoryAccess.getProcess(), dataAddress, buffer, dataSize, NULL) &&
					memcmp(buffer, data, dataSize) == 0) {
					foundObjects.push_back(objectAddress);
				}
			}
		}

		return foundObjects;
	}
	static std::vector<uint32_t> getAllObjects() {
		std::vector<uint32_t> foundObjects;
		if (!memoryAccess.checkProcess()) return foundObjects;

		const size_t stackSize = 0xEFEC;
		const size_t jumpSize = 0x14;

		for (size_t offset = stackSize; offset > 0; offset -= jumpSize) {
			uint32_t objectAddress = 0;
			LPVOID objectPtrPtr = LPVOID(OBJECT_STACK_ADDRESS + offset);

			if (ReadProcessMemory(memoryAccess.getProcess(), objectPtrPtr, &objectAddress, sizeof(objectAddress), NULL)) {
				if(objectAddress != 0)
					foundObjects.push_back(objectAddress);
			}
		}

		return foundObjects;
	}

	static std::vector<uint32_t> readObjectData(size_t dataSize, uint32_t shift) {
		std::vector<uint32_t> foundData;
		if (!memoryAccess.checkProcess()) return foundData;

		const size_t stackSize = 0xEFEC;
		const size_t jumpSize = 0x14;

		for (size_t offset = stackSize; offset > 0; offset -= jumpSize) {
			uint32_t objectAddress = 0;
			LPVOID objectPtrPtr = LPVOID(OBJECT_STACK_ADDRESS + offset);

			if (ReadProcessMemory(memoryAccess.getProcess(), objectPtrPtr, &objectAddress, sizeof(objectAddress), NULL)) {
				LPVOID dataAddress = LPVOID(uintptr_t(objectAddress) + shift);
				uint8_t buffer[256]; // Assume max size of data is 256 bytes; adjust as needed
				if (dataSize <= sizeof(buffer) &&
					ReadProcessMemory(memoryAccess.getProcess(), dataAddress, buffer, dataSize, NULL)) {
					uint32_t* dataPtr = reinterpret_cast<uint32_t*>(buffer);
					foundData.insert(foundData.end(), dataPtr, dataPtr + dataSize);
				}
			}
		}

		return foundData;
	}

	static std::vector<uint32_t> findObjectsByData(std::string dataStr, uint32_t shift)
	{
		// Convert the hexadecimal string to a vector of bytes
		std::vector<uint8_t> data;
		for (size_t i = 0; i < dataStr.length(); i += 2) {
			std::string byteString = dataStr.substr(i, 2);
			uint8_t byte = static_cast<uint8_t>(std::stoul(byteString, nullptr, 16));
			data.push_back(byte);
		}

		// Reverse the order of the bytes if the system is little-endian
		if (isLittleEndian()) {
			std::reverse(data.begin(), data.end());
		}

		return HobbitMemoryAccess::findObjectsByData(data.data(), data.size(), shift);
	}

	static bool isGameOpen()
	{
		return  memoryAccess.setProcess("Meridian.exe");
	}
};

