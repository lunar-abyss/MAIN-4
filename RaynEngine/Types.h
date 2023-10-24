#pragma once

//// BOOL.h

// data type, that can be only true or false
typedef _Bool bool;

#define true 1
#define null 0


//// UINTS.h

// 8-bit unsigned integer
typedef unsigned char      u8;

// 16-bit unsigned integer
typedef unsigned short     u16;

// 32-bit unsigned integer
typedef unsigned long      u32;

// 64-bit unsigned integer
typedef unsigned long long u64;


//// INTS.h

// 8-bit signed integer
typedef signed char      i8;

// 16-bit unsigned integer
typedef signed short     i16;

// 32-bit unsigned integer
typedef signed long      i32;

// 64-bit unsigned integer
typedef signed long long i64;


// FLOATS.h

// 32-bit float
typedef float  f32;

// 64-bit float
typedef double f64;


//// OTHER.h

// function type
typedef void fn;

// no value or void pointer
#define nova ((void*)0)

// skip block
#define pass { }