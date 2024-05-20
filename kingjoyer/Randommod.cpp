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
std::uniform_int_distribution<> distrib(0, 12);
int random_number = distrib(gen);
void RandomMod(float vremaeffectof) {
	if (timer2 == 0 ) {
		int random_number = distrib(gen);
		if (random_number == 0) change_1Byte_hobbit((LPVOID)0x00777AA0, 0x01, 0x00);  //невидимый Бильбо
		if (random_number == 1) change_float_hobbit((LPVOID)0x0075B850, 100); //медленный Бильбо
		if (random_number == 2) change_float_hobbit((LPVOID)0x0075B850, 3000); //быстрый Бильбо
		if (random_number == 3) change_float_hobbit((LPVOID)0x0075B850, 10000); //Супербыстрый Бильбо
		if (random_number == 4)
		{
			change_1Byte_hobbit((LPVOID)0x00777A8C, 0x01, 0x00); //рендер объектов
			change_1Byte_hobbit((LPVOID)0x00777A98, 0x01, 0x00); //рендер ландшафта
		}
		if (random_number == 5) change_float_hobbit((LPVOID)0x0075B888, 1000);
		if (random_number == 6) change_float_hobbit((LPVOID)0x0075B888, 100);
		if (random_number == 7) change_float_hobbit((LPVOID)0x00772448, 4); //широкий Бильбо
		if (random_number == 8) change_1Byte_hobbit((LPVOID)0x0045CA39, 0xD8, 0xD4);//типо диабло
		if (random_number == 9) change_float_hobbit((LPVOID)0x00772BF8, 0); //видимость всех объектов через объекты
		if (random_number == 10) change_float_hobbit((LPVOID)0x00772BF0, 4.2); //перевернутая камера
		if (random_number == 11)
		{
			change_float_hobbit((LPVOID)0x00772A70, 1);
			change_float_hobbit((LPVOID)0x00772B38, 1);  //первое лицо
			change_float_hobbit((LPVOID)0x00772B3C, 1);
		}
		if (random_number == 12)
		{
			change_float_hobbit((LPVOID)0x00772A70, -300);
			change_float_hobbit((LPVOID)0x00772B38, -300);  //второе лицо
			change_float_hobbit((LPVOID)0x00772B3C, -300);
		}
	}
	timer2 += ImGui::GetIO().DeltaTime;
	if (timer2 > vremaeffectof) {
		if (random_number == 0) change_1Byte_hobbit((LPVOID)0x00777AA0, 0x00, 0x00); //отключение невидимого Бильбо
		if (random_number == 1) change_float_hobbit((LPVOID)0x0075B850, 300); //стандартна скорость
		if (random_number == 2) change_float_hobbit((LPVOID)0x0075B850, 300);
		if (random_number == 3) change_float_hobbit((LPVOID)0x0075B850, 300);
		if (random_number == 4)
		{
			change_1Byte_hobbit((LPVOID)0x00777A8C, 0x00, 0x00); //рендер объектов
			change_1Byte_hobbit((LPVOID)0x00777A98, 0x00, 0x00); //рендер ландшафта
		}
		if (random_number == 5) change_float_hobbit((LPVOID)0x0075B888, 3000);
		if (random_number == 6) change_float_hobbit((LPVOID)0x0075B888, 3000);
		if (random_number == 7) change_float_hobbit((LPVOID)0x00772448, 0); //широкий Бильбо
		if (random_number == 8) change_1Byte_hobbit((LPVOID)0x0045CA39, 0xD4, 0xD4); //типо диабло
		if (random_number == 9) change_float_hobbit((LPVOID)0x00772BF8, 15); //видимость всех объектов через объекты
		if (random_number == 10) change_float_hobbit((LPVOID)0x00772BF0, 1.7); //перевернутая камера
		if (random_number == 11)
		{
			change_float_hobbit((LPVOID)0x00772A70, 100);
			change_float_hobbit((LPVOID)0x00772B38, 100);  //первое лицо
			change_float_hobbit((LPVOID)0x00772B3C, 300);
		}
		if (random_number == 12)
		{
			change_float_hobbit((LPVOID)0x00772A70, 100);
			change_float_hobbit((LPVOID)0x00772B38, 100);  //первое лицо
			change_float_hobbit((LPVOID)0x00772B3C, 300);
		}
		timer2 = 0;
	}
	return ;
}