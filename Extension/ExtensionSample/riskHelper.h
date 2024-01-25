#include "BaseSampleDialogSettingOption.h"
using namespace std;

#pragma once

struct accountConditionStruct
{
	Account* account;
	double triggerPnL; // check on initialize > 0
	double trailingStep; // check on initialize > 0
	bool blackListTrack;
	bool prePostCoverTrack;
};


class RiskHelper : public BaseSampleDialogSettingOption
{
public:
	RiskHelper(TakionMainWnd* mainWnd, TakionSettingTabDialog* parentTab);
	~RiskHelper();

protected:
	// ================== default =======================
	void Cleanup();

	virtual void Entering() override;
	virtual void Leaving() override;

	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;
	virtual void BeforeDestroy() override;

	bool CanSubscribe() const;
	void EnableSubscribeButton();

	bool DoSubscribe();
	void SecurityFailed();

	afx_msg void OnRun();
	afx_msg void OnStop();

	DECLARE_MESSAGE_MAP();

	virtual void Notify(const Message* message, const Observable* from, const Message* info) override;

	CButton m_ButtonRun;
	CButton m_ButtonStop;
	CStatic m_StatusCheck;

	bool isRun;
	Security* m_security;

	unsigned int m_updateFrequency;
	unsigned int m_listsUpdateFrequency;
	unsigned int m_lastNotificationTime;
	unsigned int m_lastListUpdateTime;

	// ==========================================
	// ==========================================

	void checkForProhibittedTickers(Account* account);
	void checkForPrePostLosses(Account* account);
	void checkForHighPnL(accountConditionStruct account_conditions);

	void initializeSecurityBlackList();
	void initializeAccountConditions();
	void CheckLogic();
	
	void SendOrder(Security* security, char side, Price order_price, unsigned int size, unsigned char order_type);
	void CancelPositionOrders(Account* account, Position* position, string event);

	std::unordered_set<string> SecurityBlackList;
	std::vector<accountConditionStruct> accountConditions;
};
