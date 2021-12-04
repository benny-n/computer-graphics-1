
#include "stdafx.h"
#include "CG_skel_w_MFC.h"
#include "InputDialog.h"

#define IDC_CMD_EDIT 200
#define IDC_X_EDIT 201
#define IDC_Y_EDIT 202
#define IDC_Z_EDIT 203

#define IDC_R_EDIT 204
#define IDC_G_EDIT 205
#define IDC_B_EDIT 206

#define IDC_LEFT_EDIT 207
#define IDC_RIGHT_EDIT 208
#define IDC_BOTTOM_EDIT 209
#define IDC_TOP_EDIT 210
#define IDC_ZNEAR_EDIT 211
#define IDC_ZFAR_EDIT 212

#define IDC_FOVY_EDIT 213
#define IDC_ASPECT_EDIT 214

#define IDC_KAR_EDIT 215
#define IDC_KAG_EDIT 216
#define IDC_KAB_EDIT 217
#define IDC_KDR_EDIT 218
#define IDC_KDG_EDIT 219
#define IDC_KDB_EDIT 220
#define IDC_KSR_EDIT 221
#define IDC_KSG_EDIT 222
#define IDC_KSB_EDIT 223
#define IDC_ALPHA_EDIT 224

#define IDC_LAR_EDIT 225
#define IDC_LAG_EDIT 226
#define IDC_LAB_EDIT 227
#define IDC_LDR_EDIT 228
#define IDC_LDG_EDIT 229
#define IDC_LDB_EDIT 230
#define IDC_LSR_EDIT 231
#define IDC_LSG_EDIT 232
#define IDC_LSB_EDIT 233


#define CMD_EDIT_TITLE "Command"
#define X_EDIT_TITLE "X ="
#define Y_EDIT_TITLE "Y ="
#define Z_EDIT_TITLE "Z ="

#define R_EDIT_TITLE "R ="
#define G_EDIT_TITLE "G ="
#define B_EDIT_TITLE "B ="

#define LEFT_EDIT_TITLE "Left ="
#define RIGHT_EDIT_TITLE "Right ="
#define BOTTOM_EDIT_TITLE "Bottom ="
#define TOP_EDIT_TITLE "Top ="
#define ZNEAR_EDIT_TITLE "Z-Near ="
#define ZFAR_EDIT_TITLE "Z-Far ="

#define FOVY_EDIT_TITLE "Fovy ="
#define ASPECT_EDIT_TITLE "Aspect Ratio ="

#define KAR_EDIT_TITLE "R ="
#define KAG_EDIT_TITLE "G ="
#define KAB_EDIT_TITLE "B ="
#define KDR_EDIT_TITLE "R ="
#define KDG_EDIT_TITLE "G ="
#define KDB_EDIT_TITLE "B ="
#define KSR_EDIT_TITLE "R ="
#define KSG_EDIT_TITLE "G ="
#define KSB_EDIT_TITLE "B ="
#define ALPHA_EDIT_TITLE "Shininess ="
#define AMBIENT_REFLECTION_EDIT_TITLE "Ambient Light Reflection:"
#define DIFFUSE_REFLECTION_EDIT_TITLE "Diffuse Light Reflection:"
#define SPECULAR_REFLECTION_EDIT_TITLE "Specular Light Reflection:"

#define LAR_EDIT_TITLE "R ="
#define LAG_EDIT_TITLE "G ="
#define LAB_EDIT_TITLE "B ="
#define LDR_EDIT_TITLE "R ="
#define LDG_EDIT_TITLE "G ="
#define LDB_EDIT_TITLE "B ="
#define LSR_EDIT_TITLE "R ="
#define LSG_EDIT_TITLE "G ="
#define LSB_EDIT_TITLE "B ="
#define AMBIENT_INTENSITY_EDIT_TITLE "Ambient Light Intensity:"
#define DIFFUSE_INTENSITY_EDIT_TITLE "Diffuse Light Intensity:"
#define SPECULAR_INTENSITY_EDIT_TITLE "Specular Light Intensity:"

// ------------------------
//    Class CInputDialog
// ------------------------

