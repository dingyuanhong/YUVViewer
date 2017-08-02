
// YUVViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "YUVViewer.h"
#include "YUVViewerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#pragma comment(lib,"jpeg.lib")
#pragma comment(lib,"libyuv.lib")

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


// CYUVViewerDlg dialog



CYUVViewerDlg::CYUVViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_YUVVIEWER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CYUVViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CYUVViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_FILE, &CYUVViewerDlg::OnBnClickedBtnFile)
	ON_BN_CLICKED(IDC_BTN_REFRESH, &CYUVViewerDlg::OnBnClickedBtnRefresh)
	ON_BN_CLICKED(IDC_BTN_JUMP, &CYUVViewerDlg::OnBnClickedBtnJump)
	ON_BN_CLICKED(IDC_BTN_FORWARD, &CYUVViewerDlg::OnBnClickedBtnForward)
	ON_BN_CLICKED(IDC_BTN_BACK, &CYUVViewerDlg::OnBnClickedBtnBack)
	ON_BN_CLICKED(IDC_BTN_PLAY, &CYUVViewerDlg::OnBnClickedBtnPlay)
	ON_BN_CLICKED(IDC_BTN_PAUSE, &CYUVViewerDlg::OnBnClickedBtnPause)
	ON_BN_CLICKED(IDC_BTN_STOP, &CYUVViewerDlg::OnBnClickedBtnStop)
	ON_CBN_SELCHANGE(IDC_COMBO_FOURCC, &CYUVViewerDlg::OnCbnSelchangeComboFourcc)
	ON_CBN_SELCHANGE(IDC_COMBO_RESOLUTION, &CYUVViewerDlg::OnCbnSelchangeComboResolution)
	ON_CBN_SELCHANGE(IDC_COMBO_DISPLAY, &CYUVViewerDlg::OnCbnSelchangeComboDisplay)
END_MESSAGE_MAP()


// CYUVViewerDlg message handlers

BOOL CYUVViewerDlg::OnInitDialog()
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
	m_filesize = 0;
	m_index = 0;

	CComboBox * box = (CComboBox*)GetDlgItem(IDC_COMBO_FOURCC);
	for (int i = 0; i < TABLESIZE(YUV_TYPE); i++)
	{
		box->AddString(YUV_TYPE[i]);
	}
	box->SetCurSel(0);
	CEdit * edit = (CEdit*)GetDlgItem(IDC_EDIT_ORDER);
	edit->SetWindowText(YUV_TYPE_ORDER[0]);

	TCHAR buf[MAX_PATH];
	box = (CComboBox*)GetDlgItem(IDC_COMBO_RESOLUTION);
	for (int i = 0; i < TABLESIZE(RESOLUTION_TYPE_NAME); i++)
	{
		_sntprintf_s(buf, MAX_PATH, _T("%4d x %4d (%s)"), WIDTH_TABLE[i], HEIGHT_TABLE[i], RESOLUTION_TYPE_NAME[i]);
		box->AddString(buf);
	}
	_sntprintf_s(buf, MAX_PATH, _T("    W x H (Customized)"));
	box->AddString(buf);
	box->SetCurSel(0);
	SetResolutionSel(0);

	box = (CComboBox*)GetDlgItem(IDC_COMBO_DISPLAY);
	for (int i = 0; i < TABLESIZE(YUV_DISPLAY); i++)
	{
		box->AddString(YUV_DISPLAY[i]);
	}
	box->SetCurSel(0);

	SetFrame(0,0);
	UpdataFrame(m_index, 0);
	InitBITMAP(GetWidth(),GetHeight());
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CYUVViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CYUVViewerDlg::OnPaint()
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
HCURSOR CYUVViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CYUVViewerDlg::SetResolutionSel(int sel)
{
	if (sel >= TABLESIZE(RESOLUTION_TYPE_NAME))
	{
		GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_HEIGHT)->EnableWindow(TRUE);
		return;
	}
	int width = WIDTH_TABLE[sel];
	int height = HEIGHT_TABLE[sel];
	CString strWidth;
	strWidth.Format(_T("%d"), width);
	CString strHeight;
	strHeight.Format(_T("%d"), height);
	GetDlgItem(IDC_EDIT_WIDTH)->SetWindowText(strWidth);
	GetDlgItem(IDC_EDIT_HEIGHT)->SetWindowText(strHeight);
	GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_HEIGHT)->EnableWindow(FALSE);
}

