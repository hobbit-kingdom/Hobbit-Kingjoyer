#pragma once

int change_1Byte_hobbit(LPVOID Address, BYTE Znachenie, BYTE Iznachalnoe) noexcept;
int change_2Byte_hobbit(LPVOID Address, WORD Znachenie, WORD Iznachalnoe) noexcept;
int plusA_float_hobbit(LPVOID Address, float a) noexcept;
int change_float_hobbit(LPVOID Address, float a) noexcept;
int save_float_hobbit(LPVOID Address) noexcept;
LPDWORD ukazatel_hobbit(LPVOID Address) noexcept;