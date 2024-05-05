#include "gui.h"
#include "byte_functions.h"
#include "Randommod.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"

#include <iostream>
#include "string"
#include <chrono>
#include <random>
#include <thread>
float timer2 = 0;
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distrib(0, 4);
int random_number = distrib(gen);
void RandomMod() {
	if (timer2 == 0 ) {
		int random_number = distrib(gen);
		if (random_number == 0) change_1Byte_hobbit((LPVOID)0x00777AA0, 0x01, 0x00);  //невидимый Бильбо
		if (random_number == 1) change_float_hobbit((LPVOID)0x0075B850, 100); //медленный Бильбо
		if (random_number == 2) change_float_hobbit((LPVOID)0x0075B850, 3000); //быстрый Бильбо
		if (random_number == 3) change_float_hobbit((LPVOID)0x0075B850, 10000); //Супербыстрый Бильбо
		if (random_number == 4) {
			change_1Byte_hobbit((LPVOID)0x00777A8C, 0x01, 0x00); //рендер объектов
			change_1Byte_hobbit((LPVOID)0x00777A98, 0x01, 0x00); //рендер ландшафта
		}
	}
	timer2 += ImGui::GetIO().DeltaTime;
	if (timer2 > 10.0f) {
		timer2 = 0;
		if (random_number == 0) change_1Byte_hobbit((LPVOID)0x00777AA0, 0x00, 0x00); //отключение невидимого Бильбо
		if (random_number == 1) change_float_hobbit((LPVOID)0x0075B850, 300); //стандартна скорость
		if (random_number == 2) change_float_hobbit((LPVOID)0x0075B850, 300);
		if (random_number == 3) change_float_hobbit((LPVOID)0x0075B850, 300);
		if (random_number == 4) {
			change_1Byte_hobbit((LPVOID)0x00777A8C, 0x00, 0x00); //рендер объектов
			change_1Byte_hobbit((LPVOID)0x00777A98, 0x00, 0x00); //рендер ландшафта
		}
	}
	return ;
}