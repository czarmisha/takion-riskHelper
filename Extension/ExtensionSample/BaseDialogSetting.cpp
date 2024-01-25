#include "stdafx.h"
#include "BaseDialogSetting.h"
#include "ExtensionSample.h"
//#include "ExtensionDerivedConfig.h"

extern ExtensionSample theApp;
//extern ExtensionDerivedConfig* extensionConfig;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool BaseDialogSetting::UpdateDestinationRoutingStatic(const Destination*& controlDestination, const Destination* const& currentDestination, ComboBoxDestination& comboBoxDestination, ComboBoxRouting& comboBoxRouting, ValidEdit* editMnemonic)
{
	bool ret = false;
	int sel = comboBoxDestination.GetCurSel();
	if(sel >= 0)
	{
		unsigned int routingName = 0;
		unsigned int routingId = RI_NONE;
		const Destination* dest = (const Destination*)comboBoxDestination.GetItemDataPtr(sel);
		if(controlDestination != dest)
		{
			sel = comboBoxRouting.GetCurSel();
			if(sel >= 0)
			{
				const DestinationRouting* routing = (const DestinationRouting*)comboBoxRouting.GetItemDataPtr(sel);
				if(routing)
				{
					routingName = routing->GetNumericName();
					routingId = routing->GetId();
				}
			}
			controlDestination = dest;
			if(editMnemonic)
			{
				editMnemonic->Invalidate(FALSE);
			}
			comboBoxRouting.ResetContent();
			if(!controlDestination->GetNoRoutingReplacement())
			{
				comboBoxRouting.AddString(NULL);
			}
			const Destination::RoutingMap& routingMap = controlDestination->GetRoutingMap();
			for(Destination::RoutingMap::const_iterator it = routingMap.begin(), itend = routingMap.end(); it != itend; ++it)
			{
				comboBoxRouting.AddString((const char*)it->second);
			}
			if(routingName)
			{
				char buf[sizeof(unsigned int) + 1];
				*(unsigned int*)buf = routingName;
				buf[sizeof(unsigned int)] = '\0';
				sel = comboBoxRouting.FindItemByName(buf);
				if(sel < 0)
				{
					const DestinationRouting* routing = routingId == RI_PROACTIVE ? controlDestination->GetProActiveReplacement() : controlDestination->GetNoRoutingReplacement();
					if(routing)
					{
						sel = comboBoxRouting.FindItem(routing);
						if(sel < 0)
						{
							sel = 0;
						}
					}
					else
					{
						sel = 0;
					}
				}
				comboBoxRouting.SetCurSel(sel);
			}
			else
			{
				const DestinationRouting* routing = controlDestination->GetNoRoutingReplacement();
				if(routing)
				{
					sel = comboBoxRouting.FindItem(routing);
					if(sel < 0)
					{
						sel = 0;
					}
				}
				else
				{
					sel = 0;
				}
				comboBoxRouting.SetCurSel(sel);
			}
//			OnSelchangeRouting();
			ret = true;
		}
	}
	else if(controlDestination)
	{
		controlDestination = NULL;
		if(editMnemonic)
		{
			editMnemonic->Invalidate(FALSE);
		}
		comboBoxRouting.ResetContent();
		comboBoxRouting.AddString(NULL);
		comboBoxRouting.SetCurSel(0);
//		OnSelchangeRouting();
		ret = true;
	}
	if(ret)
	{
		comboBoxDestination.SetInvalid(controlDestination != currentDestination);
	}
	return ret;
}

void BaseDialogSetting::SetRoutingIdStatic(unsigned short routingId, unsigned short routingSubType, const Destination* currentDestination, ComboBoxRouting& comboBoxRouting)
{
	if(routingId && currentDestination)
	{
		const DestinationRouting* routing = currentDestination->FindRouting(routingId, routingSubType);
		if(!routing)
		{
			routing = routingId == RI_PROACTIVE ? currentDestination->GetProActiveReplacement() : currentDestination->GetNoRoutingReplacement();
		}
		if(routing)
		{
			int found = comboBoxRouting.FindItem(routing);
			if(found < 0)
			{
				routing = routingId == RI_PROACTIVE ? currentDestination->GetProActiveReplacement() : currentDestination->GetNoRoutingReplacement();
				if(routing)
				{
					found = comboBoxRouting.FindItem(routing);
					if(found < 0)
					{
						found = 0;
					}
				}
				else
				{
					found = 0;
				}
			}
			comboBoxRouting.SetCurSel(found);
		}
		else
		{
			comboBoxRouting.SetCurSel(0);
		}
	}
	else
	{
		comboBoxRouting.SetCurSel(0);
	}
}

const DestinationRouting* BaseDialogSetting::OnSelchangeRoutingStatic(ComboBoxRouting& comboBoxRouting, UnsignedNumberTypeSpin<unsigned int>& spinReserveSize, CStatic& staticReserverSize, unsigned int* routingName)
{
	int sel = comboBoxRouting.GetCurSel();
	if(sel >= 0)
	{
		const DestinationRouting* routing = (const DestinationRouting*)comboBoxRouting.GetItemDataPtr(sel);
		if(routing)
		{
			//comboBoxRouting.SetToolTipText(routing->GetDescription().c_str());
			if(routingName)*routingName = routing->GetNumericName();
			unsigned int minReserve = routing->GetMinReserveQuantity();
			unsigned int maxReserve = routing->GetMaxReserveQuantity();
			spinReserveSize.SetMinMaxValue(minReserve, maxReserve);
			if(minReserve < maxReserve)
			{
				spinReserveSize.SetBkColorEnabled(GetSysColor(COLOR_WINDOW));
			}
			else
			{
				spinReserveSize.SetBkColorEnabled(m_visualBase->GetGrayColor());
			}
			const std::string& reserveName = routing->GetReserveName();
			staticReserverSize.SetWindowText(reserveName.empty() ? "Reserve" : reserveName.c_str());
		}
		else
		{
			//comboBoxRouting.SetToolTipText("No Routing");
			if(routingName)*routingName = 0;
			spinReserveSize.SetMinMaxValue(0, 0);
			spinReserveSize.SetBkColorEnabled(m_visualBase->GetGrayColor());
			staticReserverSize.SetWindowText("Reserve");
		}
		return routing;
	}
	else
	{
		//comboBoxRouting.SetToolTipText(NULL);
		if(routingName)*routingName = 0;
		spinReserveSize.SetMinMaxValue(0, 0);
		spinReserveSize.SetBkColorEnabled(m_visualBase->GetGrayColor());
		staticReserverSize.SetWindowText("Reserve");
		return NULL;
	}
}

void BaseDialogSetting::Entering()
{
	theApp.SetInfo(m_label.c_str());
}
