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
#include "pppauth.h"
#if EMBEDDED_ONLY
#include "l2tpcfg.h"
#endif
#include "common.h"

CmdLineParams       cmdParameters;
RASConnectionType   eRasConnection;

// This entry will need to be created as the first step. Later on,
// this entry will be used to connect and disconnect.
bool 
CreatePhoneBookEntry(
    IN DWORD dwAuthOptions, 
    IN DWORD dwEAPExtension, 
    OUT DWORD *dwErrorReceived
    )
{
    DWORD dwRasBufSize = sizeof(RASENTRY)*ENTRYBUFSIZE;
    LPRASENTRY lpRasEntry = NULL;
    RASDIALPARAMS DialParams = {0};
    DWORD dwErr = 0;
    BYTE DevConfigBuf[DEVBUFSIZE] = {0};
    DWORD dwDevConfigSize = 0;
    LPBYTE lpDevConfig = DevConfigBuf + DEVCONFIGOFFSET;
    LPVARSTRING lpVarString = (LPVARSTRING)&DevConfigBuf;

#if EMBEDDED_ONLY
    PL2TP_CONFIG_DATA pl2tpDevConfig = NULL;
    PBYTE lpbL2TPConfig = NULL;
    DWORD cbDevConfig = 0;
#endif   
    DWORD cbKey=(wcslen(cmdParameters.strPresharedKey))*sizeof(WCHAR);

    lpRasEntry = (LPRASENTRY)LocalAlloc(LPTR, ENTRYBUFSIZE);
    if(lpRasEntry == NULL)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("LocalAlloc() failed with %d"), GetLastError());
        return false;
    }

#if EMBEDDED_ONLY
    if (eRasConnection == RAS_VPN_L2TP)
    {
        pl2tpDevConfig = (PL2TP_CONFIG_DATA)LocalAlloc(LPTR, sizeof(L2TP_CONFIG_DATA)+cbKey+1);
        if(pl2tpDevConfig == NULL)
        {
            RASPPPAuthLog(RAS_LOG_FAIL, TEXT("Unable to allocate space for L2TP Preshared key"));
            LocalFree(lpRasEntry);
            return FALSE;
        }

        pl2tpDevConfig->dwVersion = 1;
        pl2tpDevConfig->dwAuthType = L2TP_IPSEC_AUTH_PRESHAREDKEY;
        pl2tpDevConfig->cbKey = cbKey;
        pl2tpDevConfig->dwOffsetKey = sizeof(L2TP_CONFIG_DATA);
        if (pl2tpDevConfig->cbKey)
        {
            memcpy((PBYTE)pl2tpDevConfig+pl2tpDevConfig->dwOffsetKey, cmdParameters.strPresharedKey, cbKey);
        }
        lpbL2TPConfig = (PBYTE)pl2tpDevConfig;
        cbDevConfig = sizeof(L2TP_CONFIG_DATA) + cbKey;
    }
