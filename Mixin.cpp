// http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Curiously_Recurring_Template_Pattern

#include "idioms.h"

#include <iostream>

struct Timer
{
    Timer()
    {
    }
    
    void Reset()
    {
    }
    
    double GetElapsedTimeSecs()
    {
        return 0.0;
    }
};

struct ITask
{
    virtual ~ITask() = 0;
    virtual std::string GetName() = 0;
    virtual void Execute() = 0;
};

template< class T >
class LoggingTask : public T
{
public:
    void Execute()
    {
        std::cout << "LOG: The task is starting - " << this->GetName().c_str() << std::endl;
        T::Execute();
        std::cout << "LOG: The task has completed - " << this->GetName().c_str() << std::endl;
    }
};
 
template< class T >
class TimingTask : public T
{
    Timer timer_;
public:
    void Execute()
    {
        timer_.Reset();
        T::Execute();
        double t = timer_.GetElapsedTimeSecs();
        std::cout << "Task Duration: " << t << " seconds" << std::endl;
    }
};
 
class MyTask
{
public:
    void Execute()
    {
        std::cout << "...This is where the task is executed..." << std::endl;
    }
 
    std::string GetName()
    {
        return "My task name";
    }
};

template< class T >
class TaskAdapter 
        : public ITask
        , public T
{
public:
    virtual void Execute()
    {
        T::Execute();
    }
 
    virtual std::string GetName()
    {
        return T::GetName();
    }
};
 
void testMixin()
{
    // A plain old MyTask
    std::cout << "A plain old MyTask:" << std::endl;
    MyTask t1;
    t1.Execute();

    // A MyTask with added timing:
    std::cout << "A MyTask with added timing:" << std::endl;
    TimingTask< MyTask > t2;
    t2.Execute();

    // A MyTask with added logging and timing:
    std::cout << "A MyTask with added logging and timing:" << std::endl;
    LoggingTask< TimingTask< MyTask > > t3;
    t3.Execute();

    // A MyTask with added logging and timing written to look a bit like the composition example
    std::cout << "A MyTask with added logging and timing written to look a bit like the composition example:" << std::endl;
    typedef LoggingTask< 
                TimingTask< 
                    MyTask > > Task;
    Task t4;
    t4.Execute();
}