IMPLEMENT_DYNAMIC(CInputDialog, CDialog)

CInputDialog::CInputDialog(CString title) : CDialog(CInputDialog::IDD, NULL), mTitle(title) {}

CInputDialog::~CInputDialog() {}

BOOL CInputDialog::OnInitDialog() {
    CDialog::OnInitDialog();

    SetWindowText(mTitle);

    return TRUE;
}

void CInputDialog::DoDataExchange(CDataExchange* pDX) { CDialog::DoDataExchange(pDX); }

// ----------------------
//    Class CCmdDialog
// ----------------------

CCmdDialog::CCmdDialog(CString title) : CInputDialog(title), mCmd("") {}

CCmdDialog::~CCmdDialog() {}

string CCmdDialog::GetCmd() { return ((LPCTSTR)mCmd); }

void CCmdDialog::DoDataExchange(CDataExchange* pDX) {
    CInputDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_CMD_EDIT, mCmd);
}

// CCmdDialog message handlers
BEGIN_MESSAGE_MAP(CCmdDialog, CInputDialog)
    ON_WM_CREATE ()
    ON_WM_PAINT()
END_MESSAGE_MAP()

int CCmdDialog::OnCreate(LPCREATESTRUCT lpcs) {
    mCmdEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
      CRect(10, 30, 450, 100), this, IDC_CMD_EDIT);

    return 0;
}

void CCmdDialog::OnPaint() {   
    CPaintDC dc(this);
    dc.SetBkMode(TRANSPARENT);

    CRect cmd_rect(10, 10, 450, 30);
    dc.DrawText(CString(CMD_EDIT_TITLE), -1, &cmd_rect, DT_SINGLELINE);

    mCmdEdit.SetFocus();
}

// ----------------------
//    Class CXyzDialog
// ----------------------

CXyzDialog::CXyzDialog(CString title) : CInputDialog(title), mX(0.0), mY(0.0), mZ(0.0) {}

CXyzDialog::~CXyzDialog() {}

vec3 CXyzDialog::getXYZ() { return vec3(mX, mY, mZ); }

void CXyzDialog::DoDataExchange(CDataExchange* pDX) {
    CInputDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_X_EDIT, mX);
    DDX_Text(pDX, IDC_Y_EDIT, mY);
    DDX_Text(pDX, IDC_Z_EDIT, mZ);
}

// CXyzDialog message handlers
BEGIN_MESSAGE_MAP(CXyzDialog, CInputDialog)
    ON_WM_CREATE ()
    ON_WM_PAINT()
END_MESSAGE_MAP()

int CXyzDialog::OnCreate(LPCREATESTRUCT lpcs) {
    mXEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
      CRect(130, 70, 170, 90), this, IDC_X_EDIT);

    mYEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
      CRect(130, 140, 170, 160), this, IDC_Y_EDIT);

    mZEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
      CRect(130, 210, 170, 230), this, IDC_Z_EDIT);

    return 0;
}

void CXyzDialog::OnPaint() {   
    CPaintDC dc(this);
    dc.SetBkMode(TRANSPARENT);

    CRect x_rect(100, 72, 170, 90);
    dc.DrawText(CString(X_EDIT_TITLE), -1, &x_rect, DT_SINGLELINE);

    CRect y_rect(100, 142, 170, 160);
    dc.DrawText(CString(Y_EDIT_TITLE), -1, &y_rect, DT_SINGLELINE);

    CRect z_rect(100, 212, 170, 230);
    dc.DrawText(CString(Z_EDIT_TITLE), -1, &z_rect, DT_SINGLELINE);

    mXEdit.SetFocus();
}

// -------------------------
//    Class CCmdXyzDialog
// -------------------------

CCmdXyzDialog::CCmdXyzDialog(CString title) :  CInputDialog(title), mCmd(""), mX(0.0), mY(0.0), mZ(0.0) {}

CCmdXyzDialog::~CCmdXyzDialog() {}

string CCmdXyzDialog::GetCmd() { return ((LPCTSTR)mCmd); }

vec3 CCmdXyzDialog::getXYZ() { return vec3(mX, mY, mZ); }

