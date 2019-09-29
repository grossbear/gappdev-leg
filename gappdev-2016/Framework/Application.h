/////////////////////////////////////////////////////////////////////////////////////////////
// Application.h
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <TCHAR.H>
#include "PlatformTypes.h"

enum ButtonType
{
    BTYPE_LBUTTON,
    BTYPE_RBUTTON,
    BTYPE_MBUTTON,
};

/////////////////////////////////////////////////////////////////////////////////////////////
class CApplication
{
public:
    static CApplication* GetSingleton();
    virtual ~CApplication();

    virtual bool Run(CApplWindow *pApplWnd); 

	virtual void SetSize(int width, int height){}
	virtual void KeyDown(unsigned int key){}
	virtual void KeyUp(unsigned int key){}
	virtual void Activate(bool active){}

	virtual void MouseMove(int x, int y){}
	virtual void MouseButtonDown(ButtonType btype, int x, int y){}
	virtual void MouseButtonUp(ButtonType btype, int x, int y){}
	virtual void MouseButtonDblClk(ButtonType btype, int x, int y){}

    virtual bool IsActive() const;

    virtual bool RunMainProcess();

    virtual void CloseMsg();
    virtual void QuitMsg();

protected:
    CApplication(const char *pAppName);

    virtual bool StartAppl(CApplWindow *pApplWnd);
	virtual bool InitAppl();
    virtual bool RunAppl();
    virtual bool CloseAppl();

    virtual bool RunAfterProcess();

    virtual bool CreateGraphicAPI();

    static CApplication *m_pAppl;
    CApplWindow *m_pApplWnd;

    IGraphicApi *m_gapi;

    int         m_width;
    int         m_height;

    bool        m_bKeys[256];
    bool        m_bActive;
    bool        m_bFullScreen;

    bool        m_bCloseAppl;
    bool        m_bApplStarted;
    bool        m_bRunning;

    char        *m_strApplName;
};
/////////////////////////////////////////////////////////////////////////////////////////////

#endif //_APPLICATION_H_