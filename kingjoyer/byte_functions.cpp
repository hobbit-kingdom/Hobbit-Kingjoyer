#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <memoryapi.h>
#include <tlhelp32.h>

LPDWORD ukazatel_hobbit(LPVOID Address) {
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
    LPDWORD value, ukazatel=0x00;  //���������� �������� ����� �� ������
    if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //������ �������� �����
        CloseHandle(Process);
        return ukazatel;
    }
    ukazatel = value;
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
    float value, x, y, z;  //���������� �������� ����� �� ������
    if (!ReadProcessMemory(Process, Address, &value, sizeof(value), NULL)) { //������ �������� �����
        CloseHandle(Process);
        return 1;
    }
    x = value;
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
    Znachenie = a;
    SIZE_T dwSize = sizeof(Znachenie);
    VirtualProtectEx(Process, Address, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect);
    BOOL bWriteSuccess = WriteProcessMemory(Process, Address, &Znachenie, dwSize, NULL);
    VirtualProtectEx(Process, Address, dwSize, oldProtect, &oldProtect);
    if (bWriteSuccess) return 0;
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