void CCmdXyzDialog::DoDataExchange(CDataExchange* pDX) {
    CInputDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_CMD_EDIT, mCmd);
    DDX_Text(pDX, IDC_X_EDIT, mX);
    DDX_Text(pDX, IDC_Y_EDIT, mY);
    DDX_Text(pDX, IDC_Z_EDIT, mZ);
}

// CCmdXyzDialog message handlers
BEGIN_MESSAGE_MAP(CCmdXyzDialog, CInputDialog)
    ON_WM_CREATE ()
    ON_WM_PAINT()
END_MESSAGE_MAP()

int CCmdXyzDialog::OnCreate(LPCREATESTRUCT lpcs) {    
    mCmdEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
      CRect(10, 30, 450, 100), this, IDC_CMD_EDIT);

    mXEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
      CRect(40, 135, 250, 155), this, IDC_X_EDIT);

    mYEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
      CRect(40, 190, 250, 210), this, IDC_Y_EDIT);

    mZEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
      CRect(40, 245, 250, 265), this, IDC_Z_EDIT);

    return 0;
}

void CCmdXyzDialog::OnPaint() {   
    CPaintDC dc(this);
    dc.SetBkMode(TRANSPARENT);

    CRect cmd_rect(10, 10, 450, 30);
    dc.DrawText(CString(CMD_EDIT_TITLE), -1, &cmd_rect, DT_SINGLELINE);

    CRect x_rect(10, 137, 450, 155);
    dc.DrawText(CString(X_EDIT_TITLE), -1, &x_rect, DT_SINGLELINE);

    CRect y_rect(10, 192, 450, 210);
    dc.DrawText(CString(Y_EDIT_TITLE), -1, &y_rect, DT_SINGLELINE);

    CRect z_rect(10, 247, 450, 265);
    dc.DrawText(CString(Z_EDIT_TITLE), -1, &z_rect, DT_SINGLELINE);

    mCmdEdit.SetFocus();
}

// -------------------------
//    Class CRGBDialog
// -------------------------

CRGBDialog::CRGBDialog(CString title) : CInputDialog(title), mR(1.0), mG(1.0), mB(1.0) {}

CRGBDialog::~CRGBDialog() {}

Color CRGBDialog::getRGB() { return Color{ mR, mG, mB }; }

void CRGBDialog::DoDataExchange(CDataExchange* pDX) {
    CInputDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_R_EDIT, mR);
    DDV_MinMaxFloat(pDX, mR, 0, 1);
    DDX_Text(pDX, IDC_G_EDIT, mG);
    DDV_MinMaxFloat(pDX, mG, 0, 1);
    DDX_Text(pDX, IDC_B_EDIT, mB);
    DDV_MinMaxFloat(pDX, mB, 0, 1);
}

// CRGBDialog message handlers
BEGIN_MESSAGE_MAP(CRGBDialog, CInputDialog)
    ON_WM_CREATE()
    ON_WM_PAINT()
END_MESSAGE_MAP()

int CRGBDialog::OnCreate(LPCREATESTRUCT lpcs) {
    mREdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(130, 70, 170, 90), this, IDC_R_EDIT);

    mGEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(130, 140, 170, 160), this, IDC_G_EDIT);

    mBEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(130, 210, 170, 230), this, IDC_B_EDIT);

    return 0;
}

void CRGBDialog::OnPaint() {
    CPaintDC dc(this);
    dc.SetBkMode(TRANSPARENT);

    CRect r_rect(100, 72, 170, 90);
    dc.DrawText(CString(R_EDIT_TITLE), -1, &r_rect, DT_SINGLELINE);

    CRect g_rect(100, 142, 170, 160);
    dc.DrawText(CString(G_EDIT_TITLE), -1, &g_rect, DT_SINGLELINE);

    CRect b_rect(100, 212, 170, 230);
    dc.DrawText(CString(B_EDIT_TITLE), -1, &b_rect, DT_SINGLELINE);

    mREdit.SetFocus();
}

// -------------------------
//    Class CBoxDialog
// -------------------------

