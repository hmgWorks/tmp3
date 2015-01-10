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
