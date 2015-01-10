#pragma once

class cCube;
class cGrid;
class cCamera;
class cCubeMan;
class cGroup;
class iMap;
class cIndexCube;
class cMtlTex;
class cGeometryObj;

class cMainGame
{
private:
	cGrid*					m_pGrid;
	cCamera*				m_pCamera;
	cCubeMan*				m_pCubeMan;
	iMap*					m_pMap;
	cIndexCube*				m_pIndexCube;
	LPD3DXFONT				m_pFont;
	std::vector<cGroup*>	m_vecGroup;
	std::vector<cGroup*>	m_vecSurface;
	LPD3DXMESH				m_pMesh;
	std::vector<cMtlTex*>	m_vecMtlTex;
	
	cGeometryObj*			m_pGeometryObj;

public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

