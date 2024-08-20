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
int a = 0;
bool vkl = false;
void RandomMod(float vremaeffectof)
{
	if (vkl == false or timer2 >= vremaeffectof) 
	{
		if (random_number == 0) change_1Byte_hobbit((LPVOID)0x00777AA0, 0x01, 0x00);  //невидимый Бильбо
		else if (random_number == 1) change_2Byte_hobbit((LPVOID)0x0075B852, 0x42C8, 0x4396); //медленный Бильбо
		else if (random_number == 2) change_4Byte_hobbit((LPVOID)0x0075B850, 0x453B8000, 0x43960000); //быстрый Бильбо
		else if (random_number == 3) change_4Byte_hobbit((LPVOID)0x0075B850, 0x461C4000, 0x43960000); //Супербыстрый Бильбо
		else if (random_number == 4)
		{
			change_1Byte_hobbit((LPVOID)0x00777A8C, 0x01, 0x00); //рендер объектов
			change_1Byte_hobbit((LPVOID)0x00777A98, 0x01, 0x00); //рендер ландшафта
		}
		else if (random_number == 5) change_4Byte_hobbit((LPVOID)0x0075B888, 0x447A0000, 0x453B8000); //высокий прыжок
		else if (random_number == 6) change_4Byte_hobbit((LPVOID)0x0075B888, 0x42C80000, 0x453B8000); //очень высокий прыжок
		else if (random_number == 7) change_2Byte_hobbit((LPVOID)0x0077244A, 0x4080, 0x0000); //широкий Бильбо
		else if (random_number == 8) change_1Byte_hobbit((LPVOID)0x0045CA39, 0xD4, 0xD8);//типо диабло
		else if (random_number == 9) change_2Byte_hobbit((LPVOID)0x00772BF8, 0x0000, 0x4170); //видимость всех объектов через объекты
		else if (random_number == 10) change_4Byte_hobbit((LPVOID)0x00772BF0, 0x40866666 ,0x3F99999A); //перевернутая камера
		else if (random_number == 11)
		{
			change_4Byte_hobbit((LPVOID)0x00772A70, 0x3F800000, 0x42C80000);
			change_4Byte_hobbit((LPVOID)0x00772B38, 0x3F800000, 0x42C80000);  //первое лицо
			change_4Byte_hobbit((LPVOID)0x00772B3C, 0x3F800000, 0x43960000);
		}
		else if (random_number == 12)
		{
			change_4Byte_hobbit((LPVOID)0x00772A70, 0xC3960000, 0x42C80000);
			change_4Byte_hobbit((LPVOID)0x00772B38, 0xC3960000, 0x42C80000);  //второе лицо
			change_4Byte_hobbit((LPVOID)0x00772B3C, 0xC3960000, 0x43960000);
		}
		vkl = !vkl;
	}
	if (timer2 >= vremaeffectof) {
		timer2 = 0;
		random_number = distrib(gen);
	}
	else timer2 += ImGui::GetIO().DeltaTime;
	return;

}
