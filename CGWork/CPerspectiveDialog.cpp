// CPerspectiveDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "CPerspectiveDialog.h"
#include "afxdialogex.h"


// CPerspectiveDialog dialog

IMPLEMENT_DYNAMIC(CPerspectiveDialog, CDialogEx)

CPerspectiveDialog::CPerspectiveDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PRESPECTIVE_DIALOG, pParent)
{

}

CPerspectiveDialog::~CPerspectiveDialog()
{
}

void CPerspectiveDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NEAR, NearPlane);
	DDV_MinMaxDouble(pDX, NearPlane, 0.001, FarPlane);
	DDX_Text(pDX, IDC_EDIT_FAR, FarPlane);
	DDV_MinMaxDouble(pDX, FarPlane, NearPlane, 10000.0);
	DDX_Text(pDX, IDC_EDIT_FOV, FOV);
	DDV_MinMaxDouble(pDX, FOV, 10.0, 180.0);
}


BEGIN_MESSAGE_MAP(CPerspectiveDialog, CDialogEx)
END_MESSAGE_MAP()


// CPerspectiveDialog message handlers
