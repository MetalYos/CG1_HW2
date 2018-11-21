// CGWorkView.cpp : implementation of the CCGWorkView class
//
#include "ALMath.h"
#include "stdafx.h"
#include "CGWork.h"

#include "CGWorkDoc.h"
#include "CGWorkView.h"

#include <iostream>
using std::cout;
using std::endl;
#include "MaterialDlg.h"
#include "LightDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "PngWrapper.h"
#include "iritSkel.h"

#include <assert.h>
#include "Scene.h"

// For Status Bar access
#include "MainFrm.h"

// Use this macro to display text messages in the status bar.
#define STATUS_BAR_TEXT(str) (((CMainFrame*)GetParentFrame())->getStatusBar().SetWindowText(str))


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView

IMPLEMENT_DYNCREATE(CCGWorkView, CView)

BEGIN_MESSAGE_MAP(CCGWorkView, CView)
	//{{AFX_MSG_MAP(CCGWorkView)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_LOAD, OnFileLoad)
	ON_COMMAND(ID_VIEW_ORTHOGRAPHIC, OnViewOrthographic)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ORTHOGRAPHIC, OnUpdateViewOrthographic)
	ON_COMMAND(ID_VIEW_PERSPECTIVE, OnViewPerspective)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PERSPECTIVE, OnUpdateViewPerspective)
	ON_COMMAND(ID_ACTION_ROTATE, OnActionRotate)
	ON_UPDATE_COMMAND_UI(ID_ACTION_ROTATE, OnUpdateActionRotate)
	ON_COMMAND(ID_ACTION_SCALE, OnActionScale)
	ON_UPDATE_COMMAND_UI(ID_ACTION_SCALE, OnUpdateActionScale)
	ON_COMMAND(ID_ACTION_TRANSLATE, OnActionTranslate)
	ON_UPDATE_COMMAND_UI(ID_ACTION_TRANSLATE, OnUpdateActionTranslate)
	ON_COMMAND(ID_AXIS_X, OnAxisX)
	ON_UPDATE_COMMAND_UI(ID_AXIS_X, OnUpdateAxisX)
	ON_COMMAND(ID_AXIS_Y, OnAxisY)
	ON_UPDATE_COMMAND_UI(ID_AXIS_Y, OnUpdateAxisY)
	ON_COMMAND(ID_AXIS_Z, OnAxisZ)
	ON_UPDATE_COMMAND_UI(ID_AXIS_Z, OnUpdateAxisZ)
	ON_COMMAND(ID_LIGHT_SHADING_FLAT, OnLightShadingFlat)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_FLAT, OnUpdateLightShadingFlat)
	ON_COMMAND(ID_LIGHT_SHADING_GOURAUD, OnLightShadingGouraud)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_GOURAUD, OnUpdateLightShadingGouraud)
	ON_COMMAND(ID_LIGHT_CONSTANTS, OnLightConstants)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_BUTTON_BBOX, &CCGWorkView::OnButtonBbox)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_BBOX, &CCGWorkView::OnUpdateButtonBbox)
	ON_COMMAND(ID_BUTTON_VERT_NORM, &CCGWorkView::OnButtonVertNorm)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_VERT_NORM, &CCGWorkView::OnUpdateButtonVertNorm)
	ON_COMMAND(ID_BUTTON_POLY_NORM, &CCGWorkView::OnButtonPolyNorm)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_POLY_NORM, &CCGWorkView::OnUpdateButtonPolyNorm)
	ON_COMMAND(ID_NORMAL_CALCULATED, &CCGWorkView::OnNormalCalculated)
	ON_UPDATE_COMMAND_UI(ID_NORMAL_CALCULATED, &CCGWorkView::OnUpdateNormalCalculated)
	ON_COMMAND(ID_BUTTON_COLORS, &CCGWorkView::OnButtonColors)
END_MESSAGE_MAP()

