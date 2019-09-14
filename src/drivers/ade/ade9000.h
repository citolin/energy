#ifndef ADE9000_H_
#define ADE9000_H_

#include <Arduino.h>

class ADE9000 {
    private:

    public:
        ADE9000();
        ~ADE9000();

        float getCurrent();
        float getVoltage();
        float getFrequency();
        float getPowerFactor();
        float getActivePower();
        float getReactivePower();
        float getApparentPower();

};

#endif
