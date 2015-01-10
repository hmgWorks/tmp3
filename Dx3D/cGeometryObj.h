#pragma once

class cMeshObj;

class cGeometryObj
{
private:
	cMeshObj*	m_pRoot;

public:
	cGeometryObj();
	~cGeometryObj();

	void SetRoot(cMeshObj* root);
	void AddChild(std::string nodeName, cMeshObj* child);
	cMeshObj* GetNode(std::string nodeName);
	void SetWorldMatrix(D3DXMATRIXA16& matWordl);
};