// A patch to fix GLaux disappearance from VS2005 to VS2008
void auxSolidCone(GLdouble radius, GLdouble height) {
        GLUquadric *quad = gluNewQuadric();
        gluQuadricDrawStyle(quad, GLU_FILL);
        gluCylinder(quad, radius, 0.0, height, 20, 20);
        gluDeleteQuadric(quad);
}

/////////////////////////////////////////////////////////////////////////////
// CCGWorkView construction/destruction

CCGWorkView::CCGWorkView()
{
	// Set default values
	m_nAxis = ID_AXIS_X;
	m_nAction = ID_ACTION_ROTATE;
	m_nView = ID_VIEW_ORTHOGRAPHIC;	
	m_bIsPerspective = false;

	m_nLightShading = ID_LIGHT_SHADING_FLAT;

	m_lMaterialAmbient = 0.2;
	m_lMaterialDiffuse = 0.8;
	m_lMaterialSpecular = 1.0;
	m_nMaterialCosineFactor = 32;

	//init the first light to be enabled
	m_lights[LIGHT_ID_1].enabled=true;
	m_pDbBitMap = NULL;
	m_pDbDC = NULL;

	isFirstDraw = true;
	isBBoxOn = false;
	
}

CCGWorkView::~CCGWorkView()
{
}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView diagnostics

#ifdef _DEBUG
void CCGWorkView::AssertValid() const
{
	CView::AssertValid();
}

void CCGWorkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCGWorkDoc* CCGWorkView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCGWorkDoc)));
	return (CCGWorkDoc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView Window Creation - Linkage of windows to CGWork

BOOL CCGWorkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	// An CGWork window must be created with the following
	// flags and must NOT include CS_PARENTDC for the
	// class style.

	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

void CCGWorkView::DrawLine(CDC * pDC, COLORREF color, CPoint a, CPoint b)
{
	int oct = GetOctant(a, b);
	b = TranslateBToFirstOctant(a, b, oct);

	int x, y, dx, dy, d, De, Dne;
	x = a.x;
	y = a.y;
	dx = b.x - a.x;
	dy = b.y - a.y;
	d = 2 * dy - dx;
	De = 2 * dy;
	Dne = 2 * (dy - dx);
	DrawPointOctant(pDC, x, y, color, a, oct);
	while (x < b.x) {
		if (d < 0) {
			d += De;
			x++;
		}
		else {
			d += Dne;
			x++;
			y++;
		}
		DrawPointOctant(pDC, x, y, color, a, oct);
	}
}

void CCGWorkView::DrawPoly(CDC * pDc, std::vector<Edge> edges)
{
	for (Edge e : edges) {
		DrawLine(pDc, e.color, e.a, e.b);
	}
}


int CCGWorkView::GetOctant(CPoint a, CPoint b)
{
	//normalize at b as if a is in (0,0)
	b.x -= a.x;
	b.y -= a.y;

	//problematic areas
	if (b == CPoint(0, 0)) return 1;
	if (b.x == 0) return b.y > 0 ? 7 : 3;
	if (b.y == 0) return b.x > 0 ? 1 : 5;

	//Right half
	if (b.x > 0) {
		//I
		if (b.y < 0) {
			return b.x > abs(b.y) ? 1 : 2;
		}
		//IV
		else {
			return b.y > b.x ? 7 : 8;
		}
	}

	//Left half
	else {
		//II
		if (b.y < 0) {
			return b.x > b.y ? 3 : 4;
		}
		//III
		else
		{
			return abs(b.x) > b.y ? 5 : 6;
		}
	}
}

void CCGWorkView::DrawPointOctant(CDC * pDC, int x, int y, COLORREF color, const CPoint& origA, int oct)
{
	CPoint normalized = CPoint(x - origA.x, y - origA.y);
	CPoint transformed = TranslatePointFrom8th(normalized, oct);
	CPoint actualPoint = origA + transformed;

	pDC->SetPixel(actualPoint, color);
}

CPoint CCGWorkView::TranslateBToFirstOctant(const CPoint & a, const CPoint & b, int oct)
{
	assert(0 < oct && oct <= 8);
	CPoint normalized = b - a;
	CPoint translated = TranslatePointTo8th(normalized, oct);
	return a + translated;
}

//Translates 8th octant point to given octant
CPoint CCGWorkView::TranslatePointFrom8th(CPoint p, int oct)
{
	assert(0 < oct && oct <= 8);

	int x, y;
	switch (oct)
	{
	case 1: x = p.x;	y = -p.y;	break;
	case 2: x = p.y;	y = -p.x;	break;
	case 3: x = -p.y;	y = -p.x;	break;
	case 4: x = -p.x;	y = -p.y;	break;
	case 5: x = -p.x;	y = p.y;	break;
	case 6: x = -p.y;	y = p.x;	break;
	case 7: x = p.y;	y = p.x;	break;
	case 8: x = p.x;	y = p.y;	break;
	default:
		break;
	}
	return CPoint(x, y);
}

CPoint CCGWorkView::TranslatePointTo8th(CPoint p, int oct)
{
	assert(0 < oct && oct <= 8);

	//Translation is identical except 2nd and 6th octants
	if (oct == 2 || oct == 6) {
		p.x *= -1;
		p.y *= -1;
	}

	return TranslatePointFrom8th(p, oct);
}

int CCGWorkView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitializeCGWork();

	return 0;
}


