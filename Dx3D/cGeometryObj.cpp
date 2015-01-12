#include "stdafx.h"
#include "cGeometryObj.h"
#include "cMeshObj.h"

cGeometryObj::cGeometryObj()
	:m_pRoot(NULL)
	, m_vPosition(0, 0, 0)
	, m_vForward(0, 0, -1)
	, m_fSpeed(0.5f)
{
	
}

cGeometryObj::~cGeometryObj()
{
	m_pRoot->Delete();
}

void cGeometryObj::Setup()
{

}

void cGeometryObj::Update()
{
	//D3DXVECTOR3 vPosition = m_vPosition;

	bool isIdle = true;
	if (GetKeyState('A') & 0x8000)
	{
		m_fAngle -= 0.1f;
		D3DXMATRIXA16 matR;
		D3DXMatrixRotationY(&matR, m_fAngle);
		m_vForward = D3DXVECTOR3(0, 0, -1);
		D3DXVec3TransformNormal(&m_vForward, &m_vForward, &matR);
	}
	if (GetKeyState('D') & 0x8000)
	{
		m_fAngle += 0.1f;
		D3DXMATRIXA16 matR;
		D3DXMatrixRotationY(&matR, m_fAngle);
		m_vForward = D3DXVECTOR3(0, 0, -1);
		D3DXVec3TransformNormal(&m_vForward, &m_vForward, &matR);
	}
	if (GetKeyState('W') & 0x8000)
	{
		m_vPosition += (m_vForward * m_fSpeed);
		isIdle = false;
	}
	if (GetKeyState('S') & 0x8000)
	{
		m_vPosition -= (m_vForward * m_fSpeed);
		isIdle = false;
	}

	/*if (pMap)
	{ 
		bool isLand = false;
		float fHeight = pMap->GetHeight(isLand, &vPosition);
		if (isLand)
		{
			m_vPosition = vPosition;
			m_vPosition.y = fHeight;
		}
	}
	else
	{
		m_vPosition = vPosition;
	}*/

	/*if (isIdle)
	{
		m_pRoot->SetForcedAngle(0.0f);
	}*/

	D3DXMATRIXA16 matR, matT;
	D3DXMatrixRotationY(&matR, m_fAngle);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	
	//D3DXMatrixTranslation(&matT, 2, 2, 1);
	m_matWorld = matR * matT;
	//matWorld = matT;

	m_pRoot->Update(&m_matWorld);
}

void cGeometryObj::Render()
{	
	m_pRoot->Render();
}

void cGeometryObj::SetRoot(cMeshObj* root)
{	
	if (m_pRoot == NULL)
	{
		m_pRoot = root;
	}
	else
	{
		//assert(false && "root 가 이미 있습니다.");
	}
}

//부모의 이름을 입력 받는다.
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