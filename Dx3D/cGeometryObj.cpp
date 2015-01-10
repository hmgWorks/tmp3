#include "stdafx.h"
#include "cGeometryObj.h"
#include "cMeshObj.h"

cGeometryObj::cGeometryObj()
	:m_pRoot(NULL)
{
}

cGeometryObj::~cGeometryObj()
{
	m_pRoot->Delete();
}


void cGeometryObj::SetRoot(cMeshObj* root)
{	
	if (m_pRoot == NULL)
	{
		m_pRoot = root;
	}
	else
	{
		//assert(false && "root �� �̹� �ֽ��ϴ�.");
	}
}

//�θ��� �̸��� �Է� �޴´�.
void cGeometryObj::AddChild(std::string nodeName, cMeshObj* child)
{	
	if (nodeName == std::string("root"))
		SetRoot(child);

	m_pRoot->AddChild(nodeName, child);
}

void cGeometryObj::SetWorldMatrix(D3DXMATRIXA16& matWordl)
{	
	m_pRoot->SetWorldMatrix(matWordl);
}

cMeshObj* cGeometryObj::GetNode(std::string nodeName)
{
	return m_pRoot->GetNode(nodeName);
}