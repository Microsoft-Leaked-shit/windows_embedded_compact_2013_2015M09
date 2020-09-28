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
/***
*strsafea.c
*
*Purpose:
*   This module defines safer C library routine
*   replacements.  These are meant to make C a bit
*   more safe in reference to security and robustness
*
*   Note: *sprintf replacements are split into
*   strsafea_printf.c to remove an artificial dependency
*   and align with SYSGEN settings
*
*******************************************************************************/
#define STRSAFE_LIB_IMPL
#define STRSAFE_NO_W_FUNCTIONS
#define STRSAFE_NO_SPRINTF_FUNCTIONS
#include <strsafe.h>
