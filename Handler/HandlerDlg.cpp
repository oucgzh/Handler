
// HandlerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "HandlerDlg.h"
#include "afxdialogex.h"


#include <Vfw.h>
#include "ConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <mmsystem.h>
#pragma comment(lib, "WINMM.lib")
#define  MAX_SENDLENGHT  9

// CAboutDlg dialog used for App About
WORD joyx,joyy;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:

};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CHandlerDlg dialog




CHandlerDlg::CHandlerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHandlerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
}

void CHandlerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LEFT, m_ColLeft);
	DDX_Control(pDX, IDC_STATIC_CAMERA, m_Camera);
}

BEGIN_MESSAGE_MAP(CHandlerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_START, &CHandlerDlg::OnStartMotor)
	ON_WM_TIMER()
	ON_WM_DESTROY()

	ON_BN_CLICKED(IDC_BTN_CAMERA, &CHandlerDlg::OnOpenCamera)
	ON_BN_CLICKED(IDC_BTNCONFIGURATION, &CHandlerDlg::OnConfigSerial)
	ON_BN_CLICKED(IDC_BUTTON_CLOSESERIAL, &CHandlerDlg::OnBtnCloseSerial)
	ON_BN_CLICKED(IDCANCEL, &CHandlerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_ABOUT, &CHandlerDlg::OnBtnAbout)
END_MESSAGE_MAP()


// CHandlerDlg message handlers

BOOL CHandlerDlg::OnInitDialog()
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

	
	i_Flags = 0;
	b_Left = FALSE;
	b_Right = FALSE;
	b_Backword = FALSE;
	b_Forward = FALSE;
	SetDlgItemText(IDC_SERIAL_SATTE,_T("尚未打开..."));
	SetDlgItemText(IDC_WORKSTATE,_T("尚未开机..."));
	GetDlgItem(IDC_BUTTON_CLOSESERIAL)->EnableWindow(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHandlerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CHandlerDlg::OnPaint()
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
		/************************************************************************/
		/*                      Change the background of the Dialog             */
		/************************************************************************/
		CRect rect;
		CPaintDC dc(this);
		GetClientRect(rect);
		dc.FillSolidRect(rect,RGB(65,105,225));  //设置背景色
		//dc.FillSolidRect(rect,RGB(10,0,0));  //设置背景色
		/************************************************************************/
		/*                      Jeffrey                                         */
		/************************************************************************/
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHandlerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHandlerDlg::OnStartMotor()
{
	UINT result;

	result=::joySetCapture(CDialog::m_hWnd, JOYSTICKID1, 0, FALSE);//to send the joystick's message the father window
	
	if(result == JOYERR_NOCANDO )
	{
		MessageBeep(MB_ICONEXCLAMATION);
		MessageBox(_T("不能捕获游戏杆"), NULL, MB_OK | MB_ICONEXCLAMATION);
		b_all = TRUE;
		return ;
	}
	else if(result == JOYERR_UNPLUGGED )
	{
		MessageBeep(MB_ICONEXCLAMATION);
		MessageBox(_T("游戏杆未与系统连接"), NULL, MB_OK | MB_ICONEXCLAMATION);
		b_all = TRUE;
		return ;
	}
	else if (result == MMSYSERR_NODRIVER)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		MessageBox(_T("游戏杆驱动没有安装"), NULL, MB_OK | MB_ICONEXCLAMATION);
		b_all = TRUE;
		return ;
	}
	else
		b_all = FALSE;
	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	
}
//To make special button flash
void CHandlerDlg::FlashButton()
{
	if(joyx<=12)//joystick to the left
	{
		flag=GetDlgItem(IDC_LEFT)->IsWindowEnabled();
		GetDlgItem(IDC_LEFT)->EnableWindow(!flag);
		SetDlgItemText(IDC_LEFT,_T("<<左>>"));
		if(b_Left == FALSE)
			IsFastSlow(i_Flags,IDC_FASTER,IDC_SLOWER);
	}
	else if(joyx >= 20)//joystick to the right
	{
		flag=GetDlgItem(IDC_RIGHT)->IsWindowEnabled();
		GetDlgItem(IDC_RIGHT)->EnableWindow(!flag);
		SetDlgItemText(IDC_RIGHT,_T("<<右>>"));
		if(b_Right == FALSE)
			IsFastSlow(i_Flags,IDC_FASTER,IDC_SLOWER);
	}
	else if(joyy <= 12)// forward
	{
		flag=GetDlgItem(IDC_FORWARD)->IsWindowEnabled();
		GetDlgItem(IDC_FORWARD)->EnableWindow(!flag);
		SetDlgItemText(IDC_FORWARD,_T("<<前>>"));
		if(b_Forward == FALSE)
			IsFastSlow(i_Flags,IDC_FASTER,IDC_SLOWER);
	}
	else if(joyy >= 25)//backword
	{
		flag=GetDlgItem(IDC_BACKWARD)->IsWindowEnabled();
		GetDlgItem(IDC_BACKWARD)->EnableWindow(!flag);
		SetDlgItemText(IDC_BACKWARD,_T("<<后>>"));
		if(b_Backword == FALSE)
			IsFastSlow(i_Flags,IDC_FASTER,IDC_SLOWER);
	}
}


