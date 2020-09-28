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
//+----------------------------------------------------------------------------
//
//
// File:    ensoappt.cpp
//
// Contents:
//
//  implementation file
//
//      IEnumSoapPorts Interface implemenation
//
//-----------------------------------------------------------------------------
#include "headers.h"
#include "ensoappt.h"


#ifdef UNDER_CE
#include "WinCEUtils.h"
#endif


BEGIN_INTERFACE_MAP(CEnumWSDLPorts)
    ADD_IUNKNOWN(CEnumWSDLPorts, IEnumWSDLPorts)
    ADD_INTERFACE(CEnumWSDLPorts, IEnumWSDLPorts)
END_INTERFACE_MAP(CEnumWSDLPorts)

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//  function: CEnumWSDLPorts::CEnumWSDLPorts()
//
//  parameters:
//
//  description:
//
//  returns:
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
CEnumWSDLPorts::CEnumWSDLPorts()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////
//  function: CEnumWSDLPorts::~CEnumWSDLPorts()
//
//  parameters:
//
//  description:
//
//  returns:
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
CEnumWSDLPorts::~CEnumWSDLPorts()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//  function: HRESULT CEnumWSDLPorts::Next(long celt, IWSDLPort **ppWSDLPort, long *pulFetched)
//
//  parameters:
//
//  description:
//
//  returns:
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CEnumWSDLPorts::Next(long celt, IWSDLPort **ppWSDLPort, long *pulFetched)
{
    return(m_portList.Next(celt, ppWSDLPort, pulFetched));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////
//  function: HRESULT CEnumWSDLPorts::Skip(long celt)
//
//  parameters:
//
//  description:
//
//  returns:
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CEnumWSDLPorts::Skip(long celt)
{
    return(m_portList.Skip(celt));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//  function: HRESULT CEnumWSDLPorts::Reset(void)
//
//  parameters:
//
//  description:
//
//  returns:
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CEnumWSDLPorts::Reset(void)
{
    return(m_portList.Reset());
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//  function: HRESULT CEnumWSDLPorts::Clone(IEnumWSDLPorts **ppenum)
//
//  parameters:
//
//  description:
//
//
//  returns:
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CEnumWSDLPorts::Clone(IEnumWSDLPorts **ppenum)
{
    HRESULT hr = S_OK;
    CEnumWSDLPorts *pPorts;
    pPorts = new CSoapObject<CEnumWSDLPorts>(INITIAL_REFERENCE);
    CHK_BOOL(pPorts, E_OUTOFMEMORY);
    CHK(pPorts->Copy(this));
    pPorts->Reset();
    *ppenum = pPorts;
Cleanup:
    return (hr);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//  function: HRESULT CEnumWSDLPorts::Copy(CEnumWSDLPorts *pOrg)
//
//  parameters:
//
//  description:
//
//
//  returns:
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CEnumWSDLPorts::Copy(CEnumWSDLPorts *pOrg)
{
    return (pOrg->m_portList.Clone(m_portList));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////






/////////////////////////////////////////////////////////////////////////////////////////////////////////
//  function: HRESULT CEnumWSDLPorts::Find(BSTR bstrPortToFind, IWSDLPort **ppIWSDLPort)
//
//  parameters:
//
//  description:
//
//  returns:
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CEnumWSDLPorts::Find(BSTR bstrPortToFind, IWSDLPort **ppIWSDLPort)
{
    return(m_portList.Find(bstrPortToFind, ppIWSDLPort));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////
//  function: HRESULT CEnumWSDLPorts::Add(IWSDLPort *pWSDLPort)
//
//  parameters:
//
//  description:
//
//  returns:
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CEnumWSDLPorts::Add(IWSDLPort *pWSDLPort)
{
    return(m_portList.Add(pWSDLPort));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////
//  function: HRESULT CEnumWSDLPorts::Size(long *pulSize)
//
//  parameters:
//
//  description:
//
//  returns:
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CEnumWSDLPorts::Size(long *pulSize)
{
    return(m_portList.Size(pulSize));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
