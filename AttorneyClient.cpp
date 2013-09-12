#include <iostream>

#include "idioms.h"

namespace idiom_attorney
{

class Elevator
{
    bool _broken;

    void setRepaired()
    {
        _broken = false;
        std::cout << "Elavator is repaired" << std::endl;
    }

public:
    friend class Repair;
    
    Elevator()
    :   _broken(false)
    {
    }

    void setBroken()
    {
        _broken = true;
    }

};

class Repair
{
    static void setRepaired(Elevator& e)
    {
        e.setRepaired();
    }
    friend class Repairman;
};

class Repairman
{
    Repair r;
public:
    void fix(Elevator& e)
    {
        r.setRepaired(e);
    }
};

};

void testAttorneyClient()
{
    using namespace idiom_attorney;
    
    Elevator elevator;
    elevator.setBroken();
    Repairman repairman;
    repairman.fix(elevator);
}
