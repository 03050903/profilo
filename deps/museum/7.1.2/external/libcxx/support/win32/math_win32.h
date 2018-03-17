// -*- C++ -*-
//===---------------------- support/win32/math_win32.h --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef _MUSEUM_LIBCPP_SUPPORT_WIN32_MATH_WIN32_H
#define _MUSEUM_LIBCPP_SUPPORT_WIN32_MATH_WIN32_H

#if !defined(_MUSEUM_LIBCPP_MSVCRT)
#error "This header complements Microsoft's C Runtime library, and should not be included otherwise."
#else

#include <museum/7.1.2/bionic/libm/math.h>
#include <float.h> // _FPCLASS_PN etc.
#include <crtversion.h>

#if ((_VC_CRT_MAJOR_VERSION-0) < 12)
// Necessary?
typedef float float_t;
typedef double double_t;

_MUSEUM_LIBCPP_ALWAYS_INLINE bool isfinite( double num )
{
    return _finite(num) != 0;
}
_MUSEUM_LIBCPP_ALWAYS_INLINE bool isinf( double num )
{
    return !isfinite(num) && !_isnan(num);
}
_MUSEUM_LIBCPP_ALWAYS_INLINE bool isnan( double num )
{
    return _isnan(num) != 0;
}
_MUSEUM_LIBCPP_ALWAYS_INLINE bool isnormal( double num )
{
    int class_ = _fpclass(num);
    return class_ == _FPCLASS_NN || class_ == _FPCLASS_PN;
}

_MUSEUM_LIBCPP_ALWAYS_INLINE bool isgreater( double x, double y )
{
    if(_fpclass(x) == _FPCLASS_SNAN || _fpclass(y) == _FPCLASS_SNAN) return false;
    else return x > y;
}

_MUSEUM_LIBCPP_ALWAYS_INLINE bool isgreaterequal( double x, double y )
{
    if(_fpclass(x) == _FPCLASS_SNAN || _fpclass(y) == _FPCLASS_SNAN) return false;
    else return x >= y;
}

_MUSEUM_LIBCPP_ALWAYS_INLINE bool isless( double x, double y )
{
    if(_fpclass(x) == _FPCLASS_SNAN || _fpclass(y) == _FPCLASS_SNAN) return false;
    else return x < y;
}

_MUSEUM_LIBCPP_ALWAYS_INLINE bool islessequal( double x, double y )
{
    if(::_fpclass(x) == _FPCLASS_SNAN || ::_fpclass(y) == _FPCLASS_SNAN) return false;
    else return x <= y;
}

_MUSEUM_LIBCPP_ALWAYS_INLINE bool islessgreater( double x, double y )
{
    if(::_fpclass(x) == _FPCLASS_SNAN || ::_fpclass(y) == _FPCLASS_SNAN) return false;
    else return x < y || x > y;
}

_MUSEUM_LIBCPP_ALWAYS_INLINE bool isunordered( double x, double y )
{
    return isnan(x) || isnan(y);
}
_MUSEUM_LIBCPP_ALWAYS_INLINE bool signbit( double num )
{
    switch(_fpclass(num))
    {
        case _FPCLASS_SNAN:
        case _FPCLASS_QNAN:
        case _FPCLASS_NINF:
        case _FPCLASS_NN:
        case _FPCLASS_ND:
        case _FPCLASS_NZ:
            return true;
        case _FPCLASS_PZ:
        case _FPCLASS_PD:
        case _FPCLASS_PN:
        case _FPCLASS_PINF:
            return false;
    }
    return false;
}
_MUSEUM_LIBCPP_ALWAYS_INLINE float copysignf( float x, float y )
{
    return (signbit (x) != signbit (y) ? - x : x);
}
_MUSEUM_LIBCPP_ALWAYS_INLINE double copysign( double x, double y )
{
    return ::_copysign(x,y);
}
_MUSEUM_LIBCPP_ALWAYS_INLINE double copysignl( long double x, long double y )
{
    return ::_copysignl(x,y);
}
_MUSEUM_LIBCPP_ALWAYS_INLINE int fpclassify( double num )
{
    return _fpclass(num);
}
#endif
#endif // _MUSEUM_LIBCPP_MSVCRT

#endif // _MUSEUM_LIBCPP_SUPPORT_WIN32_MATH_WIN32_H
