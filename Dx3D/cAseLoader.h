#pragma once
class cAseLoader
{
private:
	FILE*	m_pFile;
	char	m_cstrToken[1024];
	int		m_nLevel;
public:
	cAseLoader();
	~cAseLoader();

	void Load(std::string& sFolder, std::string& sFileName);
	void Load(const char* sFolder, const char* sFileName);
	char* GetToken();
	BOOL IsEqual(char* str1, char* str2);

	void SkipProc();
	void SceneProc();
	void MaterialListPorc();
	void MaterialPorc();
};

