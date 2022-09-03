
// BrightnessAutomaticDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CBrightnessAutomaticDlg dialog
class CBrightnessAutomaticDlg : public CDialogEx
{
// Construction
public:
	CBrightnessAutomaticDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BRIGHTNESSAUTOMATIC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnAutobrightnessStart();
	afx_msg void OnBnClickedBtnAutobrightnessStop();
	CSliderCtrl slid_value_second;
	CSliderCtrl slid_value_first;
	CStatic lightsensor_value;
	CStatic bright_value_first;
	CStatic bright_value_second;
	CWinThread* udpThread;
	int lastbrightness_dsp1;
	int lastbrightness_dsp2;
	int lastSensorValue_Light = 0;
	int newSensorValue_Light = 0;
	int udpport;
	void UpdateBrightness(void);
	void UpdateControls(void);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
