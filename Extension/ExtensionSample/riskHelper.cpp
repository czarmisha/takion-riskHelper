#include "stdafx.h"
#include "riskHelper.h"
#include "TakionDataApi.h"
#include "ExtensionDerivedConfig.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_set>


extern ExtensionDerivedConfig* extensionConfig;

ofstream logFile;
ofstream eventFile;


void WriteToFile(string line, bool log=true)
{
	static char time[33];
	U_FormatMillisecond(TL_GetCurrentMillisecond(), time, sizeof(time));
	string times(time);

	if (log) {
		logFile << times << ": " << line << endl;
		logFile.flush();
	}
	else {
		eventFile << times << ": " << line << endl;
		eventFile.flush();
	}
	
}


bool is_number(const std::string& s)
{
	char* end = 0;
	double val = strtod(s.c_str(), &end);
	return end != s.c_str() && val != HUGE_VAL;
}

string GetCurrentDate()
{
	string date = TL_GetDay() < 10 ? ("0" + to_string(TL_GetDay()) + "_") : (to_string(TL_GetDay()) + "_");
	date += TL_GetMonth() < 10 ? ("0" + to_string(TL_GetMonth()) + "_") : (to_string(TL_GetMonth()) + "_");
	date += to_string(TL_GetYear());

	return date;
}

Account* GetAccountById(const char* accountId)
{
	Account* account = NULL;

	TakionIterator* iterator = TD_CreateAccountIterator();
	TD_LockAccountStorage();
	iterator->Reset();

	while (account = (Account*)TD_GetNextAccount(iterator))
	{
		if (!std::strcmp(account->GetId(), accountId))
			break;
	}

	TD_UnlockAccountStorage();
	delete iterator;

	if (account)
		account->LoadExecutions();

	return account;
}

// ======================== DEFAULT =================================================================

enum RiskHelperIds
{
	buttonRunId = baseSymbolOptionIdsCount,
	buttonStopId,

	groupStatusdId,
	staticStatusId,
};

RiskHelper::RiskHelper(TakionMainWnd* mainWnd, TakionSettingTabDialog* parentTab) :
	BaseSampleDialogSettingOption(mainWnd, parentTab, true, "RiskHelper", 52),
	m_lastNotificationTime(0),
	m_lastListUpdateTime(0),
	m_updateFrequency(0),
	m_listsUpdateFrequency(0),
	isRun(false)
{
	int xc = hgap;
	int w = m_accountWidth - hgap - hgap;
	m_bottomControl += vgap;

	const int statusGroupHeight = groupOffset + staticHeight * 3 + 4 * vgap + hgap;//30;

	AddButtonIndirect("Status", WS_VISIBLE | BS_GROUPBOX, WS_EX_TRANSPARENT, xc, m_bottomControl, m_accountWidth * 6, statusGroupHeight, groupStatusdId);
	AddStaticIndirect("Sts:", WS_VISIBLE | SS_NOPREFIX, 0, 2 * xc, m_bottomControl += groupOffset, w * 2, staticHeight, staticStatusId);

	m_bottomControl += 15;

	AddButtonIndirect("Run", WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP, 0, xc, m_bottomControl, m_accountWidth / 2, comboBoxHeight, buttonRunId);
	AddButtonIndirect("Stop", WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP, 0, xc + m_accountWidth / 2 + 2, m_bottomControl, m_accountWidth / 2, comboBoxHeight, buttonStopId);
}

RiskHelper::~RiskHelper()
{
}

void RiskHelper::Cleanup()
{
	if (m_security)
		SecurityFailed();
}

void RiskHelper::Entering()
{
	DoSubscribe();
	BaseSampleDialogSettingOption::Entering();
}

void RiskHelper::Leaving()
{
	Cleanup();
	BaseSampleDialogSettingOption::Leaving();
}

void RiskHelper::DoDataExchange(CDataExchange* pDX)
{
	BaseSampleDialogSettingOption::DoDataExchange(pDX);
	DDX_Control(pDX, buttonRunId, m_ButtonRun);
	DDX_Control(pDX, buttonStopId, m_ButtonStop);
	DDX_Control(pDX, staticStatusId, m_StatusCheck);
}

void RiskHelper::BeforeDestroy()
{
	Cleanup();
	BaseSampleDialogSettingOption::BeforeDestroy();
}

bool RiskHelper::CanSubscribe() const
{
	return (m_security || !m_symbol.empty());
}