// This method initialized the CGWork system.
BOOL CCGWorkView::InitializeCGWork()
{
	m_pDC = new CClientDC(this);
	
	if ( NULL == m_pDC ) { // failure to get DC
		::AfxMessageBox(CString("Couldn't get a valid DC."));
		return FALSE;
	}

	CRect r;
	GetClientRect(&r);
	m_pDbDC = new CDC();
	m_pDbDC->CreateCompatibleDC(m_pDC);
	SetTimer(1, 1, NULL);
	m_pDbBitMap = CreateCompatibleBitmap(m_pDC->m_hDC, r.right, r.bottom);	
	m_pDbDC->SelectObject(m_pDbBitMap);

	Scene::GetInstance().GetCamera()->LookAt(Vec4(0.0, 0.0, -15.0), Vec4(0.0, 0.0, 1.0), Vec4(0.0, 1.0, 0.0));

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView message handlers


void CCGWorkView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	if ( 0 >= cx || 0 >= cy ) {
		return;
	}

	// save the width and height of the current window
	m_WindowWidth = cx;
	m_WindowHeight = cy;

	// compute the aspect ratio
	// this will keep all dimension scales equal
	m_AspectRatio = (GLdouble)m_WindowWidth/(GLdouble)m_WindowHeight;

	CRect r;
	GetClientRect(&r);
	DeleteObject(m_pDbBitMap);
	m_pDbBitMap = CreateCompatibleBitmap(m_pDC->m_hDC, r.right, r.bottom);	
	m_pDbDC->SelectObject(m_pDbBitMap);
}


BOOL CCGWorkView::SetupViewingFrustum(void)
{
    return TRUE;
}


// This viewing projection gives us a constant aspect ration. This is done by
// increasing the corresponding size of the ortho cube.
BOOL CCGWorkView::SetupViewingOrthoConstAspect(void)
{
	return TRUE;
}





BOOL CCGWorkView::OnEraseBkgnd(CDC* pDC) 
{
	// Windows will clear the window with the background color every time your window 
	// is redrawn, and then CGWork will clear the viewport with its own background color.

	
	return true;
}



/////////////////////////////////////////////////////////////////////////////
// CCGWorkView drawing
/////////////////////////////////////////////////////////////////////////////

