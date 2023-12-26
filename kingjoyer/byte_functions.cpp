#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <memoryapi.h>
#include <tlhelp32.h>

LPDWORD ukazatel_hobbit(LPVOID Address) {
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
    LPDWORD value, ukazatel=0x00;  //переменная значения байта по адресу
    if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //Чтение значения байта
        CloseHandle(Process);
        return ukazatel;
    }
    ukazatel = value;
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
    float value, x, y, z;  //переменная значения байта по адресу
    if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //Чтение значения байта
        CloseHandle(Process);
        return 1;
    }
    x = value;
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
    Znachenie = a;
    SIZE_T dwSize = sizeof(Znachenie);
    VirtualProtectEx(Process, Address, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect);
    BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &Znachenie, dwSize, NULL);
    VirtualProtectEx(Process, Address, dwSize, oldProtect, &oldProtect);
    if (bWriteSuccess) return 0;
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