LRESULT CHandlerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	char senddata[MAX_SENDLENGHT];
	char *cp;
	char recedata[MAX_SENDLENGHT];
	char *receive;
	char B;
	B='b';
	cp=senddata;
	receive=recedata;
	//	serial.Read(receive,7);
//JoyStick' pos (range from :0 to 40)
	switch(message)
	{
	case MM_JOY1BUTTONDOWN:
		if (wParam&JOY_BUTTON1)
		{
			Sleep(5);
			
		}
		break;
	case MM_JOY1BUTTONUP:
		if (wParam&JOY_BUTTON1CHG)
		{
			Sleep(5);
		
		}
		break;
		
	case MM_JOY1MOVE:

		if (!b_all)
		{
			joyx = LOWORD(lParam)/2048;
			joyy = HIWORD(lParam)/2048;
			//left to the center
			if(joyx <= 12)
			{
				FlashButton();				
				if (wParam & JOY_BUTTON3)
				{
					
					/************************************************************************/
					/* Here send the command to the handler(WriteChar())                    */
					/************************************************************************/
					//command :
					//WritePort();
					
					Sleep(5);
					i_Flags = 1;//加速
					b_Left =FALSE;
					SetDlgItemText(IDC_WORKSTATE,_T("加速+左转"));
				} 
				else if(wParam & JOY_BUTTON4)
				{
					Sleep(5);
					/************************************************************************/
					/* Here send the command to the handler(WriteChar())                    */
					/************************************************************************/
					//command :
					//WritePort();
					i_Flags = 2;//减速
					b_Left =FALSE;
					SetDlgItemText(IDC_WORKSTATE,_T("减速+左转"));

				}
				else
				{
					/************************************************************************/
					/* Here send the command to the handler(WriteChar())                    */
					/************************************************************************/
				
					b_Left = TRUE;
					/*
					CString command = "cfc8";
					char data[512] = {0};
					int len = Str2Hex(command,data);
					//m_Port.WriteToPort(data,len);
					*/
					char ch[2] = {0xcf,0xc8};
					
					m_Port.WriteToPort(ch);
					
					Sleep(5);
					SetDlgItemText(IDC_WORKSTATE,_T("左转"));
					SetDlgItemText(IDC_FASTER,_T("加速"));
					SetDlgItemText(IDC_SLOWER,_T("减速"));
				}
			}
			else if(joyx >= 20)
			{
				FlashButton();				
				if (wParam & JOY_BUTTON3)
				{
					/************************************************************************/
					/* Here send the command to the handler(WriteChar())                    */
					/************************************************************************/
					//command :
					//WritePort();
					Sleep(5);
					i_Flags = 1;
					b_Right = FALSE;
					SetDlgItemText(IDC_WORKSTATE,_T("加速+右转"));
				} 
				else if (wParam & JOY_BUTTON4)
				{
					Sleep(5);
					i_Flags = 2;
					b_Right = FALSE;
					SetDlgItemText(IDC_WORKSTATE,_T("减速+右转"));
				}
				else
				{
					/************************************************************************/
					/* Here send the command to the handler(WriteChar())                    */
					/************************************************************************/
				

					Sleep(5);
					b_Right = TRUE;
					SetDlgItemText(IDC_WORKSTATE,_T("右转"));
					SetDlgItemText(IDC_FASTER,_T("加速"));
					SetDlgItemText(IDC_SLOWER,_T("减速"));
				}
			}

			//down to the center

			if(joyy <= 12)
			{
				FlashButton();				
				if (wParam & JOY_BUTTON3)
				{
					/************************************************************************/
					/* Here send the command to the handler(WriteChar())                    */
					/************************************************************************/
					//command :
					//WritePort();
					Sleep(5);
					i_Flags = 1;
					b_Forward = FALSE;
					SetDlgItemText(IDC_WORKSTATE,_T("加速+前进"));
				} 
				else if (wParam & JOY_BUTTON4)
				{
					/************************************************************************/
					/* Here send the command to the handler(WriteChar())                    */
					/************************************************************************/
					//command :
					//WritePort();
					Sleep(5);
					i_Flags = 2;
					b_Forward = FALSE;
					SetDlgItemText(IDC_WORKSTATE,_T("减速+前进")); 
				} 
				
				else
				{
					/************************************************************************/
					/* Here send the command to the handler(WriteChar())                    */
					/************************************************************************/
					/*
					CString command44 = "23 30 50 32 30 30 30 0D 0A";//the sign of the end "0D 0A"
					char data44[512] = {0};
					int len44 = Str2Hex(command44,data44);
					m_Port.WriteToPort(data44,len44);
					*/
					//the sign of the steering engin is "\r\n"
					CString comdSteeringEngine ;
					comdSteeringEngine = "#0P800";
					char tempComdSteer[512] = {0};
					sprintf(tempComdSteer,"%s\r\n",comdSteeringEngine);
					m_Port.WriteToPort(tempComdSteer);

					Sleep(5);
					b_Forward = TRUE;
					SetDlgItemText(IDC_WORKSTATE,_T("前进"));
					SetDlgItemText(IDC_FASTER,_T("加速"));
					SetDlgItemText(IDC_SLOWER,_T("减速"));
				}
				
			}
			else if(joyy >= 25)
			{
				FlashButton();				
				if (wParam & JOY_BUTTON3)
				{
					/************************************************************************/
					/* Here send the command to the handler(WriteChar())                    */
					/************************************************************************/
					//command :
					//WritePort();
					Sleep(5);
					i_Flags = 1;
					b_Backword = FALSE;
					SetDlgItemText(IDC_WORKSTATE,_T("加速+后退"));
				} 
				else if (wParam & JOY_BUTTON4)
				{
					/************************************************************************/
					/* Here send the command to the handler(WriteChar())                    */
					/************************************************************************/
					//command :
					//WritePort();

					Sleep(5);
					i_Flags = 2;
					b_Backword = FALSE;
					SetDlgItemText(IDC_WORKSTATE,_T("减速+后退"));
				} 
				else
				{
					/************************************************************************/
					/* Here send the command to the handler(WriteChar())                    */
					/************************************************************************/
					//command :
					//WritePort();
					//position = 2000
					/*
					CString command444 = "23 30 50 38 30 30 0D 0A";
					char data444[512] = {0};
					int len444 = Str2Hex(command444,data444);
					m_Port.WriteToPort(data444,len444);
					*/

					CString comdSteeringEngine2 ;
					comdSteeringEngine2 = "#0P1500";
					char tempComdSteer2[512] = {0};
					sprintf(tempComdSteer2,"%s\r\n",comdSteeringEngine2);//the steering engine's command must be "\r\n" at the end .
					m_Port.WriteToPort(tempComdSteer2);

					Sleep(5);
					b_Backword = TRUE;
					SetDlgItemText(IDC_WORKSTATE,_T("后退"));
					SetDlgItemText(IDC_FASTER,_T("加速"));
					SetDlgItemText(IDC_SLOWER,_T("减速"));
				}
			}
			if(joyx>12 && joyx<20 && joyy<20&&joyy>12)
			{
				if (wParam & JOY_BUTTON1)
				{
					/************************************************************************/
					/* Here send the command to the handler(WriteChar())                    */
					/************************************************************************/
					//command :
					//WritePort();
					Sleep(5);
					SetDlgItemText(IDC_WORKSTATE,_T("抓取"));
					SetDlgItemText(IDC_SNACH,_T("<<抓>>"));
				} 
				else if(wParam & JOY_BUTTON2)
				{
					/************************************************************************/
					/* Here send the command to the handler(WriteChar())                    */
					/************************************************************************/
					//command :
					//WritePort();
					Sleep(5);
					SetDlgItemText(IDC_WORKSTATE,_T("复位"));
				}
				else if(wParam & JOY_BUTTON3)
				{
					/************************************************************************/
					/* Here send the command to the handler(WriteChar())                    */
					/************************************************************************/
					//command :
					//WritePort();
					Sleep(5);
					SetDlgItemText(IDC_WORKSTATE,_T("上升"));
					SetDlgItemText(IDC_UPDOWN,_T("上升"));	
				} 
				else if(wParam & JOY_BUTTON4)
				{
					
					/************************************************************************/
					/* Here send the command to the handler(WriteChar())                    */
					/************************************************************************/
					//command :
					//WritePort();
					Sleep(5);
					SetDlgItemText(IDC_WORKSTATE,_T("下降"));
					SetDlgItemText(IDC_UPDOWN,_T("下降"));

				} 
				else if (wParam & JOY_BUTTON6)
				{
					
					Sleep(5);
					SetDlgItemText(IDC_WORKSTATE,_T("松开"));
					SetDlgItemText(IDC_LOOSE,_T("<<松>>"));
				}
				else
				{
					/************************************************************************/
					/* Here send the command to the handler(WriteChar())                    */
					/************************************************************************/
					//command :
					//WritePort();
					Sleep(5);
					SetDlgItemText(IDC_WORKSTATE,_T("运行中..."));
					SetDlgItemText(IDC_UPDOWN,_T("升/降"));	
					SetDlgItemText(IDC_SNACH,_T("抓取"));
					SetDlgItemText(IDC_LOOSE,_T("松开"));
				}	
			OnSetWinState();
			}
		}
		break;
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CHandlerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	::joyReleaseCapture(JOYSTICKID1); 
	b_all = TRUE;
}
//To set buttons state
void CHandlerDlg::OnSetWinState()
{
	GetDlgItem(IDC_LEFT)->EnableWindow(TRUE);
	GetDlgItem(IDC_RIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_FORWARD)->EnableWindow(TRUE);
	GetDlgItem(IDC_BACKWARD)->EnableWindow(TRUE);

	SetDlgItemText(IDC_BACKWARD,_T("后"));
	SetDlgItemText(IDC_FORWARD,_T("前"));
	SetDlgItemText(IDC_LEFT,_T("左"));
	SetDlgItemText(IDC_RIGHT,_T("右"));

	SetDlgItemText(IDC_FASTER,_T("加速"));
	SetDlgItemText(IDC_SLOWER,_T("减速"));

	i_Flags = 0;
	b_Left = FALSE;
	b_Right = FALSE;
	b_Backword = FALSE;
	b_Forward = FALSE;
}

