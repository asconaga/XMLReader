#pragma once

class CVarData
{
public:

	union
	{
		double dblVal;
		BOOL bBool;
		int intVal;
		unsigned int uintVal;
		char *pcVal;
	} vData;

	char GetType(void) { return m_type; };
	void SetType(char type) { m_type = type; };

private:
	char m_type;
};

class CXMLSchemaData;

class CXMLSchemaElem;

class CXMLSchemaAttr
{
public:
	CXMLSchemaAttr(CString name, char type, bool bMandatory)
	{
		m_name = name;
		m_type = type;
		m_bMandatory = bMandatory;
	};

	~CXMLSchemaAttr()
	{
	};

	virtual void PerformAction(CXMLSchemaData*) { ; };

	CXMLSchemaElem* m_pElem; // pointer to parent elem

	char GetType(void) { return m_type; };
	CString GetName(void) { return m_name; };
	bool IsMandatory(void) { return m_bMandatory; };

private:
	CString m_name;
	char m_type;
	bool m_bMandatory;
};

class CXMLSchemaElem
{
public:
	CXMLSchemaElem(CString name, char type, bool bProcess = true)
	{
		m_name = name;
		m_type = type;
		m_bProcess = bProcess;
	};

	~CXMLSchemaElem();

	CXMLSchemaElem* AddElem(CString name, char type, bool bProcess = true)
	{
		CXMLSchemaElem* pNewElem = new CXMLSchemaElem(name, type, bProcess);

		m_arrElems.Add(pNewElem);

		return pNewElem;
	};

	CXMLSchemaAttr* CreateAttr(CString name, char type, bool bMandatory = true)
	{
		CXMLSchemaAttr* pNewAttr = new CXMLSchemaAttr(name, type, bMandatory);

		pNewAttr->m_pElem = this;

		m_arrAttrs.Add(pNewAttr);

		return pNewAttr;
	};

	void AttachAttr(CXMLSchemaAttr* pNewAttr)
	{
		pNewAttr->m_pElem = this;

		m_arrAttrs.Add(pNewAttr);
	};

	void AddLimpet(CXMLSchemaElem* pLimpet)
	{
		m_arrLimpets.Add(pLimpet);
	};

	int GetVarIndex(CString varName)
	{
		int nMatch = -1;

		for (int i = 0; i < m_arrAttrs.GetCount(); i++)
		{
			CXMLSchemaAttr* pNextAttr = m_arrAttrs[i];

			if (varName == pNextAttr->GetName())
			{
				nMatch = i;
			}
		}

		return nMatch;
	}

private:
	char m_type;
	CString m_name;
	CArray <CXMLSchemaAttr*> m_arrAttrs;
	CArray <CXMLSchemaElem*> m_arrElems;
	CArray <CXMLSchemaElem*> m_arrLimpets;
	CArray <CXMLSchemaData*> m_arrData;

public:
	bool m_bProcess;

	char GetType(void) { return m_type; };
	CString GetName(void) { return m_name; };

	CXMLSchemaAttr* GetAttr(int idx) { return m_arrAttrs[idx]; };
	CXMLSchemaElem* GetElem(int idx) { return m_arrElems[idx]; };
	CXMLSchemaElem* GetLimpet(int idx) { return m_arrLimpets[idx]; };
	CXMLSchemaData* GetData(int idx) { return m_arrData[idx]; };

	void AddData(CXMLSchemaData* pData) { m_arrData.Add(pData); };
	
	int GetAttrCount(void) { return m_arrAttrs.GetCount(); };
	int GetElemCount(void) { return m_arrElems.GetCount(); };
	int GetLimpetCount(void) { return m_arrLimpets.GetCount(); };
};

class CXMLSizeSchemaAttr : public CXMLSchemaAttr
{
public:
	CXMLSizeSchemaAttr(CString name, char type, bool bMandatory = true) : CXMLSchemaAttr(name, type, bMandatory)
	{
	};

	virtual void PerformAction(CXMLSchemaData* pData);
};

class CXMLSchemaElem;

class CXMLSchemaData
{
public:
	CXMLSchemaData(CXMLSchemaElem* pParent)
	{
		m_pParent = pParent;
	};

	CString ToString(int idx);

	CVarData GetData(CString varName);

	CVarData GetData(int idx);

	double GetDouble(CString varName);

	void SetData(CString varName, CVarData vob);

	void CXMLSchemaData::SetDouble(CString varName, double dbVal);

	HTREEITEM m_hTreeItem;

public:

	int GetDataCount(void) { return m_arrRealData.GetCount(); };

	void AddData(CVarData data) { m_arrRealData.Add(data); };

	CXMLSchemaElem* GetParentElem(void) { return m_pParent; };

	CString GetName(void) { return m_name; };

	void SetName(CString name) { m_name = name; };

private:
	CArray <CVarData> m_arrRealData;

	CXMLSchemaElem* m_pParent;

	CString m_name;
};
