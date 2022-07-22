#include "stdafx.h"
#include "dialog.h"
#include "AcExtensionModule.h"
#include <rxregsvc.h>
#include "jigHeader.h"
#include "afxdialogex.h"

// Диалоговое окно button
IMPLEMENT_DYNAMIC(MyDlg, CDialog)

MyDlg::MyDlg(CWnd* pParent =nullptr)
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


AC_IMPLEMENT_EXTENSION_MODULE(theArxDLL);

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


// Ввод длины заклепки
void MyDlg::OnEnChangeEdit2()
{
}
// Ввод диаметра заклепки
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



// =============================Функция добавления блока к базе==============================
void add_block()
{
    Acad::ErrorStatus es;
    // Запрос центра для нового блока
    AcGePoint3d center;
    acedGetPoint(NULL, _T("\nENTER THE ENTRY POINT OF NEW BLOCK: "),
        asDblArray(center));


    AcDbObjectPointerBase< AcDbBlockReference> pBlkRef;
    pBlkRef.create();

    //Acad::ErrorStatus es;
    pBlkRef->setPosition(center);

    AcDbBlockTable* pBlockTable;
    acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);

    AcDbBlockTableRecord* mspacepBlockTableRecord;
    es = pBlockTable->getAt(ACDB_MODEL_SPACE, mspacepBlockTableRecord, AcDb::kForWrite);
    es = pBlockTable->close();


    AcDbObjectId new_blockId;
    AcDbBlockTableRecord* pBlockTableRecord;

    // Если найден FIRST BLOCK
    if (pBlockTable->getAt(_T("FIRST BLOCK"), pBlockTableRecord,
        AcDb::kForWrite) != eKeyNotFound)
    {
        new_blockId = pBlockTableRecord->id();
        pBlockTableRecord->close();

    }
    else
    {
        

    }
    if (!new_blockId.isNull())
    {
        pBlkRef->setBlockTableRecord(new_blockId);
        // Append the block reference to the model space
        // block Table Record.
        //
        //AcDbObjectId new_blockId;
      //  es=pBlockTableRecord->upgradeOpen();

        es = mspacepBlockTableRecord->appendAcDbEntity(new_blockId, pBlkRef);
    }


    mspacepBlockTableRecord->close();

    return;
}
// =============================Функция создания произвольной джиг_заклепки==============================
void add_random_rivet() 
{
    CustomJig* jig = new CustomJig();

    jig->startJig();
    delete jig;
}
// =============================Функция добавления примитива к базе =============================
AcDbObjectId add_primitive_to_base(AcDbEntity* entity)
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
void make_window()
{
    MyDlg btn;
    CAcModuleResourceOverride resOverride;
    INT_PTR nRes = btn.DoModal();


    // Если нажали Ок и модальное окно закрылось
    if (nRes == 1)
    {
        if (btn.currentСommand == 1)
            add_random_rivet();

        if (btn.currentСommand == 2)
            add_block();

        if (btn.currentСommand == 3)
        {
            AcGePoint3d center;
            acedGetPoint(NULL, _T("\nENTER THE CENTER POINT: "),
                asDblArray(center));
            customObject* obj = new customObject(center);
            /*if (btn.rivet_direction == L"horizontal")
                obj->setDirection({ 1,0,0 });
            if (btn.rivet_direction == L"vertical")
                obj->setDirection({ 0,1,0 });*/

            /*obj->set_length_of_rivet(btn.m_rivet_length);
            obj->set_diameter_of_rivet(btn.m_rivet_diam);*/
            AcDbObjectId objID = add_primitive_to_base(obj);
        }
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////