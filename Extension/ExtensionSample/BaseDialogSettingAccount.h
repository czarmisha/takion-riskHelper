#pragma once

#include "BaseDialogSetting.h"

enum BaseDialogSettingAccountIds
{
	comboBoxAccountId = 1000,
	groupAccountId,

	baseDialogSettingAccountIdsCount,
};

class BaseDialogSettingAccount : public BaseDialogSetting, public Observer
{
public:
	virtual void UpdateSettings() override;
	virtual HWND GetFirstTabControl() const override{return m_ComboBoxAccount.m_hWnd;}
	virtual void RestoreLayout() override;
	virtual StringSetting* GetAccountIdSetting(){return NULL;}

	void CurrentAccountSet(Account* account);

	Account* GetAccount() const{return m_account;}
	const char* GetAccountId() const{return m_accountId;}

//	const Position* FindPosition() const;
protected:
	BaseDialogSettingAccount(TakionMainWnd* mainWnd,
		TakionSettingTabDialog* parentTab,
		bool useAccount,
		const char* label,
		bool includeCurrentAccount,
		int accountWidth,
		int topOffset = 0);
	virtual BOOL OnInitDialog() override;
//	void AccountDestroyed(const Account* account);

	virtual void AddAccountObserver();
	virtual void RemoveAccountObserver();

	void SetAccount(Account* account);
	void AddAccount(Account* account);
//	void SetPosition(Position* position);

	void UpdateComboBoxAccount();

	virtual void AccountChanged(){}
	virtual void OnHistoryLoaded(){}
	virtual void OnExecutorConnected(){}
	virtual bool ShouldAddAccountObserver() const{return false;}
	void SetAccountId(const char* accountId);
	bool m_useAccount;
	Account* m_account;
//	const Position* m_position;
	char m_accountId[MAX_ACCOUNT_SIZE];
	bool m_includeCurrentAccount;
/*
	std::string m_symbol;
	std::string m_controlSymbol;
	bool m_symbolCurrent;
*/
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void BeforeDestroy() override;
//	virtual BOOL PreTranslateMessage(MSG* pMsg) override;
/*
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSymbolUpdate();
	afx_msg void OnSymbolChange();
	afx_msg void OnSymbol();
	afx_msg void OnSelchangeOrder();
*/
	afx_msg void OnSelchangeAccount();

	DECLARE_MESSAGE_MAP()
/*
	ValidEdit m_EditSymbol;
	CButton m_ButtonSymbol;
	CStatic m_StaticSymbol;
*/
	ComboBoxAccount m_ComboBoxAccount;

	int m_accountGroupHeight;
	int m_accountWidth;
	int m_bottomControl;

	int m_xc;

	void SetExecutorConnected(bool connected);
	void SetHistoryLoaded(bool loaded);
	bool m_executorConnected;
	bool m_historyLoaded;

	virtual void Notify(const Message* message, const Observable* from, const Message* info) override;
};
