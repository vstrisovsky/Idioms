// http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Scope_Guard

#include "idioms.h"

#include <utility>
#include <iostream>

template <class Fun>
class ScopeGuard 
{
        Fun f_;
	bool active_;
public:
	ScopeGuard(Fun f)
		: f_(std::move(f))
		, active_(true)
                {
	}
	~ScopeGuard()
        {
            if (active_) f_();
        }
	void dismiss()
        {
            active_ = false;
        }
	ScopeGuard() = delete;
	ScopeGuard(const ScopeGuard&) = delete;
	ScopeGuard& operator=(const ScopeGuard&) = delete;
	ScopeGuard(ScopeGuard&& rhs)
		: f_(std::move(rhs.f_))
		, active_(rhs.active_)
        {
            rhs.dismiss();
	}
};
 
namespace detail
{
	enum class ScopeGuardOnExit
        {
        };
	template <typename Fun>
	ScopeGuard<Fun> operator+(ScopeGuardOnExit, Fun&& fn)
        {
                return ScopeGuard<Fun>(std::forward<Fun>(fn));
	}
}
 
#define SCOPE_EXIT \
	auto ANONYMOUS_VARIABLE(SCOPE_EXIT_STATE) \
	= ::detail::ScopeGuardOnExit() + [&]()
 
#define SCOPE_EXIT_NAMED(NAME) \
	auto NAME \
	= ::detail::ScopeGuardOnExit() + [&]()

#define CONCATENATE_IMPL(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_IMPL(s1, s2)
#ifdef __COUNTER__
#define ANONYMOUS_VARIABLE(str) \
	CONCATENATE(str, __COUNTER__)
#else
#define ANONYMOUS_VARIABLE(str) \
	CONCATENATE(str, __LINE__)
#endif


void testScopeGuard11()
{
    auto buf = malloc(1024);
    SCOPE_EXIT{ free(buf); std::cout << "called free(buff);" << std::endl;};
    {
        auto buf2 = malloc(1024);
        SCOPE_EXIT_NAMED(buffer){ free(buf2); std::cout << "called free(buff2);" << std::endl;};
        buffer.dismiss();
    }
}