void CHandlerDlg::IsFastSlow(int flags,int id1,int id2)
{
	switch(flags)
	{
	case 1://To be faster
		SetDlgItemText(id1,_T("<<加>>"));
		break;
	case 2://To be slower
		SetDlgItemText(id2,_T("<<减>>"));
		break;
	}
}



//Open the camera
void CHandlerDlg::OnOpenCamera()
{
	
	// TODO: Add your control notification handler code here
	CWnd *pWnd = GetDlgItem(IDC_STATIC_CAMERA);
	CRect rectStatic,rectDlg;
	int x,y;

	pWnd->GetClientRect(rectStatic);//to get the Static's top-left position(that's(0,0))
	pWnd->ClientToScreen(rectStatic);//position relative to the screen

	GetClientRect(rectDlg);//to get the Dlg's top-left position (0,0)
	ClientToScreen(rectDlg);//position relative to the screen
	
	x = rectStatic.left - rectDlg.left;
	y = rectStatic.top - rectDlg.top;//static's position  relative to the Dialog

	CAPTUREPARMS m_CaptureParams;
	hCamera=capCreateCaptureWindow("Cam",WS_CHILD|WS_VISIBLE,x,y,rectStatic.Width(),rectStatic.Height(),m_hWnd,0);
	
	//create the catch window
    if (!hCamera)
	{
		AfxMessageBox(_T("CreateCaptureWindow Failure")); //Failure to connect
	}

	if(!capDriverConnect(hCamera,0))//connect to the device's driver
		AfxMessageBox(_T("Failure to connect to the driver")); //Driver Connect Failure

	if(!capCaptureGetSetup(hCamera,&m_CaptureParams,sizeof(m_CaptureParams)))//get the relative configuration
		AfxMessageBox(_T("视频格式设置失败"));//如果驱动不支持此格式将返回0 ，Fail to set the video format
	GetDlgItem(IDC_BTN_CAMERA)->EnableWindow(FALSE);
	capPreviewScale(hCamera,TRUE);
	capPreviewRate(hCamera,1);//sets the frame display rate in preview mode
	capPreview(this->hCamera,TRUE);//preview
}

