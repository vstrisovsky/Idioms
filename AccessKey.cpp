#include "idioms.h"

#include <iostream>

// High-Order friendship
template< typename T >
class AccessKey
{
        friend T; // only T can construct keys
private:
        AccessKey() {} // default constructor is private
};

class Repairman;

class Elevator
{
    bool _broken;
public:
    
    Elevator()
    :   _broken(false)
    {
    }
    
    void setBroken()
    {
        _broken = true;
    }

    void setRepaired(AccessKey<Repairman>)
    {
        _broken = false;
        std::cout << "Elavator is repaired" << std::endl;
    }

};

class Repairman
{
public:
    void fix(Elevator& e)
    {
        e.setRepaired(AccessKey<Repairman>());
    }
};

void testAccessKey()
{
        Elevator e;
        e.setBroken();
        Repairman r;
        r.fix(e);
}
