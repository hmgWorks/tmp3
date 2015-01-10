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

std::string cMeshObj::GetNodeName()
{
	return m_strNodeName;
}

void cMeshObj::SetNodeName(char* nodeName)
{
	m_strNodeName = std::string(nodeName);
}

//�ڽ��� ������ �ڽ��� ����� ������ �����
//�ڽ��� ������ ������ ����
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