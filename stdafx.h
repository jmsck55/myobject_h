
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// On Microsoft, uncomment this line:
//#include "targetver.h"

// Use this macro on 64-bit systems: "BITS_64", and comment out "BITS_128"
// Comment out both "BITS_64" and "BITS_128" on 32-bit systems
// To use 128-bit on gcc, use "BITS_128", and comment out "BITS_64" (may be buggy)

#define BITS_64
//#define BITS_128

#ifdef BITS_128
#define BITS 128
// For "gcc" 128-bit mode:
#include <quadmath.h>
#else
#ifdef BITS_64
#define BITS 64
#else
#define BITS 32
#endif // BITS_64
#endif // BITS_128

// C headers:
#include <stdio.h>
//#include <tchar.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stddef.h>

// C++ headers:
#include <new>
//#include <initializer_list>
#include <list>
#include <forward_list>
//#define __USE_DEQUE__
//#ifdef __USE_DEQUE__
//#include <deque> // used optionally, it can do: "repeat", "append", or "prepend"
//#endif // __USE_DEQUE__
// For Input and Output routines:
#include <iostream>
#include <sstream>
#include <string>

// TODO: reference additional headers your program requires here
