#include <iostream>
#include "idioms.h"

template <class Derived>
struct Base
{

    void interface()
    {
        // ...
        static_cast<Derived*> (this)->implementation();
        // ...
    }

    static void static_interface()
    {
        // ...
        Derived::static_implementation();
        // ...
    }

    // The default implementation may be (if exists) or should be (otherwise) 
    // overriden by inheriting in derived classes (see below)
    void implementation()
    {
        std::cout << "Base::implementation" << std::endl;
    }
    static void static_implementation()
    {
        std::cout << "Base::static_implementation" << std::endl;
    }
};

// The Curiously Recurring Template Pattern (CRTP)

struct Derived_1 : Base<Derived_1>
{
    // This class uses base variant of implementation
    //void implementation();

    // ... and overrides static_implementation
    static void static_implementation()
    {
        std::cout << "Derived_1::static_implementation" << std::endl;
    }
};

struct Derived_2 : Base<Derived_2>
{
    // This class overrides implementation
    void implementation()
    {
        std::cout << "Derived_2::implementation" << std::endl;
    }

    // ... and uses base variant of static_implementation
    //static void static_implementation();
};

void testStaticPolymorphism()
{
    Derived_1 d1;
    Derived_2 d2;
    
    d1.interface();
    d1.static_interface();

    d2.interface();
    d2.static_interface();

}