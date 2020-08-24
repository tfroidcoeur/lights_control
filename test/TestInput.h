#include "Input.h"

class TestInput : public Input {
public:
    TestInput(): value(0) {};
    virtual int read() { return value;}
    void set(int value){this->value = value;}
private:
    int value;
};