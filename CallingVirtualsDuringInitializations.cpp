// http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Calling_Virtuals_During_Initialization

#include "idioms.h"

#include <memory>
#include <iostream>

/*

class Base 
{
public:
Base();
...
virtual void foo(int n) const; // often pure virtual
virtual double bar() const;    // often pure virtual
};

Base::Base()
{
    foo(3);
    bar();
// these will not use dynamic binding
// goal: simulate dynamic binding in those calls
}

class Derived : public Base
{
public:
...
virtual void foo(int n) const;
virtual double bar() const;
};

*/

namespace Solution_one
{

class Base
{
public:
        void init();  // may or may not be virtual
//...
        virtual void foo(int n) const {}; // often pure virtual
        virtual double bar() const {return 1.0;};    // often pure virtual
};

void Base::init()
{
    foo(3);
    bar();
}

class Derived : public Base
{
public:
    Derived (const char *)
    {
        std::cout << "1. Derived::Derived()\n";
    }
    virtual void foo(int n) const
    {
        std::cout << "1. Derived::foo()\n";
    }
    virtual double bar() const
    {
        std::cout << "1. Derived::bar()\n";
        return 4.0;
    }
};

template <class Derived, class Parameter>
std::unique_ptr<Base> factory (Parameter p)
{
    std::unique_ptr <Base> ptr (new Derived (p));
    ptr->init (); 
    return ptr;
}

}

namespace Solution_two
{

class Base
{
public:
    void init();  // may or may not be virtual
//...
    virtual void foo(int n) const {}; // often pure virtual
    virtual double bar() const {return 3.0;}    // often pure virtual
        
    template <class D, class Parameter>
    static std::unique_ptr<Base> Create (Parameter p)
    {
       std::unique_ptr <Base> ptr (new D (p));       
       ptr->init (); 
       return ptr;
    }
        
};

void Base::init()
{
    foo(3);
    bar();
}

class Derived : public Base
{
public:
    Derived (const char *)
    {
            std::cout << "2. Derived::Derived()\n";
    }

    virtual void foo(int n) const
    {
        std::cout << "2. Derived::foo()\n";
    }

    virtual double bar() const
    {
        std::cout << "2. Derived::bar()\n";
        return 4.0;
    }
};

}

namespace Solution_three
{

class Base 
{
};

template <class D>
class InitFooBar
        : public Base 
{
  protected:
    InitFooBar ()
    {
       static_cast<D*> (this)->foo(3);
       static_cast<D*> (this)->bar();
    }
};

class Derived : public InitFooBar <Derived> 
{
public:
    Derived () : InitFooBar <Derived> ()
    {
        std::cout << "3. Derived::Derived()\n";
    }
        
    void foo (int n)
    {
        std::cout << "3. Derived::foo()\n";
    }
    
    double bar () const
    {
        std::cout << "3. Derived::bar()\n";
        return 4.0;
    }
};

}

void testCVDI()
{
    auto ptrOne = Solution_one::factory<Solution_one::Derived>("One");

    auto ptrTwo = Solution_two::Base::Create<Solution_two::Derived>("Two");
    
    Solution_three::Derived d;
}