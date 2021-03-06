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
	, m_sensitivityRotate(100)
	, m_sensitivityTranslate(150)
	, m_sensitivityScale(300)
{
}

CSensitivityDialog::~CSensitivityDialog()
{
}

Vec4 CSensitivityDialog::GetSensitivity()
{	
	return Vec4(m_sensitivityTranslate,
				m_sensitivityRotate,
				m_sensitivityScale);
}

void CSensitivityDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ROT, m_sensitivityRotate);
	DDV_MinMaxDouble(pDX, m_sensitivityRotate, 3, 4000);
	DDX_Text(pDX, IDC_EDIT_TRANS, m_sensitivityTranslate);
	DDV_MinMaxDouble(pDX, m_sensitivityTranslate, 10, 4000);
	DDX_Text(pDX, IDC_EDIT_SCALE, m_sensitivityScale);
	DDV_MinMaxDouble(pDX, m_sensitivityScale, 100, 4000);
}


BEGIN_MESSAGE_MAP(CSensitivityDialog, CDialogEx)
END_MESSAGE_MAP()


// CSensitivityDialog message handlers