CBoxDialog::CBoxDialog(CString title) : CInputDialog(title), mLeft(-5), mRight(5), mBottom(-5), mTop(5), mZNear(5), mZFar(15) {}

CBoxDialog::~CBoxDialog() {}

float CBoxDialog::getLeft() { return mLeft; }
float CBoxDialog::getRight() { return mRight; }
float CBoxDialog::getBottom() { return mBottom; }
float CBoxDialog::getTop() { return mTop; }
float CBoxDialog::getZNear() { return mZNear; }
float CBoxDialog::getZFar() { return mZFar; }

void CBoxDialog::DoDataExchange(CDataExchange* pDX) {
    CInputDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_LEFT_EDIT, mLeft);
    DDX_Text(pDX, IDC_RIGHT_EDIT, mRight);
    DDX_Text(pDX, IDC_BOTTOM_EDIT, mBottom);
    DDX_Text(pDX, IDC_TOP_EDIT, mTop);
    DDX_Text(pDX, IDC_ZNEAR_EDIT, mZNear);
    DDX_Text(pDX, IDC_ZFAR_EDIT, mZFar);
}

// CBoxDialog message handlers
BEGIN_MESSAGE_MAP(CBoxDialog, CInputDialog)
    ON_WM_CREATE()
    ON_WM_PAINT()
END_MESSAGE_MAP()

int CBoxDialog::OnCreate(LPCREATESTRUCT lpcs) {
    mLeftEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(150, 70, 190, 90), this, IDC_LEFT_EDIT);

    mRightEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(330, 70, 370, 90), this, IDC_RIGHT_EDIT);

    mBottomEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(150, 140, 190, 160), this, IDC_BOTTOM_EDIT);

    mTopEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(330, 140, 370, 160), this, IDC_TOP_EDIT);

    mZNearEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(150, 210, 190, 230), this, IDC_ZNEAR_EDIT);

    mZFarEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(330, 210, 370, 230), this, IDC_ZFAR_EDIT);

    return 0;
}

void CBoxDialog::OnPaint() {
    CPaintDC dc(this);
    dc.SetBkMode(TRANSPARENT);

    CRect left_rect(70, 72, 190, 95);
    dc.DrawText(CString(LEFT_EDIT_TITLE), -1, &left_rect, DT_SINGLELINE);

    CRect right_rect(270, 72, 390, 95);
    dc.DrawText(CString(RIGHT_EDIT_TITLE), -1, &right_rect, DT_SINGLELINE);

    CRect bottom_rect(70, 142, 190, 165);
    dc.DrawText(CString(BOTTOM_EDIT_TITLE), -1, &bottom_rect, DT_SINGLELINE);

    CRect top_rect(270, 142, 390, 165);
    dc.DrawText(CString(TOP_EDIT_TITLE), -1, &top_rect, DT_SINGLELINE);

    CRect z_near_rect(70, 212, 190, 235);
    dc.DrawText(CString(ZNEAR_EDIT_TITLE), -1, &z_near_rect, DT_SINGLELINE);

    CRect z_far_rect(270, 212, 390, 235);
    dc.DrawText(CString(ZFAR_EDIT_TITLE), -1, &z_far_rect, DT_SINGLELINE);

    mLeftEdit.SetFocus();
}

// -------------------------
//    Class CFovyDialog
// -------------------------

CFovyDialog::CFovyDialog(CString title) : CInputDialog(title), mFovy(90), mAspectRatio(1), mZNear(5), mZFar(15) {}

CFovyDialog::~CFovyDialog() {}

float CFovyDialog::getFovy() { return mFovy; }
float CFovyDialog::getAspectRatio() { return mAspectRatio; }
float CFovyDialog::getZNear() { return mZNear; }
float CFovyDialog::getZFar() { return mZFar; }

void CFovyDialog::DoDataExchange(CDataExchange* pDX) {
    CInputDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_FOVY_EDIT, mFovy);
    DDX_Text(pDX, IDC_ASPECT_EDIT, mAspectRatio);
    DDX_Text(pDX, IDC_ZNEAR_EDIT, mZNear);
    DDX_Text(pDX, IDC_ZFAR_EDIT, mZFar);
}

