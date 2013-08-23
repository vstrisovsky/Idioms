#include <iostream>

#include "idioms.h"

class Base
{
private:
    virtual void Func(int x) = 0;
    friend class Attorney;
public:

    virtual ~Base()
    {
    }
};

class Derived : public Base
{
private:

    virtual void Func(int x)
    {
        std::cout << "Derived::Func" << std::endl;
    }

public:

    ~Derived()
    {
    }
};

class Attorney
{

    static void callFunc(Base & b, int x)
    {
        b.Func(x);
    }
    friend struct Test;
};

struct Test
{
    Derived d;
    Attorney a;

    void callFunc()
    {
        a.callFunc(d, 3);
    }
};

void testAttorneyClient()
{
    Test t;
    t.callFunc();

}
