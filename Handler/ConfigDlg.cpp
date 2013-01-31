// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "ConfigDlg.h"
#include "afxdialogex.h"

#include "HandlerDlg.h"
#include "CSerialPort.h"

// CConfigDlg dialog

IMPLEMENT_DYNAMIC(CConfigDlg, CDialogEx)

CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfigDlg::IDD, pParent)
{

	m_strBaudRate = _T("");
	m_strParity0 = _T("");
	m_strPort = _T("");
	m_strDatabit = _T("");
	m_strStopbit = _T("");
}

CConfigDlg::CConfigDlg(CWnd* pParent,DCB dcb)
	: CDialogEx(CConfigDlg::IDD, pParent)
{
	m_dcb = dcb;
	m_strBaudRate = _T("");

	m_strParity0 = _T("");

	m_strPort = _T("");

	m_strDatabit = _T("");

	m_strStopbit = _T("");

}

CConfigDlg::~CConfigDlg()
{
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//  DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_port);
	DDX_Control(pDX, IDC_COMBO_PARITY, m_parity);
	DDX_Control(pDX, IDC_COMBO_PORT, m_portnr);
	DDX_Control(pDX, IDC_COMBODATABIT, m_databit);
	DDX_Control(pDX, IDC_COMBOSTOPBIT, m_stopbit);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_baud);
	DDX_CBString(pDX, IDC_COMBO_BAUDRATE, m_strBaudRate);
	DDX_CBString(pDX, IDC_COMBO_PARITY, m_strParity0);
	DDX_CBString(pDX, IDC_COMBO_PORT, m_strPort);
	DDX_CBString(pDX, IDC_COMBODATABIT, m_strDatabit);
	DDX_CBString(pDX, IDC_COMBOSTOPBIT, m_strStopbit);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CConfigDlg::OnConfigure)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDCANCEL, &CConfigDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CConfigDlg message handlers


void CConfigDlg::OnConfigure()
{
	// TODO: Add your control notification handler code here
	tempPortIndex = m_portnr.GetCurSel()+1;

	strTemp.Format("%d", m_dcb.BaudRate);
	((CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE))->SelectString(0, strTemp);

	((CComboBox*)GetDlgItem(IDC_COMBO_PARITY))->SetCurSel(m_dcb.Parity);
	((CComboBox*)GetDlgItem(IDC_COMBOSTOPBIT))->SetCurSel(m_dcb.StopBits);

	strTemp.Format("%d",m_dcb.ByteSize);
	((CComboBox*)GetDlgItem(IDC_COMBODATABIT))->SelectString(0,strTemp);

	CDialog::OnOK();
}


BOOL CConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	

	m_stopbit.SetCurSel(0);
	m_portnr.SetCurSel(0);
	m_parity.SetCurSel(0);
	m_databit.SetCurSel(0);
	m_baud.SetCurSel(0);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CConfigDlg::OnPaint()
{
	CPaintDC dc2(this); // device context for painting
	// TODO: Add your message handler code here
	CRect rect;
	
	GetClientRect(rect);
	dc2.FillSolidRect(rect,RGB(65,105,225));  //…Ë÷√±≥æ∞…´
	
	CDialogEx::OnPaint();
}


void CConfigDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
