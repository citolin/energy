#ifndef LORA_H_
#define LORA_H_

#include <Arduino.h>
#include <functional>

#define LORA_BAUDRATE 9600

#include "../../abstraction/CallbackClass.h"

class LORA : public CallbackClass {
    public:
        LORA();
        ~LORA();

        void loop();
        void write(String data);
};

#endif
