
// mfcCImageDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "mfcCImage.h"
#include "mfcCImageDlg.h"
#include "afxdialogex.h"

#include <direct.h>   // _mkdir 사용을 위한 헤더
#include <shlwapi.h>  // 경로 처리를 위한 헤더
#include <atlimage.h> // CImage 사용을 위한 헤더
#include <time.h>      // time_t, time(), localtime(), struct tm
#include <tchar.h>     // TCHAR, MAX_PATH
#include <windows.h>   // Windows API 관련 함수 및 구조체
#include <stdio.h>    // _stprintf_s

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CmfcCImageDlg 대화 상자



CmfcCImageDlg::CmfcCImageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCCIMAGE_DIALOG, pParent)
	, nSttX(0)
	, nSttY(0)
	, nEndX(0)
	, nEndY(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


	m_drawCross = false; // 예: 십자가를 그릴지 여부를 초기화
}

void CmfcCImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STT_X, nSttX);
	DDX_Text(pDX, IDC_EDIT_STT_Y, nSttY);
	DDX_Text(pDX, IDC_EDIT_END_X, nEndX);
	DDX_Text(pDX, IDC_EDIT_END_Y, nEndY);
}

BEGIN_MESSAGE_MAP(CmfcCImageDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_IMAGE, &CmfcCImageDlg::OnBnClickedBtnImage)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CmfcCImageDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_LOAD, &CmfcCImageDlg::OnBnClickedBtnLoad)
	ON_BN_CLICKED(IDC_BTN_ACTION, &CmfcCImageDlg::OnBnClickedBtnAction)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_ON_OFF, &CmfcCImageDlg::OnBnClickedBtnOnOff)
	ON_BN_CLICKED(IDC_BTN_DRAW, &CmfcCImageDlg::OnBnClickedBtnDraw)
END_MESSAGE_MAP()


// CmfcCImageDlg 메시지 처리기

BOOL CmfcCImageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	//외부 버튼
	//initButtons();
	InitDisplay();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CmfcCImageDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

//void CmfcCImageDlg::OnPaint()
//{
//	if (IsIconic())
//	{
//		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
//
//		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
//
//		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
//		int cxIcon = GetSystemMetrics(SM_CXICON);
//		int cyIcon = GetSystemMetrics(SM_CYICON);
//		CRect rect;
//		GetClientRect(&rect);
//		int x = (rect.Width() - cxIcon + 1) / 2;
//		int y = (rect.Height() - cyIcon + 1) / 2;
//
//		// 아이콘을 그립니다.
//		dc.DrawIcon(x, y, m_hIcon);
//	}
//	else
//	{
//		CDialogEx::OnPaint();
//	}
//}

void CmfcCImageDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 DC 생성

	if (!m_image.IsNull()) {
		// 이미지의 너비와 높이 얻기
		int imgWidth = m_image.GetWidth();
		int imgHeight = m_image.GetHeight();

		// 최대 크기 지정 (640x480)
		int maxWidth = 640;
		int maxHeight = 480;

		// 실제 그릴 크기 초기화
		int drawWidth = imgWidth;
		int drawHeight = imgHeight;

		// 이미지가 최대 크기를 초과하는 경우 비율에 맞게 조정
		if (imgWidth > maxWidth || imgHeight > maxHeight) {
			float aspectRatio = static_cast<float>(imgWidth) / imgHeight;

			if (imgWidth > imgHeight) { // 가로가 더 긴 경우
				drawWidth = maxWidth;
				drawHeight = static_cast<int>(maxWidth / aspectRatio);
			}
			else { // 세로가 더 긴 경우
				drawHeight = maxHeight;
				drawWidth = static_cast<int>(maxHeight * aspectRatio);
			}
		}

		// 왼쪽 상단에 이미지를 그리기
		int x = 0; // X 좌표
		int y = 0; // Y 좌표

		// 이미지 그리기
		m_image.Draw(dc, x, y, drawWidth, drawHeight);

		// 십자가 그리기 (조건부)
		if (m_drawCross) {

			// 십자가 길이
			int crossLength = 5;

			// 가운데 정렬
			int centerX = drawWidth / 2;
			int centerY = drawHeight / 2;


			// 빨간색으로 설정
			CPen redPen(PS_SOLID, 2, RGB(255, 0, 0));
			CPen* pOldPen = dc.SelectObject(&redPen);

			// 십자가 그리기
			dc.MoveTo(centerX - crossLength, centerY);
			dc.LineTo(centerX + crossLength, centerY);
			dc.MoveTo(centerX + 1, centerY - crossLength);
			dc.LineTo(centerX + 1, centerY + crossLength);

			dc.SelectObject(pOldPen); // 원래 펜으로 복원
		}
	}
	else
	{
		// 기본 그리기
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CmfcCImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CmfcCImageDlg::OnBnClickedBtnImage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_drawCross = false;
	
	// 기존 이미지가 존재하면 삭제
	if (!m_image.IsNull()) {
		m_image.Destroy();
	}

	int nWidth = 640;
	int nHeight = 480;
	int nBpp = 8;

	m_image.Create(nWidth, -nHeight, nBpp);
	if (nBpp == 8) {
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++) {
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		}
		m_image.SetColorTable(0, 256, rgb);
	}

	int nPitch = m_image.GetPitch();
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	
	//for (int y = 0; y < nHeight; y++)
	//	for (int x = 0; x < nWidth; x++)
	//		fm[y * nPitch + x] = y % 255;


	memset(fm, 0xff, sizeof(unsigned char) * nWidth * nHeight);


	UpdateDisplay();
	
}


