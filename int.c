

/* int.c */

#include "int.h"

const double MAX_INT_DOUBLE = 9007199254740991.0 ; /* 2^53-1 */
const double MIN_INT_DOUBLE = -9007199254740991.0 ; /* 2^53-1 */

#if  LONG64
const int have_long64 = 1 ;
#else 
const int have_long64 = 0 ;
#endif 


const double INT64_MAX_DOUBLE = 9223372036854775807.000000 ; /* 2^63 -1 */
const double INT64_MIN_DOUBLE = -9223372036854775808.000000 ; /* -2^63 */
const double UINT64_MAX_DOUBLE = 18446744073709551615.000000 ; /* 2^64 - 1 */


int64_t d_to_i64(double d)
{
    if (d >= INT64_MAX_DOUBLE) return INT64_MAX ;
    if (d >  INT64_MIN_DOUBLE) return (int64_t) d ;
    return INT64_MIN ;
}

uint64_t d_to_u64(double d)
{
    if (d >= UINT64_MAX_DOUBLE) return UINT64_MAX ;
    if (d >= 0.0) return (uint64_t) d ;
    {
        int64_t x = INT64_MIN ;
        if (d > INT64_MIN_DOUBLE) x = (int64_t) d ;
	return (uint64_t) x ;
    }
}


int d_to_int(double d)
{
    if (d >= 2147483647.0) return 2147483647 ;
    if (d > -2147483648.0) return (int) d ;
    return  -2147483648 ;
}