#endif
    lpRasEntry->dwSize = sizeof(RASENTRY);
    DialParams.dwSize = sizeof(RASDIALPARAMS);

    lpRasEntry->dwSize = sizeof(RASENTRY);
    dwErr = RasGetEntryProperties(NULL, TEXT(""), lpRasEntry, &dwRasBufSize, NULL, NULL);
    if (dwErr)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("Unable to create default RASENTRY structure: %d"), dwErr);
        LocalFree(lpRasEntry);
        *dwErrorReceived = ERROR_OUTOFMEMORY;
        return false;
    }

    // Fill in default parameters:
    if (eRasConnection == RAS_DCC_MODEM)
    {
        COPY_STRINGS(DialParams.szUserName, cmdParameters.strDCCUserName, UNLEN+1);
        COPY_STRINGS(DialParams.szPassword, cmdParameters.strDCCPassword, PWLEN+1);
        COPY_STRINGS(DialParams.szDomain, cmdParameters.strDCCDomain, DNLEN+1);
    }
    else
    {
        COPY_STRINGS(DialParams.szUserName, cmdParameters.strUserName, UNLEN+1);
        COPY_STRINGS(DialParams.szPassword, cmdParameters.strPassword, PWLEN+1);
        COPY_STRINGS(DialParams.szDomain, cmdParameters.strDomain, DNLEN+1);
    }

    DialParams.szPhoneNumber[0] = TEXT('\0');
    DialParams.szCallbackNumber[0] = TEXT('\0');

    lpRasEntry->dwfOptions =  dwAuthOptions;
    lpRasEntry->dwCountryCode = 1;
    lpRasEntry->szAreaCode[0] = TEXT('\0');
    COPY_STRINGS(lpRasEntry->szLocalPhoneNumber, cmdParameters.strServerName, RAS_MaxPhoneNumber+1);
    lpRasEntry->dwfNetProtocols = RASNP_Ip;
    lpRasEntry->dwFramingProtocol = RASFP_Ppp;
    lpRasEntry->dwCustomAuthKey = dwEAPExtension;

    GetRasDeviceName(lpRasEntry->szDeviceName, cmdParameters.dwDeviceID);
    GetRasDeviceType(lpRasEntry->szDeviceType, cmdParameters.dwDeviceID);

    // Check for old entry with same name and delete it if found:
    dwErr = RasValidateEntryName(NULL, cmdParameters.strEntryName);
    if (dwErr)
    {
        if (dwErr != ERROR_ALREADY_EXISTS)
        {
            RASPPPAuthLog(RAS_LOG_FAIL, TEXT("Entry \"%s\" name validation failed: %d"),
                cmdParameters.strEntryName, dwErr);
            LocalFree(lpRasEntry);
            *dwErrorReceived = dwErr;
            return false;
        }

        dwErr = RasDeleteEntry(NULL, cmdParameters.strEntryName);
        if (dwErr)
        {
            RASPPPAuthLog(RAS_LOG_FAIL, TEXT("Unable to delete old entry: %d"),dwErr);
            LocalFree(lpRasEntry);
            *dwErrorReceived = dwErr;
            return false;
        }
    }

    COPY_STRINGS(DialParams.szEntryName, cmdParameters.strEntryName, RAS_MaxEntryName+1);

    dwDevConfigSize = DEVCONFIGSIZE;
    if (GetUnimodemDevConfig(lpRasEntry->szDeviceName, lpVarString) == 0xFFFFFFFF)
    {
        // could be VPN connectoid
        lpDevConfig = NULL;
        dwDevConfigSize = 0;

        // VPN or PPPoE does not support custom-scripting DLL
        COPY_STRINGS(lpRasEntry->szScript, TEXT(""), MAX_PATH);
        lpRasEntry->dwfOptions &= (~RASEO_CustomScript);
    }

#if EMBEDDED_ONLY
    // Save default entry configuration
    if (eRasConnection == RAS_VPN_L2TP)
    {
        dwErr = RasSetEntryProperties(NULL, cmdParameters.strEntryName, lpRasEntry, dwRasBufSize,
            lpbL2TPConfig, cbDevConfig);
        if (dwErr)
        {
            RASPPPAuthLog(RAS_LOG_FAIL, TEXT("\"RasSetEntryProperties\" failed: %d"), dwErr);
            LocalFree(lpRasEntry);
            *dwErrorReceived = dwErr;
            return false;
        }
    }
    else
    {
#endif
        dwErr = RasSetEntryProperties(NULL, cmdParameters.strEntryName, lpRasEntry, dwRasBufSize,
            NULL, 0);
        if (dwErr)
        {
            RASPPPAuthLog(RAS_LOG_FAIL, TEXT("\"RasSetEntryProperties\" failed: %d"), dwErr);
            LocalFree(lpRasEntry);
            *dwErrorReceived = dwErr;
            return false;
        }
#if EMBEDDED_ONLY
    }
#endif

    dwErr = RasSetEntryDialParams(NULL, &DialParams, FALSE);
    if (dwErr)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("\"RasSetEntryDialParams\" failed: %d"),dwErr);
        LocalFree(lpRasEntry);
        *dwErrorReceived = dwErr;
        return false;
    }

    //
    // Set baudrate for DCC or modem connections only
    //
    if (eRasConnection == RAS_DCC_MODEM || eRasConnection == RAS_PPP_MODEM)
    {
        dwErr = RasLinkSetBaudrate(cmdParameters.strEntryName, DEFAULT_BAUDRATE, 'N', 8, 1);
        if (dwErr)
        {
            LocalFree(lpRasEntry);
            *dwErrorReceived = dwErr;
            return false;
        }
    }

#if EMBEDDED_ONLY
    if (eRasConnection == RAS_VPN_L2TP)
    {
        LocalFree(pl2tpDevConfig);
    }
#endif

    //
    // If still here, then everything else passed
    //
    LocalFree(lpRasEntry);
    return true;
}

