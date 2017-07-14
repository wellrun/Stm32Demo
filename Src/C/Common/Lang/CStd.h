#ifndef C_STD_H_
#define C_STD_H_

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <setjmp.h>
#include <stdint.h>

#ifdef C_PLATFORM_STM32
    #include <arm_math.h>
#endif //C_PLATFORM_STM32

#if defined(C_PLATFORM_LINUX) || defined(C_PLATFORM_ANDROID)
    #include <unistd.h>
#endif

#endif //C_STD_H_