void CCGWorkView::OnDraw(CDC* pDC)
{
	static float theta = 0.0f;
	CCGWorkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	    return;
	CRect r;

	GetClientRect(&r);
	CDC *pDCToUse = /*m_pDC*/m_pDbDC;

	if (isFirstDraw)
	{
		m_WindowWidth = r.Width();
		m_WindowHeight = r.Height();
		m_AspectRatio = m_WindowWidth / m_WindowHeight;
		double height = 5;
		double width = 5 * m_AspectRatio;
		
		Scene::GetInstance().GetCamera()->SetPerspective(45.0, m_AspectRatio, 1.0, 1000.0);
		Scene::GetInstance().GetCamera()->SetOrthographic(-width / 2.0,
			width / 2.0, height / 2.0, -height / 2.0, 1, 1000.0);
		isFirstDraw = false;
	}
	
	Vec4 bgColor = Scene::GetInstance().GetBackgroundColor();
	COLORREF bGColorRef = RGB((int)bgColor[0], (int)bgColor[1], (int)bgColor[2]);
	pDCToUse->FillSolidRect(&r, bGColorRef);
	/*
	Vec4 bgColor = Scene::GetInstance().GetBackgroundColor();
	COLORREF bGColorRef = RGB((int)bgColor[0], (int)bgColor[1], (int)bgColor[2]);
	for (int x = r.left; x < r.right; x++)
	{
		for (int y = r.top; y < r.bottom; y++)
		{
			pDCToUse->SetPixel(x, y, bGColorRef);
		}
	}
	*/
	
	std::vector<Model*> models = Scene::GetInstance().GetModels();
	Camera* camera = Scene::GetInstance().GetCamera();
	Mat4 camTransform = camera->GetTranform();
	Mat4 projection = camera->GetProjection();

	Mat4 toView(Vec4(m_WindowWidth / 2.0, 0.0, 0.0, 0.0),
				Vec4(0.0, m_WindowHeight / 2.0, 0.0, 0.0),
				Vec4(0.0, 0.0, 1.0, 0.0),
				Vec4((m_WindowWidth - 1) / 2.0, (m_WindowHeight - 1) / 2.0, 0.0, 1.0));

	
	for (Model* model : models)
	{
		Mat4 transform = model->GetTransform();
		Vec4 color = model->GetColor();
		Vec4 normalColor = model->GetNormalColor();
		for (Geometry* geo : model->GetGeometries())
		{
			// Draw Polys
			std::vector<Poly*> polygons = geo->Polygons;
			for (Poly* p : polygons)
			{
				std::vector<Edge> poly;
				Vec4 polyCenter;
				for (int i = 0; i < p->Vertices.size(); i++)
				{
					// Add edge to poly struct
					Vec4 pos1 = p->Vertices[i]->Pos;
					Vec4 pos2 = p->Vertices[(i + 1) % p->Vertices.size()]->Pos;

					Vec4 clipped1 = pos1 * transform * camTransform * projection;
					Vec4 clipped2 = pos2 * transform * camTransform * projection;

					clipped1 /= clipped1[3];
					clipped2 /= clipped2[3];

					Vec4 pix1Vec(clipped1 * toView);
					Vec4 pix2Vec(clipped2 * toView);

					CPoint pix1(pix1Vec[0], pix1Vec[1]);
					CPoint pix2(pix2Vec[0], pix2Vec[1]);

					poly.push_back({ pix1, pix2 , AL_RAINBOW_CREF });

					// Draw vertex normal if needed
					if (model->AreVertexNormalsOn())
					{
						Vec4 nPos2 = pos1 + p->Vertices[i]->Normal * 0.5;
						Vec4 nClipped2 = nPos2 * transform * camTransform * projection;
						nClipped2 /= nClipped2[3];
						Vec4 nPix2Vec(nClipped2 * toView);
						CPoint nPix2(nPix2Vec[0], nPix2Vec[1]);

						DrawLine(pDCToUse, RGB(normalColor[0], normalColor[1], normalColor[2]), pix1, nPix2);
					}

					polyCenter += pos1;
				}
				polyCenter /= p->Vertices.size();

				DrawPoly(pDCToUse, poly);

				// Draw poly normal if needed
				if (model->ArePolyNormalsOn())
				{
					Vec4 polyNorm = polyCenter - p->Normal * 0.5;

					polyCenter = polyCenter * transform * camTransform * projection;
					polyNorm = polyNorm * transform * camTransform * projection;

					polyCenter /= polyCenter[3];
					polyNorm /= polyNorm[3];

					polyCenter = polyCenter * toView;
					polyNorm = polyNorm * toView;

					CPoint polyCenterPix(polyCenter[0], polyCenter[1]);
					CPoint polyNormPix(polyNorm[0], polyNorm[1]);

					DrawLine(pDCToUse, RGB(normalColor[0], normalColor[1], normalColor[2]), polyCenterPix, polyNormPix);
				}
			}
		}

		// Draw Bounding Box
		if (model->IsBBoxOn())
		{
			std::vector<Poly*> polygons = model->GetBBox();
			for (Poly* p : polygons)
			{
				std::vector<Edge> poly;
				for (int i = 0; i < p->Vertices.size(); i++)
				{
					Vec4 pos1 = p->Vertices[i]->Pos;
					Vec4 pos2 = p->Vertices[(i + 1) % p->Vertices.size()]->Pos;

					Vec4 clipped1 = pos1 * transform * camTransform * projection;
					Vec4 clipped2 = pos2 * transform * camTransform * projection;

					clipped1 /= clipped1[3];
					clipped2 /= clipped2[3];

					Vec4 pix1Vec(clipped1 * toView);
					Vec4 pix2Vec(clipped2 * toView);

					CPoint pix1(pix1Vec[0], pix1Vec[1]);
					CPoint pix2(pix2Vec[0], pix2Vec[1]);

					poly.push_back({ pix1, pix2 , RGB((BYTE)rand() % 255, (BYTE)rand() % 255, (BYTE)rand() % 255) });
				}

				DrawPoly(pDCToUse, poly);
			}
		}
	}

	if (pDCToUse != m_pDC) 
	{
		m_pDC->BitBlt(r.left, r.top, r.Width(), r.Height(), pDCToUse, r.left, r.top, SRCCOPY);
	}
	
	theta += 5;	
}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView CGWork Finishing and clearing...

