#pragma once
#include "stdafx.h"
#include "dialog.h"
#include "AcExtensionModule.h"
#include "jig.h"
#include "extension.h"
using namespace ATL;

#include "dbxHeaders.h"
IMPLEMENT_DYNAMIC(MyDlg, CDialog)

MyDlg::MyDlg(CWnd* pParent)
	: CDialog(IDD_DIALOG1, pParent), currentСommand(2)
{
}

MyDlg::~MyDlg()
{
}

void MyDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX); 
}


BEGIN_MESSAGE_MAP(MyDlg, CDialog)
    ON_BN_CLICKED(IDC_RADIO1, &MyDlg::OnBnClickedRadio1)
    ON_BN_CLICKED(IDC_RADIO2, &MyDlg::OnBnClickedRadio2)
    ON_BN_CLICKED(IDC_RADIO3, &MyDlg::OnBnClickedRadio3)
    ON_EN_CHANGE(IDC_EDIT1, &MyDlg::OnEnChangeEdit1)
    ON_EN_CHANGE(IDC_EDIT2, &MyDlg::OnEnChangeEdit2)
    ON_EN_CHANGE(IDC_EDIT3, &MyDlg::OnEnChangeEdit3)
    ON_EN_CHANGE(IDC_EDIT4, &MyDlg::OnEnChangeEdit4)
END_MESSAGE_MAP()


NC_IMPLEMENT_EXTENSION_MODULE(theArxDLL);

HINSTANCE _hdllInstance = NULL;
extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) {

    // Remove this if you use lpReserved
    UNREFERENCED_PARAMETER(lpReserved);

    if (dwReason == DLL_PROCESS_ATTACH) {
        theArxDLL.AttachInstance(hInstance);
        _hdllInstance = hInstance;
    }
    else if (dwReason == DLL_PROCESS_DETACH) {
        theArxDLL.DetachInstance();
    }
    return 1;   // ok
}


// Сделать окна доступными (1) / недоступными (0)  для записи
void MyDlg::setWindowState(const bool& mode)
{
    auto enable = [&](int nId)->void
    {
        CWnd* pWnd = GetDlgItem(nId);
        if (pWnd)
        {
            pWnd->EnableWindow(mode);
        }
            
    };
    enable(IDC_EDIT1);
    enable(IDC_EDIT2);
    enable(IDC_EDIT3);
    enable(IDC_EDIT4);
}

BOOL MyDlg::OnInitDialog()
{
    CheckDlgButton(IDC_RADIO2, 1);
    CDialog::OnInitDialog();          
    return TRUE;  
}

// =============================Функция добавления примитива к базе =============================
AcDbObjectId addToBase(AcDbEntity* entity)
{
    AcDbObjectId objectID;
    AcDbBlockTable* pBlockTable;
    acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
    AcDbBlockTableRecord* pBlockTableRecord;
    pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);

    pBlockTable->close();

    pBlockTableRecord->appendAcDbEntity(objectID, entity);
    pBlockTableRecord->close();
    entity->close();
    return objectID;
}

void openWindow()
{
    MyDlg dlg;
    CAcModuleResourceOverride resOverride;
    INT_PTR nRes = dlg.DoModal();

    // Если нажали Ок и модальное окно закрылось
    if (nRes == 1)
    {
        if (dlg.currentСommand == 1)
        {
            CustomJig* jig = new CustomJig();
            jig->startJig();
            delete jig;
        }
        if (dlg.currentСommand == 2)
        {
            AcGePoint3d center;
            acedGetPoint(NULL, _T("\nENTER THE CENTER POINT: "),asDblArray(center));
            customObject* obj = new customObject(center); 
            obj->setR(dlg.R);
            obj->setr(dlg.r);
            obj->setr1(dlg.r1);
            obj->setH(dlg.h);
            addToBase(obj);
        }
        if (dlg.currentСommand == 3)
        {
          
        }
    }
};

void MyDlg::OnBnClickedRadio1()
{
    setWindowState(false);
    currentСommand = 1;
}


void MyDlg::OnBnClickedRadio2()
{
    setWindowState(true);
    currentСommand = 2;
}


void MyDlg::OnBnClickedRadio3()
{
    setWindowState(false);
    currentСommand = 3;
}


void MyDlg::OnEnChangeEdit1()
{
    double val = getValue(IDC_EDIT1);
    if (val < (r + 20))
    {
        R = r + 20;
        setValue(R, IDC_EDIT1);
    }
    else
    {
        R=val;
    }    
}


void MyDlg::OnEnChangeEdit2()
{
    double val = getValue(IDC_EDIT2);
    if (val > (R - 20))
    {
        r = R-20;
        setValue(r, IDC_EDIT2);
    }
    else
    if (val < (r1 + 50))
    {
        r = r1 + 50;
        setValue(r, IDC_EDIT2);
    }
    else
    {
        r=val;
    }
}


void MyDlg::OnEnChangeEdit3()
{
    double val = getValue(IDC_EDIT3);
    if (val > (r - 50))
    {
        r1 = r - 50;
        setValue(r1, IDC_EDIT3);
    }
    else
    if (val < (h/(2 * sin(PI / 8))))
    {
        r1 = (h / (2 * sin(PI / 8)));
        setValue(r1, IDC_EDIT3);
    }
    else
    {
        r1 = val;
    }
}


void MyDlg::OnEnChangeEdit4()
{
    double val = getValue(IDC_EDIT4);
    if (val > (2 * r1 * sin(PI / 8)))
    {
        h = (2 * r1 * sin(PI / 8));
        setValue(h, IDC_EDIT4);
    }
    else
    if (val < 5)
    {
        h=5;
        setValue(h, IDC_EDIT4);
    }
    else
    {
        h = val;
    }
}

void MyDlg::setValue(double value, int index)
{
    CString sVal;
    sVal.Format(_T("%.2f"), value);
    CWnd* pWnd = GetDlgItem(index);
    if (!pWnd)
    {
        return;
    }
    pWnd->SetWindowText(sVal);
}

double MyDlg::getValue(int index)
{
    CString sVal;
    CWnd* pWnd = GetDlgItem(index);
    if (!pWnd)
    {
        return index;
    }
    pWnd->GetWindowText(sVal);
    return _wtof(sVal);
}
