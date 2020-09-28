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
/* _Dunscale function -- IEEE 754 version */
#include "xmath.h"
_C_STD_BEGIN
 #if !defined(MRTDLL)
_C_LIB_DECL
 #endif /* defined(MRTDLL) */
_CRTIMP2_PURE short __CLRCALL_PURE_OR_CDECL _Dunscale(short *pex, double *px)
	{	/* separate *px to 1/2 <= |frac| < 1 and 2^*pex */
	_Dval *ps = (_Dval *)(char *)px;
	short xchar = (ps->_Sh[_D0] & _DMASK) >> _DOFF;

	if (xchar == _DMAX)
		{	/* NaN or INF */
		*pex = 0;
		return ((ps->_Sh[_D0] & _DFRAC) != 0 || ps->_Sh[_D1] != 0
			|| ps->_Sh[_D2] != 0 || ps->_Sh[_D3] != 0 ? _NANCODE : _INFCODE);
		}
	else if (0 < xchar || (xchar = _Dnorm(ps)) <= 0)
		{	/* finite, reduce to [1/2, 1) */
		ps->_Sh[_D0] = ps->_Sh[_D0] & ~_DMASK | _DBIAS << _DOFF;
		*pex = xchar - _DBIAS;
		return (_FINITE);
		}
	else
		{	/* zero */
		*pex = 0;
		return (0);
		}
	}
 #if !defined(MRTDLL)
_END_C_LIB_DECL
 #endif /* !defined(MRTDLL) */
_C_STD_END

/*
 * Copyright (c) 1992-2012 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V6.00:0009 */