void CCGWorkView::OnDestroy() 
{
	CView::OnDestroy();

	// delete the DC
	if ( m_pDC ) {
		delete m_pDC;
	}

	if (m_pDbDC) {
		delete m_pDbDC;
	}
}



/////////////////////////////////////////////////////////////////////////////
// User Defined Functions

void CCGWorkView::RenderScene() {
	// do nothing. This is supposed to be overriden...

	return;
}


void CCGWorkView::OnFileLoad() 
{
	TCHAR szFilters[] = _T ("IRIT Data Files (*.itd)|*.itd|All Files (*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("itd"), _T("*.itd"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ,szFilters);

	if (dlg.DoModal () == IDOK) {

		// Create Model
		Scene::GetInstance().CreateModel();

		m_strItdFileName = dlg.GetPathName();		// Full path and filename
		PngWrapper p;
		CGSkelProcessIritDataFiles(m_strItdFileName, 1);
		
		// Build Building Box
		Scene::GetInstance().GetModels().back()->BuildBoundingBox();

		// TODO: Reset Scene

		Invalidate();	// force a WM_PAINT for drawing.
	} 

}





// VIEW HANDLERS ///////////////////////////////////////////

// Note: that all the following Message Handlers act in a similar way.
// Each control or command has two functions associated with it.

void CCGWorkView::OnViewOrthographic() 
{
	m_nView = ID_VIEW_ORTHOGRAPHIC;
	m_bIsPerspective = false;

	Scene::GetInstance().GetCamera()->SwitchProjection(m_bIsPerspective);

	Invalidate();		// redraw using the new view.
}

void CCGWorkView::OnUpdateViewOrthographic(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nView == ID_VIEW_ORTHOGRAPHIC);
}

void CCGWorkView::OnViewPerspective() 
{
	m_nView = ID_VIEW_PERSPECTIVE;
	m_bIsPerspective = true;

	Scene::GetInstance().GetCamera()->SwitchProjection(m_bIsPerspective);

	Invalidate();
}

void CCGWorkView::OnUpdateViewPerspective(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nView == ID_VIEW_PERSPECTIVE);
}




// ACTION HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnActionRotate() 
{
	m_nAction = ID_ACTION_ROTATE;
}