//
// Connect
//
bool
ConnectRasConnection(
    OUT DWORD *dwErrorReceived 
    )
{
    RASDIALPARAMS DialParams = {0};
    BOOL bPasswordSaved = FALSE;
    HRASCONN hRasConn=NULL;
    DWORD dwRet = 0;

    DialParams.dwSize = sizeof(RASDIALPARAMS);
    COPY_STRINGS(DialParams.szEntryName, cmdParameters.strEntryName, RAS_MaxEntryName+1);
    dwRet = RasGetEntryDialParams(NULL, &DialParams, &bPasswordSaved);
    if (dwRet)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("Error reading dialing parameters: %d"), dwRet);
        *dwErrorReceived = dwRet;
    }

    //
    // We will be using the synchronous version of RasDial
    // In this case, RasDial will only return after connection either passed or
    // failed.
    //
    dwRet = RasDial(NULL, NULL, &DialParams, 0, NULL, &hRasConn);
    if (dwRet)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("\"RasDial\" failed: %d"), dwRet);

        *dwErrorReceived = dwRet;
        dwRet = RasHangUp(hRasConn);
        Sleep(10000);
        RASPPPAuthLog(RAS_LOG_COMMENT, TEXT("\"RasHangUp\" returned: %d"), dwRet);
        return false;
    }

    return true;
}

//
// Disconnect
//
bool 
DisconnectRasConnection( 
    OUT DWORD *dwErrorReceived
    )
{
    RASCONN RasConnBuf[10] = {{0}};        // Allowing 10 max RAS connections
    DWORD dwRasConnBufSize = 0;
    DWORD dwConnections = 0;
    DWORD dwErr = 0;
    DWORD i = 0;

    RasConnBuf[0].dwSize = sizeof(RASCONN);
    dwRasConnBufSize = sizeof(RasConnBuf);

    dwErr = RasEnumConnections(RasConnBuf, &dwRasConnBufSize, &dwConnections);
    if (dwErr)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("\"RasEnumConnections\" error: %d"), dwErr);
        *dwErrorReceived = dwErr;
        return false;
    }

    //
    // Any open RAS connections?
    //
    if (!dwConnections)
    {
        RASPPPAuthLog(RAS_LOG_COMMENT, TEXT("No open RAS connections"));

        return true;
    }

    //
    // Disconnect all the active RAS connections
    //
    for (i = 0; i < dwConnections; ++i)
    {
        if ((cmdParameters.strEntryName == NULL) ||
            (CMP_STRINGS(cmdParameters.strEntryName, RasConnBuf[i].szEntryName, RAS_MaxEntryName+1)))
        {
            dwErr = RasHangUp(RasConnBuf[i].hrasconn);
            if (dwErr)
            {
                RASPPPAuthLog(RAS_LOG_FAIL, TEXT("Error hanging up connection #%d: %d"),i+1, dwErr);
                // At least kill the rest of the RAS connections
                continue;
            }
        }
    }

    RASPPPAuthLog(RAS_LOG_COMMENT, TEXT("All RAS connections dropped"));

    return true;
}

