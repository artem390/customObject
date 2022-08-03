#pragma once
#include "Resource.h"


class MyDlg : public CDialog
{
	DECLARE_DYNAMIC(MyDlg)

public:
	MyDlg(CWnd* pParent = nullptr);   
	virtual ~MyDlg();
	
	BOOL OnInitDialog() override;
	
	// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;    // поддержка DDX/DDV
	DECLARE_MESSAGE_MAP()
	void setWindowState(const bool& mode);
	void setValue(double value,int index);
	double getValue(int index);
public:

	int currentСommand;
	double R = 100;
	double r = 80;
	double r1 = 20;
	double h = 10;

	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
};

// Команда для запуска приложения
void  openWindow();