//To config the port
void CHandlerDlg::OnConfigSerial()
{
	// TODO: Add your control notification handler code here
	CConfigDlg *dlgConfig = new CConfigDlg(this,m_Port.GetDCB());
	BOOL bPortOpen = FALSE;

	if(dlgConfig->DoModal() == IDOK)
	{

		bPortOpen = m_Port.InitPort(this,
									dlgConfig->tempPortIndex,
									atoi(dlgConfig->m_strBaudRate),
									(dlgConfig->m_strParity0).GetAt(0),
									atoi(dlgConfig->m_strDatabit),
									atoi(dlgConfig->m_strStopbit),
									EV_RXCHAR|EV_RXFLAG,
									512);

		if(bPortOpen == TRUE)
		{
			m_Port.StartMonitoring();

			SetDlgItemText(IDC_EDIT_PORTNUM,dlgConfig->m_strPort);
			SetDlgItemText(IDC_EDIT_BAUDERATE,dlgConfig->m_strBaudRate);
			SetDlgItemText(IDC_EDIT_PARITY,dlgConfig->m_strParity0);
			SetDlgItemText(IDC_EDIT_DATABIT,dlgConfig->m_strDatabit);
			SetDlgItemText(IDC_EDIT_STOPBIT,dlgConfig->m_strStopbit);

			SetDlgItemText(IDC_SERIAL_SATTE,_T("串口已成功打开..."));

			GetDlgItem(IDC_BTNCONFIGURATION)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_CLOSESERIAL)->EnableWindow(TRUE);
		}
		else
		{
			SetDlgItemText(IDC_SERIAL_SATTE,_T("串口打开失败..."));
			
		}
	}
	delete dlgConfig;	
}

//Close the opened port
void CHandlerDlg::OnBtnCloseSerial()
{
	// TODO: Add your control notification handler code here
	m_Port.ClosePort();
	
	GetDlgItem(IDC_BTNCONFIGURATION)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CLOSESERIAL)->EnableWindow(FALSE);

	SetDlgItemText(IDC_SERIAL_SATTE,_T("串口已被关闭"));
	SetDlgItemText(IDC_EDIT_PORTNUM,_T(" "));
	SetDlgItemText(IDC_EDIT_PARITY,_T(" "));
	SetDlgItemText(IDC_EDIT_DATABIT,_T(" "));
	SetDlgItemText(IDC_EDIT_BAUDERATE,_T(" "));
	SetDlgItemText(IDC_EDIT_STOPBIT,_T(" "));

}


void CHandlerDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CHandlerDlg::OnBtnAbout()
{
	// TODO: Add your control notification handler code here
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}