void RiskHelper::EnableSubscribeButton()
{
	EnableControl(m_ButtonRun.m_hWnd, !isRun, m_EditSymbol.m_hWnd);
	EnableControl(m_ButtonStop.m_hWnd, isRun, m_EditSymbol.m_hWnd);
}

BEGIN_MESSAGE_MAP(RiskHelper, BaseSampleDialogSettingOption)
	ON_BN_CLICKED(buttonRunId, OnRun)
	ON_BN_CLICKED(buttonStopId, OnStop)
END_MESSAGE_MAP()

void RiskHelper::OnRun()
{
	isRun = true;
	m_symbol = "BAC";
	DoSubscribe();
	EnableSubscribeButton();
}

void RiskHelper::OnStop()
{
	isRun = false;
	m_symbol.clear();
	DoSubscribe();
	m_StatusCheck.SetWindowText("Sts: Off");
	EnableSubscribeButton();
}

void RiskHelper::SecurityFailed()
{
	if (m_security)
	{
		m_security->RemoveObserver(this);
		m_security = NULL;
	}

	m_StatusCheck.SetWindowText("Sts: Off");
}


bool RiskHelper::DoSubscribe()
{
	if (CanSubscribe())
	{
		Security* security;

		if (m_symbol.empty())
			security = NULL;
		else
			security = TD_ObtainStock(m_symbol.c_str(), false);

		if (security != m_security)
		{
			if (m_security)
				m_security->RemoveObserver(this);

			m_security = security;

			if (m_security)
			{
				m_security->AddObserver(this);
				m_security->Subscribe();
				TD_ReleaseStock(m_security);

				m_StatusCheck.SetWindowText("Sts: Active");
			}
			return true;
		}
	}

	return false;
}

BOOL RiskHelper::OnInitDialog()
{
	BOOL ret = BaseSampleDialogSettingOption::OnInitDialog();

	EnableSubscribeButton();

	if (eventFile.is_open())
		eventFile.close();
	eventFile.open(extensionConfig->m_WorkDirectoryPath.GetValue() + "event_" + GetCurrentDate() + ".txt", ios::app);

	m_account = TD_GetCurrentAccount(); //GetAccountById(extensionConfig->m_account.GetValue().c_str()); 
	m_updateFrequency = extensionConfig->m_updateFrequency.GetValue();
	m_listsUpdateFrequency = extensionConfig->m_listsUpdateFrequency.GetValue();

	m_account->LoadExecutions();
	m_account->LoadOrders();

	//initializeSecurityBlackList();
	//initializeAccountConditions();

	OnRun();

	return ret;
}

void RiskHelper::Notify(const Message* message, const Observable* from, const Message* info)
{
	if (logFile.is_open())
		logFile.close();
	logFile.open(extensionConfig->m_WorkDirectoryPath.GetValue() + "log_" + GetCurrentDate() + ".txt");

	unsigned int currentTime = TL_GetCurrentMillisecond();
	if (currentTime >= m_lastListUpdateTime + m_listsUpdateFrequency) {
		initializeAccountConditions();
		initializeSecurityBlackList();

		m_lastListUpdateTime = currentTime;
	}

	if (currentTime >= m_lastNotificationTime + m_updateFrequency)
	{
		CheckLogic();
		m_lastNotificationTime = currentTime;
	}
}

//========================================================================================================
//========================================================================================================

void RiskHelper::initializeAccountConditions()
{
	WriteToFile("initializeAccountConditions");

	ifstream inputfile;

	inputfile.open(extensionConfig->m_WorkDirectoryPath.GetValue() + extensionConfig->m_FixerConditionsFilename.GetValue()); // csv

	string accountId, trigger, step, blackListTrack, prePostCoverTrack;
	accountConditions.clear();

	if (inputfile.is_open())
	{
		while (getline(inputfile, accountId, ';'))
		{
			getline(inputfile, trigger, ';');
			getline(inputfile, step, ';');
			getline(inputfile, blackListTrack, ';');
			getline(inputfile, prePostCoverTrack);

			Account* account = GetAccountById(accountId.c_str());
			if (account) {
				accountConditionStruct accStruct = { account, stod(trigger) <= 0 ? 0 : stod(trigger), stod(step) <= 0 ? 0 : stod(step), stoi(blackListTrack) <= 0 ? false : true, stoi(prePostCoverTrack) <= 0 ? false : true };
				accountConditions.push_back(accStruct);
			}
			
		}

		inputfile.close();
	}
	WriteToFile("! initializeAccountConditions");
}


