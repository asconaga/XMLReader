#include "pch.h"
#include "framework.h"
#include "XMLReader.h"
#include "DlgXMLReader.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CFlexieDlg dialog

CFlexieDlg::CFlexieDlg(CWnd* pParent /*=nullptr*/) : CDialogEx(IDD_FLEXIE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pOrigRoot = nullptr;

	m_pXMLDoc = nullptr;
}

void CFlexieDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TREE2, m_ctlSchemaTree);
	DDX_Control(pDX, IDC_LIST3, m_ctlAttrs);
	DDX_Control(pDX, IDC_LIST4, m_ctlChildren);
}

BEGIN_MESSAGE_MAP(CFlexieDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, OnSave)
	ON_BN_CLICKED(IDC_BUTTON3, OnLoad)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnChangeFileName)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE2, OnTreeSelect)
END_MESSAGE_MAP()

// CFlexieDlg message handlers

BOOL CFlexieDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_pXMLDoc = new TiXmlDocument();

	OnLoad();
	
	return TRUE;  
}

CXMLSchemaElem* CFlexieDlg::CreateOrigSchema(bool bOrig)
{
	CXMLSchemaElem* pRoot = new CXMLSchemaElem("SavedLayouts", 's');
	CXMLSchemaElem* pLayoutElem = pRoot->AddElem("Layout", '0');
		pLayoutElem->CreateAttr("name", 's');

	CXMLSchemaElem* pOverlayElem = pLayoutElem->AddElem("Overlay", '1');
		pOverlayElem->CreateAttr("name", 's');

	CXMLSchemaElem* pPropertiesElem = pOverlayElem->AddElem("Properties", 's');
	//pPropertiesElem->CreateAttr("name", 's'); // ignore as duplicate
		pPropertiesElem->CreateAttr("style", 'i');
		pPropertiesElem->CreateAttr("minValue", 'd');
		pPropertiesElem->CreateAttr("maxValue", 'd');
		pPropertiesElem->CreateAttr("minArc", 'd', false); // only for Dial
		pPropertiesElem->CreateAttr("maxArc", 'd', false); // only for Dial
		pPropertiesElem->CreateAttr("dp", 'i', false); // only for Text and Digital
		pPropertiesElem->CreateAttr("opacity", 'i');
		pPropertiesElem->CreateAttr("isTransparent", 'b');
		pPropertiesElem->CreateAttr("label", 's');

	// Colour
	CXMLSchemaElem* pColourElem = pOverlayElem->AddElem("Colour", 's', !bOrig);
		pColourElem->CreateAttr("foregroundColour", 'u');
		pColourElem->CreateAttr("backgroundColour", 'u');

	if (bOrig)
		pPropertiesElem->AddLimpet(pColourElem);

	// Component
	CXMLSchemaElem* pComponentElem = pOverlayElem->AddElem("Component", 's', !bOrig);
		pComponentElem->CreateAttr("inputType", 'i');
		pComponentElem->CreateAttr("inputID", 'i', false);
		pComponentElem->CreateAttr("measurementType", 'i');
		pComponentElem->CreateAttr("componentType", 'i');
		pComponentElem->CreateAttr("componentId", 'i');
		pComponentElem->CreateAttr("nComponents", 'i');

	if (bOrig)
		pPropertiesElem->AddLimpet(pComponentElem);
	
	// Position
	CXMLSchemaElem* pPositionElem = pOverlayElem->AddElem("Position", 's', !bOrig);
		pPositionElem->CreateAttr("cx", 'i');
		pPositionElem->CreateAttr("cy", 'i');
		
		pPositionElem->CreateAttr("size", 'd', !bOrig); // from width / height

		if (bOrig)
		{
			pPositionElem->CreateAttr("width", 'd');
			pPositionElem->AttachAttr(new CXMLSizeSchemaAttr("height", 'd'));
		} 

	if (bOrig)
		pPropertiesElem->AddLimpet(pPositionElem);

	CXMLSchemaElem* pWindowsElem = pOverlayElem->AddElem("Windows", 's');
		pWindowsElem->CreateAttr("bName", 'b');
		pWindowsElem->CreateAttr("bLabel", 'b');
		pWindowsElem->CreateAttr("bDp", 'b');
		pWindowsElem->CreateAttr("bMin", 'b');
		pWindowsElem->CreateAttr("bMax", 'b');
		pWindowsElem->CreateAttr("bStart", 'b');
		pWindowsElem->CreateAttr("bEnd", 'b');
		pWindowsElem->CreateAttr("bRanges", 'b');

	CXMLSchemaElem* pRangesElem = pOverlayElem->AddElem("Ranges", 's');
		pRangesElem->CreateAttr("ranges", 'i');

	CXMLSchemaElem* pRangeElem = pRangesElem->AddElem("Range", '0');
		pRangeElem->CreateAttr("min", 'd');
		pRangeElem->CreateAttr("max", 'd');
		pRangeElem->CreateAttr("colour", 'u');
		pRangeElem->CreateAttr("label", 's');

	return pRoot;
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFlexieDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFlexieDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFlexieDlg::OnSave()
{
	//TiXmlDocument* pXMLDoc = new TiXmlDocument();

	//TiXmlElement* pColorContainerElem = new TiXmlElement("container");
	//pColorContainerElem->SetAttribute("count", m_nRanges);

	//for (int iBand = 0; iBand < m_nRanges; iBand++)
	//{
	//	TiXmlElement* pColorElem = new TiXmlElement("color");

	//	CColorObj* pObj = new CColorObj();

	//	const long iRow = m_nHeaderRows + iBand;

	//	pObj->m_min = atof(m_ctlContours.GetTextMatrix(iRow, (long)columnMin));
	//	pObj->m_max = atof(m_ctlContours.GetTextMatrix(iRow, (long)columnMax));

	//	m_ctlContours.Select(iRow, (long)columnColour, COleVariant((long)iRow), COleVariant((long)columnColour));
	//	pObj->m_color = m_ctlContours.GetCellBackColor();

	//	pObj->m_label = m_ctlContours.GetTextMatrix(iRow, (long)columnLabel);

	//	m_arrColours.Add(pObj);

	//	CString text;
	//	text.Format("%g", pObj->m_min);
	//	pColorElem->SetAttribute("min", text);

	//	text.Format("%g", pObj->m_max);
	//	pColorElem->SetAttribute("max", text);
	//	pColorElem->SetAttribute("color", pObj->m_color);
	//	pColorElem->SetAttribute("label", pObj->m_label);

	//	pColorContainerElem->LinkEndChild(pColorElem);
	//}
	//
	//TiXmlNode* pRoot = pXMLDoc->LinkEndChild(pColorContainerElem);

	//pXMLDoc->SaveFile("out.xml");

	//delete pXMLDoc;
}

CVarData CFlexieDlg::ValidateType(const char* value, char chType)
{
	CVarData variant;

	char* pcEnd = nullptr;
	variant.SetType(chType);

	if (chType == 's') {
		variant.vData.pcVal = (char*)value;
	}
	else if (chType == 'd') {
		double dbVal = strtod(value, &pcEnd);

		if (*pcEnd == '\0')
		{
			variant.vData.dblVal = dbVal;
		}
		else
		{
			variant.SetType(0); // fail
		}
	}
	else 
	{
		const long lVal = strtol(value, &pcEnd, 10);

		if (*pcEnd == '\0') {
			variant.vData.intVal = lVal;

			switch (chType) {
			case 'u':
				if (lVal < 0)
				{
					variant.SetType(0); // fail
				}
				else
				{
					variant.vData.uintVal = lVal;
				}
				break;
			case 'b':
				if ((lVal < 0) || (lVal > 1))
				{
					variant.SetType(0); // fail
				}
				else
				{
					variant.vData.bBool = lVal;
				}
				break;
			}
		}
		else
		{
			variant.SetType(0); // fail
		}
	}

	return variant;
}

void CFlexieDlg::ParseXMLFromSchema(CXMLSchemaElem* pThisElem, CXMLSchemaData* pThisData, TiXmlNode* pElem)
{
	CString szFmt;

	int nElements = 0;

	bool bIsSingle = (pThisElem->GetType() == 's');

	for (TiXmlElement* pChild = pElem->FirstChildElement(pThisElem->GetName()); pChild != nullptr; pChild = pChild->NextSiblingElement(pThisElem->GetName()))
	{
		nElements++;

		CXMLSchemaData* pNewData = new CXMLSchemaData(pThisElem);

		pNewData->SetName(pChild->Value());

		pNewData->m_hTreeItem = m_ctlSchemaTree.InsertItem(pNewData->GetName(), (pThisData) ? pThisData->m_hTreeItem : TVI_ROOT);

		if (bIsSingle && nElements > 1)
			ThrowException("\"%s\" is has incorrect number of elements [%d] should be at least 1", pThisElem->GetName(), nElements);

		m_ctlSchemaTree.SetItemData(pNewData->m_hTreeItem, (DWORD_PTR)pNewData);

		pThisElem->AddData(pNewData);

		for (int i = 0; i < pThisElem->GetAttrCount(); i++)
		{
			CXMLSchemaAttr* pAttr = pThisElem->GetAttr(i);

			ProcessAttr(pThisElem->GetName(), pAttr, pChild, pNewData);
		}

		for (int i = 0; i < pThisElem->GetLimpetCount(); i++)
		{
			CXMLSchemaElem* pLimpetElem = pThisElem->GetLimpet(i);

			CXMLSchemaData* pLimpetData = new CXMLSchemaData(pLimpetElem);

			pLimpetData->SetName(pLimpetElem->GetName());

			pLimpetData->m_hTreeItem = m_ctlSchemaTree.InsertItem(pLimpetData->GetName(), (pThisData) ? pThisData->m_hTreeItem : TVI_ROOT);

			m_ctlSchemaTree.SetItemData(pLimpetData->m_hTreeItem, (DWORD_PTR)pLimpetData);

			pLimpetElem->AddData(pLimpetData);

			for (int nAttr = 0; nAttr < pLimpetElem->GetAttrCount(); nAttr++)
			{
				CXMLSchemaAttr* pAttr = pLimpetElem->GetAttr(nAttr);

				ProcessAttr(pThisElem->GetName(), pAttr, pChild, pLimpetData);
			}
		}   	 	  

		for (int i = 0; i < pThisElem->GetElemCount(); i++)
		{
			CXMLSchemaElem* pNextElem = pThisElem->GetElem(i);

			if (pNextElem->m_bProcess)
				ParseXMLFromSchema(pNextElem, pNewData, pChild);
		}
	}

	switch (pThisElem->GetType())
	{
	case 's':
		if (nElements != 1)
			ThrowException("\"%s\" is has incorrect number of elements [%d] should be at least 1", pThisElem->GetName(), nElements);
		break;
	case '1':
		if (nElements < 1)
			ThrowException("\"%s\" is has incorrect number of elements [%d] should be 1", pThisElem->GetName(), nElements);
		break;
	}
}

void CFlexieDlg::ProcessAttr(CString elemName, CXMLSchemaAttr* pAttr, TiXmlElement* pChild, CXMLSchemaData* pNewData)
{
	CString szFmt;

	char chType = pAttr->GetType();

	CString szAttrName = pAttr->GetName();

	const char* pDat = pChild->Attribute(szAttrName);

	if (pDat)
	{
		CVarData opt = ValidateType(pDat, chType);

		if (opt.GetType() != chType)
			ThrowException("\"%s\" with value \"%s\"is incorrect type - expected\"%c\"", szAttrName, pDat, chType);

		pNewData->AddData(opt);
	}
	else
	{
		if (pAttr->IsMandatory())
		{
			ThrowException("\"%s\" is missing attribute \"%s\"", elemName, szAttrName);
		}
		else
		{
			CVarData opt;

			opt.SetType(0); // fail
			opt.vData.intVal = -1;

			pNewData->AddData(opt);
		}
	}

	pAttr->PerformAction(pNewData);
}

void CFlexieDlg::ThrowException(LPCTSTR lpszFormat, ...)
{
	CString fmt;
	va_list args;

	va_start(args, lpszFormat);
	fmt.FormatV(lpszFormat, args);
	va_end(args);

	THROW((CException*) new CFileException(CFileException::none, -1, fmt));
}

void CFlexieDlg::OnLoad()
{
	CString fname = "SLTest.xml";

	bool bLoadXML = m_pXMLDoc->LoadFile(fname);

	if (bLoadXML)
	{
		TiXmlElement* pRoot = m_pXMLDoc->RootElement();

		const char* pValue = pRoot->Value();

		bool bIsCorrect = (strcmp(pValue, "SavedLayouts") == 0);

		const char* pVersion = pRoot->Attribute("version");

		if (pVersion)
		{
			m_pOrigRoot = CreateOrigSchema(false);

			int version = atoi(pVersion);

			LoadCurrent(version);
		}
		else
		{
			m_pOrigRoot = CreateOrigSchema(true);

			LoadOrig();
		}
	}
	else
	{
		if (m_pXMLDoc->Error())
		{
			if (m_pXMLDoc->ErrorId() == TiXmlBase::TIXML_ERROR_OPENING_FILE)
			{
				TiXmlElement* element = new TiXmlElement("SavedLayouts");
				m_pXMLDoc->LinkEndChild(element);

				element->SetAttribute("version", 1);

				m_pXMLDoc->SaveFile(fname);
			}
			else
			{
				int col = m_pXMLDoc->ErrorCol();
				int row = m_pXMLDoc->ErrorRow();

				CString desc = m_pXMLDoc->ErrorDesc();

				CString szErr;

				szErr.Format("%s - row=%d col = %d", desc, row, col);

				SetWindowText(szErr);
			}
		}
	}
}

void CFlexieDlg::LoadCurrent(int nVersion)
{
	try
	{
		ParseXMLFromSchema(m_pOrigRoot, nullptr, m_pXMLDoc);
	}
	catch (CException* pe)
	{
		CFileException* de = (CFileException*)pe;

		SetWindowText(de->m_strFileName);

		pe->Delete();
	}

	HTREEITEM hItem = m_ctlSchemaTree.GetFirstVisibleItem();
	while (hItem != nullptr)
	{
		m_ctlSchemaTree.Expand(hItem, TVE_EXPAND);
		hItem = m_ctlSchemaTree.GetNextItem(hItem, TVGN_NEXTVISIBLE);
	}
}

void CFlexieDlg::LoadOrig(void)
{
	try
	{
		ParseXMLFromSchema(m_pOrigRoot, nullptr, m_pXMLDoc);
	}
	catch (CException* pe)
	{
		CFileException* de = (CFileException*)pe;

		SetWindowText(de->m_strFileName);

		pe->Delete();
	}

	HTREEITEM hItem = m_ctlSchemaTree.GetFirstVisibleItem();
	while (hItem != nullptr)
	{
		m_ctlSchemaTree.Expand(hItem, TVE_EXPAND);
		hItem = m_ctlSchemaTree.GetNextItem(hItem, TVGN_NEXTVISIBLE);
	}
}

void CFlexieDlg::OnChangeFileName()
{
	OnLoad();
}

void CFlexieDlg::OnTreeSelect(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM hSelectedItem = m_ctlSchemaTree.GetSelectedItem();

	CXMLSchemaData* pThisData = (CXMLSchemaData*)m_ctlSchemaTree.GetItemData(hSelectedItem);
	
	m_ctlChildren.ResetContent();
	m_ctlAttrs.ResetContent();

	if (pThisData)
	{
		if (pThisData->GetParentElem())
		{
			int nVals = pThisData->GetDataCount();
			int nAttrs = pThisData->GetParentElem()->GetAttrCount();

			CString datAttr;
			for (int i = 0; i < nAttrs; i++)
			{
				if (i < nVals)
				{
					CVarData vData = pThisData->GetData(i);

					if (vData.GetType() != 0)
					{
						datAttr.Format("%s[%s]", pThisData->GetParentElem()->GetAttr(i)->GetName(), pThisData->ToString(i));
						m_ctlAttrs.AddString(datAttr);
					}
				}
				else 
				{
					datAttr.Format("%s[%s]", pThisData->GetParentElem()->GetAttr(i)->GetName(), "null");
					m_ctlAttrs.AddString(datAttr);
				}
			}
		}
	}

	*pResult = 0;
}

BOOL CFlexieDlg::DestroyWindow()
{
	delete m_pOrigRoot;

	delete m_pXMLDoc;

	return CDialogEx::DestroyWindow();
}
