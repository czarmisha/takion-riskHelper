// ExtensionSample.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TakionAdditionalColumns.h"
#include "TakionAdditionalColumnsVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

TakionMainWnd* takionMainWnd = nullptr;

// The one and only ExtensionSample object
TakionAdditionalColumns theApp;

#ifdef __cplusplus
extern "C"
{
#endif

//unsigned int WINAPI GetExtensionCode(){return ClientExtensionDll::validExtensionCode;}
unsigned __int64 WINAPI GetExtensionVersion(){return theApp.GetVersionNum();}
unsigned __int64 WINAPI GetUtilsVersion(){return U_GetModuleVersionNum();}
unsigned __int64 WINAPI GetUtilsHeaderVersion(){return U_VersionStrToNum(UtilsHeaderVersion);}
unsigned __int64 WINAPI GetTakionUtilsVersion(){return TU_GetModuleVersionNum();}
unsigned __int64 WINAPI GetTakionUtilsHeaderVersion(){return U_VersionStrToNum(TakionUtilsHeaderVersion);}
unsigned __int64 WINAPI GetTakionGuiVersion(){return TGUI_GetModuleVersionNum();}
unsigned __int64 WINAPI GetTakionGuiHeaderVersion(){return U_VersionStrToNum(TakionGuiHeaderVersion);}
unsigned __int64 WINAPI GetTakionDataVersion(){return TD_GetModuleVersionNum();}
unsigned __int64 WINAPI GetTakionDataHeaderVersion(){return U_VersionStrToNum(TakionDataHeaderVersion);}
unsigned __int64 WINAPI GetTakionLogVersion(){return TL_GetModuleVersionNum();}
unsigned __int64 WINAPI GetTakionLogHeaderVersion(){return U_VersionStrToNum(TakionLogHeaderVersion);}
unsigned __int64 WINAPI GetObserverVersion(){return O_GetModuleVersionNum();}
unsigned __int64 WINAPI GetObserverHeaderVersion(){return U_VersionStrToNum(ObserverHeaderVersion);}
const char* WINAPI GetTakionExtensionDescription(){return theApp.GetFileDescription().c_str();}
const char* WINAPI GetTakionExtensionProductName(){return ExtensionColumnsProductNameStr;}

void WINAPI ExtensionInitialize()
{
	takionMainWnd = TGUI_GetTakionMainWnd();
}

void WINAPI ExtensionTerminate()
{
}

#ifdef __cplusplus
}
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CExtensionSampleApp
/*
BEGIN_MESSAGE_MAP(CExtensionSampleApp, CWinApp)
END_MESSAGE_MAP()
*/

// ExtensionSample construction

TakionAdditionalColumns::TakionAdditionalColumns()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// CExtensionSampleApp initialization

BOOL TakionAdditionalColumns::InitInstance()
{
	CWinApp::InitInstance();
	U_InitializeInstanceInfo(m_hInstance, m_filePathAndName, m_fileDescription, m_platform, m_versionNum, m_versionStr);
//	takionMainWnd = TGUI_GetTakionMainWnd();
	return TRUE;
}

int TakionAdditionalColumns::ExitInstance()
{
	int ret = CWinApp::ExitInstance();
	return ret;
}

