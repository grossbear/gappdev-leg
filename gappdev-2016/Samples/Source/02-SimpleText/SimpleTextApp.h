/////////////////////////////////////////////////////////////////////////////////////////////
// BaseAppl.h
/////////////////////////////////////////////////////////////////////////////////////////////

#include "Application.h"

class CFontManager;

/////////////////////////////////////////////////////////////////////////////////////////////
class CSimpleTextApp : public CApplication
{
public:
	virtual ~CSimpleTextApp();
	static void CreateSingleton();

	virtual void SetSize(int width, int height);
	virtual bool RunMainProcess();

	virtual void KeyDown(unsigned int key);
	virtual void KeyUp(unsigned int key);

protected:
	CSimpleTextApp();
	bool StartAppl(CApplWindow *pApplWnd);
	bool InitAppl();

	CFontManager *m_pFontMgr;
};
/////////////////////////////////////////////////////////////////////////////////////////////