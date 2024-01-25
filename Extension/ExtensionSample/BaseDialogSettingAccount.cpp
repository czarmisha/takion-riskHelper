#include "stdafx.h"
#include "BaseDialogSettingAccount.h"
//#include "ExtensionSample.h"
#include "ExtensionDerivedConfig.h"

//extern ExtensionSample theApp;
extern ExtensionDerivedConfig* extensionConfig;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////
BaseDialogSettingAccount::BaseDialogSettingAccount(TakionMainWnd* mainWnd,
	TakionSettingTabDialog* parentTab,
	bool useAccount,
	const char* label,
	bool includeCurrentAccount,
	int accountWidth, int topOffset):

	BaseDialogSetting(mainWnd, parentTab, label),
	m_useAccount(useAccount),
	m_account(NULL),
	m_includeCurrentAccount(includeCurrentAccount),
//	m_position(NULL),
//	m_symbolCurrent(true),
//	m_EditSymbol(U_GetValidStockChars(), false),
	m_ComboBoxAccount(0xFFFFFFFF, "Current"),
	m_accountGroupHeight(24),
	m_accountWidth(accountWidth),
	m_bottomControl(topOffset),
	m_xc(hgap),

	m_executorConnected(false),
	m_historyLoaded(false)
{
	U_CopyAndPad(m_accountId, sizeof(m_accountId), "", '\0');

	if(m_useAccount)
	{
		AddButtonIndirect("Account", WS_VISIBLE | BS_GROUPBOX, WS_EX_TRANSPARENT, m_xc - hgap, m_bottomControl, m_accountWidth, m_accountGroupHeight, groupAccountId);
		AddComboBoxIndirect(NULL, WS_VISIBLE | WS_TABSTOP | CBS_SORT | CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED | WS_VSCROLL, 0, m_xc, m_bottomControl + groupOffset, m_accountWidth - hgap - hgap, 400, comboBoxAccountId);

		m_bottomControl += m_accountGroupHeight;
	}
	SetExecutorConnected(TD_IsExecutorConnected());
}

void BaseDialogSettingAccount::BeforeDestroy()
{
	SetAccount(NULL);

	TD_GetAdminObservable().RemoveObserver(this);
	TD_GetNewAccountObservable().RemoveObserver(this);
	BaseDialogSetting::BeforeDestroy();
}

BOOL BaseDialogSettingAccount::OnInitDialog()
{
	TD_GetAdminObservable().AddObserver(this);//to get notified about connect / disconnect
	TD_GetNewAccountObservable().AddObserver(this);
	BOOL ret = BaseDialogSetting::OnInitDialog();

	if(m_useAccount)
	{
		TakionIterator* iterator = TD_CreateAccountIterator();
		Account* account;
		TD_LockAccountStorage();
		iterator->Reset();
		while(account = (Account*)TD_GetNextAccount(iterator))
		{
			m_ComboBoxAccount.AddString((const char*)account);
		}
		TD_UnlockAccountStorage();
		delete iterator;

		if(m_includeCurrentAccount)
		{
			m_ComboBoxAccount.AddString(NULL);
			m_ComboBoxAccount.SetCurSel(0);
			OnSelchangeAccount();
		}
	}
//	RestoreLayout();

	return ret;
}

void BaseDialogSettingAccount::RestoreLayout()
{
	if(m_useAccount)
	{
		StringSetting* accountId = GetAccountIdSetting();
		if(accountId)
		{
			SetAccountId(accountId->GetValue().c_str());
		}
	}
}

void BaseDialogSettingAccount::UpdateSettings()
{
	if(m_useAccount)
	{
		StringSetting* accountId = GetAccountIdSetting();
		if(accountId)
		{
			int sel = m_ComboBoxAccount.GetCurSel();
			if((sel > 0 || !m_includeCurrentAccount && !sel) && m_account)
			{
				accountId->SetValue(m_account->GetId());
			}
			else
			{
				accountId->SetValue(std::string());
			}
		}
	}
}

void BaseDialogSettingAccount::DoDataExchange(CDataExchange* pDX)
{
	BaseDialogSetting::DoDataExchange(pDX);

	if(m_useAccount)
	{
		DDX_Control(pDX, comboBoxAccountId, m_ComboBoxAccount);
	}
}

