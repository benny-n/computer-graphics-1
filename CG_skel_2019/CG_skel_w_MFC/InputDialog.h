
#pragma once

#include <string>
using std::string;

#include "vec.h"
#include "Color.h"
#include "Resource.h"

// ------------------------
//    Class CInputDialog
// ------------------------

class CInputDialog : public CDialog
{
    DECLARE_DYNAMIC(CInputDialog)

public:
    CInputDialog(CString title = "Input Dialog");
    virtual ~CInputDialog();

    virtual BOOL OnInitDialog();

    enum { IDD = IDD_INPUTDIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

private:
    CString mTitle;
};

// ----------------------
//    Class CCmdDialog
// ----------------------

class CCmdDialog : public CInputDialog
{
public:
    CCmdDialog(CString title = "Input Dialog");
    virtual ~CCmdDialog();

    string GetCmd();

protected:
    CString mCmd;
    CEdit mCmdEdit;

    virtual void DoDataExchange(CDataExchange* pDX);

    afx_msg int OnCreate (LPCREATESTRUCT lpcs);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};

// ----------------------
//    Class CXyzDialog
// ----------------------

class CXyzDialog : public CInputDialog
{
public:
    CXyzDialog(CString title = "Input Dialog");
    virtual ~CXyzDialog();

    vec3 getXYZ();

protected:
    float mX;
    float mY;
    float mZ;
    CEdit mXEdit;
    CEdit mYEdit;
    CEdit mZEdit;

    virtual void DoDataExchange(CDataExchange* pDX);

    afx_msg int OnCreate (LPCREATESTRUCT lpcs);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};

// -------------------------
//    Class CCmdXyzDialog
// -------------------------

class CCmdXyzDialog : public CInputDialog
{
public:
    CCmdXyzDialog(CString title = "Input Dialog");
    virtual ~CCmdXyzDialog();

    string GetCmd();
    vec3 getXYZ();

protected:
    CString mCmd;
    float mX;
    float mY;
    float mZ;
    CEdit mCmdEdit;
    CEdit mXEdit;
    CEdit mYEdit;
    CEdit mZEdit;

    virtual void DoDataExchange(CDataExchange* pDX);

    afx_msg int OnCreate (LPCREATESTRUCT lpcs);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};

// -------------------------
//    Class CRGBDialog
// -------------------------

class CRGBDialog : public CInputDialog
{
public:
    CRGBDialog(CString title = "Input Dialog");
    virtual ~CRGBDialog();

    Color getRGB();

protected:
    float mR;
    float mG;
    float mB;
    CEdit mREdit;
    CEdit mGEdit;
    CEdit mBEdit;

    virtual void DoDataExchange(CDataExchange* pDX);

    afx_msg int OnCreate(LPCREATESTRUCT lpcs);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};

// -------------------------
//    Class CReflectivityDialog
// -------------------------

class CReflectivityDialog : public CInputDialog
{
public:
    CReflectivityDialog(CString title = "Input Dialog");
    virtual ~CReflectivityDialog();

    float getReflectivity();

protected:
    float mReflectivity;
    CEdit mReflectivityEdit;

    virtual void DoDataExchange(CDataExchange* pDX);

    afx_msg int OnCreate(LPCREATESTRUCT lpcs);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};

// -------------------------
//    Class CBoxDialog
// -------------------------

class CBoxDialog : public CInputDialog
{
public:
    CBoxDialog(CString title = "Input Dialog");
    virtual ~CBoxDialog();

    float getLeft();
    float getRight();
    float getBottom();
    float getTop();
    float getZNear();
    float getZFar();

protected:
    float mLeft;
    float mRight;
    float mBottom;
    float mTop;
    float mZNear;
    float mZFar;
    CEdit mLeftEdit;
    CEdit mRightEdit;
    CEdit mBottomEdit;
    CEdit mTopEdit;
    CEdit mZNearEdit;
    CEdit mZFarEdit;

    virtual void DoDataExchange(CDataExchange* pDX);

    afx_msg int OnCreate(LPCREATESTRUCT lpcs);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};

// -------------------------
//    Class CFovyDialog
// -------------------------

class CFovyDialog : public CInputDialog
{
public:
    CFovyDialog(CString title = "Input Dialog");
    virtual ~CFovyDialog();

    float getFovy();
    float getAspectRatio();
    float getZNear();
    float getZFar();

protected:
    float mFovy;
    float mAspectRatio;
    float mZNear;
    float mZFar;
    CEdit mFovyEdit;
    CEdit mAspectRatioEdit;
    CEdit mZNearEdit;
    CEdit mZFarEdit;

    virtual void DoDataExchange(CDataExchange* pDX);

    afx_msg int OnCreate(LPCREATESTRUCT lpcs);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};


// -------------------------
//    Class CMaterialDialog
// -------------------------

class CMaterialDialog : public CInputDialog
{
public:
    CMaterialDialog(CString title = "Input Dialog");
    virtual ~CMaterialDialog();

    Color getKa();
    Color getKd();
    Color getKs();
    Color getEmission();
    float getAlpha();


protected:
    float mKaR;
    float mKaG;
    float mKaB;
    float mKdR;
    float mKdG;
    float mKdB;
    float mKsR;
    float mKsG;
    float mKsB;
    float mEmissionR;
    float mEmissionG;
    float mEmissionB;
    float mAlpha;

    CEdit mKaREdit;
    CEdit mKaGEdit;
    CEdit mKaBEdit;
    CEdit mKdREdit;
    CEdit mKdGEdit;
    CEdit mKdBEdit;
    CEdit mKsREdit;
    CEdit mKsGEdit;
    CEdit mKsBEdit;
    CEdit mEmissionREdit;
    CEdit mEmissionGEdit;
    CEdit mEmissionBEdit;
    CEdit mAlphaEdit;

    virtual void DoDataExchange(CDataExchange* pDX);

    afx_msg int OnCreate(LPCREATESTRUCT lpcs);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};

// -------------------------
//    Class CLightDialog
// -------------------------

class CLightDialog : public CInputDialog
{
public:
    CLightDialog(CString title = "Input Dialog");
    virtual ~CLightDialog();

    Color getLa();
    Color getLd();
    Color getLs();

protected:
    float mLaR;
    float mLaG;
    float mLaB;
    float mLdR;
    float mLdG;
    float mLdB;
    float mLsR;
    float mLsG;
    float mLsB;

    CEdit mLaREdit;
    CEdit mLaGEdit;
    CEdit mLaBEdit;
    CEdit mLdREdit;
    CEdit mLdGEdit;
    CEdit mLdBEdit;
    CEdit mLsREdit;
    CEdit mLsGEdit;
    CEdit mLsBEdit;

    virtual void DoDataExchange(CDataExchange* pDX);

    afx_msg int OnCreate(LPCREATESTRUCT lpcs);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};
