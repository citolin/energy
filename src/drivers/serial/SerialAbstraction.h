#ifndef SERIALABSTRACTION_H_
#define SERIALABSTRACTION_H_

#include <Arduino.h>
#include <functional>

#include "../../abstraction/CallbackClass.h"

class SerialAbstraction : public CallbackClass {
    public:
        SerialAbstraction();
        ~SerialAbstraction();

        void loop();
};

#endif
