#include "Input.h"

class TestInput : public Input {
public:
    virtual int read() { return value;}
    void set(int value){this->value = value;}
private:
    int value;
};