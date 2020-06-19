
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once


#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") //Dos prompt 

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.


#include <afxdisp.h>        // MFC 자동화 클래스입니다.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원

//KAISION
#include <stdint.h>
#include <_sdkim_2008_algebra.h>
#include <_sdkim_2008_array.h>
#include <_sdkim_2008_bootstrap.h>
#include <_sdkim_2008_camera_digital.h>
#include <_sdkim_2008_classifier.h>
#include <_sdkim_2008_color.h>
#include <_sdkim_2008_depth.h>
#include <_sdkim_2008_geometry.h>
#include <_sdkim_2008_grabber.h>
#include <_sdkim_2008_graph_3d.h>
#include <_sdkim_2008_graph_line.h>
#include <_sdkim_2008_graph_xy.h>
#include <_sdkim_2008_hello.h>
#include <_sdkim_2008_io.h>
#include <_sdkim_2008_jpeg.h>
#include <_sdkim_2008_object_geometric.h>
#include <_sdkim_2008_screen.h>
#include <_sdkim_2008_screen3D.h>
#include <_sdkim_2008_sound.h>
#include <_sdkim_2008_tempo.h>
//#include <_sdkim_2016_matlab.h>

//VCL_3DO_Lib
#include <_VCL_3DO_HEADERS.h>
#include "VCL_DoCube_X_Color.h"



#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


