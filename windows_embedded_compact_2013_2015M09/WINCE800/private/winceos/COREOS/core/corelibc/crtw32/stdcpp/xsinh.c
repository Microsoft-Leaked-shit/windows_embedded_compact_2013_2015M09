//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft shared
// source or premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license agreement,
// you are not authorized to use this source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the SOURCE.RTF on your install media or the root of your tools installation.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
/* _Sinh function */
#include "xmath.h"
_C_STD_BEGIN

/* coefficients */
#define NP	(sizeof (p) / sizeof (p[0]) - 1)
static const double p[] = {	/* courtesy Dr. Tim Prince */
	0.0000000001632881,
	0.0000000250483893,
	0.0000027557344615,
	0.0001984126975233,
	0.0083333333334816,
	0.1666666666666574,
	1.0000000000000001};

_CRTIMP2_PURE double __CLRCALL_PURE_OR_CDECL _Sinh(double x, double y)
	{	/* compute y*sinh(x), |y| <= 1 */
	short neg;

	switch (_Dtest(&x))
		{	/* test for special codes */
	case _NANCODE:
		return (x);
	case _INFCODE:
		return (y != 0.0 ? x : DSIGN(x) ? -y : y);
	case 0:
		return (x * y);
	default:	/* finite */
		if (y == 0.0)
			return (x < 0.0 ? -y : y);
		if (x < 0.0)
			x = -x, neg = 1;
		else
			neg = 0;

		if (x < _Rteps._Double)
			x *= y;	/* x tiny */
		else if (x < 1.0)
			{
			double w = x * x;

			x += x * w * _Poly(w, p, NP - 1);
			x *= y;
			}
		else if (x < _Xbig)
			{	/* worth adding in exp(-x) */
			_Exp(&x, 1.0, -1);
			x = y * (x - 0.25 / x);
			}
		else
			switch (_Exp(&x, y, -1))
				{	/* report over/underflow */
			case 0:
				_Feraise(_FE_UNDERFLOW);
				break;
			case _INFCODE:
				_Feraise(_FE_OVERFLOW);
				}
		return (neg ? -x : x);
		}
	}
_C_STD_END

/*
 * Copyright (c) 1992-2007 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V5.03:0009 */
