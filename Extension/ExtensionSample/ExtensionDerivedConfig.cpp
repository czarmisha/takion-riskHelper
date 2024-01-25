#include "stdafx.h"
#include "ExtensionDerivedConfig.h"
#include "MainDialog.h"
#include "ConfigEnums.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern MainDialog* mainDialog;

ExtensionDerivedConfig::ExtensionDerivedConfig():
	ExtensionConfig(mainDialog),
#ifndef TAKION_NO_OPTIONS
	m_mdOptionCall(true),
#endif
	m_winType(0xFF),
	m_watchMessageCount(1000)
{
	AddSettings();
}

ExtensionDerivedConfig::ExtensionDerivedConfig(const ExtensionDerivedConfig& other):
	ExtensionConfig(other),

	m_mdLoadOnStart(other.m_mdLoadOnStart),
	m_mdStockFilePath(other.m_mdStockFilePath),

	m_mdAccountId(other.m_mdAccountId),
	m_mdSymbol(other.m_mdSymbol),
#ifndef TAKION_NO_OPTIONS
	m_mdOption(other.m_mdOption),
	m_mdOptionCall(other.m_mdOptionCall),
	m_mdOptionSuffix(other.m_mdOptionSuffix),
	m_mdOptionStrikePrice(other.m_mdOptionStrikePrice),
	m_mdOptionExpiration(other.m_mdOptionExpiration),
#endif

	m_SecurityBlackListFilename(other.m_SecurityBlackListFilename),
	m_FixerConditionsFilename(other.m_FixerConditionsFilename),
	m_WorkDirectoryPath(other.m_WorkDirectoryPath),
	m_account(other.m_account),
	m_updateFrequency(other.m_updateFrequency),
	m_listsUpdateFrequency(other.m_listsUpdateFrequency),

	m_winType(other.m_winType),
	m_extTools(other.m_extTools),
	m_watchRequestFilter(other.m_watchRequestFilter),
	m_watchResponseFilter(other.m_watchResponseFilter),
	m_watchMessageCount(other.m_watchMessageCount),
	m_watchSuspended(other.m_watchSuspended),

	m_commandPost(other.m_commandPost),
	m_commandName(other.m_commandName),
	m_commandSymbol(other.m_commandSymbol)
{
	AddSettings();
}

void ExtensionDerivedConfig::AddSettings()
{
	AddSetting("MD_LoadOnStart", m_mdLoadOnStart);
	AddSetting("MD_StockFilePath", m_mdStockFilePath);
	AddSetting("MD_AccountId", m_mdAccountId);
	AddSetting("MD_Symbol", m_mdSymbol);
#ifndef TAKION_NO_OPTIONS
	AddSetting("MD_Option", m_mdOption);
	AddSetting("MD_OptionCall", m_mdOptionCall);
	AddSetting("MD_OptionSuffix", m_mdOptionSuffix);
	AddSetting("MD_OptionStrikePrice", m_mdOptionStrikePrice);
	AddSetting("MD_OptionExpiration", m_mdOptionExpiration);
#endif

	AddSetting("SecurityBlackListFilename|", m_SecurityBlackListFilename);
	AddSetting("FixerConditionsFilename|", m_FixerConditionsFilename);
	AddSetting("WorkDirectoryPath|", m_WorkDirectoryPath);
	AddSetting("Account", m_account);
	AddSetting("UpdateFrequency", m_updateFrequency);
	AddSetting("ListsUpdateFrequency", m_listsUpdateFrequency);

	AddSetting("Win_Type", m_winType);
	AddSetting("EWL", m_extTools);
	AddSetting("ReqFilter", m_watchRequestFilter);
	AddSetting("RspFilter", m_watchResponseFilter);
	AddSetting("MessageCount", m_watchMessageCount);
	AddSetting("WatchSuspended", m_watchSuspended);

	AddSetting("CommandPost", m_commandPost);
	AddSetting("CommandName", m_commandName);
	AddSetting("CommandSymbol", m_commandSymbol);
}

ConfigBase* ExtensionDerivedConfig::Clone() const
{
	ExtensionDerivedConfig* clone = new ExtensionDerivedConfig;
	clone->Copy(*this);
	return clone;
}

void ExtensionDerivedConfig::Copy(const ConfigBase& other)
{
//	TakionConfig::Copy(other);
	operator=((const ExtensionDerivedConfig&)other);
}

const ExtensionDerivedConfig& ExtensionDerivedConfig::operator=(const ExtensionDerivedConfig& other)
{
	ExtensionConfig::operator=(other);
//	m_orderReplaceTemplateVersion = other.m_orderReplaceTemplateVersion;
	return *this;
}

void ExtensionDerivedConfig::Update()
{
	ExtensionConfig::Update();
	if(mainDialog)
	{
		MainDialog::UIntStrMap windowsMap;
		if(mainDialog->GetWindowsLayout(windowsMap, 0))
		{
			m_extTools.Clear();
			UIntSetting key;
			StringSettingRest stringSetting;
			for(MainDialog::UIntStrMap::const_iterator it = windowsMap.begin(), itend = windowsMap.end(); it != itend; ++it)
			{
				key.SetValue(it->first);
				stringSetting.SetValue(it->second);
				m_extTools.AddValue(key, stringSetting);
			}
		}
	}
}
