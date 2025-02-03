// OutputDevice.cpp
#include "OutputDevice.h"

void OutputDevice::write(uint8_t value) {
    std::cout << "Output: " << (int)value << std::endl;
}
