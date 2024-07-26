#include "lights.h"
#include "pros/misc.hpp"
/*
namespace lib {


void Lights::loop() {

    int rotationPixel = 0;
    const int maxPixel = 32;
    const int blobSize = 3;

    while (true) {

        if (pros::competition::is_disabled()) {
            rotationPixel++;
            if (rotationPixel > maxPixel + blobSize - 1) {rotationPixel = 0;}

            leftArmLed->clear();
            for (int i = 0; i < blobSize; i++) {
                leftArmLed->set_pixel(0xccff00, rotationPixel + i);
            }
        }



        pros::delay(20);
    }
}


}
*/