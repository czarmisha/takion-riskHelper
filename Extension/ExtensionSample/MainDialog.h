#pragma once

//#include "TakionGui.h"

class RiskHelper;
class SampleDialogSettingLevel2InThread;
class SampleDialogSettingWindow;
class SampleDialogSettingTestRequest;

class MainDialog : public ExtensionMainDialog//TakionSettingTopDialog
{
public:
	MainDialog(const char* productName, unsigned __int64 productVersion, const char* configExtension);
	virtual bool CurrentAccountSet(Account* account) override;
	void TakionWindowAdded(TakionTool* tool);
	void TakionWindowRemoved(TakionTool* tool);
	void TakionWindowRepaintTool(TakionTool* tool);
	typedef std::map<unsigned int, std::string> UIntStrMap;
	bool GetWindowsLayout(UIntStrMap& windowsMap, unsigned int contentModifier) const;
	void AddMessageReport(unsigned char connectionType, const Message* message, bool outgoing);

	bool MainThreadIdleState(unsigned int idleCount, bool takionIdleDone);
protected:
	virtual BOOL OnInitDialog() override;
	virtual void BeforeDestroy() override;

	SampleDialogSettingLevel2InThread* m_level2InThreadDialog;
	RiskHelper* m_riskHelper;
	SampleDialogSettingWindow* m_windowDialog;
	SampleDialogSettingTestRequest* m_testRequestDialog;
};
