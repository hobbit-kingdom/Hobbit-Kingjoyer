
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "byte_functions.h"

#include <iostream>
#include <thread>
#include <atomic>


 
using namespace std;
atomic<BYTE> level(-1);
bool isLevelLoading = true;
bool allkill, allAgres = false;
bool allkillWas, allAgresWas = false;
void threadFunction1() {
    
        BYTE currentLevel = read_value_hobbit<BYTE>((LPVOID)0x00762B5C);
        isLevelLoading = read_value_hobbit<bool>((LPVOID)0x0076035C);
        if (isLevelLoading) level = -1;
        if ((level != currentLevel and isLevelLoading == false) or (allkillWas != allkill or allAgresWas!= allAgres))
        {
            this_thread::sleep_for(chrono::seconds(1));
            level = currentLevel;
            allkillWas = allkill;
            allAgresWas = allAgres;
            vector<uint32_t> allEnemieAddrs = findAllGameObjByPattern<uint64_t>(0xFFFFFFFF7FFFFFFF, 0x134); //0000000200000002
            for (uint32_t e : allEnemieAddrs) 
            {
                if (0x04004232 != read_value_hobbit<uint32_t>((LPVOID)(e + 0x10)))
                    continue;
                if (0xABCABCABCABCABC0 == read_value_hobbit<uint64_t>((LPVOID)(e + 0x8)))
                    continue;
                if (read_value_hobbit<uint32_t>((LPVOID)(e + 0xE8)) % 2 != 0 and allkill == true)
                    plusA_value_hobbit<BYTE>((LPVOID)(e + 0xE8), -1);
                if (allAgres == true) change_value_hobbit<DWORD>((LPVOID)(e + 0x1A4), 0x00000002);
            }
        }

    
}

void CanKillAll(bool k, bool a) {
    allkill = k;
    allAgres = a;
    std::thread t(threadFunction1);
    t.join(); // Запускаем в фоновом режиме
}
