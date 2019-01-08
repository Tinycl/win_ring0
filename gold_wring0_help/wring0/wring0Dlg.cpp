
// wring0Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wring0.h"
#include "wring0Dlg.h"
#include "afxdialogex.h"

//add for WinRing0
#include "OlsDef.h"
#include "OlsApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MEMORY_DUMP_SIZE 256

#define MAP_MEMORY_SIZE 64*1024

#define _PHYSICAL_MEMORY_SUPPORT

static BOOL IsNT();

typedef struct _DmiHeader
{
	BYTE Type;
	BYTE Length;
	WORD Handle;
}DmiHeader;

static CString DmiString(DmiHeader* dmi, UCHAR id, BOOL rn = TRUE)
{
	static CString cstr;
	char *p = (char *)dmi;

	p += dmi->Length;

	while(id > 1 && *p)
	{
		p += strlen(p);
		p++;
		id--;
	}
	// ASCII Filter
	for(DWORD i = 0; i < strlen(p); i++){
		if(p[i] < 32 || p[i] == 127){
			p[i]='.';
		}
	}
	cstr = p;
	if(rn){
		cstr += "\r\n";
	}

	return cstr;
}

static CString DmiStringB(BYTE b, BOOL rn = TRUE)
{
	static CString cstr;
	cstr.Format(_T("%d"), b);
	if(rn){
		cstr += "\r\n";
	}
	return cstr;
}


// CAboutDlg dialog used for App About
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
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cwring0Dlg dialog

Cwring0Dlg::Cwring0Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cwring0Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cwring0Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT, m_Edit);
}

