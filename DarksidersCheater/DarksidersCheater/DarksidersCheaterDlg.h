
// DarksidersCheaterDlg.h: 头文件
//

#pragma once


// CDarksidersCheaterDlg 对话框
class CDarksidersCheaterDlg : public CDialogEx
{
	
// 构造
public:
	CDarksidersCheaterDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DARKSIDERSCHEATER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CButton m_killBtn;
	CButton m_richSpiritBtn;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedKill();
	afx_msg void OnBnClickedRichspirit();
private:
	friend DWORD monitorThreadFunc(LPVOID);
	HANDLE m_monitorThread; // 用于循环检测游戏是否启动的线程
};
