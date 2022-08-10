#pragma once
#include "Resource.h"
enum class commands {
	jigObj, objWithParams, block
};

class MyDlg : public CDialog
{
	DECLARE_DYNAMIC(MyDlg)
public:
	MyDlg(CWnd* pParent = nullptr);   
	virtual ~MyDlg();
	
	BOOL OnInitDialog() override;
	
	bool checkObj();
	static MyDlg* getInstance();
	AcDbObjectId createBlock(const CString& blockName) const;
	void addBlock() const;

	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedOk();
	// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;    // поддержка DDX/DDV
	DECLARE_MESSAGE_MAP()
	void setWindowState(const bool mode);
	void setValue(double value,int index);
	bool getValue(int index, double& value) const;
private:
	static unique_ptr<MyDlg> dlg;
	commands currentСommand;
	double R = 100;
	double r = 80;
	double r1 = 20;
	double h = 10;
	
};

// Команда для запуска приложения
void  openWindow();