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
////////////////////////////////////////////////////////////////////////////////
//
//  TUXTEST TUX DLL
//  Copyright (c) Microsoft Corporation
//
//  Module: TAMIL.h
//          
//
//  Revision History:changed #define values:
//
////////////////////////////////////////////////////////////////////////////////

#include "..\kbddef.h"

// ****************************************************
// Tamil keyboard
//
// Tamil and other Indic keyboards can compose multiple
// keystrokes into composed characters so be careful!
//
//  Num  VK1   VK2    Num   Unicode
testSequence KBDTests_Tamil[] = 
{
    // Without VK_SHIFT
    {1, {0x20, 0x00}, 1,    0x0020}, // Space key
    //  {1, {0x1B, 0x00}, 1,    0x001B}, // Esc key
    {1, {0x08, 0x00}, 1,    0x0008}, // BackSpace key
    {1, {0x0D, 0x00}, 1,    0x000D}, // Enter key
    {1, {0x09, 0x00}, 1,    0x0009}, // Tab key
    /* Keys 1234567890-= */
    {1, {0x31, 0x00}, 1,    0x0031},      
    {1, {0x32, 0x00}, 1,    0x0032},      
    {1, {0x33, 0x00}, 1,    0x0033},      
    {1, {0x34, 0x00}, 1,    0x0034},      
    {1, {0x35, 0x00}, 1,    0x0035},      
    {1, {0x36, 0x00}, 1,    0x0036},      
    {1, {0x37, 0x00}, 1,    0x0037},      
    {1, {0x38, 0x00}, 1,    0x0038},      
    {1, {0x39, 0x00}, 1,    0X0039},      
    {1, {0x30, 0x00}, 1,    0x0030},      
    {1, {0xBD, 0x00}, 1,    0x002D},      
    {1, {0xBB, 0x00}, 0,    0x0000},      
    /* Keys qwertyuiop[]\ */
    {1, {0x51, 0x00}, 1,    0x0BCC},            
    {1, {0x57, 0x00}, 1,    0x0BC8},            
    {1, {0x45, 0x00}, 1,    0x0BBE},            
    {1, {0x52, 0x00}, 1,    0x0BC0},            
    {1, {0x54, 0x00}, 1,    0x0BC2},            
    {1, {0x59, 0x00}, 1,    0x0BAA},            
    {1, {0x55, 0x00}, 1,    0x0BB9},            
    {1, {0x49, 0x00}, 1,    0x0B95},            
    {1, {0x4F, 0x00}, 1,    0x0BA4},            
    {1, {0x50, 0x00}, 1,    0x0B9C},            
    {1, {0xDB, 0x00}, 1,    0x0B9F},            
    {1, {0xDD, 0x00}, 1,    0x0B9E},            
    {1, {0xDC, 0x00}, 0,    0x0000},            
    /* Keys asdfghjkl;'~ */
    {1, {0x41, 0x00}, 1,    0x0BCB},            
    {1, {0x53, 0x00}, 1,    0x0BC7},            
    {1, {0x44, 0x00}, 1,    0x0BCD},            
    {1, {0x46, 0x00}, 1,    0x0BBF},            
    {1, {0x47, 0x00}, 1,    0x0BC1},            
    {1, {0x48, 0x00}, 1,    0x0BAA},            
    {1, {0x4A, 0x00}, 1,    0x0BB0},            
    {1, {0x4B, 0x00}, 1,    0x0B95},            
    {1, {0x4C, 0x00}, 1,    0x0BA4},            
    {1, {0xBA, 0x00}, 1,    0x0B9A},      
    {1, {0xDE, 0x00}, 1,    0x0B9F},
    {1, {0xC0, 0x00}, 1,    0x0BCA},
    /* Keys zcvbnm,./ */
    {1, {0x5A, 0x00}, 1,    0x0bc6},
    {1, {0x43, 0x00}, 1,    0x0BAE},            
    {1, {0x56, 0x00}, 1,    0x0BA8},
    {1, {0x42, 0x00}, 1,    0x0BB5},
    {1, {0x4E, 0x00}, 1,    0x0BB2},            
    {1, {0x4D, 0x00}, 1,    0x0BB8},            
    {1, {0xBC, 0x00}, 1,    0x002C},      
    {1, {0xBE, 0x00}, 1,    0x002E},      
    {1, {0xBF, 0x00}, 1,    0x0BAF},      
    // With VK_SHIFT
    /* Keys 1290-= */
    {2, {0x10, 0x31}, 0,    0x0000},      
    {2, {0x10, 0x32}, 0,    0x0000},
    {2, {0x10, 0x33}, 0,    0x0000},
    {2, {0x10, 0x34}, 0,    0x0000},
    {2, {0x10, 0x35}, 0,    0x0000},
    {2, {0x10, 0x39}, 0,    0x0000},     
    {2, {0x10, 0x30}, 0,    0x0000},      
    {2, {0x10, 0xBD}, 1,    0x0B83},     
    {2, {0x10, 0xBB}, 0,    0x0000},
    /* Keys qwertyuiop[]\ */
    {2, {0x10, 0x51}, 1,    0x0B94},            
    {2, {0x10, 0x57}, 1,    0x0B90},            
    {2, {0x10, 0x45}, 1,    0x0B86},        
    {2, {0x10, 0x52}, 1,    0x0B88},            
    {2, {0x10, 0x54}, 1,    0x0B8A},
    {2, {0x10, 0x59}, 1,    0x0BAA},            
    {2, {0x10, 0x55}, 1,    0x0B99},            
    {2, {0x10, 0x49}, 1,    0x0B95},            
    {2, {0x10, 0x4F}, 1,    0x0BA4},            
    {2, {0x10, 0x50}, 1,    0x0B9A},            
    {2, {0x10, 0xDB}, 1,    0x0B9F},            
    {2, {0x10, 0xDD}, 0,    0x0000},            
    /* Keys asdfghjkl;' */
    {2, {0x10, 0x41}, 1,    0x0B93},            
    {2, {0x10, 0x53}, 1,    0x0B8F},            
    {2, {0x10, 0x44}, 1,    0x0B85},            
    {2, {0x10, 0x46}, 1,    0x0B87},            
    {2, {0x10, 0x47}, 1,    0x0B89},
    {2, {0x10, 0x48}, 1,    0x0BAA},            
    {2, {0x10, 0x4A}, 1,    0x0BB1},            
    {2, {0x10, 0x4B}, 1,    0x0B95},            
    {2, {0x10, 0x4C}, 1,    0x0BA4},            
    {2, {0x10, 0xBA}, 1,    0x0B9A},    
    {2, {0x10, 0xDE}, 1,    0x0B9F},
    {2, {0x10, 0xC0}, 1,    0x0B92},
    /* Keys zcvbnm,/ */          
    {2, {0x10, 0x5A}, 1,    0x0B8E},            
    {2, {0x10, 0x43}, 1,    0x0BA3},                        
    {2, {0x10, 0x56}, 1,    0x0BA9},
    {2, {0x10, 0x42}, 1,    0x0BB4},
    {2, {0x10, 0x4E}, 1,    0x0BB3},
    {2, {0x10, 0x4D}, 1,    0x0BB7},            
    {2, {0x10, 0xBC}, 1,    0x0BB7},     
    {2, {0x10, 0xBF}, 1,    0x0BAF},
    // Control chars
    // Row ZXCV
    {2, {0x11, 90}, 1, 0x1A},           // Ctrl Z
    {2, {0x11, 88}, 1, 0x18},           // Ctrl X
    {2, {0x11, 67}, 1, 0x03},           // Ctrl C
    {2, {0x11, 86}, 1, 0x16},           // Ctrl V
    {2, {0x11, 66}, 1, 0x02},           // Ctrl B
    {2, {0x11, 78}, 1, 0x0E},           // Ctrl N
    {2, {0x11, 77}, 1, 0x0D},           // Ctrl M
    //Row ASDF
    {2, {0x11, 65}, 1, 0x01},           // Ctrl A
    {2, {0x11, 83}, 1, 0x13},           // Ctrl S
    {2, {0x11, 68}, 1, 0x04},           // Ctrl D
    {2, {0x11, 70}, 1, 0x06},           // Ctrl F
    {2, {0x11, 71}, 1, 0x07},           // Ctrl G
    {2, {0x11, 72}, 1, 0x08},           // Ctrl H
    {2, {0x11, 74}, 1, 0x0A},           // Ctrl J
    {2, {0x11, 75}, 1, 0x0B},           // Ctrl K
    {2, {0x11, 76}, 1, 0x0C},           // Ctrl L
    // Row QWER
    {2, {0x11, 81}, 1, 0x11},           // Ctrl Q
    {2, {0x11, 87}, 1, 0x17},           // Ctrl W
    {2, {0x11, 69}, 1, 0x05},           // Ctrl E
    {2, {0x11, 82}, 1, 0x12},           // Ctrl R
    {2, {0x11, 84}, 1, 0x14},           // Ctrl T
    {2, {0x11, 89}, 1, 0x19},           // Ctrl Y
    {2, {0x11, 85}, 1, 0x15},           // Ctrl U
    {2, {0x11, 73}, 1, 0x09},           // Ctrl I
    {2, {0x11, 79}, 1, 0x0F},           // Ctrl O
    {2, {0x11, 80}, 1, 0x10},           // Ctrl P
    // With ALT-Gr key (VK_CONTROL, VK_MENU)
    /*keys `1234567890-= */
    {3, {0x0011, 0x0012, 0x00C0}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x0031}, 1, 0x0BE7},
    {3, {0x0011, 0x0012, 0x0032}, 1, 0x0BE8},
    {3, {0x0011, 0x0012, 0x0033}, 1, 0x0BE9},
    {3, {0x0011, 0x0012, 0x0034}, 1, 0x0BEA},
    {3, {0x0011, 0x0012, 0x0035}, 1, 0x0BEB},
    {3, {0x0011, 0x0012, 0x0036}, 1, 0x0BEC},
    {3, {0x0011, 0x0012, 0x0037}, 1, 0x0BED},
    {3, {0x0011, 0x0012, 0x0038}, 1, 0x0BEE},
    {3, {0x0011, 0x0012, 0x0039}, 1, 0x0BEF},
    {3, {0x0011, 0x0012, 0x0030}, 1, 0x0BF0},
    {3, {0x0011, 0x0012, 0x00BD}, 1, 0x0BF1},
    {3, {0x0011, 0x0012, 0x00BB}, 1, 0x0BF2},
    /* Keys qwertyuiop[]\ */
    {3, {0x0011, 0x0012, 0x0051}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x0057}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x0045}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x0052}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x0054}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x0059}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x0055}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x0049}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x004F}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x0050}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x00DB}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x00DD}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x00DC}, 0, 0x0000},
    /* Keys asdfghjkl;' */
    {3, {0x0011, 0x0012, 0x0041}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x0053}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x0044}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x0046}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x0047}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x0048}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x004A}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x004B}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x004C}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x00BA}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x00DE}, 0, 0x0000},
    /* Keys zxcvbnm,./ */
    {3, {0x0011, 0x0012, 0x005A}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x0058}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x0043}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x0056}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x0042}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x004E}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x004D}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x00BC}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x00BE}, 0, 0x0000},
    {3, {0x0011, 0x0012, 0x00BF}, 0, 0x0000},
    // With ALT-Gr + VK_SHIFT
    /*keys `1234567890-= */
    {4, {0x0010, 0x0011, 0x0012, 0x00C0}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0031}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0032}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0033}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0034}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0035}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0036}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0037}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0038}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0039}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0030}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x00BD}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x00BB}, 0, 0x0000},
    /* Keys qwertyuiop[]\ */
    {4, {0x0010, 0x0011, 0x0012, 0x0051}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0057}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0045}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0052}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0054}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0059}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0055}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0049}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x004F}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0050}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x00DB}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x00DD}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x00DC}, 0, 0x0000},
    /* Keys asdfghjkl;' */
    {4, {0x0010, 0x0011, 0x0012, 0x0041}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0053}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0044}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0046}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0047}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0048}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x004A}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x004B}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x004C}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x00BA}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x00DE}, 0, 0x0000},
    /* Keys zxcvbnm,/ */
    {4, {0x0010, 0x0011, 0x0012, 0x005A}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0058}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0043}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0056}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x0042}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x004E}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x004D}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x00BC}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x00BE}, 0, 0x0000},
    {4, {0x0010, 0x0011, 0x0012, 0x00BF}, 0, 0x0000},
    // Keystrokes that generate multiple chars
    // With VK_SHIFT
    /*Keys 345678 */
    {2, {0x10, 0x36}, 3, {0x0BA4, 0x0BCD, 0x0BB0}},     
    {2, {0x10, 0x37}, 3, {0x0B95, 0x0BCD, 0x0BB7}},     
    {2, {0x10, 0x38}, 3, {0x0BB7, 0x0BCD, 0x0BB0}}, 
    {2, {0x10, 0xBE}, 4, {0x0BB8, 0x0BCD, 0x0BB0, 0x0BC0}}, 
}; // end Tamil keyboard
