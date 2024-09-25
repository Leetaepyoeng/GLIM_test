
// mfcCImageDlg.h: 헤더 파일
//

#pragma once


// CmfcCImageDlg 대화 상자
class CmfcCImageDlg : public CDialogEx
{
// 생성입니다.
private:
	CImage m_image;
	CImage m_imgFile;
	bool m_drawCross; // 십자가 여부
	BOOL validImgPos(int x, int y);
	

public:
	CmfcCImageDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCCIMAGE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

private:
	CBitmapButton* m_pBtnOnOff;
	void initButtons();

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnImage();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnLoad();
	int Clamp(int value, int min, int max);
	void InitDisplay();
	void UpdateDisplay();
	void MoveCircle(int nSttX, int nSttY, int nEndX, int nEndY, float progress);
	void drawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray);
	bool isInCircle(int x, int y, int nCenterX, int nCenterY, int nRadius);
	afx_msg void OnBnClickedBtnAction();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnOnOff();
	int nSttX;
	int nSttY;
	int nEndX;
	int nEndY;
};