void CYUVViewerDlg::SetFrame(int First,int End)
{
	if (End < 0) return;

	CString strFirst;
	CString strEnd;
	strFirst.Format(_T("%d"),First);
	strEnd.Format(_T("%d"), End);
	
	GetDlgItem(IDC_EDIT_FIRST)->SetWindowText(strFirst);
	GetDlgItem(IDC_EDIT_END)->SetWindowText(strEnd);
	
}

void CYUVViewerDlg::UpdataFrame(int Cur, int End)
{
	if (End < 0) return;
	if (Cur >= End) {
		Cur = End;
	}

	CString strRatio;
	strRatio.Format(_T("%d/%d"), Cur, End);
	GetDlgItem(IDC_EDIT_RATIO)->SetWindowText(strRatio);
}

int CYUVViewerDlg::GetWidth()
{
	CString strWidth;
	GetDlgItem(IDC_EDIT_WIDTH)->GetWindowText(strWidth);
	int width = _ttoi(strWidth);
	return width;
}

int CYUVViewerDlg::GetHeight()
{
	CString strHeight;
	GetDlgItem(IDC_EDIT_HEIGHT)->GetWindowText(strHeight);
	int height = _ttoi(strHeight);
	return height;
}


CString CYUVViewerDlg::GetFilePath()
{
	CString strPath;
	GetDlgItem(IDC_EDIT_FILE)->GetWindowText(strPath);
	return strPath;
}

ULONGLONG CYUVViewerDlg::GetFileSize()
{
	return m_filesize;
}

int CYUVViewerDlg::GetFrameSize()
{
	CComboBox * box = (CComboBox*)GetDlgItem(IDC_COMBO_FOURCC);
	int sel = box->GetCurSel();
	if (sel == -1) return -1;
	int w = m_width;
	int h = m_height;
	if (w <= 0) return -1;
	if (h <= 0) return -1;
	return w*h * YUV_Division[sel].num / YUV_Division[sel].den;
}

uint32 CYUVViewerDlg::GetFourcc()
{
	CComboBox * box = (CComboBox*)GetDlgItem(IDC_COMBO_FOURCC);
	int sel = box->GetCurSel();
	if (sel == -1) return -1;
	return libyuv::CanonicalFourCC(FOURCC_YUV_TYPE[sel]);
}

