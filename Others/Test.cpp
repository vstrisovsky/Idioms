#include "../idioms.h"
#include <iostream>
#include <future>
#include <unistd.h> 
#include <functional>

struct Message
{
    template<typename _F>
    Message(_F f)
    : fun(std::move(f))
    {}
    std::function<void()> fun;
};

template<class Name>
class Task
{
public:
    template<class Fun>
    void execute(Fun f) const
    {
        auto a = std::async(std::launch::async, f);
        sleep(2);
    }
};

template <class T, class Fun>
class MessageQueueCall
{
public: 
    	MessageQueueCall(T& t, Fun&& f)
        {
            t.execute(f);
	}
};

namespace detail
{
        template<class  T>
      	struct _MessageQueueCall
        {
            T& task_;
            
            _MessageQueueCall(T& t)
            : task_(t)
            {
            }
        };

        template<class  T>
      	 _MessageQueueCall<T> _MakeCall(T& t)
        {
            return _MessageQueueCall<T>(t);
        }
        
	template <typename T, typename Fun>
	MessageQueueCall<T, Fun> operator+(_MessageQueueCall<T>&& t, Fun&& fn)
        {
                return MessageQueueCall<T, Fun>(t.task_, std::forward<Fun>(fn));
	}
}
 
#define ASYNC_CALL(TASK) \
	auto ANONYMOUS_VARIABLE(SCOPE_EXIT_STATE) \
	= ::detail::_MakeCall(TASK) + [&]()

#define CONCATENATE_IMPL(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_IMPL(s1, s2)
#ifdef __COUNTER__
#define ANONYMOUS_VARIABLE(str) \
	CONCATENATE(str, __COUNTER__)
#else
#define ANONYMOUS_VARIABLE(str) \
	CONCATENATE(str, __LINE__)
#endif

class IService
{
public:
    virtual void foo(int a) const = 0;
};

class Service : public IService
{
public:
    virtual void foo(int a) const
    {
        std::cout << "Service foo" << std::endl;
    }
};

class ServiceTask : public Task<ServiceTask>, public IService
{
    Service m_impl;
public:
    virtual void foo(int a) const
    {
        std::cout << "Async foo ";
        ASYNC_CALL(*this)
        {
            std::cout << " -> ";
            m_impl.foo(a);
        };
    }
};

void test()
{
    Message m([](){});
    Message x(m);
    
    
    ServiceTask task;
    IService* intf = &task;
    intf->foo(5);
    std::cout << " wait 5 seconds " << std::endl;
    sleep(5);
    std::cout << " end " << std::endl;
}
