/////////////////////////////////////////////////////////////////////////////////////////////
// BaseAppl.h
/////////////////////////////////////////////////////////////////////////////////////////////

#include "Application.h"

class CFontManager;

/////////////////////////////////////////////////////////////////////////////////////////////
class CBaseAppl : public CApplication
{
public:
	virtual ~CBaseAppl();
	static void CreateSingleton();

	virtual void SetSize(int width, int height);
	virtual bool RunMainProcess();

    virtual void KeyDown(unsigned int key);
    virtual void KeyUp(unsigned int key);

protected:
	CBaseAppl();
    bool StartAppl(CApplWindow *pApplWnd);
	bool InitAppl();

    CFontManager *m_pFontMgr;
};
/////////////////////////////////////////////////////////////////////////////////////////////