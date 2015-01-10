#pragma once

class cMtlTex;
class cMeshObj
{
private:
	std::string				m_strNodeName;
	std::vector<cMeshObj*>	m_vecChild;
	LPD3DXMESH				m_pMesh;
	cMtlTex*				m_pMtlTex;
	D3DXMATRIXA16			m_matWorld;
	D3DXMATRIXA16			m_matLocal;

public:
	cMeshObj();
	~cMeshObj();

	void AddChild(std::string nodeName, cMeshObj* node);
	cMeshObj* GetNode(std::string nodeName);
	std::string GetNodeName();
	void SetNodeName(char* nodeName);
	void SetWorldMatrix(D3DXMATRIXA16& matWorld);
	void SetLocalMatrix(D3DXMATRIX matLocal);
	void Setup();
	void Delete();
};

