#pragma once


// CConfigDlg dialog

class CConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(CWnd* pParent = NULL);   // standard constructor
	CConfigDlg(CWnd* pParent,DCB dcb);

	virtual ~CConfigDlg();

// Dialog Data
	enum { IDD = IDD_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnConfigure();

public:

	
	HANDLE m_hComm;
	virtual BOOL OnInitDialog();
	CString strTemp;
	int tempPortIndex;
	DCB		m_dcb;

	CComboBox m_parity;
	CComboBox m_portnr;
	CComboBox m_databit;
	CComboBox m_stopbit;
	CComboBox m_baud;

	CString m_strBaudRate;
	CString m_strParity0;
	CString m_strPort;
	CString m_strDatabit;
	CString m_strStopbit;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedCancel();
};
