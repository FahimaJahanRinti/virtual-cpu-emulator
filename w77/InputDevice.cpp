// InputDevice.cpp
#include "InputDevice.h"

uint8_t InputDevice::read() {
    uint8_t input;
    std::cout << "Enter a value (0-255): ";
    std::cin >> input;
    return input;
}
