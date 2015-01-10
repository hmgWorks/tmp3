#include "stdafx.h"
#include "cMeshObj.h"
#include "cMtlTex.h"

cMeshObj::cMeshObj()
	:m_pMesh(NULL)
	, m_pMtlTex(NULL)
{
}

cMeshObj::~cMeshObj()
{
	SAFE_RELEASE(m_pMtlTex);
	SAFE_RELEASE(m_pMesh);
}

void cMeshObj::AddChild(std::string nodeName, cMeshObj* node)
{
	if (GetNodeName() == nodeName)
	{
		this->m_vecChild.push_back(node);
	}
	else if (!m_vecChild.empty())
	{
		for (auto p : m_vecChild)
		{
			p->AddChild(nodeName, node);
		}
	}
	else
	{
		return;
	}
}

cMeshObj* cMeshObj::GetNode(std::string nodeName)
{
	if (GetNodeName() == nodeName)
	{
		return this;
	}
	else if (!m_vecChild.empty())
	{
		for (auto p : m_vecChild)
		{
			return p->GetNode(nodeName);
		}
	}
	else
	{
		return NULL;
	}
}


std::string cMeshObj::GetNodeName()
{
	return m_strNodeName;
}

void cMeshObj::SetNodeName(char* nodeName)
{
	m_strNodeName = std::string(nodeName);
}

//자식이 있으면 자식을 지우고 본인을 지우고
//자식이 없으면 본인을 지움
void cMeshObj::Delete()
{
	if (!m_vecChild.empty())
	{
		for (auto p : m_vecChild)
		{
			p->Delete();
		}
	}	
		delete	this;
}

void cMeshObj::SetWorldMatrix(D3DXMATRIXA16& matWorld)
{
	if (GetNodeName() == "root")
	{
		//this->SetLocalMatrix();
	}
	m_matWorld = matWorld;
	

}

void cMeshObj::SetLocalMatrix(D3DXMATRIX matLocal)
{
	m_matLocal = matLocal;
}