//http://www.cnblogs.com/biglucky/p/4599331.html
//如果要保留Y分量，那么需将UV置为0x80,如需置白/黑色,Y分量设为FF/00
//UV是色差分量，UV为0就会全是绿色，全为0x80的时候才能看到灰度图。
//通过RGB转换函数就可见原因
//R = Y + 1.4075 *（V - 128）
//G = Y – 0.3455 *（U –128） – 0.7169 *（V –128）
//B = Y + 1.779 *（U – 128）
//RGB整数转换算法
//r = Y + (V - 128) + (((U - 128) * 103) >> 8);
//g = Y - (((U - 128) * 88) >> 8) + (((V - 128) * 183) >> 8)
//b = Y + (U - 128) + (((U - 128) * 198) >> 8)
void CYUVViewerDlg::ProcessYUV(uint8_t * frame, int size)
{
	int display = GetDisplay();
	if (display <= 0) return;
	int type = 0;
	switch (display)
	{
	case 1://Y
		type |= 0x1;
		break;
	case 2://U
		type |= 0x02;
		break;
	case 3://V
		type |= 0x04;
		break;
	case 4://UV
		type |= 0x02 | 0x04;
		break;
	}

#define Y_CLEAN 0x7F //灰色

	CComboBox * box = (CComboBox*)GetDlgItem(IDC_COMBO_FOURCC);
	int sel = box->GetCurSel();
	if (sel != -1)
	{
		switch (FOURCC_YUV_TYPE[sel])
		{
		case libyuv::FOURCC_YV12: //YV12
			{
				int planar = size * 2 / 3;
				if ((type & 0x1) != 0x1)//Y
				{
					memset(frame, Y_CLEAN, planar);
				}
				if ((type & 0x2) != 0x2)//U
				{
					int count = planar / 4;
					memset(frame + planar + count, 0x80, count);
				}
				if ((type & 0x4) != 0x4)//V
				{
					int count = planar / 4;
					memset(frame + planar, 0x80, count);
				}
			}
			break;
		case libyuv::FOURCC_I420:  //I420
			{
				int planar = size * 2 / 3;
				if ((type & 0x1) != 0x1)//Y
				{
					memset(frame, Y_CLEAN, planar);
				}
				if ((type & 0x2) != 0x2)//U
				{
					int count = planar / 4;
					memset(frame + planar, 0x80, count);
				}
				if ((type & 0x4) != 0x4)//V
				{
					int count = planar / 4;
					memset(frame + planar + count, 0x80, count);
				}
			}
			break;
		case libyuv::FOURCC_YUYV:  //YUYV
			if ((type & 0x1) != 0x1)//Y
			{
				int i = 0;
				while (i < size)
				{
					frame[i] = Y_CLEAN;
					i += 2;
				}
			}
			if ((type & 0x2) != 0x2)//U
			{
				int i = 1;
				while (i < size)
				{
					frame[i] = 0x80;
					i += 4;
				}
			}
			if ((type & 0x4) != 0x4)//V
			{
				int i = 3;
				while (i < size)
				{
					frame[i] = 0x80;
					i += 4;
				}
			}
			break;
		case libyuv::FOURCC_I400:  //Y
			if ((type & 0x1) != 0x1)//Y
			{
				memset(frame, Y_CLEAN, size);
			}
			if ((type & 0x2) != 0x2)
			{
			}
			if ((type & 0x4) != 0x4)
			{
			}
			break;
		case libyuv::FOURCC_UYVY:  //UYVY
			if ((type & 0x1) != 0x1)//Y
			{
				int i = 1;
				while (i < size)
				{
					frame[i] = Y_CLEAN;
					i += 2;
				}
			}
			if ((type & 0x2) != 0x2)//U
			{
				int i = 0;
				while (i < size)
				{
					frame[i] = 0x80;
					i += 4;
				}
			}
			if ((type & 0x4) != 0x4)//V
			{
				int i = 2;
				while (i < size)
				{
					frame[i] = 0x80;
					i += 4;
				}
			}
			break;
		case libyuv::FOURCC_NV12:  //NV12
			{
				if ((type & 0x1) != 0x1)//Y
				{
					int i = 1;
					while (i < size)
					{
						frame[i] = Y_CLEAN;
						i += 2;
					}
				}
				int planar = size * 2 / 3;
				if ((type & 0x2) != 0x2)//U
				{
					int count = planar / 4;
					int i = planar;
					while (i < size)
					{
						frame[i] = 0x80;
						i+=2;
					}
				}
				if ((type & 0x4) != 0x4)//V
				{
					int count = planar / 4;
					int i = planar + 1;
					while (i < size)
					{
						frame[i] = 0x80;
						i+=2;
					}
				}
			}
			break;
		case libyuv::FOURCC_NV21:  //NV21
			{
				int planar = size * 2 / 3;
				if ((type & 0x1) != 0x1)
				{
					memset(frame, Y_CLEAN, planar);
				}
				if ((type & 0x2) != 0x2)//U
				{
					int count = planar / 4;
					int i = planar + 1;
					while (i < size)
					{
						frame[i] = 0x80;
						i += 2;
					}
				}
				if ((type & 0x4) != 0x4)//V
				{
					int count = planar / 4;
					int i = planar;
					while (i < size)
					{
						frame[i] = 0x80;
						i += 2;
					}
				}
			}
			break;
		}
	}
}

static void YVYU2YUYV(uint8_t * frame, int size)
{
	int planar = size / 4;
	int uIndex = 1;
	int vIndex = 3;
	for (int i = 0; i < planar; i++)
	{
		uint8_t tmp = frame[uIndex + i * 4];
		frame[uIndex + i * 4] = frame[vIndex + i * 4];
		frame[vIndex + i * 4] = tmp;
	}
}

void CYUVViewerDlg::Process(uint8_t * frame, int size)
{
	if (frame == NULL) return;
	CComboBox * box = (CComboBox*)GetDlgItem(IDC_COMBO_FOURCC);
	int sel = box->GetCurSel();
	if (sel == -1) return ;
	//当YVYU 时,讲数据处理成YUYV用于libyuv处理
	if (sel == 3)
	{
		YVYU2YUYV(frame,size);
	}
}

int CYUVViewerDlg::GetFrameCount()
{
	int size = GetFrameSize();
	if (size < 0) return 0;
	ULONGLONG fszie = GetFileSize();
	return (int)(fszie / size);
}

int CYUVViewerDlg::GetDisplay()
{
	CComboBox *box = (CComboBox*)GetDlgItem(IDC_COMBO_DISPLAY);
	int sel = box->GetCurSel();
	if (sel == -1) return -1;
	return sel;
}

void CYUVViewerDlg::InitBITMAP(int width, int height)
{
	memset(&bmi_, 0, sizeof(bmi_));
	bmi_.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi_.bmiHeader.biPlanes = 1;
	bmi_.bmiHeader.biBitCount = 32;
	bmi_.bmiHeader.biCompression = BI_RGB;
	bmi_.bmiHeader.biWidth = width;
	bmi_.bmiHeader.biHeight = -height;
	bmi_.bmiHeader.biSizeImage = width * height *
		(bmi_.bmiHeader.biBitCount >> 3);
}