// CBoxDialog message handlers
BEGIN_MESSAGE_MAP(CFovyDialog, CInputDialog)
    ON_WM_CREATE()
    ON_WM_PAINT()
END_MESSAGE_MAP()

int CFovyDialog::OnCreate(LPCREATESTRUCT lpcs) {
    mFovyEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(200, 70, 240, 90), this, IDC_FOVY_EDIT);

    mAspectRatioEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(200, 140, 240, 160), this, IDC_ASPECT_EDIT);

    mZNearEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(200, 210, 240, 230), this, IDC_ZNEAR_EDIT);

    mZFarEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(200, 280, 240, 300), this, IDC_ZFAR_EDIT);

    return 0;
}

void CFovyDialog::OnPaint() {
    CPaintDC dc(this);
    dc.SetBkMode(TRANSPARENT);

    CRect fovy_rect(70, 72, 240, 95);
    dc.DrawText(CString(FOVY_EDIT_TITLE), -1, &fovy_rect, DT_SINGLELINE);

    CRect aspect_rect(70, 142, 240, 165);
    dc.DrawText(CString(ASPECT_EDIT_TITLE), -1, &aspect_rect, DT_SINGLELINE);

    CRect z_near_rect(70, 212, 240, 235);
    dc.DrawText(CString(ZNEAR_EDIT_TITLE), -1, &z_near_rect, DT_SINGLELINE);

    CRect z_far_rect(70, 282, 240, 305);
    dc.DrawText(CString(ZFAR_EDIT_TITLE), -1, &z_far_rect, DT_SINGLELINE);

    mFovyEdit.SetFocus();
}

// -------------------------
//    Class CMaterialDialog
// -------------------------

CMaterialDialog::CMaterialDialog(CString title) : CInputDialog(title), mKaR(0.5), mKaG(0.5), mKaB(0.5), 
                         mKdR(0.5), mKdG(0.5), mKdB(0.5), mKsR(0.5), mKsG(0.5), mKsB(0.5), mAlpha(0.5) {}

CMaterialDialog::~CMaterialDialog() {}

Color CMaterialDialog::getKa() { return Color{ mKaR, mKaG, mKaB }; }
Color CMaterialDialog::getKd() { return Color{ mKdR, mKdG, mKdB }; }
Color CMaterialDialog::getKs() { return Color{ mKsR, mKsG, mKsB }; }
float CMaterialDialog::getAlpha() { return mAlpha; }

void CMaterialDialog::DoDataExchange(CDataExchange* pDX) {
    CInputDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_KAR_EDIT, mKaR);
    DDX_Text(pDX, IDC_KAG_EDIT, mKaG);
    DDX_Text(pDX, IDC_KAB_EDIT, mKaB);
    DDX_Text(pDX, IDC_KDR_EDIT, mKdR);
    DDX_Text(pDX, IDC_KDG_EDIT, mKdG);
    DDX_Text(pDX, IDC_KDB_EDIT, mKdB);
    DDX_Text(pDX, IDC_KSR_EDIT, mKsR);
    DDX_Text(pDX, IDC_KSG_EDIT, mKsG);
    DDX_Text(pDX, IDC_KSB_EDIT, mKsB);
    DDX_Text(pDX, IDC_ALPHA_EDIT, mAlpha);
}

// CBoxDialog message handlers
BEGIN_MESSAGE_MAP(CMaterialDialog, CInputDialog)
    ON_WM_CREATE()
    ON_WM_PAINT()
END_MESSAGE_MAP()

int CMaterialDialog::OnCreate(LPCREATESTRUCT lpcs) {
    // Ka
    mKaREdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(150, 70, 190, 90), this, IDC_KAR_EDIT);

    mKaGEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(330, 70, 370, 90), this, IDC_KAG_EDIT);

    mKaBEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(510, 70, 550, 90), this, IDC_KAB_EDIT);

    // Kd
    mKdREdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(150, 152, 190, 172), this, IDC_KDR_EDIT);

    mKdGEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(330, 152, 370, 172), this, IDC_KDG_EDIT);

    mKdBEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(510, 152, 550, 172), this, IDC_KDB_EDIT);

    // Ks
    mKsREdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(150, 234, 190, 254), this, IDC_KSR_EDIT);

    mKsGEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(330, 234, 370, 254), this, IDC_KSG_EDIT);

    mKsBEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(510, 234, 550, 254), this, IDC_KSB_EDIT);

    // Alpha
    mAlphaEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(330, 348, 370, 370), this, IDC_ALPHA_EDIT);
    

    return 0;
}