//
//
//
DWORD 
RasLinkSetBaudrate(
    LPTSTR lpszEntryName, 
    DWORD dwBaudrate, 
    BYTE bParity,
    BYTE bWordLength, 
    float fStopBits
    )
{
    UNREFERENCED_PARAMETER(dwBaudrate);
    DWORD dwRasBufSize = ENTRYBUFSIZE;
    LPRASENTRY lpRasEntry = NULL;
    DWORD dwErr = 0;
    BYTE DevConfigBuf[DEVBUFSIZE] = {0};
    DWORD dwDevConfigSize = DEVCONFIGSIZE;
    LPBYTE lpDevConfig = DevConfigBuf + DEVCONFIGOFFSET;
    LPVARSTRING lpVarString = (LPVARSTRING)&DevConfigBuf;
    HLINEAPP hLineApp = 0;
    HLINE hLine = 0;
    LINEEXTENSIONID ExtensionID = {0};
    DWORD dwDeviceNum = 0, dwAPIVersion = 0, dwNumDevs = 0;
    UNIMDM_CHG_DEVCFG UCD ={0};
    long lRet = 0;
    TCHAR szErr[80] = {0};

    lpRasEntry = (LPRASENTRY)LocalAlloc(LPTR, ENTRYBUFSIZE);
    if(lpRasEntry == NULL)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("LocalAlloc() failed with %d"), GetLastError());
        return false;
    }
    lpRasEntry->dwSize = sizeof(RASENTRY);

    // Retrieve device configuration:
    dwErr = RasGetEntryProperties(NULL, lpszEntryName, lpRasEntry, &dwRasBufSize,
        lpDevConfig, &dwDevConfigSize);
    if (dwErr)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("Unable to read properties of entry \"%s\": %d"),
            lpszEntryName, dwErr);
        LocalFree(lpRasEntry);        
        return dwErr;
    }

    if (dwDevConfigSize) // If device configuration already exists, use it
    {
        lpDevConfig = NULL;
        lpVarString->dwTotalSize = DEVBUFSIZE; // Fill in rest of VARSTRING structure
        lpVarString->dwNeededSize = DEVBUFSIZE;
        lpVarString->dwUsedSize = DEVBUFSIZE;
        lpVarString->dwStringFormat = STRINGFORMAT_BINARY;
        lpVarString->dwStringSize = DEVCONFIGSIZE;
        lpVarString->dwStringOffset = DEVCONFIGOFFSET;
    }

    if ((dwDeviceNum = GetUnimodemDevConfig(lpRasEntry->szDeviceName, lpVarString))
        == 0xFFFFFFFF)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("Couldn't get default device configuration"));

        return ERROR_CANNOT_SET_PORT_INFO;
    }
    lpDevConfig = DevConfigBuf + DEVCONFIGOFFSET;

    lRet = lineInitialize(&hLineApp, NULL, lineCallbackFunc, TEXT("OEMRasClient"),
        &dwNumDevs);
    if (lRet)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("Can't initialize TAPI: error %.8x (%s)"),
            lRet, FormatLineError(lRet, szErr));
        LocalFree(lpRasEntry);
        return ERROR_CANNOT_SET_PORT_INFO;
    }

    lRet = lineNegotiateAPIVersion(hLineApp, 0, 0x00010000, 0x00020001,
        &dwAPIVersion, &ExtensionID);
    if (lRet)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, 
            TEXT("TAPI version negotiation failed: error %.8x (%s)"),
            lRet, FormatLineError(lRet, szErr));
        lineShutdown(hLineApp);
        LocalFree(lpRasEntry);
        return ERROR_CANNOT_SET_PORT_INFO;
    }

    lRet = lineOpen(hLineApp, dwDeviceNum, &hLine, dwAPIVersion, 0, 0,
        LINECALLPRIVILEGE_NONE, LINEMEDIAMODE_DATAMODEM, NULL);
    if (lRet)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, 
            TEXT("Couldn't open TAPI device %lu: error %.8x (%s)"),
            dwDeviceNum, lRet, FormatLineError(lRet, szErr));
        lineShutdown(hLineApp);

        return ERROR_CANNOT_SET_PORT_INFO;
    }

    // Set up the lineDevSpecific data block:
    UCD.dwCommand = UNIMDM_CMD_CHG_DEVCFG;
    UCD.lpszDeviceClass = TEXT("comm/datamodem");
    UCD.lpDevConfig = lpVarString;
    dwErr = 0;

    // Set the baudrate:
    UCD.dwOption = UNIMDM_OPT_BAUDRATE;
    UCD.dwValue = DEFAULT_BAUDRATE;

    // lineDevSpecific is an asynchronous function; however, there is no need to
    //   wait for the LINE_REPLY callback
    if ((lRet = lineDevSpecific(hLine, 0, NULL, &UCD, sizeof(UCD))) < 0)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("Couldn't reset baudrate: error %.8x (%s)"),
            lRet, FormatLineError(lRet, szErr));
        dwErr = ERROR_CANNOT_SET_PORT_INFO;
    }

    // Set the parity:
    UCD.dwOption = UNIMDM_OPT_PARITY;
    switch (bParity)
    {
    case 'o':
    case 'O':
        UCD.dwValue = ODDPARITY;
        break;
    case 'e':
    case 'E':
        UCD.dwValue = EVENPARITY;
        break;
    case 'm':
    case 'M':
        UCD.dwValue = MARKPARITY;
        break;
    case 's':
    case 'S':
        UCD.dwValue = SPACEPARITY;
        break;
    case 'n':
    case 'N':
    default:
        UCD.dwValue = NOPARITY;
        break;
    }
    if ((lRet = lineDevSpecific(hLine, 0, NULL, &UCD, sizeof(UCD))) < 0)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("Couldn't reset parity: error %.8x (%s)"),
            lRet, FormatLineError(lRet, szErr));
        dwErr = ERROR_CANNOT_SET_PORT_INFO;
    }

    // Set the wordlength:
    UCD.dwOption = UNIMDM_OPT_BYTESIZE;
    UCD.dwValue = (DWORD)bWordLength;

    if ((lRet = lineDevSpecific(hLine, 0, NULL, &UCD, sizeof(UCD))) < 0)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("Couldn't reset wordlength: error %.8x (%s)"),
            lRet, FormatLineError(lRet, szErr));

        dwErr = ERROR_CANNOT_SET_PORT_INFO;
    }

    // Set the stopbits:
    UCD.dwOption = UNIMDM_OPT_STOPBITS;
    if (fStopBits == 1.5)
    {
        UCD.dwValue = ONE5STOPBITS;
    }
    else if (fStopBits == 2)
    {
        UCD.dwValue = TWOSTOPBITS;
    }
    else
    {
        UCD.dwValue = ONESTOPBIT;
    }

    if ((lRet = lineDevSpecific(hLine, 0, NULL, &UCD, sizeof(UCD))) < 0)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("Couldn't reset stopbits: error %.8x (%s)"),
                       lRet, FormatLineError(lRet, szErr));
    }

    lRet = lineClose(hLine);
    if (lRet)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("lineClose: error %.8x (%s)"),
                       lRet, FormatLineError(lRet, szErr));
    }

    lRet = lineShutdown(hLineApp);
    if (lRet)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("lineShutdown: error %.8x (%s)"),
                       lRet, FormatLineError(lRet, szErr));
    }
    lRet = (long)dwErr;

    dwErr = RasSetEntryProperties(NULL, lpszEntryName, lpRasEntry, dwRasBufSize,
        lpDevConfig, dwDevConfigSize);
    if (dwErr)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("\"RasSetEntryProperties\" failed: %d"), dwErr);
        LocalFree(lpRasEntry);
        return dwErr;
    }

    LocalFree(lpRasEntry);
    return (DWORD)lRet;
}
//
//
//
DWORD 
GetRasDeviceName(
    LPTSTR lpszDeviceName, 
    DWORD dwDeviceNum
    )
{
    LPRASDEVINFO lpRasDevInfo = NULL;
    DWORD dwBufSize = 0;
    DWORD dwNumDevices = 0;
    DWORD dwErr = 0;

    if (lpszDeviceName == NULL)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("GetRasDeviceName(): \"RasEnumDevices\": buffer error"));

        return ERROR_READING_DEVICENAME;
    }
    lpszDeviceName[0] = TEXT('\0');

    // find the buffer size needed
    dwErr = RasEnumDevices(NULL, &dwBufSize, &dwNumDevices);
    if (!dwBufSize)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("GetRasDeviceName(): Unable to find \"RasEnumDevices\" buffer size: %d"), dwErr);
        return dwErr;
    }

    lpRasDevInfo = (LPRASDEVINFO)LocalAlloc(LPTR, dwBufSize);
    if (!lpRasDevInfo)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("GetRasDeviceName(): Couldn't allocate memory for \"RasEnumDevices\""));

        return ERROR_READING_DEVICENAME;
    }
    lpRasDevInfo->dwSize = sizeof(RASDEVINFO);

    dwErr = RasEnumDevices(lpRasDevInfo, &dwBufSize, &dwNumDevices);
    if (dwErr)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("GetRasDeviceName(): \"RasEnumDevices\" failed: %d"), dwErr);

        LocalFree(lpRasDevInfo);
        return dwErr;
    }

    if (dwDeviceNum >= dwNumDevices)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("GetRasDeviceName(): %d devices available; device %d not in list"),
            dwNumDevices, dwDeviceNum);

        LocalFree(lpRasDevInfo);
        return ERROR_READING_DEVICENAME;
    }

    COPY_STRINGS(lpszDeviceName, (lpRasDevInfo + dwDeviceNum)->szDeviceName,
        RAS_MaxDeviceName);
    lpszDeviceName[RAS_MaxDeviceName] = TEXT('\0');

    LocalFree(lpRasDevInfo);
    return 0;
}