void CYUVViewerDlg::SetBITMAPSize(int width, int height)
{
	if (width == bmi_.bmiHeader.biWidth && height == abs(bmi_.bmiHeader.biHeight)) {
		return;
	}
	bmi_.bmiHeader.biWidth = width;
	bmi_.bmiHeader.biHeight = -height;
	bmi_.bmiHeader.biSizeImage = width * height *
		(bmi_.bmiHeader.biBitCount >> 3);
}

static void DrawPicture(CWnd * wnd, const BITMAPINFO bmi, const uint8_t* image)
{
	int height = abs(bmi.bmiHeader.biHeight);
	int width = bmi.bmiHeader.biWidth;
	if (image == NULL) return;

	CWnd * pPicture = wnd;
	if (pPicture == NULL) return;

	HDC hdc = ::GetDC(pPicture->GetSafeHwnd());
	if (hdc == NULL) return;

	RECT rc;
	pPicture->GetClientRect(&rc);

	{
		HDC dc_mem = ::CreateCompatibleDC(hdc);
		::SetStretchBltMode(dc_mem, HALFTONE);

		// Set the map mode so that the ratio will be maintained for us.
		HDC all_dc[] = { hdc, dc_mem };
		for (int i = 0; i < ARRAYSIZE(all_dc); ++i) {
			SetMapMode(all_dc[i], MM_ISOTROPIC);
			SetWindowExtEx(all_dc[i], width, height, NULL);
			SetViewportExtEx(all_dc[i], rc.right, rc.bottom, NULL);
		}

		HBITMAP bmp_mem = ::CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
		HGDIOBJ bmp_old = ::SelectObject(dc_mem, bmp_mem);

		POINT logical_area = { rc.right, rc.bottom };
		DPtoLP(hdc, &logical_area, 1);

		HBRUSH brush = ::CreateSolidBrush(RGB(0, 0, 0));
		RECT logical_rect = { 0, 0, logical_area.x, logical_area.y };
		::FillRect(dc_mem, &logical_rect, brush);
		::DeleteObject(brush);

		int x = 0;
		int y = 0;

		StretchDIBits(dc_mem, x, y, logical_area.x, logical_area.y,
			0, 0, width, height, image, &bmi, DIB_RGB_COLORS, SRCCOPY);

		BitBlt(hdc, 0, 0, logical_area.x, logical_area.y,
			dc_mem, 0, 0, SRCCOPY);

		// Cleanup.
		::SelectObject(dc_mem, bmp_old);
		::DeleteObject(bmp_mem);
		::DeleteDC(dc_mem);
	}
	::ReleaseDC(pPicture->GetSafeHwnd(), hdc);
}

void CYUVViewerDlg::DrawYUV()
{
	UpdataFrame(m_index, GetFrameCount());

	int frameSize = GetFrameSize();
	if (frameSize <= 0) return;
	ULONGLONG fileSize = GetFileSize();
	if (frameSize > fileSize) return;
	int width = m_width;
	int height = m_height;

	int pos = m_index * frameSize;
	CString strPath = GetFilePath();
	std::string file =  P2AString(strPath.GetBuffer());
	
	uint8_t * frame = (uint8_t*)malloc(frameSize);
	if (frame == NULL) return;
	FILE * fp = NULL;
	fopen_s(&fp,file.c_str(), "rb");
	if (fp != NULL) {
		fseek(fp, pos, SEEK_SET);
		size_t ret = fread(frame, 1, frameSize, fp);
		fclose(fp);
		if (ret != frameSize)
		{
			free(frame);
			return;
		}

		ProcessYUV(frame, frameSize);

		int len = width * height * 4;
		uint8_t * argb = (uint8_t*)malloc(len);
		if (argb != NULL)
		{
			Process(frame, frameSize);
			libyuv::ConvertToARGB(frame, frameSize, argb, width * 4, 0, 0, width, height, width, height,
				libyuv::RotationMode::kRotateNone, GetFourcc());

			CWnd * wnd = GetDlgItem(IDC_STATIC_CANVAS);
			SetBITMAPSize(width, height);
			DrawPicture(wnd, bmi_, argb);

			free(argb);
		}
		
	}
	free(frame);
}

void CYUVViewerDlg::OnTimer(UINT_PTR id)
{
	if (id == TIME_PALY_YUV)
	{
		DrawYUV();
		if (m_index < GetFrameCount())
		{
			m_index++;
		}
		else
		{
			m_index = 0;
			KillTimer(TIME_PALY_YUV);
		}
	}
}

