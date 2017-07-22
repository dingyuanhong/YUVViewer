
// YUVViewerDlg.h : header file
//

#pragma once

#include <regex>
#include "CodeTransport.h"
#include "yuvConfigure.h"
#include "libyuv.h"

#define TIME_PALY_YUV 1

// CYUVViewerDlg dialog
class CYUVViewerDlg : public CDialogEx
{
// Construction
public:
	CYUVViewerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_YUVVIEWER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	void SetResolutionSel(int sel);
	void SetFrame(int First, int End);
	void UpdataFrame(int Cur, int End);
	int GetWidth();
	int GetHeight();
	ULONGLONG GetFileSize();
	CString GetFilePath();
	int GetFrameSize();
	int GetFrameCount();
	int GetDisplay();
	void DrawYUV();
	void ProcessYUV(uint8_t * frame, int size);
	void Process(uint8_t * frame, int size);

	uint32 GetFourcc();
	void InitBITMAP(int width, int height);
	void SetBITMAPSize(int width, int height);
private:
	ULONGLONG m_filesize;
	int m_index;
	int m_width;
	int m_height;
	BITMAPINFO bmi_;
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
	afx_msg void OnTimer(UINT_PTR);
	afx_msg void OnBnClickedBtnFile();
	afx_msg void OnBnClickedBtnRefresh();
	afx_msg void OnBnClickedBtnJump();
	afx_msg void OnBnClickedBtnForward();
	afx_msg void OnBnClickedBtnBack();
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnBnClickedBtnPause();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnCbnSelchangeComboFourcc();
	afx_msg void OnCbnSelchangeComboResolution();
	afx_msg void OnCbnSelchangeComboDisplay();
};