void CmfcCImageDlg::OnBnClickedBtnSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 실행 파일 경로 얻기
	TCHAR path[MAX_PATH];
	GetModuleFileName(NULL, path, MAX_PATH);

	// 실행 파일 경로에서 디렉토리 부분만 가져오기
	PathRemoveFileSpec(path);

	// image 폴더 경로 설정
	CString imagePath = CString(path) + _T("\\image\\");

	// image 폴더가 존재하는지 확인하고, 없으면 생성
	if (!PathFileExists(imagePath))
	{
		_mkdir(CT2A(imagePath)); // C++ 표준 라이브러리로 폴더 생성
	}


	// 현재 날짜 및 시간 가져오기
	time_t t = time(NULL);
	struct tm tm;
	localtime_s(&tm, &t); // localtime_s를 사용하여 안전하게 변환

	// 날짜 및 시간을 파일 이름에 추가
	CString bmpFileName;
	bmpFileName.Format(_T("%s%04d%02d%02d_%02d%02d%02d.jpg"),
		imagePath, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
		tm.tm_hour, tm.tm_min, tm.tm_sec);


	//CString bmpFileName = imagePath + _T("save.bmp");

	m_image.Save(bmpFileName);
}



void CmfcCImageDlg::OnBnClickedBtnLoad()
{
	m_drawCross = true;

	// 기존 이미지가 존재하면 삭제
	if (!m_image.IsNull()) {
		m_image.Destroy(); // 기존 이미지 해제
	}

	// 파일 열기 다이얼로그를 통해 이미지를 불러오는 코드 추가
	CFileDialog fileDlg(TRUE, _T("*.bmp;*.jpg;*.jpeg"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Image Files (*.bmp;*.jpg;*.jpeg)|*.bmp;*.jpg;*.jpeg||"));

	if (fileDlg.DoModal() == IDOK) {
		CString filePath = fileDlg.GetPathName();
		m_image.Load(filePath); // 이미지 로드

	}

	// 이미지 업데이트를 화면에 반영
	Invalidate(); // 화면 새로 고침
}



void CmfcCImageDlg::UpdateDisplay() 
{
	CClientDC dc(this);
	m_image.Draw(dc, 0, 0);
}

void CmfcCImageDlg::MoveCircle(int nSttX, int nSttY, int nEndX, int nEndY, float progress)
{
	//static int nSttX = 0;
	//static int nSttY = 0;
	//int nWidth = m_image.GetWidth();
	//int nHeight = m_image.GetHeight();
	//int nPitch = m_image.GetPitch();
	//int nRadius = 10;
	//unsigned char* fm = (unsigned char*)m_image.GetBits();

	////memset(fm, 0xff, nWidth * nHeight);
	//drawCircle(fm, nSttX, nSttY, nRadius, 0xff);
	//drawCircle(fm, ++nSttX, ++nSttY, nRadius, nGray);

	int nGray = 80;
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	int nPitch = m_image.GetPitch();
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	// 현재 위치 계산
	int currentX = static_cast<int>(nSttX + (nEndX - nSttX) * progress);
	int currentY = static_cast<int>(nSttY + (nEndY - nSttY) * progress);

	// 유효한 좌표인지 확인
	if (validImgPos(currentX, currentY)) {
		InitDisplay();
		// 원 그리기
		drawCircle(fm, currentX, currentY, nRadius, nGray);
		UpdateDisplay();
	}

}

void CmfcCImageDlg::OnBnClickedBtnDraw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 기존 이미지가 존재하면 초기화
	if (!m_image.IsNull()) {
		InitDisplay();
	}

	// nRadius를 3부터 50까지의 랜덤 값으로 설정
	nRadius = rand() % (50 - 3 + 1) + 3;  // 3 ~ 50
	
	// 에딧 컨트롤에서 좌표 읽기
	CString strSttX, strSttY;
	GetDlgItem(IDC_EDIT_STT_X)->GetWindowText(strSttX);
	GetDlgItem(IDC_EDIT_STT_Y)->GetWindowText(strSttY);

	// 문자열을 정수로 변환 및 예외처리
	int nSttX = Clamp(_ttoi(strSttX), 0, 640 - 10);
	int nSttY = Clamp(_ttoi(strSttY), 0, 480 - 10);

	int nGray = 80;
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	int nPitch = m_image.GetPitch();
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	drawCircle(fm, nSttX, nSttY, nRadius, nGray);
	UpdateDisplay();
}

void CmfcCImageDlg::OnBnClickedBtnAction()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 기존 이미지가 존재하면 초기화
	if (!m_image.IsNull()) {
		InitDisplay();
	}
	
	// 에딧 컨트롤에서 좌표 읽기
	CString strSttX, strSttY, strEndX, strEndY;
	GetDlgItem(IDC_EDIT_STT_X)->GetWindowText(strSttX);
	GetDlgItem(IDC_EDIT_STT_Y)->GetWindowText(strSttY);
	GetDlgItem(IDC_EDIT_END_X)->GetWindowText(strEndX);
	GetDlgItem(IDC_EDIT_END_Y)->GetWindowText(strEndY);

	// 문자열을 정수로 변환 및 예외처리
	int nSttX = Clamp(_ttoi(strSttX), 0, 640-10);
	int nSttY = Clamp(_ttoi(strSttY), 0, 480-10);
	int nEndX = Clamp(_ttoi(strEndX), 0, 640-10);
	int nEndY = Clamp(_ttoi(strEndY), 0, 480-10);


	// 20번 이동
	for (int i = 1; i <= 30; i++) {
		float progress = i / 30.0f; // 0.0에서 1.0까지의 진행 비율
		MoveCircle(nSttX, nSttY, nEndX, nEndY, progress);
		Sleep(10); // 이동 간격 조절
	}

	OnBnClickedBtnSave();
}

