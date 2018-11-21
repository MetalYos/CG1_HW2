#pragma once


// CColorsDialog dialog

class CColorsDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CColorsDialog)

public:
	CColorsDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CColorsDialog();

	//Dialog Params
	COLORREF WireframeColor;
	COLORREF NormalColor;
	COLORREF BackgroundColor;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLOR_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedWireframeColorButton();
	afx_msg void OnBnClickedNormalColorButton();
	afx_msg void OnBnClickedBackgroundColorButton();
	afx_msg void OnBnClickedIsDiscoCheckbox();
	BOOL IsDiscoMode;
};
