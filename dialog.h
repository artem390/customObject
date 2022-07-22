#pragma once

#include "Resource.h"

#include <afxwin.h>

// Диалоговое окно button

class MyDlg : public CDialog
{
	DECLARE_DYNAMIC(MyDlg)

public:
	MyDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~MyDlg();
	
	BOOL OnInitDialog() override;
	
	// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnCbnSelchangeCombo1();
	// две панели для ввода длины и диаметра заклепки
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();

	// 3 радио для выбора одного из 3 режимов
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();

	// Запоминает текущую команду в radioBtn
	int currentСommand;

	double R = 10000;
	double r = 8000;
	double r1 = 2000;
	double h = 1000;
	// Горизонтальная или вертикальная ориентация
	CString direction;

	// При переключении выбора сделать поля серыми
	void make_edit_disabled(const bool& mode);
};

// Команда для запуска приложения
void  make_window();