//좌표에 대한 예외처리
BOOL CmfcCImageDlg::validImgPos(int x, int y)
{
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	CRect rect(0, 0, nWidth, nHeight);

	return rect.PtInRect(CPoint(x, y));
}


void CmfcCImageDlg::drawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray)
{
	int nCenterX = x + nRadius;
	int nCenterY = y + nRadius;
	int nPitch = m_image.GetPitch();
	for (int nY = y; nY < y + nRadius * 2; nY++){
		for (int nX = x; nX < x + nRadius * 2; nX++){
			// 원의 내부 여부 검사
			if (isInCircle(nX, nY, nCenterX, nCenterY, nRadius))
				// nY와 nX가 fm 배열의 유효 범위인지 확인
				if (nY >= 0 && nY < m_image.GetHeight() && nX >= 0 && nX < m_image.GetWidth())
					fm[nY * nPitch + nX] = nGray;
		}
	}
}	

bool CmfcCImageDlg::isInCircle(int x, int y, int nCenterX, int nCenterY, int nRadius)
{
	bool nRet = false;

	double dX = x - nCenterX;
	double dY = y - nCenterY;
	double dDist = dX * dX + dY * dY;

	if (dDist < nRadius * nRadius)
		nRet = true;


	return nRet;
}

void CmfcCImageDlg::initButtons()
{
	CRect rect(0, 0, 0, 0);
	GetDlgItem(IDC_BTN_ON_OFF)->GetWindowRect(&rect);
	//GetDlgItem(IDC_BTN_ON_OFF)->GetClientRect(&rect);
	m_pBtnOnOff = new CBitmapButton;
	m_pBtnOnOff->Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, rect, this, IDC_BTN_ON_OFF);
	m_pBtnOnOff->LoadBitmaps(IDB_OFF);
	m_pBtnOnOff->SizeToContent();
}

void CmfcCImageDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_pBtnOnOff) delete m_pBtnOnOff;
}


void CmfcCImageDlg::OnBnClickedBtnOnOff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static bool bOn = true;
	if(bOn)
		m_pBtnOnOff->LoadBitmaps(IDB_ON);
	else
		m_pBtnOnOff->LoadBitmaps(IDB_OFF);

	bOn = !bOn;
}

void CmfcCImageDlg::InitDisplay() {
	// 기존 이미지가 존재하면 삭제
	if (!m_image.IsNull()) {
		m_image.Destroy();
	}

	int nWidth = 640;
	int nHeight = 480;
	int nBpp = 8;

	m_image.Create(nWidth, -nHeight, nBpp);
	if (nBpp == 8) {
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++) {
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		}
		m_image.SetColorTable(0, 256, rgb);
	}

	int nPitch = m_image.GetPitch();
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	//for (int y = 0; y < nHeight; y++)
	//	for (int x = 0; x < nWidth; x++)
	//		fm[y * nPitch + x] = y % 255;


	memset(fm, 0xff, sizeof(unsigned char) * nWidth * nHeight);


	UpdateDisplay();
}

int CmfcCImageDlg::Clamp(int value, int min, int max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}


