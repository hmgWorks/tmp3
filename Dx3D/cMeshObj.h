#pragma once

class cMtlTex;
class cMeshObj
{
private:
	std::string				m_strNodeName;
	std::vector<cMeshObj*>	m_vecChild;
	LPD3DXMESH				m_pMesh;
	cMtlTex*				m_pMtlTex;
	D3DXMATRIXA16			m_matWorld,m_matAse;
	D3DXMATRIXA16			m_matLocal;

	D3DXVECTOR3				m_vPosintion;
public:
	cMeshObj();
	~cMeshObj();

	void Setup(std::vector<ST_PNT_VERTEX> vecVertex, cMtlTex* mtltex = NULL);
	void Update(D3DXMATRIXA16* matPWorld = NULL);
	void Render();
	void Delete();

	void AddChild(std::string nodeName, cMeshObj* node);
	cMeshObj* GetNode(std::string nodeName);
	std::string GetNodeName();
	void SetNodeName(char* nodeName);
	void SetWorldMatrix(D3DXMATRIX mat);
	D3DXMATRIXA16 GetWorldMatrix();
	void SetLocalMatrix(D3DXMATRIX mat);
	void SetPos(float x, float y, float z)
	{
		m_vPosintion.x = x;
		m_vPosintion.y = y;
		m_vPosintion.z = z;
	}
};

