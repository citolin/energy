#ifndef CALLBACKCLASS_H_
#define CALLBACKCLASS_H_

#include <Arduino.h>
#include <functional>

class CallbackClass
{
private:
    // TODO -- Is there a way of doing this by template?
    //      or using a void jump table
    std::function<void(String)> callback;
    std::function<void(String, String)> secondCallback;

public:
    CallbackClass();
    ~CallbackClass();
    void registerCallback(std::function<void(String)> callback);
    void registerCallback(std::function<void(String, String)> callback);
    void callCallback(String data);
    void callCallback(String data, String secondData);
};

#endif