void CYUVViewerDlg::OnBnClickedBtnFile()
{
	CFileDialog dlg(true);
	if (dlg.DoModal())
	{
		CString Path = dlg.GetPathName();
		CString Name = dlg.GetFileName();
		ULONGLONG size;
		CFileStatus fileStatus;
		if (CFile::GetStatus(Path, fileStatus))
		{
			size = fileStatus.m_size;
		}
		GetDlgItem(IDC_EDIT_FILE)->SetWindowText(Path);
		m_filesize = size;
		m_index = 0;

		//通过正则获取宽高
		std::string name = P2AString(Name.GetBuffer());
		std::regex regPattern("(\\d+)\\D+(\\d+)\\D*");
		std::cmatch result;
		BOOL bValid = std::regex_search(name.c_str(), result, regPattern);
		int width = 0;
		int height = 0;

		if (result.size() >= 3)
		{
			std::string ws = result[1].str();
			std::string hs = result[2].str();
			width = atoi(ws.c_str());
			height = atoi(hs.c_str());
		}

		int index = -1;
		for (int i = 0; i < TABLESIZE(RESOLUTION_TYPE_NAME); i++)
		{
			if (width == WIDTH_TABLE[i] && height == HEIGHT_TABLE[i])
			{
				index = i;
				break;
			}
		}
		CComboBox *box = (CComboBox*)GetDlgItem(IDC_COMBO_RESOLUTION);
		if (index == -1)
		{
			if (width != 0 || height != 0)
			{
				box->SetCurSel(TABLESIZE(RESOLUTION_TYPE_NAME));
				SetResolutionSel(-1);

				CString strWidth;
				strWidth.Format(_T("%d"), width);
				CString strHeight;
				strHeight.Format(_T("%d"), height);
				GetDlgItem(IDC_EDIT_WIDTH)->SetWindowText(strWidth);
				GetDlgItem(IDC_EDIT_HEIGHT)->SetWindowText(strHeight);
			}
		}
		else
		{
			box->SetCurSel(index);
			SetResolutionSel(index);
		}
		
		m_width = GetWidth();
		m_height = GetHeight();

		int count = GetFrameCount();
		SetFrame(0, count);
		UpdataFrame(m_index, count);
	}
}

void CYUVViewerDlg::OnBnClickedBtnRefresh()
{
	m_width = GetWidth();
	m_height = GetHeight();

	int count = GetFrameCount();
	SetFrame(0, count);
	UpdataFrame(m_index, count);
}

void CYUVViewerDlg::OnBnClickedBtnJump()
{
}

void CYUVViewerDlg::OnBnClickedBtnForward()
{
	if (m_index > 0)
	{
		m_index--;
	}
}

void CYUVViewerDlg::OnBnClickedBtnBack()
{
	if (m_index < GetFrameCount())
	{
		m_index++;
	}
}

void CYUVViewerDlg::OnBnClickedBtnPlay()
{
	m_width = GetWidth();
	m_height = GetHeight();
	SetTimer(TIME_PALY_YUV,1000/30,NULL);
}

void CYUVViewerDlg::OnBnClickedBtnPause()
{
	KillTimer(TIME_PALY_YUV);
}

void CYUVViewerDlg::OnBnClickedBtnStop()
{
	KillTimer(TIME_PALY_YUV);
	m_index = 0;
}

void CYUVViewerDlg::OnCbnSelchangeComboFourcc()
{
	CComboBox * box = (CComboBox*)GetDlgItem(IDC_COMBO_FOURCC);
	int sel = box->GetCurSel();
	if (sel == -1) return;
	CEdit * edit = (CEdit*)GetDlgItem(IDC_EDIT_ORDER);
	edit->SetWindowText(YUV_TYPE_ORDER[sel]);

	int count = GetFrameCount();
	SetFrame(0, count);
	UpdataFrame(m_index,count);
}

void CYUVViewerDlg::OnCbnSelchangeComboResolution()
{
	CComboBox *box = (CComboBox*)GetDlgItem(IDC_COMBO_RESOLUTION);
	int sel = box->GetCurSel();
	if (sel == -1) return;
	SetResolutionSel(sel);

	int count = GetFrameCount();
	SetFrame(0, count);
	UpdataFrame(m_index, count);
}

void CYUVViewerDlg::OnCbnSelchangeComboDisplay()
{
	CComboBox *box = (CComboBox*)GetDlgItem(IDC_COMBO_DISPLAY);
	int sel = box->GetCurSel();
	if (sel == -1) return;
}