BEGIN_MESSAGE_MAP(BaseDialogSettingAccount, BaseDialogSetting)
	ON_CBN_SELCHANGE(comboBoxAccountId, OnSelchangeAccount)
END_MESSAGE_MAP()

void BaseDialogSettingAccount::OnSelchangeAccount()
{
	if(m_useAccount)
	{
		int sel = m_ComboBoxAccount.GetCurSel();
		Account* account = sel >= 0 ? (Account*)m_ComboBoxAccount.GetItemDataPtr(sel) : NULL;
		if(account)
		{
			U_CopyAndPad(m_accountId, sizeof(m_accountId), account->GetId(), '\0');
		}
		else
		{
			U_CopyAndPad(m_accountId, sizeof(m_accountId), "", '\0');
			account = m_includeCurrentAccount ? TD_GetCurrentAccount() : NULL;
		}
		SetAccount(account);
		m_modified = true;
	}
}

void BaseDialogSettingAccount::UpdateComboBoxAccount()
{
	if(m_useAccount)
	{
		int sel = m_ComboBoxAccount.FindItem(m_account);
		m_ComboBoxAccount.SetCurSel(sel);
		OnSelchangeAccount();
	}
}

void BaseDialogSettingAccount::AddAccount(Account* account)
{
	if(m_useAccount)
	{
		int sel = m_ComboBoxAccount.AddString((const char*)account);
		if(*m_accountId)
		{
			if(!strcmp(m_accountId, account->GetId()))
			{
				m_ComboBoxAccount.SetCurSel(sel);
				SetAccount(account);
			}
		}
	}
}

void BaseDialogSettingAccount::AddAccountObserver()
{
	if(m_account)
	{
		m_account->AddObserver(this);
	}
}

void BaseDialogSettingAccount::RemoveAccountObserver()
{
	if(m_account)
	{
		m_account->RemoveObserver(this);
	}
}

void BaseDialogSettingAccount::SetAccount(Account* account)
{
	if(m_useAccount)
	{
		if(m_account != account)
		{
			if(ShouldAddAccountObserver())
			{
				RemoveAccountObserver();
				m_account = account;
				AddAccountObserver();
			}
			else
			{
				m_account = account;
			}
			AccountChanged();
		}
	}
}

void BaseDialogSettingAccount::CurrentAccountSet(Account* account)
{
	if(m_ComboBoxAccount.m_hWnd && m_ComboBoxAccount.GetCurSel() <= 0)
	{
		SetAccount(account);
	}
}

void BaseDialogSettingAccount::SetAccountId(const char* accountId)
{
	if(m_useAccount)
	{
		int sel;
		if(accountId && *accountId)
		{
			sel = m_ComboBoxAccount.FindItemByName(accountId);
			if(sel < 0 && m_includeCurrentAccount)sel = 0;
		}
		else
		{
			sel = m_includeCurrentAccount ? 0 : -1;
		}
		m_ComboBoxAccount.SetCurSel(sel);
		if(sel < 0)
		{
			SetAccount(m_includeCurrentAccount ? TD_GetCurrentAccount() : NULL);
		}
		else if(m_includeCurrentAccount && sel == 0)
		{
			SetAccount(TD_GetCurrentAccount());
		}
		else
		{
			SetAccount((Account*)m_ComboBoxAccount.GetItemDataPtr(sel));
		}
		m_modified = false;
	}
}

void BaseDialogSettingAccount::SetHistoryLoaded(bool loaded)
{
	if(m_historyLoaded != loaded)
	{
		m_historyLoaded = loaded;
		OnHistoryLoaded();
	}
}

void BaseDialogSettingAccount::SetExecutorConnected(bool connected)
{
	if(m_executorConnected != connected)
	{
		m_executorConnected = connected;
		OnExecutorConnected();
	}
}

