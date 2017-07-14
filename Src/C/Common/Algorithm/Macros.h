#ifndef MACROS_H_
#define MACROS_H_

#ifdef __cplusplus
extern "C" {
#endif
	
    #define MACRO_CONS(x, y) MACRO_CONS_(x, y)
    #define MACRO_CONS_(x, y) x##y
    
    #define LIST_MAP(list, proc)  do{                                 \
        for(int list_iter_ = 0; list_iter_ < GET_ARRAY_SIZE(list); list_iter_++){        \
            proc(&(list[list_iter_]));                                      \
        }                                                              \
    }   while(0)
    
	#define GET_ARRAY_SIZE(a) 				(sizeof(a) / sizeof(a[0]))
		
	#define BIT_SET(a, n)					a = a | (((uint32_t)1) << n)
	#define BIT_RESET(a, n) 				a = a & ((((uint32_t)1) << n) ^ 0xFFFFFFFF)
	#define BIT_REVERSE(a, n) 				a = a ^ (((uint32_t)1) << n)
			
	#define MEM_NEW_OBJ(T, n) 				(T*)malloc(sizeof(T) * n)
		
	#define MEMBER_START_OFST(obj, m) 		((uint32_t)(&obj.m) - (uint32_t)(&obj))
	#define MEMBER_END_OFST(obj, m)			((uint32_t)(&obj.m) - (uint32_t)(&obj) + sizeof(obj.m))
	
#ifdef __cplusplus
}
#endif

#endif //MACROS_H_
