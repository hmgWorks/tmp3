#pragma once
class cMtlTex;
class cMeshObj;
class cGeometryObj;

class cAseLoader
{
private:
	FILE*							m_pFile;
	std::string						m_strFolder;
	char							m_cstrToken[1024];
	
	int								m_nLevel;
	int								m_nMtlId;
	std::vector<cMtlTex*>			m_vecMtlTex;

	cGeometryObj*					m_pGeometryObj;
	cMeshObj*						m_pNode;
	cMeshObj*						m_pPrentNode;
	std::string						m_strParentName;

	std::vector<D3DXVECTOR3>		m_vecV;
	std::vector<D3DXVECTOR2>		m_vecVT;
	std::vector<D3DXVECTOR3>		m_vecVN;

	std::vector<std::vector<int>>	m_vecVF;
	std::vector<std::vector<int>>	m_vecVTF;

	D3DXMATRIXA16 m_matLocalTM, m_matWorldTM, m_matInverseWorldTM;
	D3DXMATRIXA16 m_matPWorldTM, m_matInversePWorldTM;

public:
	cAseLoader();
	~cAseLoader();

	void Load(cGeometryObj* geometryhObj, std::string& sFolder, std::string& sFileName);
	void Load(cGeometryObj* geometryhObj, const char* sFolder, const char* sFileName);
	char* GetToken();
	BOOL IsEqual(char* str1, char* str2);

	void SkipProc();
	void SceneProc();

	void MaterialListPorc();
	void MaterialPorc();
	void MapDiffusePorc();
	
	void GeometryProc();
	void NodeTMProc(D3DXVECTOR3& row0, D3DXVECTOR3& row1, D3DXVECTOR3& row2, D3DXVECTOR3& row3);
	void MeshProc();
	void MeshVertexListProc();
	void MeshFaceListProc();
	void MeshTVertListProc();
	void MeshTFaceListProc();
	void MeshTNormalsProc();
	
};