void BaseDialogSettingAccount::Notify(const Message* message, const Observable* from, const Message* info)
{
	switch(message->m_type)
	{
		case TS_NEW_ACCOUNT:
		if(m_useAccount && info && info->m_type == TM_NEW_ACCOUNT)
		{
			TMsgNewAccountObject* msg = (TMsgNewAccountObject*)info;
			Account* account = msg->m_account;
			m_ComboBoxAccount.AddString((const char*)account);
			if(*m_accountId && !strcmp(account->GetId(), m_accountId))
			{
				SetAccount(account);
				UpdateComboBoxAccount();
			}
		}
		break;

		case TM_NEW_ACCOUNT:
		if(m_useAccount)
		{
			TMsgNewAccountObject* msg = (TMsgNewAccountObject*)message;
			Account* account = msg->m_account;
			m_ComboBoxAccount.AddString((const char*)account);
			if(*m_accountId && !strcmp(account->GetId(), m_accountId))
			{
				SetAccount(account);
				UpdateComboBoxAccount();
			}
		}
		break;

		case TM_UPDATED_ACCOUNT://TM_ACCOUNT_UPDATE:
//		UpdateAccount(((TMsgAccount*)message)->m_account);
		break;

		case TM_ACCOUNT_DELETE:
		if(m_useAccount)
		{
			Account* account = ((const TMsgAccountDelete*)message)->m_account;
			int found = m_ComboBoxAccount.FindItem(account);
			if(found >= 0)
			{
				const int sel = m_ComboBoxAccount.GetCurSel();
				m_ComboBoxAccount.DeleteString(found);
				if(sel > 0)
				{
					const unsigned int count = m_ComboBoxAccount.GetItemCount();
					if(count > 0)
					{
						if((unsigned int)sel >= count)
						{
							m_ComboBoxAccount.SetCurSel(count - 1);
							OnSelchangeAccount();
						}
					}
				}
			}
		}
		break;

		case TM_DELETE_ALL_ACCOUNTS:
		if(m_useAccount)
		{
			m_ComboBoxAccount.ResetContent();
			if(m_includeCurrentAccount)m_ComboBoxAccount.AddString(NULL);
			m_ComboBoxAccount.SetCurSel(0);
			OnSelchangeAccount();
		}
		break;
/*
		case M_RESP_MD_LOGON:
//		UpdateMarketDataEntitlements();
		break;
*/
		case TS_LOGIN_RESP:
		if(m_useAccount && info && info->m_type == TM_NEW_ACCOUNT)
		{
			TMsgNewAccountObject* msg = (TMsgNewAccountObject*)info;
			Account* account = msg->m_account;
			if(m_ComboBoxAccount.FindItem(account) < 0)
			{
				m_ComboBoxAccount.AddString((const char*)account);
				if(*m_accountId && !strcmp(account->GetId(), m_accountId))
				{
					SetAccount(account);
					UpdateComboBoxAccount();
				}
			}
		}
		break;

		case TS_ACCOUNT_POSITION_RESP:
		if(info && info->m_type == TM_UPDATED_ACCOUNT)//History Loaded
		{
			TMsgUpdatedAccount* msg = (TMsgUpdatedAccount*)info;
			Account* account = msg->m_account;
			if(m_account == account)
			{
				SetHistoryLoaded(true);
			}
		}
		break;

		case TS_ACCOUNT_EXEC_HISTORY_COMPLETE:
		if(info && info->m_type == TM_UPDATED_ACCOUNT)
		{
			TMsgUpdatedAccount* msg = (TMsgUpdatedAccount*)info;
			Account* account = msg->m_account;
			if(m_account == account)
			{
			}
		}
		break;

		case TS_ACCOUNT_ORDER_HISTORY_COMPLETE:
		if(info && info->m_type == TM_UPDATED_ACCOUNT)
		{
			TMsgUpdatedAccount* msg = (TMsgUpdatedAccount*)info;
			Account* account = msg->m_account;
			if(m_account == account)
			{
			}
		}
		break;

		case TM_MARKET_STATUS:
		{
		}
		break;

		case TM_NEXT_DAY_STARTED:
		{
//			NextDayStarted(((const TMsgNextDayStarted*)message)->m_date);
		}
		break;

		case TM_CONNECTED:
		case TM_DISCONNECTED:
		if(info && info->m_type == TM_CONNECTION_OBJECT)
		{
			const TMsgConnectionObject* msg = (const TMsgConnectionObject*)info;
			Connection* connection = msg->m_connection;
			if(connection == TD_GetExecutorConnection())
			{
				SetExecutorConnected(TD_IsExecutorConnected());
			}
		}
		break;

		case TM_DATA_THREAD_BUFFER_OVERFLOW:
//		PostMessage(WM_USER + 500, ((const TMsgDataThreadBufferOverflow*)message)->m_threadId, 0);
		break;
	}
}
