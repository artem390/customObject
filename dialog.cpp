#pragma once
#include "stdafx.h"
#include "dialog.h"
#include "AcExtensionModule.h"
//#include <rxregsvc.h>
#include "jigHeader.h"
//#include "afxdialogex.h"
#include "extension.h"
//#include <afxext.h>
//#include <AtlBase.h>
//#include <AtlCom.h>
using namespace ATL;

#include "dbxHeaders.h"
// Диалоговое окно button
IMPLEMENT_DYNAMIC(MyDlg, CDialog)

MyDlg::MyDlg(CWnd* pParent)
	: CDialog(IDD_DIALOG1, pParent), currentСommand(3)
{
}

MyDlg::~MyDlg()
{
}

void MyDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    // Получение информации о направлении
    //DDX_CBString(pDX, IDC_COMBO1, rivet_direction);
    //// Получение длины и диаметра
    //DDX_Text(pDX, IDC_EDIT2, m_rivet_length);
    //DDX_Text(pDX, IDC_EDIT3, m_rivet_diam);

    //// Ограничение по максимуму/минимуму
    //DDV_MinMaxDouble(pDX, m_rivet_length, 30, 1000);
    //DDV_MinMaxDouble(pDX, m_rivet_diam, 10, 60);

}


BEGIN_MESSAGE_MAP(MyDlg, CDialog)
   /* ON_BN_CLICKED(IDC_RADIO1, &MyDlg::OnBnClickedRadio1)
    ON_BN_CLICKED(IDC_RADIO2, &MyDlg::OnBnClickedRadio2)
    ON_BN_CLICKED(IDC_RADIO3, &MyDlg::OnBnClickedRadio3)
 
    ON_EN_CHANGE(IDC_EDIT2, &MyDlg::OnEnChangeEdit2)
    ON_EN_CHANGE(IDC_EDIT3, &MyDlg::OnEnChangeEdit3)

    ON_CBN_SELCHANGE(IDC_COMBO1, &MyDlg::OnCbnSelchangeCombo1)*/
END_MESSAGE_MAP()


NC_IMPLEMENT_EXTENSION_MODULE(theArxDLL);

HINSTANCE _hdllInstance = NULL;
extern "C" int APIENTRY
//extern "C"  int _afxForceUSRDLL;
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



void MyDlg::OnEnChangeEdit2()
{
}

void MyDlg::OnEnChangeEdit3()
{
}



// создать произвольная заклепка
void MyDlg::OnBnClickedRadio1()
{
    make_edit_disabled(0);
    currentСommand = 1;
}

void MyDlg::OnBnClickedRadio2()
{
    make_edit_disabled(0);
    currentСommand = 2;
}

void MyDlg::OnBnClickedRadio3()
{
    make_edit_disabled(1);
    currentСommand = 3;
}

// Сделать окна доступными (1) / недоступными (0)  для записи
void MyDlg::make_edit_disabled(const bool& mode)
{
    CWnd* pWnd;
    //pWnd = GetDlgItem(IDC_EDIT2);
    pWnd->EnableWindow(mode);
   // pWnd = GetDlgItem(IDC_EDIT3);
    pWnd->EnableWindow(mode);
   // pWnd = GetDlgItem(IDC_COMBO1);
    pWnd->EnableWindow(mode);
  
}

void MyDlg::OnCbnSelchangeCombo1()
{}


BOOL MyDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Устанавливаем по умолчанию радио баттн 3
        //CheckDlgButton(IDC_RADIO3, 1);    
        
    return TRUE;  
}
void addJigObj()
{
    CustomJig* jig = new CustomJig();

    jig->startJig();
    delete jig;
}
// =============================Функция добавления примитива к базе =============================
AcDbObjectId addToBase(AcDbEntity* entity)
{
    Acad::ErrorStatus error;
    AcDbBlockTable* pBlockTable;
    error = acdbHostApplicationServices()->workingDatabase()
        ->getSymbolTable(pBlockTable, AcDb::kForRead);
    if (error != Acad::eOk)           // Если не получилось открыть, то возврат нулевого ID
        return AcDbObjectId::kNull;

    AcDbBlockTableRecord* pBlockTableRecord;
    error = pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,
        AcDb::kForWrite);
    if (error != Acad::eOk)         // Если не получилось открыть для записи, то закрываем pBlockTable 
    {                               // и возвращаем нулевой ID
        pBlockTable->close();
        return AcDbObjectId::kNull;
    }

    pBlockTable->close();

    AcDbObjectId objectID;
    error = pBlockTableRecord->appendAcDbEntity(objectID, entity);
    if (error != Acad::eOk)      // Если не получилось добавить примитив в базу, то закрываем  pBlockTableRecord
    {                            // и возвращаем нулевой ID
        pBlockTableRecord->close();
        return AcDbObjectId::kNull;
    }

    pBlockTableRecord->close();
    entity->close();
    return objectID;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Команда
void openWindow()
{
    MyDlg btn;
    CAcModuleResourceOverride resOverride;
    INT_PTR nRes = btn.DoModal();


    // Если нажали Ок и модальное окно закрылось
    if (nRes == 1)
    {
        if (btn.currentСommand == 1)
            addJigObj();

        if (btn.currentСommand == 2)


        if (btn.currentСommand == 3)
        {
            AcGePoint3d center;
            acedGetPoint(NULL, _T("\nENTER THE CENTER POINT: "),
                asDblArray(center));
            customObject* obj = new customObject(center);
            //установить значения
            AcDbObjectId objID = addToBase(obj);
        }
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////