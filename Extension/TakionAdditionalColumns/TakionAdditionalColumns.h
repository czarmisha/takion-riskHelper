// ExtensionSample.h : main header file for the ExtensionSample DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

// CExtensionSampleApp
// See ExtensionSample.cpp for the implementation of this class
//

class TakionAdditionalColumns : public CWinApp
{
public:
	TakionAdditionalColumns();
	virtual BOOL InitInstance() override;
	virtual int ExitInstance() override;
	const std::string& GetFilePathAndName() const{return m_filePathAndName;}
	const std::string& GetFileDescription() const{return m_fileDescription;}
	const std::string& GetPlatform() const{return m_platform;}
	const std::string& GetVersionStr() const{return m_versionStr;}
	const unsigned __int64& GetVersionNum() const{return m_versionNum;}
protected:
//	DECLARE_MESSAGE_MAP()
	std::string m_filePathAndName;
	std::string m_fileDescription;
	std::string m_platform;
	std::string m_versionStr;
	unsigned __int64 m_versionNum;
};