//
//
//
DWORD 
GetRasDeviceType(
    LPTSTR lpszDeviceType, 
    DWORD dwDeviceNum
    )
{
    LPRASDEVINFO lpRasDevInfo = NULL;
    DWORD dwBufSize = 0;
    DWORD dwNumDevices = 0;
    DWORD dwErr = 0;

    if (lpszDeviceType == NULL)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("GetRasDeviceType(): \"RasEnumDevices\": buffer error"));

        return ERROR_READING_DEVICENAME;
    }
    lpszDeviceType[0] = TEXT('\0');

    // find the buffer size needed
    dwErr = RasEnumDevices(NULL, &dwBufSize, &dwNumDevices);
    if (!dwBufSize)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("GetRasDeviceType(): Unable to find \"RasEnumDevices\" buffer size: %d"), dwErr);

        return dwErr;
    }

    lpRasDevInfo = (LPRASDEVINFO)LocalAlloc(LPTR, dwBufSize);
    if (!lpRasDevInfo)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("GetRasDeviceType(): Couldn't allocate memory for \"RasEnumDevices\""));

        return ERROR_READING_DEVICENAME;
    }
    lpRasDevInfo->dwSize = sizeof(RASDEVINFO);

    dwErr = RasEnumDevices(lpRasDevInfo, &dwBufSize, &dwNumDevices);
    if (dwErr)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("GetRasDeviceType(): \"RasEnumDevices\" failed: %d"), dwErr);

        LocalFree(lpRasDevInfo);
        return dwErr;
    }

    if (dwDeviceNum >= dwNumDevices)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("GetRasDeviceType(): %d devices available; device %d not in list"),
            dwNumDevices, dwDeviceNum);

        LocalFree(lpRasDevInfo);
        return ERROR_READING_DEVICENAME;
    }

    COPY_STRINGS(lpszDeviceType, (lpRasDevInfo + dwDeviceNum)->szDeviceType, RAS_MaxDeviceType);
    lpszDeviceType[RAS_MaxDeviceType] = TEXT('\0');

    LocalFree(lpRasDevInfo);
    return 0;
}

