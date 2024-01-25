#include "stdafx.h"
#include "MainDialog.h"
#include "riskHelper.h"
#include "SampleDialogSettingLevel2InThread.h"
#include "SampleDialogSettingWindow.h"
#include "SampleDialogSettingTestRequest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

std::string CreateTitle(const char* const& productName, const unsigned __int64& productVersion)
{
	std::string title(productName);
	title += ' ';
	U_VersionNumToStr(productVersion, title);
	return title;
}

MainDialog::MainDialog(const char* productName, unsigned __int64 productVersion, const char* configExtension):
	ExtensionMainDialog(280, 164, CreateTitle(productName, productVersion).c_str(), productName, productVersion, configExtension),
	m_level2InThreadDialog(NULL),
	m_riskHelper(NULL),
	m_windowDialog(NULL),
	m_testRequestDialog(NULL)
{
}

void MainDialog::TakionWindowAdded(TakionTool* tool)
{
	if(m_windowDialog)m_windowDialog->TakionWindowAdded(tool);
}

void MainDialog::TakionWindowRemoved(TakionTool* tool)
{
	if(m_windowDialog)m_windowDialog->TakionWindowRemoved(tool);
}

void MainDialog::TakionWindowRepaintTool(TakionTool* tool)
{
	if(m_windowDialog)m_windowDialog->TakionWindowRepaintTool(tool);
}

bool MainDialog::GetWindowsLayout(UIntStrMap& windowsMap, unsigned int contentModifier) const
{
	return m_windowDialog && m_windowDialog->GetWindowsLayout(windowsMap, contentModifier);
}

void MainDialog::AddMessageReport(unsigned char connectionType, const Message* message, bool outgoing)
{
	if(m_testRequestDialog)m_testRequestDialog->AddMessageReport(connectionType, message, outgoing);
}

BOOL MainDialog::OnInitDialog()
{
	TakionMainWnd* frame = TGUI_GetTakionMainWnd();

	TakionSettingTabDialog* tabDlg = new TakionSettingTabDialog(m_mainWnd, this, "");//, 437, 152);
	AddSettingDialog(tabDlg);
	m_riskHelper = new RiskHelper(frame, tabDlg);
	tabDlg->AddSettingDialog(m_riskHelper);

	BOOL ret = ExtensionMainDialog::OnInitDialog();

	return ret;
}

void MainDialog::BeforeDestroy()
{
	m_level2InThreadDialog = NULL;
	m_riskHelper = NULL;
	m_windowDialog = NULL;
	m_testRequestDialog = NULL;
	ExtensionMainDialog::BeforeDestroy();
}

bool MainDialog::MainThreadIdleState(unsigned int idleCount, bool takionIdleDone)
{
	bool ret = false;
	if(m_level2InThreadDialog && m_level2InThreadDialog->MainThreadIdleState(idleCount, takionIdleDone))
	{
		ret = true;
	}
	return ret;
}

bool MainDialog::CurrentAccountSet(Account* account)
{
	if(ExtensionMainDialog::CurrentAccountSet(account))
	{
		if(m_level2InThreadDialog)m_level2InThreadDialog->CurrentAccountSet(m_currentAccount);
		return true;
	}
	return false;
}

