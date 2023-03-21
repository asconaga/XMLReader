#pragma once
#include "XMLSchema.h"

// CFlexieDlg dialog
class CFlexieDlg : public CDialogEx
{
// Construction
public:
	CFlexieDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FLEXIE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CXMLSchemaElem* CreateOrigSchema(bool bOrig);
	
	void ThrowException(LPCTSTR lpszFormat, ...);
	void ParseXMLFromSchema(CXMLSchemaElem* pThisElem, CXMLSchemaData* pThisData, TiXmlNode* pElem);
	void ProcessAttr(CString elemName, CXMLSchemaAttr* pAttr, TiXmlElement* pChild, CXMLSchemaData* pNewData);
	   
	CVarData ValidateType(const char* value, char chtype);
	
	afx_msg void OnSave();
	afx_msg void OnLoad();
	afx_msg void OnChangeFileName();
	afx_msg void OnTreeSelect(NMHDR *pNMHDR, LRESULT *pResult);

	void LoadCurrent(int nVersion);
	void LoadOrig(void);

	CXMLSchemaElem* m_pOrigRoot;
	TiXmlDocument* m_pXMLDoc;
	
	CTreeCtrl m_ctlSchemaTree;
	CListBox m_ctlAttrs;
	CListBox m_ctlChildren;
	virtual BOOL DestroyWindow();
};
