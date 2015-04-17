#ifndef INCLUDED_MQF_ASSERT
#define INCLUDED_MQF_ASSERT
#include <cassert>

#define assert_equal(expr,value) assert( expr == value )
#define assert_bound(expr,lower,upper) assert( lower <= expr && expr <= upper )

#endif