void CCGWorkView::OnUpdateActionRotate(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_ROTATE);
}

void CCGWorkView::OnActionTranslate() 
{
	m_nAction = ID_ACTION_TRANSLATE;
}

void CCGWorkView::OnUpdateActionTranslate(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_TRANSLATE);
}

void CCGWorkView::OnActionScale() 
{
	m_nAction = ID_ACTION_SCALE;
}

void CCGWorkView::OnUpdateActionScale(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_SCALE);
}




// AXIS HANDLERS ///////////////////////////////////////////


// Gets calles when the X button is pressed or when the Axis->X menu is selected.
// The only thing we do here is set the ChildView member variable m_nAxis to the 
// selected axis.
void CCGWorkView::OnAxisX() 
{
	m_nAxis = ID_AXIS_X;
}

// Gets called when windows has to repaint either the X button or the Axis pop up menu.
// The control is responsible for its redrawing.
// It sets itself disabled when the action is a Scale action.
// It sets itself Checked if the current axis is the X axis.
void CCGWorkView::OnUpdateAxisX(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAxis == ID_AXIS_X);
}


void CCGWorkView::OnAxisY() 
{
	m_nAxis = ID_AXIS_Y;
}

void CCGWorkView::OnUpdateAxisY(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAxis == ID_AXIS_Y);
}


void CCGWorkView::OnAxisZ() 
{
	m_nAxis = ID_AXIS_Z;
}

void CCGWorkView::OnUpdateAxisZ(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAxis == ID_AXIS_Z);
}





// OPTIONS HANDLERS ///////////////////////////////////////////




// LIGHT SHADING HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnLightShadingFlat() 
{
	m_nLightShading = ID_LIGHT_SHADING_FLAT;
}

void CCGWorkView::OnUpdateLightShadingFlat(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nLightShading == ID_LIGHT_SHADING_FLAT);
}


void CCGWorkView::OnLightShadingGouraud() 
{
	m_nLightShading = ID_LIGHT_SHADING_GOURAUD;
}

void CCGWorkView::OnUpdateLightShadingGouraud(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nLightShading == ID_LIGHT_SHADING_GOURAUD);
}

// LIGHT SETUP HANDLER ///////////////////////////////////////////

void CCGWorkView::OnLightConstants() 
{
	CLightDialog dlg;

	for (int id=LIGHT_ID_1;id<MAX_LIGHT;id++)
	{	    
	    dlg.SetDialogData((LightID)id,m_lights[id]);
	}
	dlg.SetDialogData(LIGHT_ID_AMBIENT,m_ambientLight);

	if (dlg.DoModal() == IDOK) 
	{
	    for (int id=LIGHT_ID_1;id<MAX_LIGHT;id++)
	    {
		m_lights[id] = dlg.GetDialogData((LightID)id);
	    }
	    m_ambientLight = dlg.GetDialogData(LIGHT_ID_AMBIENT);
	}	
	Invalidate();
}

void CCGWorkView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CView::OnTimer(nIDEvent);
	if (nIDEvent == 1)
		Invalidate();
}

void CCGWorkView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	prevMousePos = point;

	CView::OnLButtonDown(nFlags, point);
}

