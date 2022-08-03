#pragma once
#include "stdafx.h"
#include "dialog.h"
#include "AcExtensionModule.h"
#include "jig.h"
#include "extension.h"
using namespace ATL;
unique_ptr<MyDlg> dlg;
#include "dbxHeaders.h"
IMPLEMENT_DYNAMIC(MyDlg, CDialog)

MyDlg::MyDlg(CWnd* pParent)
	: CDialog(IDD_DIALOG1, pParent), currentСommand(commands::objWithParams)
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
    ON_BN_CLICKED(IDOK, &MyDlg::OnBnClickedOk)
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
void MyDlg::setWindowState(const bool mode)
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
    setValue(R, IDC_EDIT1);
    setValue(r, IDC_EDIT2);
    setValue(r1, IDC_EDIT3);
    setValue(h, IDC_EDIT4);
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

AcDbObjectId createBlock(const CString& blockName)
{
    Acad::ErrorStatus es;

    AcDbBlockTableRecord* pBlockRecord = new AcDbBlockTableRecord;


    pBlockRecord->setName(blockName);

    AcDbObjectPointer<customObject> obj1;
    obj1.create();
    obj1->setCenter(AcGePoint3d(0, obj1->getR()- obj1->getr(),0));
    AcDbObjectPointer<customObject> obj2;
    obj2.create();
    obj2->setCenter(AcGePoint3d(0, -obj2->getR() + obj2->getr(), 0));
    obj2->setDirection(-AcGeVector3d::kXAxis);

    AcDbObjectId objId1;
    AcDbObjectId objId2;
    es = pBlockRecord->appendAcDbEntity(objId1, obj1);
    es = pBlockRecord->appendAcDbEntity(objId2, obj2);

    AcDbBlockTable* pBlockTable = new AcDbBlockTable;
    es = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForWrite);
    if (es != Acad::eOk)
    {
        return NULL;
    }

    AcDbObjectId blockId;
    es = pBlockTable->add(blockId, pBlockRecord);
    if (es != Acad::eOk)
    {
        return NULL;
    }

    pBlockTable->close();
    pBlockRecord->close();

    return blockId;
}

void addBlock()
{
    Acad::ErrorStatus es;
    AcGePoint3d center;
    acedGetPoint(NULL, _T("\nENTER THE ENTRY POINT OF NEW BLOCK: "),asDblArray(center));

    AcDbObjectPointerBase< AcDbBlockReference> pBlkRef;
    pBlkRef.create();
    pBlkRef->setPosition(center);

    AcDbBlockTable* pBlockTable;
    acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);

    AcDbBlockTableRecord* mspacepBlockTableRecord;
    es = pBlockTable->getAt(ACDB_MODEL_SPACE, mspacepBlockTableRecord, AcDb::kForWrite);
    es = pBlockTable->close();

    static int blockNumb = 1;
    
    AcDbObjectId blockId;
    AcDbBlockTableRecord* pBlockTableRecord;

    CString sVal;
    sVal.Format(_T("%d"), blockNumb);
    sVal += " Block";
    blockId = createBlock(sVal);
    
    blockNumb++;

    if (!blockId.isNull())
    {
        pBlkRef->setBlockTableRecord(blockId);
        es = mspacepBlockTableRecord->appendAcDbEntity(blockId, pBlkRef);
    }

    mspacepBlockTableRecord->close();
}

void openWindow()
{
    if (!dlg.get() || dlg->GetSafeHwnd() == NULL)
    {
        CAcModuleResourceOverride resOverride;
        dlg.reset(new MyDlg);

        if (dlg.get())
        {
            BOOL bRes = dlg->Create(IDD_DIALOG1);
            if (bRes)
            {
                dlg->ShowWindow(SW_SHOW);
            }

        }
    }
    else
    {
        dlg->ShowWindow(SW_SHOW);
    }
};

void MyDlg::OnBnClickedRadio1()
{
    setWindowState(false);
    currentСommand = commands::jigObj;
}


void MyDlg::OnBnClickedRadio2()
{
    setWindowState(true);
    currentСommand = commands::objWithParams;
}


void MyDlg::OnBnClickedRadio3()
{
    setWindowState(false);
    currentСommand = commands::block;
}

void MyDlg::setValue(double value, int index)
{
    CWnd* pWnd = GetDlgItem(index);
    if (!pWnd)
    {
        return;
    }
    CString sVal;
    sVal.Format(_T("%.2f"), value);
    pWnd->SetWindowText(sVal);
}

bool MyDlg::getValue(int index, double &value)
{
    CWnd* pWnd = GetDlgItem(index);
    if (!pWnd)
    {
        return false;
    }
    CString sVal;
    pWnd->GetWindowText(sVal);
    value = _wtof(sVal);
    return true;
}


void MyDlg::OnBnClickedOk()
{
    if (currentСommand == commands::jigObj)
    {
        CustomJig* jig = new CustomJig();
        jig->startJig();
        delete jig;
    }
    else
    if (currentСommand == commands::objWithParams)
    {
        if (checkEdit1())
        {
            if (checkEdit2())
            {
                if (checkEdit3())
                {
                    if (checkEdit4())
                    {
                        AcGePoint3d center;
                        acedGetPoint(NULL, _T("\nENTER THE CENTER POINT: "), asDblArray(center));
                        customObject* obj = new customObject(center);
                        obj->setR(R);
                        obj->setr(r);
                        obj->setr1(r1);
                        obj->setH(h);
                        addToBase(obj);
                    }
                }
            }
        }
    }
    else
    if (currentСommand == commands::block)
    {
        addBlock();
    }
}
bool MyDlg::checkEdit1()
{
    double val;
    getValue(IDC_EDIT1,val);
    if (getValue(IDC_EDIT1, val))
    {
        if (val < (r + 20))
        {
            acutPrintf(L"Incorrect R");
            return false;
        }
        else
        {
            R = val;
            return true;
        }
    }
    else
    {
        return false;
    }   
    return true;
}
bool MyDlg::checkEdit2()
{
    double val; 
    if (getValue(IDC_EDIT2, val))
    {
        if ((val > (R - 20)) || (val < (r1 + 50)))
        {
            acutPrintf(L"Incorrect r");
            return false;
        }
        else
        {
            r = val;
            return true;
        }
    }
    else
    {
        return false;
    }   
    return true;
}
bool MyDlg::checkEdit3()
{
    double val; 
    if (getValue(IDC_EDIT3, val))
    {
        if ((val > (r - 50)) || (val < (h / (2 * sin(PI / 8)))))
        {
            acutPrintf(L"Incorrect r1");
            return false;
        }
        else
        {
            r1 = val;
            return true;
        }
    }
    else
    {
        return false;
    }
    return true;
}

bool MyDlg::checkEdit4()
{
    double val; 
    if (getValue(IDC_EDIT4, val))
    {
        if ((val > (2 * r1 * sin(PI / 8))) || (val < 5))
        {
            acutPrintf(L"Incorrect h");
            return false;
        }
        else
        {
            h = val;
            return true;
        }
    }
    else
    {
        return false;
    }
    return true;
}