void CMaterialDialog::OnPaint() {
    CPaintDC dc(this);
    dc.SetBkMode(TRANSPARENT);

    // ka
    CRect ambient_rect(30, 30, 300, 55);
    dc.DrawText(CString(AMBIENT_REFLECTION_EDIT_TITLE), -1, &ambient_rect, DT_SINGLELINE);

    CRect kaR_rect(70, 72, 190, 95);
    dc.DrawText(CString(KAR_EDIT_TITLE), -1, &kaR_rect, DT_SINGLELINE);

    CRect kaG_rect(270, 72, 390, 95);
    dc.DrawText(CString(KAG_EDIT_TITLE), -1, &kaG_rect, DT_SINGLELINE);

    CRect kaB_rect(470, 72, 590, 95);
    dc.DrawText(CString(KAB_EDIT_TITLE), -1, &kaB_rect, DT_SINGLELINE);

    // kd
    CRect diffuse_rect(30, 112, 300, 137);
    dc.DrawText(CString(DIFFUSE_REFLECTION_EDIT_TITLE), -1, &diffuse_rect, DT_SINGLELINE);

    CRect kdR_rect(70, 154, 190, 177);
    dc.DrawText(CString(KDR_EDIT_TITLE), -1, &kdR_rect, DT_SINGLELINE);

    CRect kdG_rect(270, 154, 390, 177);
    dc.DrawText(CString(KDG_EDIT_TITLE), -1, &kdG_rect, DT_SINGLELINE);

    CRect kdB_rect(470, 154, 590, 177);
    dc.DrawText(CString(KDB_EDIT_TITLE), -1, &kdB_rect, DT_SINGLELINE);

    // ks
    CRect specular_rect(30, 194, 300, 219);
    dc.DrawText(CString(SPECULAR_REFLECTION_EDIT_TITLE), -1, &specular_rect, DT_SINGLELINE);

    CRect ksR_rect(70, 236, 190, 259);
    dc.DrawText(CString(KSR_EDIT_TITLE), -1, &ksR_rect, DT_SINGLELINE);

    CRect ksG_rect(270, 236, 390, 259);
    dc.DrawText(CString(KSG_EDIT_TITLE), -1, &ksG_rect, DT_SINGLELINE);

    CRect ksB_rect(470, 236, 590, 259);
    dc.DrawText(CString(KSB_EDIT_TITLE), -1, &ksB_rect, DT_SINGLELINE);

    // alpha
    CRect alpha_rect(200, 350, 335, 375);
    dc.DrawText(CString(ALPHA_EDIT_TITLE), -1, &alpha_rect, DT_SINGLELINE);

    mKaREdit.SetFocus();
}

// -------------------------
//    Class CLightDialog
// -------------------------

CLightDialog::CLightDialog(CString title) : CInputDialog(title), mLaR(0.5), mLaG(0.5), mLaB(0.5),
mLdR(0.5), mLdG(0.5), mLdB(0.5), mLsR(0.5), mLsG(0.5), mLsB(0.5) {}

CLightDialog::~CLightDialog() {}

Color CLightDialog::getLa() { return Color{ mLaR, mLaG, mLaB }; }
Color CLightDialog::getLd() { return Color{ mLdR, mLdG, mLdB }; }
Color CLightDialog::getLs() { return Color{ mLsR, mLsG, mLsB }; }

