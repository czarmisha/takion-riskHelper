#include "stdafx.h"
//#include "TakionGui.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern TakionMainWnd* takionMainWnd;

#ifdef __cplusplus
extern "C"
{
#endif

bool WINAPI IsTraderIdValid(const char* traderId)
{
	return true;
}

enum MsColumnId : unsigned short
{
	MSA_IMB_NYSE_VAL_REGULATORY,
	MSA_IMB_NYSE_VAL_INFO,
	MSA_IMB_NYSE_DELTA_CLOSE_INFO,
	MSA_IMB_NYSE_RATE,
	MSA_IMB_NYSE_DELTA_PV,
	MSA_IMB_ARCA_NEAR_MINUS_FAR,
	MSA_POS_BORROW_SIZE,
	MSA_IMB_NYSE_RATE_926,
	MSA_IMB_NYSE_DELTA_845,

	MSA_Count
};

unsigned short WINAPI GetMsAdditionalColumnCount()
{
	return MSA_Count;
}

bool WINAPI MsUpdateNonCumulativeRowValue(RowValue* rowValue, unsigned short i, MarketSorterRowTotal* rowTotal, const Row* rowUpdated)
{
	return false;
}

bool WINAPI MsAddNonCumulativeRowValue(RowValue* rowValue, unsigned short i, MarketSorterRowTotal* rowTotal, const Row* rowToAdd)
{
	return MsUpdateNonCumulativeRowValue(rowValue, i, rowTotal, rowToAdd);
}

bool WINAPI MsRemoveNonCumulativeRowValue(RowValue* rowValue, unsigned short i, MarketSorterRowTotal* rowTotal, const Row* rowToRemove)
{
	return MsUpdateNonCumulativeRowValue(rowValue, i, rowTotal, rowToRemove);
}

bool WINAPI IsMsColumnPosition(unsigned short i)
{
	switch(i)
	{
//		case 15:
//		case 13:
//		case 7:
		case MSA_POS_BORROW_SIZE:
		return true;

		default:
		return false;
	}
}

void WINAPI ApplyMsPosition(MarketSorterRow* row, const Position* position)
{
	unsigned short builtInColumnCount = row->GetWindowSortable()->GetBuiltInColumnCount();
//	row->ApplyPosition(builtInColumnCount + 15, position);
//	row->ApplyPosition(builtInColumnCount + 13, position);
//	row->ApplyPosition(builtInColumnCount + 7, position);
	row->ApplyPosition(builtInColumnCount + MSA_POS_BORROW_SIZE, position);
}

void WINAPI InvalidateMsPositionColumns(const MarketSorterWindow* window)
{
	unsigned short builtInColumnCount = window->GetBuiltInColumnCount();
//	window->InvalidateDataAtColumn(builtInColumnCount + 15);
//	window->InvalidateDataAtColumn(builtInColumnCount + 13);
//	window->InvalidateDataAtColumn(builtInColumnCount + 7);
	window->InvalidateDataAtColumn(builtInColumnCount + MSA_POS_BORROW_SIZE);
}

void WINAPI InvalidateMsPositionTotals(const MarketSorterWindow* window)
{
	unsigned short builtInColumnCount = window->GetBuiltInColumnCount();
	CRect rowRect(window->GetTotalRowRect());
//	window->InvalidateCellIfVisible(rowRect, builtInColumnCount + 15);
//	window->InvalidateCellIfVisible(rowRect, builtInColumnCount + 13);
//	window->InvalidateCellIfVisible(rowRect, builtInColumnCount + 7);
	window->InvalidateCellIfVisible(rowRect, builtInColumnCount + MSA_POS_BORROW_SIZE);
}

void WINAPI UpdateMsPositionValues(MarketSorterRow* row)
{
	unsigned short builtInColumnCount = row->GetWindowSortable()->GetBuiltInColumnCount();
//	row->UpdateRowValue(builtInColumnCount + 15);
//	row->UpdateRowValue(builtInColumnCount + 13);
//	row->UpdateRowValue(builtInColumnCount + 7);
	row->UpdateRowValue(builtInColumnCount + MSA_POS_BORROW_SIZE);
}

void WINAPI AdjustMsPositionTotals(MarketSorterRow* row)
{
	RowWindowSortable* window = row->GetWindowSortable();
	RowTotal* totalRow = window->GetRowTotal();
	if(totalRow)
	{
		unsigned short builtInColumnCount = window->GetBuiltInColumnCount();
//		totalRow->AdjustRowValue(row, builtInColumnCount + 15);
//		totalRow->AdjustRowValue(row, builtInColumnCount + 13);
//		totalRow->AdjustRowValue(row, builtInColumnCount + 7);
		totalRow->AdjustRowValue(row, builtInColumnCount + MSA_POS_BORROW_SIZE);
	}
}

//RowValue* WINAPI CreateMsTotalRowValue(unsigned short i, MarketSorterRowTotal* rowTotal, const MarketSorterWindow* msWindow, bool& cumulative)
RowValue* WINAPI CreateMsTotalRowValue(unsigned short i, const MarketSorterWindow* msWindow, bool& cumulative)
{
	cumulative = true;
	switch(i)
	{
//		case 2:
//		case 3:
//		case 4:
		case MSA_IMB_NYSE_VAL_REGULATORY:
		case MSA_IMB_NYSE_VAL_INFO:
//		case 2:
		return new MoneyRowValue(msWindow->isCommaMoney(), msWindow->GetDecDigits(), msWindow->isTruncateMoney(), true, true, false);

//		case 13:
//		case 7:
		case MSA_POS_BORROW_SIZE:
		return new UIntRowValue(msWindow->isCommaSize(), true, true);

		default:
		return nullptr;
	}
}

COLORREF WINAPI GetMsCellColor(unsigned short i, const MarketSorterRow* row)
{
	return 0xFFFFFFFF;
}

bool WINAPI AdditionalMsLock(const MarketSorterRow* row)
{
	return true;
}

void WINAPI AdditionalMsLockWait(const MarketSorterRow* row)
{
}

void WINAPI AdditionalMsUnlock(const MarketSorterRow* row)
{
}

RowValue* WINAPI CreateMsRowValue(unsigned short i, MarketSorterRow* row, const MarketSorterWindow* msWindow, bool& dynamic)
{
	dynamic = true;
	switch(i)
	{
//		case 0:
//		return new SecurityPriceAsUIntByPriceDivision(row->GetSecurity(), msWindow->isCommaPrice(), msWindow->GetDecDigits(), msWindow->isTruncatePrice(), &Security::GetL1Bid, &Security::GetClosePrice, true);

//		case 1:
//		return new SecurityPriceAsUIntByPriceDivision(row->GetSecurity(), msWindow->isCommaPrice(), msWindow->GetDecDigits(), msWindow->isTruncatePrice(), &Security::GetL1Ask, &Security::GetClosePrice, true);

//		case 2:
		case MSA_IMB_NYSE_VAL_REGULATORY:
		return new SecurityPriceXIntAbs(row->GetSecurity(), msWindow->isCommaMoney(), msWindow->GetDecDigits(), msWindow->isTruncateMoney(), &Security::GetRegulatoryCurrentReferencePriceNyse, &Security::GetRegulatoryImbalanceSharesNyse);

//		case 3:
		case MSA_IMB_NYSE_VAL_INFO:
		return new SecurityPriceXIntAbs(row->GetSecurity(), msWindow->isCommaMoney(), msWindow->GetDecDigits(), msWindow->isTruncateMoney(), &Security::GetCurrentReferencePriceNyse, &Security::GetImbalanceSharesNyse);

//		case 4:
//		case 2:
//		return new SecurityPriceRowValueDiffAsUIntXIntAbs(row->GetSecurity(), msWindow->isCommaMoney(), msWindow->GetDecDigitsMoney(), msWindow->isTruncateMoney(), &Security::GetNearPriceNyse, &Security::GetLastPrice, &Security::GetImbalanceSharesNyse, true, true);

//		case 5:
//		case 3:
		case MSA_IMB_NYSE_DELTA_CLOSE_INFO:
		return new SecurityPriceRowValueDiffAbs(row->GetSecurity(), msWindow->isCommaPrice(), msWindow->GetDecDigits(), msWindow->isTruncatePrice(), &Security::GetCurrentReferencePriceNyse, &Security::GetClosePrice, true);

//		case 6:
//		return new SecurityPriceDiffByPriceAsUIntDivision(row->GetSecurity(), msWindow->isCommaPrice(), msWindow->GetDecDigits(), msWindow->isTruncatePrice(), &Security::GetL1Bid, &Security::GetTodaysClosePrice, true);

//		case 7:
//		return new SecurityPriceDiffByPriceAsUIntDivision(row->GetSecurity(), msWindow->isCommaPrice(), msWindow->GetDecDigits(), msWindow->isTruncatePrice(), &Security::GetL1Ask, &Security::GetTodaysClosePrice, true);

//		case 8:
//		case 4:
		case MSA_IMB_NYSE_RATE:
		return new SecurityIntByIntDivisionFraction(row->GetSecurity(), msWindow->isCommaPrice(), msWindow->GetDecDigits(), msWindow->isTruncatePrice(), &Security::GetImbalanceSharesNyse, &Security::GetInitialImbalanceSharesNyse);//, bool useEmptyValue = true, bool useTick = true, bool useNeutralColor = false):

//		case 9:
//		case 5:
		case MSA_IMB_NYSE_DELTA_PV:
		return new SecurityUIntByIntDivisionFraction(row->GetSecurity(), msWindow->isCommaPrice(), msWindow->GetDecDigits(), msWindow->isTruncatePrice(), &Security::GetPairedSharesNyse, &Security::GetImbalanceSharesNyse);//, bool useEmptyValue = true, bool useTick = true, bool useNeutralColor = false):

//		case 10:
//		return new SecurityMoneyRowValueAverage64PriceAsUIntDiff(row->GetSecurity(), msWindow->isCommaPrice(), msWindow->GetDecDigits(), msWindow->isTruncatePrice(), &Security::GetMoneyTraded, &Security::GetVolume, &Security::GetLastPrice); 

//		case 11:
//		return new SecurityMoneyRowValueAverage64PriceAsUIntDiffPercent(row->GetSecurity(), msWindow->isCommaPrice(), msWindow->GetDecDigits(), msWindow->isTruncatePrice(), &Security::GetMoneyTraded, &Security::GetVolume, &Security::GetLastPrice); 

//		case 12:
//		case 6:
		case MSA_IMB_ARCA_NEAR_MINUS_FAR:
		return new SecurityPriceRowValueDiffAbs(row->GetSecurity(), msWindow->isCommaPrice(), msWindow->GetDecDigits(), msWindow->isTruncatePrice(), &Security::GetNearPriceArca, &Security::GetFarPriceArca, true, true);

//		case 13:
//		case 7:
		case MSA_POS_BORROW_SIZE:
		return new PositionUIntRowValueNull(row->GetPosition(), msWindow->isCommaSize(), &Position::GetBorrowSize, true, true);

//		case 14:
//		case 8:
		case MSA_IMB_NYSE_RATE_926:
		return new SecurityIntByIntDivisionFraction(row->GetSecurity(), msWindow->isCommaPrice(), msWindow->GetDecDigits(), msWindow->isTruncatePrice(), &Security::GetOpeningImbalanceSharesNyse, &Security::GetT926ImbalanceSharesNyse);//, bool useEmptyValue = true, bool useTick = true, bool useNeutralColor = false):

//		case 15:
//		case 9:
		case MSA_IMB_NYSE_DELTA_845:
		return new SecurityIntRowValueDiff(row->GetSecurity(), msWindow->isCommaVolume(), &Security::GetOpeningImbalanceSharesNyse, &Security::GetT845ImbalanceSharesNyse, true);//, bool useEmptyValue = true, bool useTick = true, bool useNeutralColor = false):

		default:
		return nullptr;
	}
}

bool WINAPI GetMsColumnInfo(unsigned short i, const char*& columnName, int& width, unsigned char& useTicks, bool& sortable, bool& defaultSortAscending, const char*& columnTooltip, unsigned char& followable, UINT& defaultHorizontalJustification)
{
	switch(i)
	{
/*
		case 0:
		columnName = "%Bid";
		width = 70;
		sortable = true;
		defaultSortAscending = false;
		columnTooltip = "(Bid / ClosePrice) * 100%";
		useTicks = 1;
		followable = 1;
		defaultHorizontalJustification = DT_RIGHT;
		break;

		case 1:
		columnName = "%Ask";
		width = 70;
		sortable = true;
		defaultSortAscending = false;
		columnTooltip = "(Ask / ClosePrice) * 100%";
		useTicks = 1;
		followable = 1;
		defaultHorizontalJustification = DT_RIGHT;
		break;
*/
//		case 2:
		case MSA_IMB_NYSE_VAL_REGULATORY:
		columnName = "ImbNYval";
		width = 80;
		sortable = true;
		defaultSortAscending = false;
		columnTooltip = "abs(NyseImbCurrentRefPriceRegulatory * NyseImbRegulatory)";
		useTicks = 1;
		followable = 2;
		defaultHorizontalJustification = DT_RIGHT;
		break;

//		case 3:
		case MSA_IMB_NYSE_VAL_INFO:
		columnName = "ImbNYvalInf";
		width = 80;
		sortable = true;
		defaultSortAscending = false;
		columnTooltip = "abs(NyseImbCurrentRefPriceInfo * NyseImbInfo)";
		useTicks = 1;
		followable = 2;
		defaultHorizontalJustification = DT_RIGHT;
		break;
/*
//		case 4:
		case 2:
		columnName = "ImbNYdeltaValInf";
		width = 80;
		sortable = true;
		defaultSortAscending = false;
		columnTooltip = "Absolute value of the Difference between NYSE Informational Clearing Price and Last Print Price multiplied by the Imbalance Shares";// columnName;
		useTicks = 1;
		followable = 1;
		defaultHorizontalJustification = DT_RIGHT;
		break;
*/
//		case 5:
//		case 3:
		case MSA_IMB_NYSE_DELTA_CLOSE_INFO:
		columnName = "ImbNYdeltaClsInf";
//		return new SecurityPriceRowValueDiffAbs(row->GetSecurity(), msWindow->isCommaPrice(), msWindow->GetDecDigits(), msWindow->isTruncatePrice(), &Security::GetCurrentReferencePriceNyse, &Security::GetClosePrice, true);
		width = 70;
		sortable = true;
		defaultSortAscending = false;
		columnTooltip = "Absolute Difference between NYSE CurrentReferencePrice and Yesterda's Close Price";
		useTicks = 1;
		followable = 1;
		defaultHorizontalJustification = DT_RIGHT;
		break;
/*
		case 6:
		columnName = "%TBidNet";
		width = 70;
		sortable = true;
		defaultSortAscending = false;
		columnTooltip = "(Bid - TodaysClosePrice) / TodaysClosePrice * 100%";
		useTicks = 1;
		followable = 2;
		defaultHorizontalJustification = DT_RIGHT;
		break;

		case 7:
		columnName = "%TAskNet";
		width = 70;
		sortable = true;
		defaultSortAscending = false;
		columnTooltip = "(Ask - TodaysClosePrice) / TodaysClosePrice * 100%";
		useTicks = 1;
		followable = 2;
		defaultHorizontalJustification = DT_RIGHT;
		break;
*/
//		case 8:
//		case 4:
		case MSA_IMB_NYSE_RATE:
		columnName = "RateImbNY";
		width = 70;
		sortable = true;
		defaultSortAscending = false;
		columnTooltip = "ImbNYinf / iImbNY";
		useTicks = 1;
		followable = 2;
		defaultHorizontalJustification = DT_RIGHT;
		break;

//		case 9:
//		case 5:
		case MSA_IMB_NYSE_DELTA_PV:
		columnName = "DeltaPvNY";
		width = 70;
		sortable = true;
		defaultSortAscending = false;
		columnTooltip = "ImbNYmatchinf/ImbNYinf";
		useTicks = 1;
		followable = 1;
		defaultHorizontalJustification = DT_RIGHT;
		break;
/*
		case 10:
		columnName = "VWAPdeltaDay";
		width = 70;
		sortable = true;
		defaultSortAscending = false;
		columnTooltip = "Last Price - DayVWAP";
		useTicks = 1;
		followable = 2;//can be followed
		defaultHorizontalJustification = DT_RIGHT;
		break;

		case 11:
		columnName = "%VWAPdeltaDay";
		width = 70;
		sortable = true;
		defaultSortAscending = false;
		columnTooltip = "(Last Price - DayVWAP) / DayVWAP";
		useTicks = 1;
		followable = 2;//can be followed
		defaultHorizontalJustification = DT_RIGHT;
		break;
*/
//		case 12:
//		case 6:
		case MSA_IMB_ARCA_NEAR_MINUS_FAR:
		columnName = "ArcaNear-Far";
		width = 70;
		sortable = true;
		defaultSortAscending = false;
		columnTooltip = "Arca Near Price - Arca Far Price";
		useTicks = 1;
		followable = 2;//can be followed
		defaultHorizontalJustification = DT_RIGHT;
		break;

//		case 13:
//		case 7:
		case MSA_POS_BORROW_SIZE:
		columnName = "BorrowSize";
		width = 60;
		sortable = true;
		defaultSortAscending = false;
		columnTooltip = "Position Borrowed Shares";
		useTicks = 1;
		followable = 2;//can be followed
		defaultHorizontalJustification = DT_RIGHT;
		break;

//		case 14:
//		case 8:
		case MSA_IMB_NYSE_RATE_926:
		columnName = "9:26RateImbNY";
		width = 70;
		sortable = true;
		defaultSortAscending = false;
		columnTooltip = "ImbNYinf / 9:26ImbNY; Current NYSE imbalance divided by NYSE imbalance at 9:26";
		useTicks = 1;
		followable = 2;
		defaultHorizontalJustification = DT_RIGHT;
		break;

//		case 15:
//		case 9:
		case MSA_IMB_NYSE_DELTA_845:
		columnName = "8:45ImbDeltaNY";
		width = 70;
		sortable = true;
		defaultSortAscending = false;
		columnTooltip = "Difference between the Current NYSE Info Imbalance and the Imbalance at 8:45";
		useTicks = 1;
		followable = 1;
		defaultHorizontalJustification = DT_RIGHT;
		break;

		default:
		return false;
	}
	return true;
}

#ifdef _DEBUG
//#define NIGHT_COLUMNS
#endif
//////////////////
//PositionWindow
enum PosColumnId : unsigned short
{
	POSA_IMB_NYSE_INFO_REGULATORY_PERCENT,
	POSA_IMB_NYSE_CLEARING_PRICE_REGULATORY,
	POSA_IMB_NYSE_CLEARING_PRICE_INFORMATIONAL,
	POSA_IMB_NYSE_MATCHING_REGULATORY,
	POSA_HTB,
	POSA_TKT_AVG_100,
	POSA_TKT_AVG_100_NET,
	POSA_TKT_AVG_1,
	POSA_TKT_AVG_1_NET,
	POSA_SIZE_TIER_FRACTION,
	POSA_SIZE_TIER_FRACTION_THIRD,
	POSA_SIZE_TEMPORARY_TIER_FRACTION,
	POSA_SIZE_TEMPORARY_TIER_FRACTION_THIRD,

#ifdef NIGHT_COLUMNS
	POSA_NIGHT_PENDING_MONEY_LONG,
	POSA_NIGHT_PENDING_MONEY_SHORT,
	POSA_NIGHT_PENDING_SIZE_LONG,
	POSA_NIGHT_PENDING_SIZE_SHORT,
	POSA_NIGHT_POTENTIAL_SIZE_LONG,
	POSA_NIGHT_POTENTIAL_SIZE_SHORT,
	POSA_NIGHT_POTENTIAL_SIZE,
	POSA_NIGHT_POTENTIAL_VALUE_LONG,
	POSA_NIGHT_POTENTIAL_VALUE_SHORT,
	POSA_NIGHT_POTENTIAL_VALUE,
#endif

	POSA_Count
};

unsigned short WINAPI GetPosAdditionalColumnCount()
{
	return POSA_Count;
}

bool WINAPI PosUpdateNonCumulativeRowValue(RowValue* rowValue, unsigned short i, PositionRowTotal* rowTotal, const Row* rowUpdated)
{
	switch(i)
	{
//		case 12:
//		case 11:
		case POSA_TKT_AVG_100:
		{
			const unsigned int& sharesTradedValue = rowTotal->GetSharesTradedPtr()->GetValue();
			return ((MoneyRowValue*)rowValue)->SetValue(sharesTradedValue ? rowTotal->GetPnlClosedPtr()->GetValue() * 100 / sharesTradedValue : Money::moneyZero);
		}
		break;

//		case 12:
		case POSA_TKT_AVG_100_NET:
		{
			const unsigned int& sharesTradedValue = rowTotal->GetSharesTradedPtr()->GetValue();
			return ((MoneyRowValue*)rowValue)->SetValue(sharesTradedValue ? rowTotal->GetPnlClosedNetPtr()->GetValue() * 100 / sharesTradedValue : Money::moneyZero);
		}
		break;

//		case 13:
		case POSA_TKT_AVG_1:
		{
			const unsigned int& sharesTradedValue = rowTotal->GetSharesTradedPtr()->GetValue();
			return ((MoneyRowValue*)rowValue)->SetValue(sharesTradedValue ? rowTotal->GetPnlClosedPtr()->GetValue() / sharesTradedValue : Money::moneyZero);
		}
		break;

//		case 14:
		case POSA_TKT_AVG_1_NET:
		{
			const unsigned int& sharesTradedValue = rowTotal->GetSharesTradedPtr()->GetValue();
			return ((MoneyRowValue*)rowValue)->SetValue(sharesTradedValue ? rowTotal->GetPnlClosedNetPtr()->GetValue() / sharesTradedValue : Money::moneyZero);
		}
		break;
	}
	return false;
}

bool WINAPI PosAddNonCumulativeRowValue(RowValue* rowValue, unsigned short i, PositionRowTotal* rowTotal, const Row* rowToAdd)
{
	return PosUpdateNonCumulativeRowValue(rowValue, i, rowTotal, rowToAdd);
}

bool WINAPI PosRemoveNonCumulativeRowValue(RowValue* rowValue, unsigned short i, PositionRowTotal* rowTotal, const Row* rowToRemove)
{
	return PosUpdateNonCumulativeRowValue(rowValue, i, rowTotal, rowToRemove);
}

RowValue* WINAPI CreatePosTotalRowValue(unsigned short i, const RowWindowSortable* posWindow, bool& cumulative)
{
	cumulative = true;
	switch(i)
	{
//		case 6:
//		return new IntRowValue(posWindow->isCommaVolume(), true, true);

		case POSA_TKT_AVG_100:
		case POSA_TKT_AVG_100_NET:
		case POSA_TKT_AVG_1:
		case POSA_TKT_AVG_1_NET:
		cumulative = false;
		return new MoneyRowValue(posWindow->isCommaMoney(), posWindow->GetDecDigitsMoney(), posWindow->isTruncateMoney());

#ifdef NIGHT_COLUMNS
		case POSA_NIGHT_PENDING_MONEY_LONG:
		case POSA_NIGHT_PENDING_MONEY_SHORT:
		case POSA_NIGHT_POTENTIAL_VALUE_LONG:
		case POSA_NIGHT_POTENTIAL_VALUE_SHORT:
		case POSA_NIGHT_POTENTIAL_VALUE:
		return new MoneyRowValue(posWindow->isCommaMoney(), posWindow->GetDecDigits(), posWindow->isTruncateMoney(), true, true);

		case POSA_NIGHT_PENDING_SIZE_LONG:
		case POSA_NIGHT_PENDING_SIZE_SHORT:
		case POSA_NIGHT_POTENTIAL_SIZE_LONG:
		case POSA_NIGHT_POTENTIAL_SIZE_SHORT:
		case POSA_NIGHT_POTENTIAL_SIZE:
		return new UIntRowValue(posWindow->isCommaSize());
#endif
		default:
		return nullptr;
	}
}

COLORREF WINAPI GetPosCellColor(unsigned short i, const PositionRow* row)
{
	switch(i)
	{
		case POSA_HTB:
		{
			const unsigned int ordinal = takionMainWnd->GetCurrentClearingFirmOrdinal();
			const Security* security = row->GetSecurity();
//			const PositionWindow* window = row->GetPositionWindow();
			const RowWindowSortable* window = row->GetWindowSortable();//GetPositionWindow();
			return security ?
				security->isHTB(ordinal) ? security->GetBorrowSize(ordinal) ? window->GetWarningColor() : window->GetNegativeColor() : window->GetPositiveColor():
				window->GetNeutralColor();
		}
		break;

		case POSA_SIZE_TIER_FRACTION:
		case POSA_SIZE_TIER_FRACTION_THIRD:
		case POSA_SIZE_TEMPORARY_TIER_FRACTION:
		case POSA_SIZE_TEMPORARY_TIER_FRACTION_THIRD:
		{
			const RowWindowSortable* window = row->GetWindowSortable();//GetPositionWindow();
			if(window->isShowPlusMinusColorProper())
			{
				const PositionData* position = row->GetPosition();
				const int posSize = position->GetSizeInv(window->isInventoryView());
				return posSize > 0 ? window->GetPositiveColor() : posSize < 0 ? window->GetNegativeColor() : window->GetNeutralColor();
			}
		}
		break;
	}
	return 0xFFFFFFFF;
}

RowValue* WINAPI CreatePosRowValue(unsigned short i, PositionRow* row, const RowWindowSortable* posWindow, bool& dynamic)
{
	dynamic = true;
	switch(i)
	{
//		case 0:
//		return new PositionSecurityIntByUInt64Division(row->GetSecurityRef(), posWindow->isCommaPrice(), posWindow->GetDecDigits(), posWindow->isTruncatePrice(), &Security::GetImbalanceSharesNyse, &Security::GetVolume);

//		case 1:
//		return new PositionSecurityPriceRowValueDiffAsUIntReversed(row->GetSecurityRef(), posWindow->isCommaPrice(), posWindow->GetDecDigits(), posWindow->isTruncatePrice(), &Security::GetNearPriceNyse, &Security::GetLastPrice, true, true);

		case POSA_IMB_NYSE_INFO_REGULATORY_PERCENT:
		return new PositionSecurityIntByIntDivision(row->GetSecurityRef(), posWindow->isCommaPrice(), posWindow->GetDecDigits(), posWindow->isTruncatePrice(), &Security::GetImbalanceSharesNyse, &Security::GetRegulatoryImbalanceSharesNyse);

		case POSA_IMB_NYSE_CLEARING_PRICE_REGULATORY:
		return new PositionSecurityPriceRowValue(row->GetSecurityRef(), posWindow->isCommaPrice(), posWindow->GetDecDigits(), posWindow->isTruncatePrice(), &Security::GetRegulatoryNearPriceNyse);

		case POSA_IMB_NYSE_CLEARING_PRICE_INFORMATIONAL:
		return new PositionSecurityPriceRowValue(row->GetSecurityRef(), posWindow->isCommaPrice(), posWindow->GetDecDigits(), posWindow->isTruncatePrice(), &Security::GetNearPriceNyse);

		case POSA_IMB_NYSE_MATCHING_REGULATORY:
		return new PositionSecurityUIntRowValue(row->GetSecurityRef(), posWindow->isCommaVolume(), &Security::GetRegulatoryPairedSharesNyse);

//		case 6:
//		return new PositionSecurityIntRowValue(row->GetSecurityRef(), posWindow->isCommaVolume(), &Security::GetImbalanceSharesNsdq, true);

		case POSA_HTB:
//		return new PositionSecurityBoolOrdinalRowValue(row->GetSecurityRef(), &Security::isHtb, takionMainWnd->GetCurrentClearingFirmOrdinal());
		return new PositionSecurityUCharAsCharOrdinalRowValue(row->GetSecurityRef(), &Security::isHTB, tolower(StockLoanInfo::regularHtb), takionMainWnd->GetCurrentClearingFirmOrdinal(), false);

//		case 7:
//		return new PositionSecurityCompactPriceRowValue(row->GetSecurityRef(), posWindow->isCommaPrice(), posWindow->GetDecDigits(), posWindow->isTruncatePrice(), &Security::GetPrimaryQOpenPriceCompact);

//		case 8:
//		return new PositionSecurityCompactPriceRowValue(row->GetSecurityRef(), posWindow->isCommaPrice(), posWindow->GetDecDigits(), posWindow->isTruncatePrice(), &Security::GetTodaysClosePriceCompact);

//		case 9:
//		return new PositionSecurityCompactPriceRowValue(row->GetSecurityRef(), posWindow->isCommaPrice(), posWindow->GetDecDigits(), posWindow->isTruncatePrice(), &Security::GetL2Bid);

//		case 10:
//		return new PositionSecurityCompactPriceRowValue(row->GetSecurityRef(), posWindow->isCommaPrice(), posWindow->GetDecDigits(), posWindow->isTruncatePrice(), &Security::GetL2Ask);

		case POSA_TKT_AVG_100:
		return new PositionMoneyPerShareInventoryRowValue(row->GetPosition(), posWindow->isInventoryView(), posWindow->isCommaMoney(), posWindow->GetDecDigitsMoney(), posWindow->isTruncateMoney(), &Position::GetClosedPnlInv, &Position::GetSharesTraded, 100);//, bool useEmptyValue = true, bool useNeutralColor = false, bool useTick = true):

		case POSA_TKT_AVG_100_NET:
		return new PositionMoneyPerShareInventoryRowValue(row->GetPosition(), posWindow->isInventoryView(), posWindow->isCommaMoney(), posWindow->GetDecDigitsMoney(), posWindow->isTruncateMoney(), &Position::GetClosedPnlNetInv, &Position::GetSharesTraded, 100);//, bool useEmptyValue = true, bool useNeutralColor = false, bool useTick = true):

		case POSA_TKT_AVG_1:
		return new PositionMoneyPerShareInventoryRowValue(row->GetPosition(), posWindow->isInventoryView(), posWindow->isCommaMoney(), posWindow->GetDecDigitsMoney(), posWindow->isTruncateMoney(), &Position::GetClosedPnlInv, &Position::GetSharesTraded, 1);//, bool useEmptyValue = true, bool useNeutralColor = false, bool useTick = true):

		case POSA_TKT_AVG_1_NET:
		return new PositionMoneyPerShareInventoryRowValue(row->GetPosition(), posWindow->isInventoryView(), posWindow->isCommaMoney(), posWindow->GetDecDigitsMoney(), posWindow->isTruncateMoney(), &Position::GetClosedPnlNetInv, &Position::GetSharesTraded, 1);//, bool useEmptyValue = true, bool useNeutralColor = false, bool useTick = true):

		case POSA_SIZE_TIER_FRACTION:
		return new PositionSecurityUIntDevidedByUIntInventoryRowValue(row->GetPosition(), row->GetSecurityRef(), posWindow->isInventoryView(), posWindow->isCommaPrice(), posWindow->GetDecDigits(), posWindow->isTruncatePrice(), &Position::GetAbsSizeInv, &Security::GetTierSize, 1);//, bool useEmptyValue = true, bool useNeutralColor = false, bool useTick = true):

		case POSA_SIZE_TIER_FRACTION_THIRD:
		return new PositionSecurityUIntDevidedByUIntInventoryRowValue(row->GetPosition(), row->GetSecurityRef(), posWindow->isInventoryView(), posWindow->isCommaPrice(), posWindow->GetDecDigits(), posWindow->isTruncatePrice(), &Position::GetAbsSizeInv, &Security::GetTierSize, 3);//, bool useEmptyValue = true, bool useNeutralColor = false, bool useTick = true):

		case POSA_SIZE_TEMPORARY_TIER_FRACTION:
		return new PositionSecurityUIntDevidedByUIntAlternateInventoryRowValue(row->GetPosition(), row->GetSecurityRef(), posWindow->isInventoryView(), posWindow->isCommaPrice(), posWindow->GetDecDigits(), posWindow->isTruncatePrice(), &Position::GetAbsSizeInv, &Security::GetTemporaryTierSize, &Security::GetTierSize, 1);//, bool useEmptyValue = true, bool useNeutralColor = false, bool useTick = true):

		case POSA_SIZE_TEMPORARY_TIER_FRACTION_THIRD:
		return new PositionSecurityUIntDevidedByUIntAlternateInventoryRowValue(row->GetPosition(), row->GetSecurityRef(), posWindow->isInventoryView(), posWindow->isCommaPrice(), posWindow->GetDecDigits(), posWindow->isTruncatePrice(), &Position::GetAbsSizeInv, &Security::GetTemporaryTierSize, &Security::GetTierSize, 3);//, bool useEmptyValue = true, bool useNeutralColor = false, bool useTick = true):

#ifdef NIGHT_COLUMNS
		case POSA_NIGHT_PENDING_MONEY_LONG://PositionWindow::POS_NIGHT_MONEY_PEND_LONG:
		return new PositionMoneyRowValue(row->GetPosition(), posWindow->isCommaMoney(), posWindow->GetDecDigits(), posWindow->isTruncateMoney(), &Position::GetPendingBuyNightInvestment, true, true);
		break;

		case POSA_NIGHT_PENDING_MONEY_SHORT://PositionWindow::POS_NIGHT_MONEY_PEND_SHORT:
		return new PositionMoneyRowValue(row->GetPosition(), posWindow->isCommaMoney(), posWindow->GetDecDigits(), posWindow->isTruncateMoney(), &Position::GetPendingSellAndShortNightInvestment, true, true);
		break;

		case POSA_NIGHT_PENDING_SIZE_LONG://PositionWindow::POS_NIGHT_SIZE_PEND_LONG:
		return new PositionUIntRowValue(row->GetPosition(), posWindow->isCommaSize(), &Position::GetPendingBuyNightShares);
		break;

		case POSA_NIGHT_PENDING_SIZE_SHORT://PositionWindow::POS_NIGHT_SIZE_PEND_SHORT:
		return new PositionUIntRowValue(row->GetPosition(), posWindow->isCommaSize(), &Position::GetPendingSellAndShortNightShares);
		break;

		case POSA_NIGHT_POTENTIAL_SIZE_LONG://PositionWindow::POS_NIGHT_SIZE_PTL_LONG:
		return new PositionUIntRowValue(row->GetPosition(), posWindow->isCommaSize(), &Position::GetPotentialLongNightSize);
		break;

		case POSA_NIGHT_POTENTIAL_SIZE_SHORT://PositionWindow::POS_NIGHT_SIZE_PTL_SHORT:
		return new PositionUIntRowValue(row->GetPosition(), posWindow->isCommaSize(), &Position::GetPotentialShortNightSize);
		break;

		case POSA_NIGHT_POTENTIAL_SIZE://PositionWindow::POS_NIGHT_SIZE_PTL:
		return new PositionUIntRowValue(row->GetPosition(), posWindow->isCommaSize(), &Position::GetPotentialNightSize);
		break;

		case POSA_NIGHT_POTENTIAL_VALUE_LONG://PositionWindow::POS_NIGHT_VALUE_PTL_LONG:
		return new PositionMoneyRowValue(row->GetPosition(), posWindow->isCommaMoney(), posWindow->GetDecDigits(), posWindow->isTruncateMoney(), &Position::GetPotentialLongNightValue, true, true);
		break;

		case POSA_NIGHT_POTENTIAL_VALUE_SHORT://PositionWindow::POS_NIGHT_VALUE_PTL_SHORT:
		return new PositionMoneyRowValue(row->GetPosition(), posWindow->isCommaMoney(), posWindow->GetDecDigits(), posWindow->isTruncateMoney(), &Position::GetPotentialShortNightValue, true, true);
		break;

		case POSA_NIGHT_POTENTIAL_VALUE://PositionWindow::POS_NIGHT_BP_USED:
		return new PositionMoneyRowValue(row->GetPosition(), posWindow->isCommaMoney(), posWindow->GetDecDigits(), posWindow->isTruncateMoney(), &Position::GetNightBpUsed, true, true);
		break;
#endif

		default:
		return nullptr;
	}
}

bool WINAPI GetPosColumnInfo(unsigned short i, const char*& columnName, int& width, unsigned char& useTicks, bool& sortable, bool& defaultSortAscending, const char*& columnTooltip, unsigned char& followable, UINT& defaultHorizontalJustification)
{
//	return false;
	width = 60;
	sortable = true;
	defaultSortAscending = false;
	useTicks = 1;
	followable = 1;
	defaultHorizontalJustification = DT_RIGHT;

	switch(i)
	{
/*
		case 0:
		columnName = "ImbNYvol%";
		columnTooltip = "Percentage of NYSE informational imbalance relative to the Day Volume";
		break;

		case 1:
		columnName = "ImbNYdeltaInf";
		columnTooltip = "Difference between NYSE Near Prise and Last Print Price";
		break;
*/
		case POSA_IMB_NYSE_INFO_REGULATORY_PERCENT:
		columnName = "Imb%RI";
		columnTooltip = "Percentage of NYSE Informational Imbalance relative to Regulatory Imbalance";
		followable = 2;
		break;

		case POSA_IMB_NYSE_CLEARING_PRICE_REGULATORY:
		columnName = "ImbNYclr";
		columnTooltip = "NYSE Regulatory Clearing Price";
		break;

		case POSA_IMB_NYSE_CLEARING_PRICE_INFORMATIONAL:
		columnName = "ImbNYclrInf";
		columnTooltip = "NYSE Informational Clearing Price";
		break;

		case POSA_IMB_NYSE_MATCHING_REGULATORY:
		columnName = "ImbNYmatch";
		columnTooltip = "NYSE Regulatory Matching Shares";
		break;
/*
		case 6:
		columnName = "ImbNQ";
		columnTooltip = "NSDQ Imbalance";
		followable = 2;
		break;
*/
		case POSA_HTB:
		columnName = "HTBstock";
		width = 30;
		defaultSortAscending = true;
		columnTooltip = "Stock in the current Clearing Account is Hard to Borrow";
		useTicks = 0;
		followable = 2;
		defaultHorizontalJustification = DT_CENTER;
		break;
/*
		case 7:
		columnName = "PQOpen";
		columnTooltip = "Primary Exchange Opening Print Price";
		break;

		case 8:
		columnName = "TClose";
		columnTooltip = "Todays Closing Price";
		break;

		case 9:
		columnName = "L2Bid";
		columnTooltip = "Level2 Bid Price";
		break;

		case 10:
		columnName = "L2Ask";
		columnTooltip = "Level2 Ask Price";
		break;
*/
		case POSA_TKT_AVG_100:
		columnName = "TktAvg100";
		columnTooltip = "Closed P&L per 100 shares";
		break;

		case POSA_TKT_AVG_100_NET:
		columnName = "TktAvgNet100";
		columnTooltip = "Net Closed P&L per 100 shares";
		break;

		case POSA_TKT_AVG_1:
		columnName = "TktAvg1";
		columnTooltip = "Closed P&L per 1 share";
		break;

		case POSA_TKT_AVG_1_NET:
		columnName = "TktAvgNet1";
		columnTooltip = "Net Closed P&L per 1 share";
		break;

		case POSA_SIZE_TIER_FRACTION:
		columnName = "TierFract";
		columnTooltip = "Position Size / Tier Size";
		break;

		case POSA_SIZE_TIER_FRACTION_THIRD:
		columnName = "PosFract";
		columnTooltip = "Position Size / (Tier Size * 3)";
		break;

		case POSA_SIZE_TEMPORARY_TIER_FRACTION:
		columnName = "TierFractTemp";
		columnTooltip = "Position Size / Temporary Tier Size";
		break;

		case POSA_SIZE_TEMPORARY_TIER_FRACTION_THIRD:
		columnName = "PosFractTemp";
		columnTooltip = "Position Size / (Temporary Tier Size * 3)";
		break;
#ifdef NIGHT_COLUMNS
		case POSA_NIGHT_PENDING_MONEY_LONG:
		columnName = "pNtMoney Long";
		columnTooltip = columnName;
		break;

		case POSA_NIGHT_PENDING_MONEY_SHORT:
		columnName = "pNtMoney Short";
		columnTooltip = columnName;
		break;

		case POSA_NIGHT_PENDING_SIZE_LONG:
		columnName = "pNtSize Long";
		columnTooltip = columnName;
		break;

		case POSA_NIGHT_PENDING_SIZE_SHORT:
		columnName = "pNtSize Short";
		columnTooltip = columnName;
		break;

		case POSA_NIGHT_POTENTIAL_SIZE_LONG:
		columnName = "LongNt Ptl Size";
		columnTooltip = columnName;
		break;

		case POSA_NIGHT_POTENTIAL_SIZE_SHORT:
		columnName = "ShortNt Ptl Size";
		columnTooltip = columnName;
		break;

		case POSA_NIGHT_POTENTIAL_SIZE:
		columnName = "PtlNt Size";
		columnTooltip = columnName;
		break;

		case POSA_NIGHT_POTENTIAL_VALUE_LONG:
		columnName = "LongNt Ptl Value";
		columnTooltip = columnName;
		break;

		case POSA_NIGHT_POTENTIAL_VALUE_SHORT:
		columnName = "ShortNt Ptl Value";
		columnTooltip = columnName;
		break;

		case POSA_NIGHT_POTENTIAL_VALUE:
		columnName = "BpNt Used";
		columnTooltip = columnName;
		break;
#endif
		default:
		return false;
	}
	return true;
}
////////////////////////
//AccountWindow
enum ActColumnId : unsigned short
{
	ACTA_TKT_AVG_100,
	ACTA_TKT_AVG_100_NET,
	ACTA_TKT_AVG_1,
	ACTA_TKT_AVG_1_NET,

#ifdef NIGHT_COLUMNS
	ACTA_NIGHT_POTENTIAL_SIZE_LONG,
	ACTA_NIGHT_POTENTIAL_SIZE_SHORT,
	ACTA_NIGHT_POTENTIAL_SIZE,
	ACTA_NIGHT_POTENTIAL_VALUE_LONG,
	ACTA_NIGHT_POTENTIAL_VALUE_SHORT,
	ACTA_NIGHT_POTENTIAL_VALUE,
#endif

	ACTA_Count
};

unsigned short WINAPI GetActAdditionalColumnCount()
{
	return ACTA_Count;
}

bool WINAPI ActUpdateNonCumulativeRowValue(RowValue* rowValue, unsigned short i, AccountRowTotal* rowTotal, const Row* rowUpdated)
{
	switch(i)
	{
		case ACTA_TKT_AVG_100:
		{
			const unsigned int& sharesTradedValue = rowTotal->GetSharesTradedPtr()->GetValue();
			return ((MoneyRowValue*)rowValue)->SetValue(sharesTradedValue ? rowTotal->GetPnlClosedPtr()->GetValue() * 100 / sharesTradedValue : Money::moneyZero);
		}
		break;

		case ACTA_TKT_AVG_100_NET:
		{
			const unsigned int& sharesTradedValue = rowTotal->GetSharesTradedPtr()->GetValue();
			return ((MoneyRowValue*)rowValue)->SetValue(sharesTradedValue ? rowTotal->GetPnlClosedNetPtr()->GetValue() * 100 / sharesTradedValue : Money::moneyZero);
		}
		break;

		case ACTA_TKT_AVG_1:
		{
			const unsigned int& sharesTradedValue = rowTotal->GetSharesTradedPtr()->GetValue();
			return ((MoneyRowValue*)rowValue)->SetValue(sharesTradedValue ? rowTotal->GetPnlClosedPtr()->GetValue() / sharesTradedValue : Money::moneyZero);
		}
		break;

		case ACTA_TKT_AVG_1_NET:
		{
			const unsigned int& sharesTradedValue = rowTotal->GetSharesTradedPtr()->GetValue();
			return ((MoneyRowValue*)rowValue)->SetValue(sharesTradedValue ? rowTotal->GetPnlClosedNetPtr()->GetValue() / sharesTradedValue : Money::moneyZero);
		}
		break;
	}
	return false;
}

bool WINAPI ActAddNonCumulativeRowValue(RowValue* rowValue, unsigned short i, AccountRowTotal* rowTotal, const Row* rowToAdd)
{
	return ActUpdateNonCumulativeRowValue(rowValue, i, rowTotal, rowToAdd);
}

bool WINAPI ActRemoveNonCumulativeRowValue(RowValue* rowValue, unsigned short i, AccountRowTotal* rowTotal, const Row* rowToRemove)
{
	return ActUpdateNonCumulativeRowValue(rowValue, i, rowTotal, rowToRemove);
}

RowValue* WINAPI CreateActTotalRowValue(unsigned short i, const AccountWindow* actWindow, bool& cumulative)
{
	cumulative = true;
	switch(i)
	{
		case ACTA_TKT_AVG_100:
		case ACTA_TKT_AVG_100_NET:
		case ACTA_TKT_AVG_1:
		case ACTA_TKT_AVG_1_NET:
		cumulative = false;
		return new MoneyRowValue(actWindow->isCommaMoney(), actWindow->GetDecDigitsMoney(), actWindow->isTruncateMoney());

#ifdef NIGHT_COLUMNS
		case ACTA_NIGHT_POTENTIAL_SIZE_LONG:
		case ACTA_NIGHT_POTENTIAL_SIZE_SHORT:
		case ACTA_NIGHT_POTENTIAL_SIZE:
		return new UIntRowValue(actWindow->isCommaSize());

		case ACTA_NIGHT_POTENTIAL_VALUE_LONG:
		case ACTA_NIGHT_POTENTIAL_VALUE_SHORT:
		case ACTA_NIGHT_POTENTIAL_VALUE:
		return new MoneyRowValue(actWindow->isCommaMoney(), actWindow->GetDecDigitsMoney(), actWindow->isTruncateMoney(), true, true);
#endif

		default:
		return nullptr;
	}
}

COLORREF WINAPI GetActCellColor(unsigned short i, const AccountRow* row)
{
	return 0xFFFFFFFF;
}

RowValue* WINAPI CreateActRowValue(unsigned short i, AccountRow* row, const AccountWindow* actWindow, bool& dynamic)
{
	dynamic = true;
	switch(i)
	{
		case ACTA_TKT_AVG_100:
		return new AccountMoneyPerShareInventoryRowValue(row->GetAccount(), actWindow->isInventoryView(), actWindow->isCommaMoney(), actWindow->GetDecDigitsMoney(), actWindow->isTruncateMoney(), &Account::GetClosedPnlInv, &Account::GetSharesTraded, 100);//, bool useEmptyValue = true, bool useNeutralColor = false, bool useTick = true):

		case ACTA_TKT_AVG_100_NET:
		return new AccountMoneyPerShareInventoryRowValue(row->GetAccount(), actWindow->isInventoryView(), actWindow->isCommaMoney(), actWindow->GetDecDigitsMoney(), actWindow->isTruncateMoney(), &Account::GetClosedPnlNetInv, &Account::GetSharesTraded, 100);//, bool useEmptyValue = true, bool useNeutralColor = false, bool useTick = true):

		case ACTA_TKT_AVG_1:
		return new AccountMoneyPerShareInventoryRowValue(row->GetAccount(), actWindow->isInventoryView(), actWindow->isCommaMoney(), actWindow->GetDecDigitsMoney(), actWindow->isTruncateMoney(), &Account::GetClosedPnlInv, &Account::GetSharesTraded, 1);

		case ACTA_TKT_AVG_1_NET:
		return new AccountMoneyPerShareInventoryRowValue(row->GetAccount(), actWindow->isInventoryView(), actWindow->isCommaMoney(), actWindow->GetDecDigitsMoney(), actWindow->isTruncateMoney(), &Account::GetClosedPnlNetInv, &Account::GetSharesTraded, 1);

#ifdef NIGHT_COLUMNS
		case ACTA_NIGHT_POTENTIAL_SIZE_LONG:
		return new AccountUIntRowValue(row->GetAccount(), actWindow->isCommaSize(), &Account::GetPotentialLongNightSize);

		case ACTA_NIGHT_POTENTIAL_SIZE_SHORT:
		return new AccountUIntRowValue(row->GetAccount(), actWindow->isCommaSize(), &Account::GetPotentialShortNightSize);

		case ACTA_NIGHT_POTENTIAL_SIZE:
		return new AccountUIntRowValue(row->GetAccount(), actWindow->isCommaSize(), &Account::GetPotentialShortNightSize);

		case ACTA_NIGHT_POTENTIAL_VALUE_LONG:
		return new AccountMoneyRowValue(row->GetAccount(), actWindow->isCommaMoney(), actWindow->GetDecDigitsMoney(), actWindow->isTruncateMoney(), &Account::GetPotentialLongNightValue, true, true);

		case ACTA_NIGHT_POTENTIAL_VALUE_SHORT:
		return new AccountMoneyRowValue(row->GetAccount(), actWindow->isCommaMoney(), actWindow->GetDecDigitsMoney(), actWindow->isTruncateMoney(), &Account::GetPotentialShortNightValue, true, true);

		case ACTA_NIGHT_POTENTIAL_VALUE:
		return new AccountMoneyRowValue(row->GetAccount(), actWindow->isCommaMoney(), actWindow->GetDecDigitsMoney(), actWindow->isTruncateMoney(), &Account::GetNightBuyingPowerUsed, true, true);
#endif
		default:
		return nullptr;
	}
}

bool WINAPI GetActColumnInfo(unsigned short i, const char*& columnName, int& width, unsigned char& useTicks, bool& sortable, bool& defaultSortAscending, const char*& columnTooltip, unsigned char& followable, UINT& defaultHorizontalJustification)
{
	width = 80;
	sortable = true;
	defaultSortAscending = false;
	useTicks = 1;
	followable = 1;
	defaultHorizontalJustification = DT_RIGHT;

	switch(i)
	{
		case ACTA_TKT_AVG_100:
		columnName = "TktAvg100";
		columnTooltip = "Closed P&L per 100 shares";
		break;

		case ACTA_TKT_AVG_100_NET:
		columnName = "TktAvgNet100";
		columnTooltip = "Net Closed P&L per 100 shares";
		break;

		case ACTA_TKT_AVG_1:
		columnName = "TktAvg1";
		columnTooltip = "Closed P&L per 1 share";
		break;

		case ACTA_TKT_AVG_1_NET:
		columnName = "TktAvgNet1";
		columnTooltip = "Net Closed P&L per 1 share";
		break;

#ifdef NIGHT_COLUMNS
		case ACTA_NIGHT_POTENTIAL_SIZE_LONG:
		columnName = "LongNt Ptl Size";
		columnTooltip = "Potential Night Long Size";// "How many pending shares in buy \"opening\" orders (excluding OPG) plus shares already long";
		break;

		case ACTA_NIGHT_POTENTIAL_SIZE_SHORT:
		columnName = "ShortNt Ptl Size";
		columnTooltip = "Potential Night Short Size";//"How many pending shares in sell and short \"opening\" orders (excluding OPG) plus shares already short";
		break;

		case ACTA_NIGHT_POTENTIAL_SIZE:
		columnName = "PtlNt Size";
		columnTooltip = "Potential Night Size";//"Total shares in longs and shorts plus all pending \"opening\" orders (excluding OPG)";
		followable = 2;
		break;

		case ACTA_NIGHT_POTENTIAL_VALUE_LONG:
		width = 120;
		columnName = "LongNt Ptl Value";
		columnTooltip = "Potential Night Long Value";//"How much pending money in buy \"opening\" orders (excluding OPG) plus dollar value already long";
		break;

		case ACTA_NIGHT_POTENTIAL_VALUE_SHORT:
		width = 120;
		columnName = "ShortNt Ptl Value";
		columnTooltip = "Potential Night Short Value";//"How much pending money in sell and short \"opening\" orders (excluding OPG) plus dollar value already short";
		break;

		case ACTA_NIGHT_POTENTIAL_VALUE:
		width = 120;
		columnName = "BpNt Used";
		columnTooltip = "Potential Night Value";//"How much money in pending \"opening\" orders (excluding OPG) plus dollar value already in positions (\"LongNt Ptl Value\" + \"ShortNt Ptl Value\")";
		break;
#endif

		default:
		return false;
	}
	return true;
}


//////////////////
//OrderWindow
unsigned short WINAPI GetOrdAdditionalColumnCount()
{
	return 0;
}

bool WINAPI OrdUpdateNonCumulativeRowValue(RowValue* rowValue, unsigned short i, OrderRowTotal* rowTotal, const Row* rowUpdated)
{
/*
	switch(i)
	{
		case 0:
		{
			const unsigned int& sharesTradedValue = rowTotal->GetSharesTradedPtr()->GetValue();
			return ((MoneyRowValue*)rowValue)->SetValue(sharesTradedValue ? rowTotal->GetPnlClosedPtr()->GetValue() * 100 / sharesTradedValue : Money::moneyZero);
		}
		break;
	}
*/
	return false;
}

bool WINAPI OrdAddNonCumulativeRowValue(RowValue* rowValue, unsigned short i, OrderRowTotal* rowTotal, const Row* rowToAdd)
{
	return OrdUpdateNonCumulativeRowValue(rowValue, i, rowTotal, rowToAdd);
}

bool WINAPI OrdRemoveNonCumulativeRowValue(RowValue* rowValue, unsigned short i, OrderRowTotal* rowTotal, const Row* rowToRemove)
{
	return OrdUpdateNonCumulativeRowValue(rowValue, i, rowTotal, rowToRemove);
}

RowValue* WINAPI CreateOrdTotalRowValue(unsigned short i, const RowWindowSortable* ordWindow, bool& cumulative)
{
	cumulative = true;
	switch(i)
	{
		case 0:
//		cumulative = false;
		return nullptr;//new IntRowValue(ordWindow->isCommaVolume(), true, true);

		default:
		return nullptr;
	}
}

COLORREF WINAPI GetOrdCellColor(unsigned short i, const OrderRow* row)
{
/*
	switch(i)
	{
		case 0:
		{
			unsigned int ordinal = takionMainWnd->GetCurrentClearingFirmOrdinal();
			const Security* security = row->GetSecurity();
			const RowWindowSortable* window = row->GetWindowSortable();//GetPositionWindow();
			return security ?
				security->isHTB(ordinal) ? security->GetBorrowSize(ordinal) ? window->GetWarningColor() : window->GetNegativeColor() : window->GetPositiveColor():
				window->GetNeutralColor();
		}
		break;
	}
*/
	return 0xFFFFFFFF;
}

RowValue* WINAPI CreateOrdRowValue(unsigned short i, OrderRow* row, const RowWindowSortable* ordWindow, bool& dynamic)
{
	dynamic = true;
	switch(i)
	{
		case 0:
		return nullptr;//new PositionSecurityIntByUInt64Division(row->GetSecurityRef(), ordWindow->isCommaPrice(), ordWindow->GetDecDigits(), ordWindow->isTruncatePrice(), &Security::GetImbalanceSharesNyse, &Security::GetVolume);


		default:
		return nullptr;
	}
}

bool WINAPI GetOrdColumnInfo(unsigned short i, const char*& columnName, int& width, unsigned char& useTicks, bool& sortable, bool& defaultSortAscending, const char*& columnTooltip, unsigned char& followable, UINT& defaultHorizontalJustification)
{
//	return false;
	width = 60;
	sortable = true;
	defaultSortAscending = false;
	useTicks = 1;
	followable = 1;
	defaultHorizontalJustification = DT_RIGHT;

	switch(i)
	{
		case 0:
//		columnName = "ImbNYvol%";
//		columnTooltip = "Percentage of NYSE informational imbalance relative to the Day Volume";
		return false;
//		break;

		default:
		return false;
	}
	return true;
}

#ifdef __cplusplus
}
#endif