//
//
//


DWORD 
GetUnimodemDeviceName(
    LPTSTR lpszDeviceName, 
    LPTSTR lpszDeviceType
    )
{
    BYTE bDevCapsBuf[DEVCAPSSIZE] = {0};
    LPLINEDEVCAPS lpDevCaps = (LPLINEDEVCAPS)bDevCapsBuf;
    HLINEAPP hLineApp = 0;
    LINEEXTENSIONID ExtensionID = {0};
    BYTE bDeviceType = 0xFF;
    DWORD dwDeviceNum = 0, dwAPIVersion = 0, dwNumDevs = 0;
    TCHAR szBuff[120] = {0};
    long lRet = 0;
    size_t bufferLen = 1024;

    if (_tcsicmp(lpszDeviceType, TEXT("serial")) == 0)
    {
        StringCchCopy(lpszDeviceType, bufferLen, TEXT("direct"));
        bDeviceType = UNIMODEM_SERIAL_DEVICE;
    }
    else if (_tcsicmp(lpszDeviceType, TEXT("direct")) == 0)
    {
        bDeviceType = UNIMODEM_SERIAL_DEVICE;
    }

    else if (_tcsicmp(lpszDeviceType, TEXT("external")) == 0)
    {
        StringCchCopy(lpszDeviceType, bufferLen, TEXT("modem"));
        bDeviceType = UNIMODEM_HAYES_DEVICE;
    }
    else if (_tcsicmp(lpszDeviceType, TEXT("modem")) == 0)
    {
        bDeviceType = UNIMODEM_HAYES_DEVICE;
    }

    else if (_tcsicmp(lpszDeviceType, TEXT("PCMCIA")) == 0)
    {
        StringCchCopy(lpszDeviceType, bufferLen, TEXT("modem"));
        bDeviceType = UNIMODEM_PCMCIA_DEVICE;
    }

    else if (_tcsicmp(lpszDeviceType, TEXT("infrared")) == 0)
    {
        StringCchCopy(lpszDeviceType, bufferLen, TEXT("direct"));
        bDeviceType = UNIMODEM_IR_DEVICE;
    }

    if (bDeviceType == 0xFF)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("Device type %l not recognized"), lRet);
        return FALSE;
    }

    lRet = lineInitialize(&hLineApp, NULL, lineCallbackFunc, TEXT("OEMRasClient"),
        &dwNumDevs);
    if (lRet)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("Can't initialize TAPI: error %.8x (%s)"),
            lRet, FormatLineError(lRet, szBuff));
        return 0xFFFFFFFF;
    }

    for (dwDeviceNum = 0; dwDeviceNum < dwNumDevs; dwDeviceNum++)
    {
        lRet = lineNegotiateAPIVersion(hLineApp, dwDeviceNum, 0x00010000,
            0x00020001, &dwAPIVersion, &ExtensionID);
        if (lRet)
        {
            RASPPPAuthLog(RAS_LOG_FAIL, 
                TEXT("TAPI version negotiation failed for device %lu: error %.8x (%s)"),
                dwDeviceNum, lRet, FormatLineError(lRet, szBuff));
            continue;
        }

        lpDevCaps->dwTotalSize = DEVCAPSSIZE;
        lRet = lineGetDevCaps(hLineApp, dwDeviceNum, dwAPIVersion, 0, lpDevCaps);
        if (lRet)
            RASPPPAuthLog(RAS_LOG_FAIL, 
            TEXT("Couldn't read TAPI device %lu capabilities: error %.8x (%s)"),
            dwDeviceNum, lRet, FormatLineError(lRet, szBuff));

        else
        {
            StringCchCopy(lpszDeviceName, RAS_MaxDeviceName, (PTSTR)(bDevCapsBuf + lpDevCaps->dwLineNameOffset) );
            lpszDeviceName[RAS_MaxDeviceName] = TEXT('\0');

            // check device name if intended to make a null modem connection
            if (bDeviceType == UNIMODEM_SERIAL_DEVICE)
            {
                // Derault device is "Serial Cable on" either COM1 or COM2
                if (CMP_STRINGS(lpszDeviceName, TEXT("Serial Cable on COM"), 19) == 0)
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }

    if (dwDeviceNum == dwNumDevs)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("No matching Unimodem device found"));
        dwDeviceNum = 0xFFFFFFFF;
    }

    lRet = lineShutdown(hLineApp);
    if (lRet)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("lineShutdown: error %.8x (%s)"),
                       lRet, FormatLineError(lRet, szBuff));
    }
    return dwDeviceNum;
}

