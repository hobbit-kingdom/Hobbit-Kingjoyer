#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "byte_functions.h"

#include <iostream>
#include <thread>
#include <atomic>

void threadFunction(std::atomic<bool>& stopFlag) {

    
        uint32_t activatedAddress = read_value_hobbit<uint32_t>((LPVOID)0x00773BD0);

        uint32_t foundObject = findObjectAddressByGUID(activatedAddress);
        //uint32_t foundObject = findObjectAddressByGUID(activatedAddress);
        std::cout << std::hex;
        change_value_hobbit<DWORD>((LPVOID)(foundObject + 0x10), 0x02001122);
        std::cout << std::dec;
}


void PickupAll()
{
 

        std::atomic<bool> stopFlag(false);
        std::thread t(threadFunction, std::ref(stopFlag));


        t.join();


    return;
}