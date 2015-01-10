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

void cMeshObj::Setup(std::vector<ST_PNT_VERTEX>& vecVertex, cMtlTex* mtltex)
{
	if (mtltex != NULL)
	{
		mtltex->AddRef();
		m_pMtlTex = mtltex;
	}

	int size = vecVertex.size();
	HRESULT hr = D3DXCreateMeshFVF(size / 3, size, D3DXMESH_MANAGED,
		ST_PNT_VERTEX::FVF, g_pD3DDevice, &m_pMesh);
	assert(hr == D3D_OK && "mesh error");

	ST_PNT_VERTEX* pV = NULL;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	{
		memcpy(pV, &vecVertex[0], sizeof(ST_PNT_VERTEX)*size);
	}
	m_pMesh->UnlockVertexBuffer();

	WORD* pI = NULL;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	{
		for (int i = 0; i < size; i++)
		{
			pI[i] = i;
		}
	}
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	m_pMesh->LockAttributeBuffer(0, &pA);
	{
		for (int i = 0; i < size / 3; i++)
		{
			pA[i] = 0;
		}
	}
	m_pMesh->UnlockAttributeBuffer();

	std::vector<DWORD> vecAdjBuffer(size);
	m_pMesh->GenerateAdjacency(0.0f, &vecAdjBuffer[0]);

	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdjBuffer[0], 0, 0, 0);
}

void cMeshObj::Render()
{
	g_pD3DDevice->SetTexture(0, m_pMtlTex->pTex);
	g_pD3DDevice->SetMaterial(&m_pMtlTex->stMtl);
	m_pMesh->DrawSubset(0);
	if (!m_vecChild.empty())
	{
		for (auto p : m_vecChild)
			p->Render();
	}
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
	std::string name = this->GetNodeName();
	if (name == nodeName)
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

void cMeshObj::SetWorldMatrix(D3DXMATRIX mat)
{	
	m_matWorld = mat;
}

D3DXMATRIXA16& cMeshObj::GetWorldMatrix()
{
	return m_matWorld;
}

void cMeshObj::SetLocalMatrix(D3DXMATRIX mat)
{
	m_matLocal = mat;
}
