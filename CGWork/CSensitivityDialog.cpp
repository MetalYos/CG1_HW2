// CSensitivityDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "CSensitivityDialog.h"
#include "afxdialogex.h"


// CSensitivityDialog dialog

IMPLEMENT_DYNAMIC(CSensitivityDialog, CDialogEx)

CSensitivityDialog::CSensitivityDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SENSITIVITY_DLG, pParent)
	, RotationSensitivity(MID_ROT)
	, TranslationSensitivity(MID_TRANS)
	, ScalingSensitivity(MID_SCALE)
{
}

CSensitivityDialog::~CSensitivityDialog()
{
}

void CSensitivityDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ROTATION_SENS_SLIDER, RotationControl);
	DDX_Control(pDX, IDC_TRANSLATION_SENS_SLIDER, TranslationControl);
	DDX_Control(pDX, IDC_SCALING_SENS_SLIDER, ScalingControl);
	DDX_Slider(pDX, IDC_ROTATION_SENS_SLIDER, RotationSensitivity);
	DDX_Slider(pDX, IDC_TRANSLATION_SENS_SLIDER, TranslationSensitivity);
	DDX_Slider(pDX, IDC_SCALING_SENS_SLIDER, ScalingSensitivity);
}


BEGIN_MESSAGE_MAP(CSensitivityDialog, CDialogEx)
END_MESSAGE_MAP()


// CSensitivityDialog message handlers


BOOL CSensitivityDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	RotationControl.SetRange(0, rotMax-rotMin, true);
	ScalingControl.SetRange(0, scaleMax-scaleMin, true);
	TranslationControl.SetRange(0, transMax-transMin, true);
	
	


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

Vec4 CSensitivityDialog::GetSensitivity()
{
	return Vec4(TranslationSensitivity, RotationSensitivity, ScalingSensitivity);
}

Vec4 CSensitivityDialog::GetInitialSensitivity()
{
	return Vec4(MID_TRANS, MID_ROT, MID_SCALE);
}
