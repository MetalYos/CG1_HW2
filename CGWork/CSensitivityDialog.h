#pragma once
#include "Vec4.h"

#define MID_ROT ((rotMin + rotMax) / 2)
#define MID_TRANS (((transMin + transMax) / 2))
#define MID_SCALE ((scaleMin + scaleMax) / 2)
// CSensitivityDialog dialog

class CSensitivityDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSensitivityDialog)

public:
	CSensitivityDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CSensitivityDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SENSITIVITY_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	Vec4 GetSensitivity();
	static Vec4 GetInitialSensitivity();

private:
	int RotationSensitivity;
	int TranslationSensitivity;
	int ScalingSensitivity;
	
	CSliderCtrl RotationControl;
	CSliderCtrl TranslationControl;
	CSliderCtrl ScalingControl;

	static const int rotMin = 1;
	static const int rotMax = 150;
	static const int transMin = 10;
	static const int transMax = 300;
	static const int scaleMin = 10;
	static const int scaleMax = 300;
};
