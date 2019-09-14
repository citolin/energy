#ifndef SERIALABSTRACTION_H_
#define SERIALABSTRACTION_H_

#include <Arduino.h>
#include <functional>

class SerialAbstraction {
    private:
        std::function<void(String)> onDataCallback;

    public:
        SerialAbstraction();
        ~SerialAbstraction();

        void registerOnDataCallback(std::function<void(String)> callback);    

        void loop();
};

#endif