void RiskHelper::initializeSecurityBlackList()
{
	WriteToFile("initializeSecurityBlackList");

	ifstream inputfile;
	inputfile.open(extensionConfig->m_WorkDirectoryPath.GetValue() + extensionConfig->m_SecurityBlackListFilename.GetValue()); // txt

	string ticker;
	SecurityBlackList.clear();

	if (inputfile.is_open())
	{
		while (getline(inputfile, ticker))
		{
			if (ticker.length())
			{
				SecurityBlackList.insert(ticker);
			}
		}

		inputfile.close();
	}
	
	WriteToFile("! initializeSecurityBlackList");
}


void RiskHelper::CheckLogic()
{
	WriteToFile("CHECK LOGIC");
	for (unsigned int i = 0; i < accountConditions.size(); ++i)
	{
		accountConditionStruct account_conditions = accountConditions[i];

		if (account_conditions.blackListTrack) {
			checkForProhibittedTickers(account_conditions.account);
		}

		if (account_conditions.prePostCoverTrack && TD_GetCurrentMinute() <570 && TD_GetCurrentMinute() >= 960) {
			checkForPrePostLosses(account_conditions.account);
		}

		if (account_conditions.triggerPnL > 0) {
			checkForHighPnL(account_conditions);
		}
	}
	
	WriteToFile("! CHECK LOGIC");
}

void RiskHelper::checkForProhibittedTickers(Account* account) {
	WriteToFile("checkForProhibittedTickers");
	PositionMap& positionMap = account->GetStockPositionsAll();
	PositionMap::CPair* it = positionMap.PGetFirstAssoc();

	while (it)
	{
		Position* position = it->value;

		string searchSymbol = position->GetSymbol();
		char side = position->GetSize() < 0 ? 'S' : 'B';

		auto search = SecurityBlackList.find(searchSymbol);
		if (side == 'S' && search != SecurityBlackList.end()) {
			WriteToFile("BLACKLIST " + string(account->GetId()) + " " + searchSymbol + " close short", false);
			//SendOrder(security, side == 'B' ? 'S' : 'B', targetPrice, position->GetSize(), ORDER_TYPE_LIMIT);
			// cancel opening orders (stop, target, opening, etc)
		}

		it = positionMap.PGetNextAssoc(it);
	}
	WriteToFile("! checkForProhibittedTickers");
}

void RiskHelper::checkForPrePostLosses(Account* account) {
	WriteToFile("checkForPrePostLosses");

	double coverValue = account->GetCtrnMaxLossClose().toDouble();
	double dayHigh = account->GetMarkedPnlNbboPrintHi().toDouble();

	if (abs(dayHigh) >= coverValue) {

		PositionMap& positionMap = account->GetStockPositionsAll();
		PositionMap::CPair* it = positionMap.PGetFirstAssoc();

		while (it)
		{
			Position* position = it->value;
			char side = position->GetSize() < 0 ? 'S' : 'B';
			CancelPositionOrders(account, position, "COVER");
			// position->CancelOpeningOrders();
			//SendOrder(security, side == 'B' ? 'S' : 'B', targetPrice, position->GetSize(), ORDER_TYPE_LIMIT);
			WriteToFile("COVER " + string(account->GetId()) + " send order " + position->GetSymbol(), false);

			it = positionMap.PGetNextAssoc(it);
		}
	}

	WriteToFile("! checkForPrePostLosses");
}

void RiskHelper::checkForHighPnL(accountConditionStruct account_conditions) {
	WriteToFile("checkForHighPnL");
	if (account_conditions.account->GetMarkedPnlNbboPrintHi().toDouble() >= account_conditions.triggerPnL) {
		if (account_conditions.account->GetAccountMarkedPnlNbboPrint().toDouble() < account_conditions.account->GetMarkedPnlNbboPrintHi().toDouble() * (1 - account_conditions.trailingStep)) {

			PositionMap& positionMap = account_conditions.account->GetStockPositionsAll();
			PositionMap::CPair* it = positionMap.PGetFirstAssoc();

			while (it)
			{
				Position* position = it->value;
				char side = position->GetSize() < 0 ? 'S' : 'B';
				CancelPositionOrders(account_conditions.account, position, "FIXER");
				// position->CancelOpeningOrders();
				//SendOrder(security, side == 'B' ? 'S' : 'B', targetPrice, position->GetSize(), ORDER_TYPE_LIMIT);
				WriteToFile("FIXER " + string(account_conditions.account->GetId()) + " send order " + position->GetSymbol(), false);

				it = positionMap.PGetNextAssoc(it);
			}
		}
	}
	WriteToFile("! checkForHighPnL");
}

