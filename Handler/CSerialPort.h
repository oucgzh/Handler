#ifndef __SERIALPORT_H__
#define __SERIALPORT_H__


#define WM_COMM_RXCHAR    WM_USER+1 // A character was received and placed in the input buffer.
#define WM_COMM_RXFLAG_DETECTED WM_USER+2

class CSerialPort
{
public:
	// contruction and destruction
	CSerialPort();
	virtual  ~CSerialPort();
	
	// port initialisation           
	BOOL InitPort(CWnd* pPortOwner, UINT portnr = 1, UINT baud = 9600, char parity = 'N', UINT databits = 8, UINT stopsbits = 1, DWORD dwCommEvents = EV_RXCHAR | EV_CTS, UINT nBufferSize = 512);
	
	// start/stop comm watching
	BOOL StartMonitoring();
	BOOL RestartMonitoring();
	BOOL StopMonitoring();
	
	DWORD GetWriteBufferSize();
	DWORD GetCommEvents();
	DCB GetDCB();
	
	//added by funaijia
	int m_nWriteSize;
	void ClosePort();
	
	void  WriteToPort(char* string);
	//added by funaijia
	void  WriteToPort(unsigned char* string);

public:

	UINT m_BaudRate;
	UINT m_PortNum;
	UINT m_Parity;
	UINT m_DataBit;
	UINT m_StopBit;

protected:
	// protected memberfunctions
	void  ProcessErrorMessage(char* ErrorText);
	static UINT CommThread(LPVOID pParam);
	static void ReceiveChar(CSerialPort* port, COMSTAT comstat);
	static void WriteChar(CSerialPort* port);
	
	// thread
	CWinThread*   m_Thread;
	
	// synchronisation objects
	CRITICAL_SECTION m_csCommunicationSync;
	BOOL    m_bThreadAlive;
	
	// handles
	HANDLE    m_hShutdownEvent;
	HANDLE    m_hComm;
	HANDLE    m_hWriteEvent;
	
	// Event array.
	// One element is used for each event. There are two event handles for each port.
	// A Write event and a receive character event which is located in the overlapped structure (m_ov.hEvent).
	// There is a general shutdown when the port is closed.
	HANDLE    m_hEventArray[3];
	
	// structures
	OVERLAPPED   m_ov;
	COMMTIMEOUTS  m_CommTimeouts;
	DCB     m_dcb;
	
	// owner window
	CWnd*    m_pOwner;
	
	// misc
	UINT    m_nPortNr;
	char*    m_szWriteBuffer;
	DWORD    m_dwCommEvents;
	DWORD    m_nWriteBufferSize;
};

#endif __SERIALPORT_H__