#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "HobbitMemoryAccess.h"


#include <iostream>
#include <thread>
#include <atomic>

HobbitMemoryAccess hobbitMemoryAccess;
void threadFunction(std::atomic<bool>& stopFlag) {


        uint32_t activatedAddress = HobbitMemoryAccess::memoryAccess.readData(0x00773BD0);

        uint32_t foundObject = HobbitMemoryAccess::findObjectAddressByGUID(activatedAddress);

        std::cout << std::hex;
        HobbitMemoryAccess::memoryAccess.writeData(foundObject + 0x10, 0x02001122);
        std::cout << std::dec;
}


void PickupAll()
{
 
    HobbitMemoryAccess::setHobbitMemoryAccess();

    // Adding the listener to the event
    bool wasHobbitOpen = false;
    bool isHobbitOpen = false;
        std::atomic<bool> stopFlag(false);
        std::thread t(threadFunction, std::ref(stopFlag));


        t.join();


    return;
}