#pragma once

#include "ConfigEnums.h"

class ExtensionDerivedConfig : public ExtensionConfig
{
public:
	ExtensionDerivedConfig();
	ExtensionDerivedConfig(const ExtensionDerivedConfig& other);
	virtual void Copy(const ConfigBase& other) override;
	virtual ConfigBase* Clone() const override;

	virtual void Update() override;

	const ExtensionDerivedConfig& operator=(const ExtensionDerivedConfig& other);

	BoolSetting m_mdLoadOnStart;
	StringSettingRest m_mdStockFilePath;

	StringSetting m_mdAccountId;
	StringSetting m_mdSymbol;
#ifndef TAKION_NO_OPTIONS
	BoolSetting m_mdOption;
	BoolSetting m_mdOptionCall;
	CharSetting m_mdOptionSuffix;
	PriceSetting m_mdOptionStrikePrice;
	UIntSetting m_mdOptionExpiration;
#endif

	//===============================================================
	StringSetting m_SecurityBlackListFilename;
	StringSetting m_FixerConditionsFilename;
	StringSetting m_WorkDirectoryPath;
	StringSetting m_account;
	UIntSetting m_updateFrequency;
	UIntSetting m_listsUpdateFrequency;
	//==============================================================

	UCharSetting m_winType;
	typedef MapSetting<UIntSetting, StringSettingRest, std::map<UIntSetting, StringSettingRest> > ExtToolMap;
	ExtToolMap m_extTools;

	UIntSetting m_watchRequestFilter;
	UIntSetting m_watchResponseFilter;
	UIntSetting m_watchMessageCount;
	BoolSetting m_watchSuspended;
	
	BoolSetting m_commandPost;
	StringSetting m_commandName;
	StringSetting m_commandSymbol;
protected:
	void AddSettings();
};