BEGIN_MESSAGE_MAP(Cwring0Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// Cwring0Dlg message handlers

BOOL Cwring0Dlg::OnInitDialog()
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
	//add for WinRing0

	m_IsNT = IsNT();

	Refresh();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Cwring0Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//add for WinRing0
void Cwring0Dlg::Refresh()
{
	CString cstr, edit;

	switch(GetDllStatus())
	{
	case OLS_DLL_NO_ERROR:
		break;
	case OLS_DLL_UNSUPPORTED_PLATFORM:
		AfxMessageBox(_T("DLL Status Error!! UNSUPPORTED_PLATFORM"));
		EndDialog(0);
		return;
		break;
	case OLS_DLL_DRIVER_NOT_LOADED:
		AfxMessageBox(_T("DLL Status Error!! DRIVER_NOT_LOADED"));
		EndDialog(0);
		return;
		break;
	case OLS_DLL_DRIVER_NOT_FOUND:
		AfxMessageBox(_T("DLL Status Error!! DRIVER_NOT_FOUND"));
		EndDialog(0);
		return;
		break;
	case OLS_DLL_DRIVER_UNLOADED:
		AfxMessageBox(_T("DLL Status Error!! DRIVER_UNLOADED"));
		EndDialog(0);
		return;
		break;
	case OLS_DLL_DRIVER_NOT_LOADED_ON_NETWORK:
		AfxMessageBox(_T("DLL Status Error!! DRIVER_NOT_LOADED_ON_NETWORK"));
		EndDialog(0);
		return;
		break;
	case OLS_DLL_UNKNOWN_ERROR:
	default:
		AfxMessageBox(_T("DLL Status Error!! UNKNOWN_ERROR"));
		EndDialog(0);
		return;
		break;
	}

	//-----------------------------------------------------------------------------
	// Version Check
	//-----------------------------------------------------------------------------
	BYTE major, minor, revision, release;
	cstr.Format(_T("[DLL Version]\r\n"));
	edit += cstr;
	//GetDllVersion
	GetDllVersion(&major, &minor, &revision, &release);
	cstr.Format(_T("%d.%d.%d.%d\r\n"), major, minor, revision, release);
	edit += cstr;
	cstr.Format(_T("[Device Driver Version]\r\n"));
	edit += cstr;
	//GetDriverVersion
	GetDriverVersion(&major, &minor, &revision, &release);
	cstr.Format(_T("%d.%d.%d.%d\r\n"), major, minor, revision, release);
	edit += cstr;
	cstr.Format(_T("[Device Driver Type]\r\n"));
	edit += cstr;
	//GetDriverType
	switch(GetDriverType())
	{
	case OLS_DRIVER_TYPE_WIN_9X:
		edit += _T("OLS_DRIVER_TYPE_WIN_9X\r\n");
		break;
	case OLS_DRIVER_TYPE_WIN_NT:
		edit += _T("OLS_DRIVER_TYPE_WIN_NT\r\n");
		break;
	case OLS_DRIVER_TYPE_WIN_NT4: 
		edit += _T("OLS_DRIVER_TYPE_WIN_NT4\r\n");
		break;
	case OLS_DRIVER_TYPE_WIN_NT_X64:
		edit += _T("OLS_DRIVER_TYPE_WIN_NT_X64\r\n");
		break;
	case OLS_DRIVER_TYPE_WIN_NT_IA64:
		edit += _T("OLS_DRIVER_TYPE_WIN_NT_IA64\r\n");
		break;
	default:
		edit += _T("OLS_DRIVER_TYPE_UNKNOWN\r\n");
		break;
	}

	cstr.Format(_T("[DLL Status]\r\n"));
	edit += cstr;
	//GetDllStatus
	switch(GetDllStatus())
	{
	case OLS_DLL_NO_ERROR:
		edit += _T("OLS_DLL_NO_ERROR\r\n");
		break;
	case OLS_DLL_UNSUPPORTED_PLATFORM:
		edit += _T("OLS_DLL_UNSUPPORTED_PLATFORM\r\n");
		break;
	case OLS_DLL_DRIVER_NOT_LOADED:
		edit += _T("OLS_DLL_DRIVER_NOT_LOADED\r\n");
		break;
	case OLS_DLL_DRIVER_NOT_FOUND:
		edit += _T("OLS_DLL_NOT_FOUND_DRIVER\r\n");
		break;
	case OLS_DLL_DRIVER_UNLOADED:
		edit += _T("OLS_DLL_DRIVER_UNLOADED\r\n");
		break;
	case OLS_DLL_DRIVER_NOT_LOADED_ON_NETWORK:
		edit += _T("OLS_DLL_DRIVER_NOT_LOADED_ON_NETWORK\r\n");
		break;
	default:
		edit += _T("OLS_DLL_UNKNOWN_ERROR\r\n");
		break;
	}

	//-----------------------------------------------------------------------------
	// MSR
	//----------------------------------------------------------------------------
	//rdmsr of current cpu core
	DWORD index, eax, ebx, ecx, edx;
	cstr.Format(_T("[RDMSR]\r\n"));
	edit += cstr;
	index = 0x00000199; 
	if(Rdmsr(index, &eax, &edx))
	{
		cstr.Format(_T("index     63-32    31-0\r\n"));
		edit += cstr;
		cstr.Format(_T("%08X: %08X %08X\r\n"), index, edx, eax);
		edit += cstr;
	}
	else
	{
		edit += _T("Failure : rdmsr 0x199(1) fail!\r\n");		
	}

	//wrmsr of current cpu core
	/*
	cstr.Format(_T("[WRMSR]\r\n"));
	edit += cstr;
	index = 0x00000199; 
	edx = 0x0; eax = 0x2400;
	if(Wrmsr(index, eax, edx))
	{
		cstr.Format(_T("index     63-32    31-0\r\n"));
		edit += cstr;
		cstr.Format(_T("%08X: %08X %08X\r\n"), index, edx, eax);
		edit += cstr;
	}
	else
	{
		edit += _T("Failure : wrmsr 0x199 fail!\r\n");		
	}*/

	//wrmsrTx of current cpu core
	// Thread Affinity Mask: 
	//0--fail; 1--for core0; 2--for core1; 3--core1; 4--core2; 8--core3;7-core1
	cstr.Format(_T("[WRMSRTX-1/2/4/8]\r\n"));
	edit += cstr;
	index = 0x00000199; 
	edx = 0x0; eax = 0x2400;
	if(WrmsrTx(index, eax, edx, 1) && WrmsrTx(index, eax, edx, 2) && WrmsrTx(index, eax, edx, 4) && WrmsrTx(index, eax, edx, 8))
	{
		cstr.Format(_T("index     63-32    31-0\r\n"));
		edit += cstr;
		cstr.Format(_T("%08X: %08X %08X\r\n"), index, edx, eax);
		edit += cstr;
	}
	else
	{
		edit += _T("Failure : wrmsrtx 0x199 fail!\r\n");		
	}

	//-----------------------------------------------------------------------------
	// TSC
	//-----------------------------------------------------------------------------
	//DWORD index, eax, ebx, ecx, edx;
	cstr.Format(_T("[TSC]\r\n"));
	edit += cstr;

	// RdtscPx uses Process Affinity Mask
	// RdtscPx
	if(RdtscPx(&eax, &edx, 1))
	{
		cstr.Format(_T("63-32    31-0\r\n"));
		edit += cstr;
		cstr.Format(_T("%08X %08X\r\n"), edx, eax);
		edit += cstr;
	}
	else
	{
		edit += _T("Failure : Change Process Affinity Mask\r\n");
	}

	//-----------------------------------------------------------------------------
	// MSR
	//-----------------------------------------------------------------------------
	cstr.Format(_T("[MSR - TSC]\r\n"));
	edit += cstr;
	index = 0x00000010; // Time Stamp Counter
	// RdmsrTx uses Thread Affinity Mask
	if(RdmsrTx(index, &eax, &edx, 1))
	{
		cstr.Format(_T("index     63-32    31-0\r\n"));
		edit += cstr;
		cstr.Format(_T("%08X: %08X %08X\r\n"), index, edx, eax);
		edit += cstr;
	}
	else
	{
		edit += _T("Failure : Change Thread Affinity Mask\r\n");		
	}

	//-----------------------------------------------------------------------------
	// CPUID (Standard/Extended)
	//-----------------------------------------------------------------------------
	DWORD maxCpuid, maxCpuidEx;

	cstr.Format(_T("[CPUID]\r\n"));
	edit += cstr;
	cstr.Format(_T("index     EAX      EBX      ECX      EDX  \r\n"));
	edit += cstr;

	// Standard
	CpuidPx(0x00000000, &maxCpuid, &ebx, &ecx, &edx, 1);
	for(index = 0x00000000; index <= maxCpuid; index++)
	{
		if(CpuidPx(index, &eax, &ebx, &ecx, &edx, 1))
		{
			cstr.Format(_T("%08X: %08X %08X %08X %08X\r\n"), index, eax, ebx, ecx, edx);
			edit += cstr;
		}
	}

	// Extended
	CpuidPx(0x80000000, &maxCpuidEx, &ebx, &ecx, &edx, 1);
	for(index = 0x80000000; index <= maxCpuidEx; index++)
	{
		if(CpuidPx(index, &eax, &ebx, &ecx, &edx, 1))
		{
			cstr.Format(_T("%08X: %08X %08X %08X %08X\r\n"), index, eax, ebx, ecx, edx);
			edit += cstr;
		}
	}

	//-----------------------------------------------------------------------------
	// I/O Write 80
	//-----------------------------------------------------------------------------
	WriteIoPortByte(0x80, 0x55);
	Sleep(100);
	WriteIoPortByte(0x80, 0x55);
	Sleep(1000);

	//-----------------------------------------------------------------------------
	// I/O (Beep)
	//-----------------------------------------------------------------------------
	DWORD freq = 1193180000 / 440000; // 440Hz

	WriteIoPortByte(0x43, 0xB6);
	WriteIoPortByte(0x42, (BYTE)(freq & 0xFF));
	WriteIoPortByte(0x42, (BYTE)(freq >> 9));

	Sleep(100);

	WriteIoPortByte(0x61, (ReadIoPortByte(0x61) | 0x03));

	Sleep(1000);

	WriteIoPortByte(0x61, (ReadIoPortByte(0x61) & 0xFC));

	cstr.Format(_T("[I/O]\r\nBeep 440Hz\r\n"));
	edit += cstr;

	//-----------------------------------------------------------------------------
	// PCI
	//-----------------------------------------------------------------------------
	DWORD address, value;

	SetPciMaxBusIndex(7);
	cstr.Format(_T("[PCI]\r\n"));
	edit += cstr;

	// All Device
	cstr.Format(_T("Bus Dev Fnc VendorDevice\r\n"));
	edit += cstr;
	for(DWORD bus = 0; bus <= 255; bus++)
	{
		for(DWORD dev = 0; dev < 32; dev++)
		{
			for(DWORD func = 0; func < 8; func++)
			{
				address = PciBusDevFunc(bus, dev, func);
				// No Error Check
				value = ReadPciConfigDword(address, 0x00);
				// Error Check
				// if(! ReadPciConfigDwordEx(address, 0x00, &value))
				//{
				//		AfxMessageBox(_T("(ReadPciConfig Read Error"));
				//}

				if((value & 0xFFFF) != 0xFFFF && (value & 0xFFFF) != 0x0000)
				{
					cstr.Format(_T("%02Xh %02Xh %02Xh %04Xh %04Xh\r\n"), 
						bus, dev, func,
						value & 0x0000FFFF,
						(value >> 16) & 0x0000FFFF
						);
					edit += cstr;
				}
			}
		}
	}

	// Host Bridge
	address = FindPciDeviceByClass(0x06, 0x00, 0x00, 0);
	if(address != 0xFFFFFFFF)
	{
        edit += _T("[PCI Confguration Space Dump] HostBridge\r\n"); 
        edit += _T("    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\r\n");
        edit += _T("---------------------------------------------------\r\n");

        for (int i = 0; i < 256; i+= 16)
        {
			cstr.Format(_T("%02X|"), i);
			edit += cstr;
			for (int j = 0; j < 16; j++)
			{
				value = ReadPciConfigByte(address, i + j);
				cstr.Format(_T(" %02X"), value);
				edit += cstr;
			}
            edit += "\r\n";
        }
	}

	//-----------------------------------------------------------------------------
	// DMI
	//-----------------------------------------------------------------------------
	// ReadDmi is obsolete.//64KB

	BYTE b1[MAP_MEMORY_SIZE];
	BYTE *p = (BYTE *)b1;
	DWORD j;

	cstr.Format(_T("[MEMORY]\r\n"));
	edit += cstr;

	if(ReadDmiMemory(b1, MAP_MEMORY_SIZE, sizeof(BYTE)) == MAP_MEMORY_SIZE)
	{
		for(j = 0; j < MAP_MEMORY_SIZE; j += 16)
		{
			if(memcmp(p, "_SM_", 4) == 0)
			{
				cstr.Format(_T("SM BIOS Version : %s.%s"), DmiStringB(p[6], 0), DmiStringB(p[7], 0));
				break;
			}
			p+=16;
		}
		edit += cstr;
	}
	cstr.Format(_T("\r\n"));
	edit += cstr;

	//-----------------------------------------------------------------------------
	// Memory dump(fail)
	//-----------------------------------------------------------------------------
	BYTE b2[MEMORY_DUMP_SIZE];
	DWORD mem_address = 0x40000000;
	DWORD result = 0x1;

	result = ReadPhysicalMemory(mem_address, b2, MEMORY_DUMP_SIZE, sizeof(BYTE));
	cstr.Format(_T("[MEMORY DUMP 0x4000_0000] result = %d\r\n"), result);
	edit += cstr;
	edit += _T("    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\r\n");
	edit += _T("---------------------------------------------------\r\n");
	
	for (int i = 0; i < MEMORY_DUMP_SIZE; i+=16)
	{
		cstr.Format(_T("%02X|"), i);
		edit += cstr;
		for (int j = 0; j < 16; j++)
		{
			cstr.Format(_T(" %02X"), b2[i+j]);
			edit += cstr;
		}
		edit += "\r\n";
	}
	cstr.Format(_T("\r\n"));
	edit += cstr;

	m_Edit.SetWindowText(edit);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cwring0Dlg::OnPaint()
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
HCURSOR Cwring0Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL IsNT()
{
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);

	return (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT);
}