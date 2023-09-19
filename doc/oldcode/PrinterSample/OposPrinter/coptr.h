#if !defined(AFX_COPTR_H__0583CAF6_70B8_4D2B_9DEE_E365608B78C4__INCLUDED_)
#define AFX_COPTR_H__0583CAF6_70B8_4D2B_9DEE_E365608B78C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CCoptr wrapper class

class CCoptr : public CWnd
{
protected:
	DECLARE_DYNCREATE(CCoptr)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x6393f0a3, 0xc4cb, 0x11d1, { 0xbd, 0x3, 0x2, 0x60, 0x8c, 0xdb, 0x57, 0xb0 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attributes
public:
	long GetBinaryConversion();
	void SetBinaryConversion(long);
	CString GetCheckHealthText();
	void SetCheckHealthText(LPCTSTR);
	BOOL GetClaimed();
	void SetClaimed(BOOL);
	BOOL GetDeviceEnabled();
	void SetDeviceEnabled(BOOL);
	BOOL GetFreezeEvents();
	void SetFreezeEvents(BOOL);
	long GetOutputID();
	void SetOutputID(long);
	long GetResultCode();
	void SetResultCode(long);
	long GetResultCodeExtended();
	void SetResultCodeExtended(long);
	long GetState();
	void SetState(long);
	CString GetControlObjectDescription();
	void SetControlObjectDescription(LPCTSTR);
	long GetControlObjectVersion();
	void SetControlObjectVersion(long);
	CString GetServiceObjectDescription();
	void SetServiceObjectDescription(LPCTSTR);
	long GetServiceObjectVersion();
	void SetServiceObjectVersion(long);
	CString GetDeviceDescription();
	void SetDeviceDescription(LPCTSTR);
	CString GetDeviceName();
	void SetDeviceName(LPCTSTR);
	long GetCapCharacterSet();
	void SetCapCharacterSet(long);
	BOOL GetCapConcurrentJrnRec();
	void SetCapConcurrentJrnRec(BOOL);
	BOOL GetCapConcurrentJrnSlp();
	void SetCapConcurrentJrnSlp(BOOL);
	BOOL GetCapConcurrentRecSlp();
	void SetCapConcurrentRecSlp(BOOL);
	BOOL GetCapCoverSensor();
	void SetCapCoverSensor(BOOL);
	BOOL GetCapTransaction();
	void SetCapTransaction(BOOL);
	BOOL GetCapJrnPresent();
	void SetCapJrnPresent(BOOL);
	BOOL GetCapJrn2Color();
	void SetCapJrn2Color(BOOL);
	BOOL GetCapJrnBold();
	void SetCapJrnBold(BOOL);
	BOOL GetCapJrnDhigh();
	void SetCapJrnDhigh(BOOL);
	BOOL GetCapJrnDwide();
	void SetCapJrnDwide(BOOL);
	BOOL GetCapJrnDwideDhigh();
	void SetCapJrnDwideDhigh(BOOL);
	BOOL GetCapJrnEmptySensor();
	void SetCapJrnEmptySensor(BOOL);
	BOOL GetCapJrnItalic();
	void SetCapJrnItalic(BOOL);
	BOOL GetCapJrnNearEndSensor();
	void SetCapJrnNearEndSensor(BOOL);
	BOOL GetCapJrnUnderline();
	void SetCapJrnUnderline(BOOL);
	BOOL GetCapRecPresent();
	void SetCapRecPresent(BOOL);
	BOOL GetCapRec2Color();
	void SetCapRec2Color(BOOL);
	BOOL GetCapRecBarCode();
	void SetCapRecBarCode(BOOL);
	BOOL GetCapRecBitmap();
	void SetCapRecBitmap(BOOL);
	BOOL GetCapRecBold();
	void SetCapRecBold(BOOL);
	BOOL GetCapRecDhigh();
	void SetCapRecDhigh(BOOL);
	BOOL GetCapRecDwide();
	void SetCapRecDwide(BOOL);
	BOOL GetCapRecDwideDhigh();
	void SetCapRecDwideDhigh(BOOL);
	BOOL GetCapRecEmptySensor();
	void SetCapRecEmptySensor(BOOL);
	BOOL GetCapRecItalic();
	void SetCapRecItalic(BOOL);
	BOOL GetCapRecLeft90();
	void SetCapRecLeft90(BOOL);
	BOOL GetCapRecNearEndSensor();
	void SetCapRecNearEndSensor(BOOL);
	BOOL GetCapRecPapercut();
	void SetCapRecPapercut(BOOL);
	BOOL GetCapRecRight90();
	void SetCapRecRight90(BOOL);
	BOOL GetCapRecRotate180();
	void SetCapRecRotate180(BOOL);
	BOOL GetCapRecStamp();
	void SetCapRecStamp(BOOL);
	BOOL GetCapRecUnderline();
	void SetCapRecUnderline(BOOL);
	BOOL GetCapSlpPresent();
	void SetCapSlpPresent(BOOL);
	BOOL GetCapSlpFullslip();
	void SetCapSlpFullslip(BOOL);
	BOOL GetCapSlp2Color();
	void SetCapSlp2Color(BOOL);
	BOOL GetCapSlpBarCode();
	void SetCapSlpBarCode(BOOL);
	BOOL GetCapSlpBitmap();
	void SetCapSlpBitmap(BOOL);
	BOOL GetCapSlpBold();
	void SetCapSlpBold(BOOL);
	BOOL GetCapSlpDhigh();
	void SetCapSlpDhigh(BOOL);
	BOOL GetCapSlpDwide();
	void SetCapSlpDwide(BOOL);
	BOOL GetCapSlpDwideDhigh();
	void SetCapSlpDwideDhigh(BOOL);
	BOOL GetCapSlpEmptySensor();
	void SetCapSlpEmptySensor(BOOL);
	BOOL GetCapSlpItalic();
	void SetCapSlpItalic(BOOL);
	BOOL GetCapSlpLeft90();
	void SetCapSlpLeft90(BOOL);
	BOOL GetCapSlpNearEndSensor();
	void SetCapSlpNearEndSensor(BOOL);
	BOOL GetCapSlpRight90();
	void SetCapSlpRight90(BOOL);
	BOOL GetCapSlpRotate180();
	void SetCapSlpRotate180(BOOL);
	BOOL GetCapSlpUnderline();
	void SetCapSlpUnderline(BOOL);
	BOOL GetAsyncMode();
	void SetAsyncMode(BOOL);
	long GetCharacterSet();
	void SetCharacterSet(long);
	CString GetCharacterSetList();
	void SetCharacterSetList(LPCTSTR);
	BOOL GetCoverOpen();
	void SetCoverOpen(BOOL);
	long GetErrorLevel();
	void SetErrorLevel(long);
	long GetErrorStation();
	void SetErrorStation(long);
	CString GetErrorString();
	void SetErrorString(LPCTSTR);
	BOOL GetFlagWhenIdle();
	void SetFlagWhenIdle(BOOL);
	CString GetFontTypefaceList();
	void SetFontTypefaceList(LPCTSTR);
	long GetMapMode();
	void SetMapMode(long);
	long GetRotateSpecial();
	void SetRotateSpecial(long);
	long GetJrnLineChars();
	void SetJrnLineChars(long);
	CString GetJrnLineCharsList();
	void SetJrnLineCharsList(LPCTSTR);
	long GetJrnLineHeight();
	void SetJrnLineHeight(long);
	long GetJrnLineSpacing();
	void SetJrnLineSpacing(long);
	long GetJrnLineWidth();
	void SetJrnLineWidth(long);
	BOOL GetJrnLetterQuality();
	void SetJrnLetterQuality(BOOL);
	BOOL GetJrnEmpty();
	void SetJrnEmpty(BOOL);
	BOOL GetJrnNearEnd();
	void SetJrnNearEnd(BOOL);
	long GetRecLineChars();
	void SetRecLineChars(long);
	CString GetRecLineCharsList();
	void SetRecLineCharsList(LPCTSTR);
	long GetRecLineHeight();
	void SetRecLineHeight(long);
	long GetRecLineSpacing();
	void SetRecLineSpacing(long);
	long GetRecLineWidth();
	void SetRecLineWidth(long);
	BOOL GetRecLetterQuality();
	void SetRecLetterQuality(BOOL);
	BOOL GetRecEmpty();
	void SetRecEmpty(BOOL);
	BOOL GetRecNearEnd();
	void SetRecNearEnd(BOOL);
	long GetRecSidewaysMaxLines();
	void SetRecSidewaysMaxLines(long);
	long GetRecSidewaysMaxChars();
	void SetRecSidewaysMaxChars(long);
	long GetRecLinesToPaperCut();
	void SetRecLinesToPaperCut(long);
	CString GetRecBarCodeRotationList();
	void SetRecBarCodeRotationList(LPCTSTR);
	long GetSlpLineChars();
	void SetSlpLineChars(long);
	CString GetSlpLineCharsList();
	void SetSlpLineCharsList(LPCTSTR);
	long GetSlpLineHeight();
	void SetSlpLineHeight(long);
	long GetSlpLineSpacing();
	void SetSlpLineSpacing(long);
	long GetSlpLineWidth();
	void SetSlpLineWidth(long);
	BOOL GetSlpLetterQuality();
	void SetSlpLetterQuality(BOOL);
	BOOL GetSlpEmpty();
	void SetSlpEmpty(BOOL);
	BOOL GetSlpNearEnd();
	void SetSlpNearEnd(BOOL);
	long GetSlpSidewaysMaxLines();
	void SetSlpSidewaysMaxLines(long);
	long GetSlpSidewaysMaxChars();
	void SetSlpSidewaysMaxChars(long);
	long GetSlpMaxLines();
	void SetSlpMaxLines(long);
	long GetSlpLinesNearEndToEnd();
	void SetSlpLinesNearEndToEnd(long);
	CString GetSlpBarCodeRotationList();
	void SetSlpBarCodeRotationList(LPCTSTR);
	long GetCapPowerReporting();
	void SetCapPowerReporting(long);
	long GetPowerNotify();
	void SetPowerNotify(long);
	long GetPowerState();
	void SetPowerState(long);

// Operations
public:
	long Open(LPCTSTR DeviceName);
	long Close();
	long Claim(long Timeout);
	long Release();
	long CheckHealth(long Level);
	long ClearOutput();
	long DirectIO(long Command, long* pData, BSTR* pString);
	long PrintNormal(long Station, LPCTSTR Data);
	long PrintTwoNormal(long Stations, LPCTSTR Data1, LPCTSTR Data2);
	long PrintImmediate(long Station, LPCTSTR Data);
	long BeginInsertion(long Timeout);
	long EndInsertion();
	long BeginRemoval(long Timeout);
	long EndRemoval();
//	long CutPaper(long Percentage);
	long RotatePrint(long Station, long Rotation);
	long PrintBarCode(long Station, LPCTSTR Data, long Symbology, long Height, long Width, long Alignment, long TextPosition);
	long PrintBitmap(long Station, LPCTSTR FileName, long Width, long Alignment);
	long TransactionPrint(long Station, long Control);
	long ValidateData(long Station, LPCTSTR Data);
	long SetBitmap(long BitmapNumber, long Station, LPCTSTR FileName, long Width, long Alignment);
	long SetLogo(long Location, LPCTSTR Data);
	void SODirectIO(long EventNumber, long* pData, BSTR* pString);
	void SOError(long ResultCode, long ResultCodeExtended, long ErrorLocus, long* pErrorResponse);
	void SOOutputComplete(long OutputID);
	void SOStatusUpdate(long Data);
	long SOProcessID();
	void AboutBox();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COPTR_H__0583CAF6_70B8_4D2B_9DEE_E365608B78C4__INCLUDED_)