void CCGWorkView::OnMouseMove(UINT nFlags, CPoint point)
{
	double transSensitivity = 100.0;
	Vec4 sensitivity = Vec4(100.0, 50.0, 100.0);
	double dx = point.x - prevMousePos.x;
	double dy = point.y - prevMousePos.y;

	if ((nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		if (Scene::GetInstance().GetModels().size() == 0)
			return;
		
		Model* model = Scene::GetInstance().GetModels().back();

		if (m_nAction == ID_ACTION_TRANSLATE)
		{
			if (m_nAxis == ID_AXIS_X)
			{
				model->SetTranform(Mat4::Translate(dx / sensitivity[0], 0.0, 0.0));
			}
			if (m_nAxis == ID_AXIS_Y)
			{
				model->SetTranform(Mat4::Translate(0.0, dx / sensitivity[0], 0.0));
			}
			if (m_nAxis == ID_AXIS_Z)
			{
				model->SetTranform(Mat4::Translate(0.0, 0.0, dx / sensitivity[0]));
			}
		}
		else if (m_nAction == ID_ACTION_ROTATE)
		{
			if (m_nAxis == ID_AXIS_X)
			{
				model->SetTranform(Mat4::RotateX(dx / sensitivity[1]));
			}
			if (m_nAxis == ID_AXIS_Y)
			{
				model->SetTranform(Mat4::RotateY(dx / sensitivity[1]));
			}
			if (m_nAxis == ID_AXIS_Z)
			{
				model->SetTranform(Mat4::RotateZ(dx / sensitivity[1]));
			}
		}
		else
		{
			if (m_nAxis == ID_AXIS_X)
			{
				model->SetTranform(Mat4::Scale(1.0 + (dx / sensitivity[2]), 1.0, 1.0));
			}
			if (m_nAxis == ID_AXIS_Y)
			{
				model->SetTranform(Mat4::Scale(1.0, 1.0 + (dx / sensitivity[2]), 1.0));
			}
			if (m_nAxis == ID_AXIS_Z)
			{
				model->SetTranform(Mat4::Scale(1.0, 1.0, 1.0 + (dx / sensitivity[2])));
			}
		}
	}

	prevMousePos = point;


	CView::OnMouseMove(nFlags, point);
}


void CCGWorkView::OnButtonBbox()
{
	// TODO: Add your command handler code here
	isBBoxOn = !isBBoxOn;
	if (Scene::GetInstance().GetModels().size() == 0)
		return;
	Model* model = Scene::GetInstance().GetModels().back();

	model->SetBBox(isBBoxOn);

	Invalidate();
}


void CCGWorkView::OnUpdateButtonBbox(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(isBBoxOn);
}


void CCGWorkView::OnButtonVertNorm()
{
	// TODO: Add your command handler code here
	if (Scene::GetInstance().GetModels().size() == 0)
		return;

	Model* model = Scene::GetInstance().GetModels().back();
	model->SetNormals(!(model->AreVertexNormalsOn()), model->ArePolyNormalsOn());
	Invalidate();
}


void CCGWorkView::OnUpdateButtonVertNorm(CCmdUI *pCmdUI)
{
	if (Scene::GetInstance().GetModels().size() == 0)
		return;
	
	Model* model = Scene::GetInstance().GetModels().back();
	pCmdUI->SetCheck(model->AreVertexNormalsOn());
}


void CCGWorkView::OnButtonPolyNorm()
{
	// TODO: Add your command handler code here
	if (Scene::GetInstance().GetModels().size() == 0)
		return;

	Model* model = Scene::GetInstance().GetModels().back();
	model->SetNormals(model->AreVertexNormalsOn(), !(model->ArePolyNormalsOn()));
	Invalidate();
}


void CCGWorkView::OnUpdateButtonPolyNorm(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (Scene::GetInstance().GetModels().size() == 0)
		return;

	Model* model = Scene::GetInstance().GetModels().back();
	pCmdUI->SetCheck(model->ArePolyNormalsOn());
}



void CCGWorkView::OnNormalCalculated()
{
	// TODO: Add your command handler code here
	Scene::GetInstance().SetCalcNormalState(!Scene::GetInstance().GetCalcNormalState());
}


void CCGWorkView::OnUpdateNormalCalculated(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CWnd* pParent = GetParent();
	CMenu* pMenu = pParent->GetMenu();
	if (pMenu != NULL)
	{
		pMenu->CheckMenuItem(ID_NORMAL_CALCULATED, MF_CHECKED | MF_BYCOMMAND);
		pMenu->CheckMenuItem(ID_NORMAL_FROMFILE, MF_UNCHECKED | MF_BYCOMMAND);
	}
}

void CCGWorkView::OnButtonColors()
{
	if (m_colorDialog.DoModal() == IDOK)
	{
		Invalidate();
	}
}