void CLightDialog::DoDataExchange(CDataExchange* pDX) {
    CInputDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_LAR_EDIT, mLaR);
    DDX_Text(pDX, IDC_LAG_EDIT, mLaG);
    DDX_Text(pDX, IDC_LAB_EDIT, mLaB);
    DDX_Text(pDX, IDC_LDR_EDIT, mLdR);
    DDX_Text(pDX, IDC_LDG_EDIT, mLdG);
    DDX_Text(pDX, IDC_LDB_EDIT, mLdB);
    DDX_Text(pDX, IDC_LSR_EDIT, mLsR);
    DDX_Text(pDX, IDC_LSG_EDIT, mLsG);
    DDX_Text(pDX, IDC_LSB_EDIT, mLsB);
}

// CBoxDialog message handlers
BEGIN_MESSAGE_MAP(CLightDialog, CInputDialog)
    ON_WM_CREATE()
    ON_WM_PAINT()
END_MESSAGE_MAP()

int CLightDialog::OnCreate(LPCREATESTRUCT lpcs) {
    // La
    mLaREdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(150, 70, 190, 90), this, IDC_LAR_EDIT);

    mLaGEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(330, 70, 370, 90), this, IDC_LAG_EDIT);

    mLaBEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(510, 70, 550, 90), this, IDC_LAB_EDIT);

    // Ld
    mLdREdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(150, 152, 190, 172), this, IDC_LDR_EDIT);

    mLdGEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(330, 152, 370, 172), this, IDC_LDG_EDIT);

    mLdBEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(510, 152, 550, 172), this, IDC_LDB_EDIT);

    // Ls
    mLsREdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(150, 234, 190, 254), this, IDC_LSR_EDIT);

    mLsGEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(330, 234, 370, 254), this, IDC_LSG_EDIT);

    mLsBEdit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        CRect(510, 234, 550, 254), this, IDC_LSB_EDIT);

    return 0;
}

void CLightDialog::OnPaint() {
    CPaintDC dc(this);
    dc.SetBkMode(TRANSPARENT);

    // La
    CRect ambient_rect(30, 30, 300, 55);
    dc.DrawText(CString(AMBIENT_INTENSITY_EDIT_TITLE), -1, &ambient_rect, DT_SINGLELINE);

    CRect laR_rect(70, 72, 190, 95);
    dc.DrawText(CString(LAR_EDIT_TITLE), -1, &laR_rect, DT_SINGLELINE);

    CRect laG_rect(270, 72, 390, 95);
    dc.DrawText(CString(LAG_EDIT_TITLE), -1, &laG_rect, DT_SINGLELINE);

    CRect laB_rect(470, 72, 590, 95);
    dc.DrawText(CString(LAB_EDIT_TITLE), -1, &laB_rect, DT_SINGLELINE);

    // Ld
    CRect diffuse_rect(30, 112, 300, 137);
    dc.DrawText(CString(DIFFUSE_INTENSITY_EDIT_TITLE), -1, &diffuse_rect, DT_SINGLELINE);

    CRect ldR_rect(70, 154, 190, 177);
    dc.DrawText(CString(LDR_EDIT_TITLE), -1, &ldR_rect, DT_SINGLELINE);

    CRect ldG_rect(270, 154, 390, 177);
    dc.DrawText(CString(LDG_EDIT_TITLE), -1, &ldG_rect, DT_SINGLELINE);

    CRect ldB_rect(470, 154, 590, 177);
    dc.DrawText(CString(LDB_EDIT_TITLE), -1, &ldB_rect, DT_SINGLELINE);

    // Ls
    CRect specular_rect(30, 194, 300, 219);
    dc.DrawText(CString(SPECULAR_INTENSITY_EDIT_TITLE), -1, &specular_rect, DT_SINGLELINE);

    CRect lsR_rect(70, 236, 190, 259);
    dc.DrawText(CString(LSR_EDIT_TITLE), -1, &lsR_rect, DT_SINGLELINE);

    CRect lsG_rect(270, 236, 390, 259);
    dc.DrawText(CString(LSG_EDIT_TITLE), -1, &lsG_rect, DT_SINGLELINE);

    CRect lsB_rect(470, 236, 590, 259);
    dc.DrawText(CString(LSB_EDIT_TITLE), -1, &lsB_rect, DT_SINGLELINE);

    mLaREdit.SetFocus();
}

