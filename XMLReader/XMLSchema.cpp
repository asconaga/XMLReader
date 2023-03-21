#include "pch.h"
#include "XMLSchema.h"

CXMLSchemaElem::~CXMLSchemaElem()
{
	for (int i = 0; i < m_arrElems.GetCount(); i++)
	{
		CXMLSchemaElem* pNextElem = m_arrElems[i];

		delete pNextElem;
	}

	for (int i = 0; i < m_arrAttrs.GetCount(); i++)
	{
		CXMLSchemaAttr* pNextAttr = m_arrAttrs[i];

		delete pNextAttr;
	}

	for (int i = 0; i < m_arrData.GetCount(); i++)
	{
		CXMLSchemaData* pNextData = m_arrData[i];

		delete pNextData;
	}
};

void CXMLSizeSchemaAttr::PerformAction(CXMLSchemaData* pData)
{
	CVarData wData = pData->GetData("width");

	double h = pData->GetDouble("height");

	double w = wData.vData.dblVal;

	double mVal = (w > h) ? w : h;

	double size = mVal / 400;

	if ((size < 50) || (size > 150))
	{
		size = 100;
	}

	pData->SetDouble("size", size);
};

CString CXMLSchemaData::ToString(int idx)
{
	CString szData;

	CVarData vob = m_arrRealData[idx];

	switch (vob.GetType())
	{
	case 'd':
		szData.Format("%g", vob.vData.dblVal);
		break;
	case 'b':
		szData.Format("%c", (vob.vData.bBool) ? 'T' : 'F');
		break;
	case 'i':
	case 'u':
		szData.Format("%d", vob.vData.intVal);
		break;
	case 's':
		szData.Format("%s", vob.vData.pcVal);
		break;
	default:
		break;
		szData.Format("Fail");
	}

	return szData;
};

CVarData CXMLSchemaData::GetData(int idx)
{
	CVarData vob = m_arrRealData[idx];

	return vob;
}

CVarData CXMLSchemaData::GetData(CString varName)
{
	int idx = m_pParent->GetVarIndex(varName);

	CVarData vob = m_arrRealData[idx];

	return vob;
}

double CXMLSchemaData::GetDouble(CString varName)
{
	int idx = m_pParent->GetVarIndex(varName);

	CVarData vob = m_arrRealData[idx];

	return vob.vData.dblVal;
}

void CXMLSchemaData::SetData(CString varName, CVarData vob)
{
	int idx = m_pParent->GetVarIndex(varName);

	m_arrRealData[idx] = vob;
}

void CXMLSchemaData::SetDouble(CString varName, double dbVal)
{
	int idx = m_pParent->GetVarIndex(varName);

	CVarData vob;
	vob.SetType('d');
	vob.vData.dblVal = dbVal;

	m_arrRealData[idx] = vob;
}