//
//
//
DWORD 
GetUnimodemDevConfig(
    LPTSTR lpszDeviceName,
    LPVARSTRING lpDevConfig
    )
{
    BYTE bDevCapsBuf[1024] = {0};
    LPLINEDEVCAPS lpDevCaps = (LPLINEDEVCAPS)bDevCapsBuf;
    HLINEAPP hLineApp = 0;
    LINEEXTENSIONID ExtensionID = {0};
    DWORD dwDeviceNum = 0, dwAPIVersion = 0, dwNumDevs = 0;
    TCHAR szBuff[120];
    long lRet = 0;

    lRet = lineInitialize(&hLineApp, NULL, lineCallbackFunc, TEXT("OEMRasClient"),
        &dwNumDevs);
    if (lRet)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("Can't initialize TAPI: error %.8x (%s)"),
            lRet, FormatLineError(lRet, szBuff));
        return 0xFFFFFFFF;
    }

    for (dwDeviceNum = 0; dwDeviceNum < dwNumDevs; dwDeviceNum++)
    {
        lRet = lineNegotiateAPIVersion(hLineApp, dwDeviceNum, 0x00010000,
            0x00020001, &dwAPIVersion, &ExtensionID);
        if (lRet)
        {
            RASPPPAuthLog(RAS_LOG_FAIL, 
                TEXT("TAPI version negotiation failed for device %lu: error %.8x (%s)"),
                dwDeviceNum, lRet, FormatLineError(lRet, szBuff));
            continue;
        }

        lpDevCaps->dwTotalSize = DEVCAPSSIZE;
        lRet = lineGetDevCaps(hLineApp, dwDeviceNum, dwAPIVersion, 0, lpDevCaps);
        if (lRet)
        {
            RASPPPAuthLog(RAS_LOG_FAIL, 
                TEXT("Couldn't read TAPI device %lu capabilities: error %.8x (%s)"),
                dwDeviceNum, lRet, FormatLineError(lRet, szBuff));
            continue;
        }

        if ((lpszDeviceName == NULL) ||
            (_tcscmp((LPTSTR)(bDevCapsBuf + lpDevCaps->dwLineNameOffset), lpszDeviceName) == 0))
        {
            if (_tcsicmp((LPTSTR)(bDevCapsBuf + lpDevCaps->dwProviderInfoOffset),
                TEXT("UNIMODEM")) != 0)
            {
                RASPPPAuthLog(RAS_LOG_FAIL, TEXT("TAPI device %lu isn't Unimodem"),
                    dwDeviceNum);
                dwDeviceNum = 0xFFFFFFFF;
            }
            else if (lpDevConfig != NULL)
            {
                lpDevConfig->dwTotalSize = DEVBUFSIZE;

                lRet = lineGetDevConfig(dwDeviceNum, lpDevConfig,
                    TEXT("comm/datamodem"));
                if (lRet)
                {
                    RASPPPAuthLog(RAS_LOG_FAIL, 
                        TEXT("Couldn't read TAPI device %lu configuration: error %.8x (%s)"),
                        dwDeviceNum, lRet, FormatLineError(lRet, szBuff));
                    dwDeviceNum = 0xFFFFFFFF;
                }
            }
            break;
        }
    }

    lRet = lineShutdown(hLineApp);
    if (lRet)
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("lineShutdown: error %.8x (%s)"),
        lRet, FormatLineError(lRet, szBuff));

    if ((dwDeviceNum >= dwNumDevs) && (dwDeviceNum < 0xFFFFFFFF))
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("No match for device lpszDeviceName(%s)"),(NULL == lpszDeviceName) ? RAS_STR_NULL : lpszDeviceName);
        dwDeviceNum = 0xFFFFFFFF;
    }

    return dwDeviceNum;
}

