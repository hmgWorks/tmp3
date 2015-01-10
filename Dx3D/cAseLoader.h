#pragma once
class cMtlTex;
class cMeshObj;
class cGeometryObj;

class cAseLoader
{
private:
	FILE*					m_pFile;
	char					m_cstrToken[1024];
	int						m_nLevel;
	int						m_nMtlId;
	std::vector<cMtlTex*>	m_vecMtlTex;
	std::string				m_strFolder;
	cGeometryObj*			m_pGeometryObj;
	std::string				m_strParentName;

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
	void GeometryProc(cMeshObj* meshObj);
	void NodeTMProc();
	void MeshProc();
};

