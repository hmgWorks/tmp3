#pragma once

class cMtlTex;
class cMeshObj
{
private:
	std::string				m_strNodeName;
	std::vector<cMeshObj*>	m_vecChild;
	LPD3DXMESH				m_pMesh;
	cMtlTex*				m_pMtlTex;


public:
	cMeshObj();
	~cMeshObj();

	void AddChild(std::string nodeName, cMeshObj* node);
	std::string GetNodeName();
	void SetNodeName(char* nodeName);
	void Setup();
	void Delete();
};

