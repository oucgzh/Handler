
// HandlerDlg.h : header file
//

#pragma once
#include "CSerialPort.h"

// CHandlerDlg dialog
class CHandlerDlg : public CDialogEx
{
// Construction
public:
	CHandlerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_HANDLER_DIALOG };

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
	
	afx_msg void OnStartMotor();
	afx_msg void FlashButton();
	afx_msg void OnSetWinState();
	afx_msg void IsFastSlow(int flags,int id1,int id2);

public:
	int i_Flags;

	BOOL b_all;
	BOOL b_Left,b_Right,b_Forward,b_Backword;
	BOOL flag;
	CRect rect;
	CSerialPort m_Port;
	HWND hCamera;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CButton m_ColLeft;
	afx_msg void OnDestroy();
	CStatic m_Camera;

	afx_msg void OnOpenCamera();
	afx_msg void OnConfigSerial();
	afx_msg void OnBtnCloseSerial();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBtnAbout();

};
