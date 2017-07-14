#ifndef MATH_LIMIT_H_
#define MATH_LIMIT_H_

#ifdef __cplusplus
extern "C" {
#endif
	
	//A general-type and robust range limit function.
	//If the lower-bound and higher-bound is passed
	//reversely, the function will detect and correct it.	
	#define RangeLimit(x, l_bound, h_bound) do {    \
		if((h_bound) > (l_bound)) {                 \
			if((x) > (h_bound))                     \
				x = (h_bound);                      \
			else if(x < (l_bound))                  \
				x = (l_bound);                      \
		}                                           \
		else if((h_bound) < (l_bound)) {            \
			if((x) < (h_bound))                     \
				x = (h_bound);                      \
			else if((x) > (l_bound))                \
				x = (l_bound);                      \
		}                                           \
		else                                        \
			x = l_bound;                            \
	} while(0)
	
	//A general-type rate limited tracking function.
	//let x tracking target by given rate limit.	
	#define RateLimitTracking(x, target, rate_limit_up, rate_limit_down) do { \
		if(rate_limit_up < 0 || rate_limit_down > 0)							\
			break;                                                            \
		if(target == x)                                                       \
			break;                                                            \
		if((target - x) < rate_limit_up && target > x) {                      \
			x = target;                                                       \
			break;                                                            \
		}                                                                     \
		if((target - x) > rate_limit_down && target < x) {                    \
			x = target;                                                       \
			break;                                                            \
		}                                                                     \
		if(err > 0)                                                           \
			x += rate_limit_up;                                               \
		else                                                                  \
			x += rate_limit_down;                                             \
	} while(0)
	
	//Adjust x into -c/2 ~ +c/2. 	
	#define CycleRoundUpN(x, c) while(1){	 \
		if(c == 0)		\
			break;			\
		if(x > (c/2))               \
			x -= c;                  \
		else if(x < -(c/2))         \
			x += c;                  \
		else                         \
			break;  }      
			 
	//Adjust x into -c/2 ~ +c/2. 	
	#define CycleRoundUp1(x, c) do{	 \
		if(c == 0)		\
			break;			\
		if(x > (c/2))               \
			x -= c;                  \
		else if(x < -(c/2))         \
			x += c;                  \
		else                         \
			break;  }  while(0)
	
	#define RANGE(value, start, end) (																			\
		((value) >= (start))?																					\
		(																										\
			(  ( (value) - (start) ) % ( (end) - (start) )  ) + (start)											\
		):																										\
		(																										\
			(  (end) - (start) - ( (end) - (value) ) % ( (end) - (start) )  ) % ( (end) - (value) ) + (start)	\
		)																										\
	)
			
			
//	//suggest modify the cycle round method to those form:
//	#define CycleRoundUpN(x, c) RANGE(x, (-c/2), (c/2))
//	#define CycleRoundUp1(x, c) RANGE(x, (-c/2), (c/2))
			
			
	#define DeadZone(input, dead_zone) (									\
		((dead_zone) > 0.0)? 												\
		(																	\
			((input) < (dead_zone) && (input) > -(dead_zone)) ? 			\
			(0.0):															\
			(																\
				((input) > 0)? 												\
				((input) - (dead_zone)):									\
				((input) + (dead_zone))										\
			)																\
		):																	\
		(0.0)																\
	)
			
#ifdef __cplusplus
}
#endif

#endif
