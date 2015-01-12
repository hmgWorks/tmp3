#pragma once

class cMeshObj;

class cGeometryObj
{
private:
	cMeshObj*	m_pRoot;
	D3DXVECTOR3 m_vPosition;
	D3DXVECTOR3 m_vForward;
	float		m_fSpeed, m_fAngle;

	D3DXMATRIXA16 m_matWorld;
public:
	cGeometryObj();
	~cGeometryObj();

	void Setup();
	void Update();
	void Render();

	void SetRoot(cMeshObj* root);
	void AddChild(std::string nodeName, cMeshObj* child);
	cMeshObj* GetNode(std::string nodeName);
	void SetWorldMatrix(D3DXMATRIXA16& matWordl);
	
};