void RiskHelper::CancelPositionOrders(Account* account, Position* position, string event) {
	const OrderMap& orderMap = position->GetOrdersPending();
	const OrderMap::CPair* it = orderMap.PGetFirstAssoc();
	while (it) 
	{
		Order* order = it->value;
		//account->CancelOrder(order);
		WriteToFile(event + " " + string(account->GetId()) + " cancel order " + order->GetSymbol() + " " + order->GetSide(), false);

	it = orderMap.PGetNextAssoc(it);
	}
}

void RiskHelper::SendOrder(Security* security, char side, Price order_price, unsigned int size, unsigned char order_type)
{
	WriteToFile("SendOrder");
	Price limitPrice = order_price;
	Price stopPrice = Price::priceZero;

	const Destination* destination = TD_GetDestinationArca();
	const DestinationRouting* routing = destination->FindRoutingByNumericName(TD_GetDefaultProActiveRoutingNumericName());

	Price m_level1BidPrice, m_level1AskPrice, m_lastPrintPrice;

	m_level1BidPrice.SetCompactPrice(security->GetL1Bid());
	m_level1AskPrice.SetCompactPrice(security->GetL1Ask());
	m_lastPrintPrice.SetCompactPrice(security->GetLastNbboPrintPrice());

	if (order_type == ORDER_TYPE_STOP)
	{
		WriteToFile("STOP ORDER");
		limitPrice = Price::priceZero;
		stopPrice = order_price;

		destination = TD_GetDestinationQsmt();
		routing = destination->FindRoutingByNumericName(0);
	}
	else if (order_type == ORDER_TYPE_MARKET) {
		limitPrice = Price::priceZero;
		stopPrice = Price::priceZero;
	}

	unsigned int orderClientId = 0;//output parameter
	m_account->SendOrder(&orderClientId,
		NULL,//orderToReplace
		false,//replaceNative
		0,//replaceSizeOffset
		security,
		PE_NOT_POSTPONED,
		0,//timeToActivate
		destination->GetMainNumericName(),
		destination->GetId(),
		side,//side
		0,//unsigned char repriceFlags
		order_type,//order type
		0,//userOrderType
		nullptr,//userOrderInfo
		nullptr,//commandName
		limitPrice,//limitPrice
		Price::priceZero,//discretionaryPrice
		stopPrice,//stopPrice
		order_type == ORDER_TYPE_STOP ? OST_SYNTHETIC : OST_NONE,//stopOrderType
		OSB_BID,//stopOrderBase
		m_level1BidPrice,
		m_level1AskPrice,
		m_lastPrintPrice,
		size,
		0,//displayQuantity
		0,//dlg->GetEnterReserveQuantity(),
		routing ? routing->GetId() : 0,
		routing ? routing->GetSubType() : 0,
		routing ? routing->GetNumericName() : 0,
		0,//destination->HasMnemonic() ? dlg->GetEnterMnemonic() : 0,
		false,//iso
		PEG_NONE,//pegType
		SignedPrice::signedPriceZero,//pegOffset
		TIF_GTX,//tifType
		0,//tifMilliseconds (for TIF_GTD only)
		WOP_CANCEL,//TD_GetWashOrderPolicy(),
		OOP_RESIZE,//TD_GetOversellSplitPolicy(),
		false,//TD_IsResizeShortToBorrowed(),
		false,//preBorrow
		Price::oneCent,//Price(30, 0 * Price::oneCentFraction),//Price::oneCent,//preBorrow Limit Price
		false,//noRedirect
		false,//closeOnly
		false,//resizeToClose
		false,//syntheticIgnoreFinra
		false,//syntheticWaitNext
		false,//imbalanceOnly,
			  //					false,//proAts
		false,//blockAggressiveOrders
		RLP_ROUND_DOWN,//TD_GetRoundLotPolicy(),
		NULL,//isoAttachment
		0,//userId
		0,//parentId
		0);//ocoId

	WriteToFile("! SendOrder");
}