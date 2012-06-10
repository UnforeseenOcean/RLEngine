#pragma once
// http://svn.boost.org/svn/boost/trunk/boost/assert.hpp

/*#define WIDEN2(x) L##x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#define __WFUNCTION__ WIDEN(__FUNCTION__)*/

//#define DISABLE_ASSERTS
#define USE_CUSTOM_ASSERTS

#ifdef DISABLE_ASSERTS

	#define RL_ASSERT(_EXPR, _MSG) ((void)0)

#else //DISABLE_ASSERTS

	#ifdef USE_CUSTOM_ASSERTS

		//#include <iostream>
		__forceinline void assertion_failed(char const* expr, const char* func, const char* file, const int line) {
			//std::cout << "Assertion (" << expr << ") failed in " << func << " : " << file << " (" << line << ')' << std::endl; //todo: eventually switch this to the customized printf.
			#ifdef _DEBUG
				__debugbreak();
			#endif //_DEBUG
		}
		#define RL_ASSERT(_EXPR, _MSG) ((_EXPR) ? ((void)0) : assertion_failed(_MSG, __FUNCTION__, __FILE__, __LINE__))

	#else

		#include <assert.h>
		#define RL_ASSERT(_EXPR, _MSG) assert(_EXPR)

	#endif //USE_CUSTOM_ASSERTS

#endif //DISABLE_ASSERTS