void 
CALLBACK lineCallbackFunc(
    DWORD dwDevice, 
    DWORD dwMsg,
    DWORD dwCallbackInstance,
    DWORD dwParam1,
    DWORD dwParam2,
    DWORD dwParam3
    )
{
    TCHAR szBuff[1024] = {0};

    RASPPPAuthLog(RAS_LOG_COMMENT, TEXT("TAPI callback function: %s"),
        FormatLineCallback(szBuff, dwDevice, dwMsg, dwCallbackInstance,
        dwParam1, dwParam2, dwParam3));
}

DWORD 
GetRasDeviceNum(
    LPTSTR lpszDeviceType,
    DWORD& dwDeviceNum
    )
{
    LPRASDEVINFO lpRasDevInfo = NULL;
    DWORD dwBufSize = 0;
    DWORD dwNumDevices = 0;
    DWORD i = 0;
    DWORD dwErr = 0;

    dwDeviceNum = (DWORD)-1;

    // find the buffer size needed
    dwErr = RasEnumDevices(NULL, &dwBufSize, &dwNumDevices);
    if (!dwBufSize)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("Unable to find \"RasEnumDevices\" buffer size: %d"),dwErr);

        return dwErr;
    }

    lpRasDevInfo = (LPRASDEVINFO)LocalAlloc(LPTR, dwBufSize);
    if (!lpRasDevInfo)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("Couldn't allocate memory for \"RasEnumDevices\""));

        return ERROR_READING_DEVICENAME;
    }
    lpRasDevInfo->dwSize = sizeof(RASDEVINFO);

    dwErr = RasEnumDevices(lpRasDevInfo, &dwBufSize, &dwNumDevices);
    if (dwErr)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("\"RasEnumDevices\" failed: %d"), dwErr);

        LocalFree(lpRasDevInfo);
        return dwErr;
    }

    //
    // Find the device which matches and return that device number
    //
    for (i = 0; i < dwNumDevices; ++i)
    {
        if (CMP_STRINGS(lpszDeviceType, (lpRasDevInfo + i)->szDeviceType, RAS_MaxDeviceType + 1) == 0)
        {
            if (eRasConnection == RAS_VPN_L2TP)
            {
                if (_tcsstr((lpRasDevInfo + i)->szDeviceName, TEXT("L2TP")))
                {
                    // The name of this line contains L2TP. This must be
                    // an L2TP line. Return this device number and break
                    dwDeviceNum = i;
                    break;
                }
                else
                {
                    continue;
                }
            }
            else if (eRasConnection == RAS_VPN_PPTP)
            {
                dwDeviceNum = i;
                break;
            }
        }
    }

    if (dwDeviceNum < 0)
    {
        RASPPPAuthLog(RAS_LOG_FAIL, TEXT("%d devices available; no device of type %s in list"),
            dwNumDevices, lpszDeviceType);

        LocalFree(lpRasDevInfo);
        return ERROR_READING_DEVICETYPE;
    }

    LocalFree(lpRasDevInfo);
    return 0;
}

// END OF FILE
