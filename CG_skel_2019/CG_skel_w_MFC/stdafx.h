// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>



#include "vec.h"
#include "mat.h"
#include "Scene.h"
#include "InitShader.h"
#include "InputDialog.h"
#include "Controls.h"
#include <string>
#include <algorithm>
#include <wtypes.h>
#include <set>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
#define MIN3(x, y, z) ( min(min( (x) , (y) ) , (z) ) );
#define MAX3(x, y, z) ( max(max( (x) , (y) ) , (z) ) );

