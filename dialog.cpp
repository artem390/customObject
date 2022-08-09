#pragma once
#include "stdafx.h"
#include "dialog.h"
#include "AcExtensionModule.h"
#include "jig.h"
#include "extension.h"
using namespace ATL;

#include "dbxHeaders.h"
IMPLEMENT_DYNAMIC(MyDlg, CDialog)
unique_ptr<MyDlg> MyDlg::dlg = NULL;
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

    UNREFERENCED_PARAMETER(lpReserved);

    if (dwReason == DLL_PROCESS_ATTACH) {
        theArxDLL.AttachInstance(hInstance);
        _hdllInstance = hInstance;
    }
    else if (dwReason == DLL_PROCESS_DETACH) {
        theArxDLL.DetachInstance();
    }
    return 1;   
}

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

AcDbObjectId addToBase(AcDbEntity* entity)
{
    Acad::ErrorStatus es;
    AcDbObjectId objectID;
    AcDbBlockTable* pBlockTable;
    es = acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
    if (es != Acad::eOk)
    {
        return NULL;
    }
    AcDbBlockTableRecord* pBlockTableRecord;
    es = pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
    if (es != Acad::eOk)
    {
        return NULL;
    }
    pBlockTable->close();

    es = pBlockTableRecord->appendAcDbEntity(objectID, entity);
    if (es != Acad::eOk)
    {
        return NULL;
    }
    pBlockTableRecord->close();
    entity->close();
    return objectID;
}

AcDbObjectId MyDlg::createBlock(const CString& blockName)
{
    Acad::ErrorStatus es;

    AcDbBlockTableRecordPointer pBlockRecord;
    pBlockRecord.create();

    pBlockRecord->setName(blockName);

    AcDbObjectPointer<customObject> obj1;
    obj1.create();
    obj1->setCenter(AcGePoint3d(0, obj1->getR()- obj1->getr(),0));
    obj1->setR(R);
    obj1->setr(r);
    obj1->setr1(r1);
    obj1->setH(h);
    AcDbObjectPointer<customObject> obj2;
    obj2.create();
    obj2->setCenter(AcGePoint3d(0, -obj2->getR() + obj2->getr(), 0));
    obj2->setDirection(-AcGeVector3d::kXAxis);
    obj2->setR(R);
    obj2->setr(r);
    obj2->setr1(r1);
    obj2->setH(h);

    AcDbObjectId objId1;
    AcDbObjectId objId2;
    es = pBlockRecord->appendAcDbEntity(objId1, obj1);
    if (es != Acad::eOk)
    {
        return NULL;
    }
    es = pBlockRecord->appendAcDbEntity(objId2, obj2);
    if (es != Acad::eOk)
    {
        return NULL;
    }
    AcDbBlockTable* pBlockTable;
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
    return blockId;
}

void MyDlg::addBlock()
{
    static int blockNumb = 1;
    AcDbObjectId blockId;
    CString sVal;
    sVal.Format(_T("%d"), blockNumb);
    sVal += " Block";
    blockId = createBlock(sVal);
    
    if (!blockId.isNull())
    {
        AcGePoint3d center;
        if (acedGetPoint(NULL, _T("\nENTER THE ENTRY POINT OF NEW BLOCK: "), asDblArray(center)))
        {
            blockNumb++;
            AcDbObjectPointerBase< AcDbBlockReference> pBlkRef;
            pBlkRef.create();
            pBlkRef->setBlockTableRecord(blockId);
            AcDbBlockTable* pBlockTable;
            acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);
            AcDbBlockTableRecord* mspacepBlockTableRecord;
            Acad::ErrorStatus es = pBlockTable->getAt(ACDB_MODEL_SPACE, mspacepBlockTableRecord, AcDb::kForWrite);
            if (es == Acad::eOk)
            {
                pBlockTable->close();
                pBlkRef->setPosition(center);
                mspacepBlockTableRecord->appendAcDbEntity(blockId, pBlkRef);
                mspacepBlockTableRecord->close();
            }       
        }   
    }    
}

void openWindow()
{ 
    MyDlg *d = MyDlg::getInstance();
    if (d->GetSafeHwnd() == NULL)
    {
        CAcModuleResourceOverride resOverride;
        if (d)
        {
            BOOL bRes = d->Create(IDD_DIALOG1, d->GetParent());
            if (bRes)
            {
                d->ShowWindow(SW_SHOW);
            }
        }
    }
    else
    {
        d->ShowWindow(SW_SHOW);
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
    setWindowState(true);
    currentСommand = commands::block;
}

void MyDlg::setValue(double value, int index)
{
    CWnd* pWnd = GetDlgItem(index);
    if (pWnd)
    {
        CString sVal;
        sVal.Format(_T("%.2f"), value);
        pWnd->SetWindowText(sVal);
    }  
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
        if (checkObj())
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
    else
    if (currentСommand == commands::block)
    {
        if (checkObj())
        {
            addBlock();
        }        
    }
}

bool MyDlg::checkObj()
{
    double enteredR, enteredr, enteredr1, enteredH;
    if (!getValue(IDC_EDIT1, enteredR))
    {
        return false;
    }
    if (!getValue(IDC_EDIT2, enteredr))
    {
        return false;
    }
    if (!getValue(IDC_EDIT3, enteredr1))
    {
        return false;
    }
    if (!getValue(IDC_EDIT4, enteredH))
    {
        return false;
    }

    if (enteredR < (enteredr + 20))
    {
        acutPrintf(L"R-r must be less than 20");
        return false;
    }

    if (enteredr < (enteredr1 + 50))
    {
        acutPrintf(L"r-r1 must be less than 50");
        return false;
    }

    if (enteredr1 < (enteredH / (2 * sin(PI / 8))))
    {
        acutPrintf(L"r1 must be bigger");
        return false;
    }

    if (enteredH < 5)
    {
        acutPrintf(L"h must be more than 5");
        return false;
    }

    R = enteredR;
    r = enteredr;
    r1 = enteredr1;
    h = enteredH;
    return true;
}

MyDlg* MyDlg::getInstance()
{
    if (!dlg)
    {
        dlg.reset(new MyDlg);
    }
    return dlg.get();
}