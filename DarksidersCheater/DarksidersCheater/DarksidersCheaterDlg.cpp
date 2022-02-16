
// DarksidersCheaterDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "DarksidersCheater.h"
#include "DarksidersCheaterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define log(fmt, ...) \
CString str; \
str.Format(CString(fmt), __VA_ARGS__); \
AfxMessageBox(str);

CDarksidersCheaterDlg* g_dlg;//保存当前dlg的指针
HANDLE g_gameProcess; // 保存游戏的进程

// 将某个值写入暗黑血统内存（后面的可变参数是地址链，要以-1结尾）
void WriteMemory(void* value, DWORD64 valueSize, ...) {
	if (value == NULL || valueSize == 0 || g_gameProcess == NULL) return;

	DWORD64 tempValue = 0;

	va_list addresses;
	va_start(addresses, valueSize);
	DWORD64 offset = 0;
	DWORD64 lastAddress = 0;
	while ((offset = va_arg(addresses, DWORD64)) != 0xffffffffffffffff) {
		lastAddress = tempValue + offset;
		//该函数用来读取指定进程的空间的数据
		/*
			BOOL ReadProcessMemory(
			  HANDLE hProcess,  // 目标进程句柄
			  LPCVOID lpBaseAddress,// 读取数据的起始地址
			  LPVOID lpBuffer,  // 存放数据的缓存区地址
			  DWORD nSize,      // 要读取的字节数
			  LPDWORD lpNumberOfBytesRead 
								// 实际读取数存放地址
			);
		*/
		::ReadProcessMemory(g_gameProcess, (LPCVOID)lastAddress, &tempValue, sizeof(DWORD64), NULL);
	}
	va_end(addresses);

	::WriteProcessMemory(g_gameProcess, (LPVOID)lastAddress, value, valueSize, NULL);
}


void WriteMemory(void* value, DWORD64 valueSize, DWORD64 address) {
	WriteMemory(value, valueSize, address, 0xffffffffffffffff);
}


DWORD monitorThreadFunc(LPVOID) {
	//获得暗黑血统的句柄
	while (1)
	{
		HWND handle = FindWindow(CString("UnrealWindow"), CString("ProjectMayhem  "));

		if (!handle) {
			//handle不存在的时候，不能开启外挂
			g_dlg->m_killBtn.SetCheck(FALSE);
			g_dlg->m_killBtn.EnableWindow(FALSE);
			g_dlg->m_richSpiritBtn.SetCheck(FALSE);
			g_dlg->m_richSpiritBtn.EnableWindow(FALSE);
		}
		else {

			//获得暗黑血统进程的句柄
			DWORD pid;
			GetWindowThreadProcessId(handle, &pid);
			g_gameProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
			g_dlg->m_killBtn.EnableWindow(TRUE);
			g_dlg->m_richSpiritBtn.EnableWindow(TRUE);

			if (g_dlg->m_richSpiritBtn.GetCheck()) {
				BYTE data[] = { 0xBA, 0x60, 0xAE,0x0A,0x00,0x8B,0xC2,0x90 };
				WriteMemory(data, sizeof(data), (DWORD64)0x00007FF674C8044B);
			}
			else {
				BYTE data[] = { 0x41, 0x03, 0x10,0x85,0xD2,0x0F,0x4E,0xD0 };
				WriteMemory(data, sizeof(data), (DWORD64)0x00007FF674C8044B);
			}
		}
		Sleep(1000);
	}

	return NULL;
} 

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CDarksidersCheaterDlg 对话框



CDarksidersCheaterDlg::CDarksidersCheaterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DARKSIDERSCHEATER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDarksidersCheaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_KILL, m_killBtn);
	DDX_Control(pDX, IDC_RICHSPIRIT, m_richSpiritBtn);
}

BEGIN_MESSAGE_MAP(CDarksidersCheaterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_KILL, &CDarksidersCheaterDlg::OnBnClickedKill)
	ON_BN_CLICKED(IDC_RICHSPIRIT, &CDarksidersCheaterDlg::OnBnClickedRichspirit)
END_MESSAGE_MAP()


// CDarksidersCheaterDlg 消息处理程序

BOOL CDarksidersCheaterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	/*
	窗口初始化的时候，就需要初始化一个新的线程，
	用于实现一直监控游戏进程是否启动
	*/
	m_monitorThread = CreateThread(NULL, NULL, monitorThreadFunc, NULL, NULL, NULL);
	g_dlg = this;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDarksidersCheaterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDarksidersCheaterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDarksidersCheaterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDarksidersCheaterDlg::OnBnClickedKill()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_killBtn.GetCheck()) { // 需要敌人自爆
		BYTE data[] = { 0x57, 0xF6};
		WriteMemory(data, sizeof(data), 0x00007FF674CF8603);
	}
	else { // 不需要敌人自爆
		BYTE data[] = { 0x28, 0xF0};
		WriteMemory(data, sizeof(data), 0x00007FF674CF8603);
	}
}


void CDarksidersCheaterDlg::OnBnClickedRichspirit()
{
	// TODO: 在此添加控件通知处理程序代码
}
