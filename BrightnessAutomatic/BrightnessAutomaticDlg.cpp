
// BrightnessAutomaticDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BrightnessAutomatic.h"
#include "BrightnessAutomaticDlg.h"
#include "afxdialogex.h"

#include <physicalmonitorenumerationapi.h>
#include <highlevelmonitorconfigurationapi.h>
#pragma comment(lib, "Dxva2.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool threadNeedstop = false;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBrightnessAutomaticDlg dialog



CBrightnessAutomaticDlg::CBrightnessAutomaticDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BRIGHTNESSAUTOMATIC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBrightnessAutomaticDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_SECOND_SCREEN, slid_value_second);
	DDX_Control(pDX, IDC_SLIDER_FIRST_SCREEN, slid_value_first);
	DDX_Control(pDX, IDC_STATIC_LIGHT_SENSOR_VALUE, lightsensor_value);
	DDX_Control(pDX, IDC_STATIC_SLIDER_VALUE_FIRST, bright_value_first);
	DDX_Control(pDX, IDC_STATIC_SLIDER_VALUE_SECOND, bright_value_second);
}

BEGIN_MESSAGE_MAP(CBrightnessAutomaticDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_AUTOBRIGHTNESS_START, &CBrightnessAutomaticDlg::OnBnClickedBtnAutobrightnessStart)
	ON_BN_CLICKED(IDC_BTN_AUTOBRIGHTNESS_STOP, &CBrightnessAutomaticDlg::OnBnClickedBtnAutobrightnessStop)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CBrightnessAutomaticDlg message handlers

BOOL CBrightnessAutomaticDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	threadNeedstop = false;
	udpport = 51996;
	UINT UDPReceiveThread(LPVOID);
	udpThread = AfxBeginThread(UDPReceiveThread, (CBrightnessAutomaticDlg*)this);

	lastbrightness_dsp1 = GetPrivateProfileInt(_T("brightness"), _T("lastbrightness_dsp1"), 50, _T(".//Settings.ini"));
	lastbrightness_dsp2 = GetPrivateProfileInt(_T("brightness"), _T("lastbrightness_dsp2"), 50, _T(".//Settings.ini"));
	UpdateBrightness();
	
	slid_value_first.SetRangeMax(100);
	slid_value_second.SetRangeMax(100);
	
	CString str;
	slid_value_first.SetPos(lastbrightness_dsp1);	
	str.Format(_T("%d"), lastbrightness_dsp1);
	bright_value_first.SetWindowTextW(str);
	slid_value_second.SetPos(lastbrightness_dsp2);
	str.Format(_T("%d"), lastbrightness_dsp2);
	bright_value_second.SetWindowTextW(str);

	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CBrightnessAutomaticDlg::UpdateControls(void) {
	
	//lightsensor_value.SetWindowTextW(_T("19.8"));
	CString str;
	str.Format(_T("%d"), lastSensorValue_Light);
	lightsensor_value.SetWindowTextW(str);

	str.Format(_T("%d"), lastbrightness_dsp1);
	bright_value_first.SetWindowTextW(str);
	
	UpdateWindow();
}
void CBrightnessAutomaticDlg::UpdateBrightness(void) {
	int ret = 0;
	char buf[50] = { 0 };
	sprintf(buf, "nircmdc.exe setbrightness %d 2", lastbrightness_dsp1);
	/*
	setbrightness [level] {mode}
	This command only works on laptop/netbook computers. It allows you to set the brightness of the laptop screen. The [level] parameter is brightness value between 0 and 100. 0 = very dark, 100 = very bright.
	{mode} is optional parameter that may conatin the following values:
	1 = Change the brightness under AC power.
	2 = Change the brightness under DC power.
	3 = Change the brightness for both AC power amd DC power (The default).
	Examples:
	setbrightness 50
	setbrightness 40 1
	setbrightness 60 2
	changebrightness [level] {mode}
	This command only works on laptop/netbook computers. It allows you to change the brightness of the laptop screen. The [level] parameter is brightness value to change. positive value increases the brightness, and negative value decreases the brightness.
	{mode} is optional parameter that may conatin the following values:
	1 = Change the brightness under AC power.
	2 = Change the brightness under DC power.
	3 = Change the brightness for both AC power amd DC power (The default).
	Examples:
	changebrightness 7
	changebrightness -10
	changebrightness 5 2
	*/
	//system(buf);
	WinExec(buf, SW_HIDE);//avoid blackwind falsh
	CString sPos = _T("100");
	sPos.Format(_T("%d"), lastbrightness_dsp1);
	ret = WritePrivateProfileString(_T("brightness"), _T("lastbrightness_dsp1"), sPos, _T(".//Settings.ini"));
}

UINT UDPReceiveThread(LPVOID param) {
#define SOCK_BUF_SIZE 1024
	char buf[SOCK_BUF_SIZE];
	int ret = 0;
	CBrightnessAutomaticDlg *dlg = (CBrightnessAutomaticDlg*)param;
	CSocket sock;
	sock.Create(51996, SOCK_DGRAM); //listen on port 51996
	while (1) {
		if (true == threadNeedstop)
			break;
		ret = sock.Receive(&buf, SOCK_BUF_SIZE);
		//light_sensor:2491.0, range:0-10000
		dlg->newSensorValue_Light = atoi(&buf[13]);		
		if (abs(dlg->newSensorValue_Light - dlg->lastSensorValue_Light)/100 > 1) {
			dlg->lastSensorValue_Light = dlg->newSensorValue_Light;
			dlg->lastbrightness_dsp1 = dlg->lastSensorValue_Light / 100;
			dlg->UpdateControls();
			dlg->UpdateBrightness();
			dlg->slid_value_first.SetPos(dlg->lastbrightness_dsp1);
		}
	}
	sock.Close();
	return 0;
}

void CBrightnessAutomaticDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBrightnessAutomaticDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBrightnessAutomaticDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CBrightnessAutomaticDlg::OnBnClickedBtnAutobrightnessStart()
{
	// TODO: Add your control notification handler code here	
}


void CBrightnessAutomaticDlg::OnBnClickedBtnAutobrightnessStop()
{
	// TODO: Add your control notification handler code here
	threadNeedstop = true;
}



void CBrightnessAutomaticDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	int ret;
	CString str;
	lastbrightness_dsp1 = slid_value_first.GetPos();
	UpdateBrightness();
	UpdateControls();

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL MyScreenEnumProc(HMONITOR hm, HDC hdc, LPRECT lprect, LPARAM lparam) {
	lparam = lparam;
	BOOL bRet = false;
	DWORD mini, current, max;
	bRet = GetMonitorBrightness(hm, &mini, &current, &max);

	return true;
}
