/////////////////////////////////////////////////////////////////////////////////////////////
// MeshDevApp.h
/////////////////////////////////////////////////////////////////////////////////////////////

#include "Application.h"

class CFontManager;
class CMeshData;

class CSimpleScene;

/////////////////////////////////////////////////////////////////////////////////////////////
class CMeshDevApp : public CApplication
{
public:
	virtual ~CMeshDevApp();
	static void CreateSingleton();

	virtual void SetSize(int width, int height);
	virtual bool RunMainProcess();

    virtual void KeyDown(unsigned int key);
    virtual void KeyUp(unsigned int key);

    virtual void MouseMove(int x, int y);
	virtual void MouseButtonDown(ButtonType btype, int x, int y);
	virtual void MouseButtonUp(ButtonType btype, int x, int y);

protected:
	CMeshDevApp();
    bool StartAppl(CApplWindow *pApplWnd);
	bool InitAppl();

    char *LoadShaderSrc(const char *filename);
    void LoadShaders();

    bool m_bMDown;
    int m_MPosX,m_MPosY;

    CSimpleScene *m_pScene;
    CFontManager *m_pFontMgr;

};
/////////////////////////////////////////////////////////////////////////////////////////////