#pragma once

#include <string>
#include <vector>
#include <list>
#include <map>
#include "../Utils/Library.h"
#include "../Utils/sharedptr.h"
#include "utils/object_source.h"
#include "utils/cache/shared_cache.h"
#include "../GUI/Image.h"
#include "../GUI/LayoutReader.h"

//SPECIAL resource_id (picture_id) for background of the forms
//to mark that this is REALLY from skin - not user specified.
//this is not defined in res.h because the res.h file is script-prepared
//basis on pictures included in skin
#define	IDI_SKIN_BG_DIALOG				1000001

// pak support
class StringPAKSystemManager;
class PAKfileSystemManager;
struct stringpakfile;
struct pakfile; 

namespace MM {

namespace Logic {

/*!
 * \brief
 * Struktura opisuj�ca definicj� j�zyka interfejsu u�ytkownika.
 * 
 * Struktura opisuj�ca definicj� j�zyka interfejsu u�ytkownika.
 * 
 */
struct LangInfo {
std::string	lang_id;		//!< Identyfikator j�zyka w konwencji ISO-639-1 ("pl_PL", "en_US", ...)
std::string	lang_name;		//!< Nazwa j�zyka zapisana w w j�zyku w�asnym
std::string	lang_dll;		//!< Nazwa biblioteki dll, zawieraj�ca zasoby w danym j�zyku
int				reserved;		//!< Zarazerwowane do przysz�ego u�ycia
LangInfo() : reserved(0) {}
};

/*!
 * \brief
 * Klasa 's�uchacza' zmian j�zyka interfejsu u�ytkownika.
 * 
 * Klasa 's�uchacza' zmian j�zyka interfejsu u�ytkownika.
 * Po klasie tej powinny dziedziczy� te klasy, kt�re chc� otrzymywa� komunikaty o zmianie j�zyka. 
 * 
 * 
 * \remarks
 * Obiekt, kt�ry chce odbiera� komunikaty j�zykowe musi przeci��y� metod� LanguageChanged,
 * a tak�e zarejestrowa� si� w obiekcie rozg��szaj�cym zdarzenie. Uwaga! W momencie destrukcji
 * powinien dokona� wyrejestrowania!
 * 
 * \see
 * ResourceManager
 */
class LanguageChangeListener {
public:
	LanguageChangeListener() {}
	virtual ~LanguageChangeListener() {}
	/*!
	 * \brief
	 * Zdarzenie zmiany j�zyka interfejsu u�ytkownika.
	 * 
	 * \param lang_id
	 * Identyfikator j�zyka, kt�ry zosta� aktualnie ustawiony.
	 * 
	 * Zdarzenie zmiany j�zyka interfejsu u�ytkownika.
	 * 
	 */
	virtual void LanguageChanged(const std::string &lang_id) {}
};

/*!
 * \brief
 * Klasa zarz�dzaj�ca zewn�trznymi zasobami aplikacji tj. stringi, obrazy.
 * 
 * Klasa udost�pnia zasoby zawarte w zewn�trznych bibliotekach dll. Dodatkowo udost�pnia
 * interfejs pozwalaj�cy sterowa� j�zykiem interfejsu w aplikacji.
 * 
 * \remarks
 * Zasoby graficzne czerpane s� z pliku "mm3res.dll". Teksty j�zykowe z bibliotek o dowolnych
 * nazwach, ale zawieraj�cych odpowiednio zdefiniowan� struktur�.
 * 
 * \see
 * Separate items with the '|' character.
 */


class ResourceManager : public ObjectSource<ResKey, MM::GUI::Image>
{
protected:
	// holds founded lang libraries
	std::vector<std::string> lang_ids;
	std::vector<LangInfo> lang_infos;
	// current languag
	std::string curr_lang_id;
	int _skin_idx;

    //holds style data
	typedef std::map<int, std::string> style_map_t;
	style_map_t styleMap;
    bool m_brescale_font;

    //holds resolution map
    typedef std::map<int, int> int_map_t;
    typedef std::map<int, int_map_t> res_map_t;

    res_map_t resMap;

    std::string _curr_res_id;
    int _curr_x;
    int _curr_y;

    StringPAKSystemManager *PAKtextManager;
	PAKfileSystemManager *PAKresourceManager;

    stringpakfile *textPak;
	pakfile *resPak; 

	MM::GUI::ImageOwnPtr _bgimage;

	// listener list
	typedef std::list<LanguageChangeListener *> LangListeners;
	LangListeners _listeners;

	SharedCache <ResKey, MM::GUI::Image> *_cacher;
	void preloadImages();

    void InitLangMap();

public:
	ResourceManager();
	~ResourceManager();
	// TODO: init should return error values, like: no lang, no res, other error
	bool Init();

	std::string LoadString(int StringID);
	MM::GUI::ImageOwnPtr LoadImage(int ImageID);
	std::string LoadStyle(int StringID);
	MM::GUI::LayoutReader * LoadLayout(int ID);
    bool IsLayoutAvailable(int ID);

	const std::vector<std::string> &GetLanguages();
	const LangInfo &GetLanguageInfo(const std::string &LangID);
	// must be called after Init()
	bool SetLanguage(const std::string LangID);
	const std::string &GetLanguage() const { return curr_lang_id; }

	SharedCache <ResKey, MM::GUI::Image> *GetCacher() { return _cacher;}

	void AddLanguageChangeListener(LanguageChangeListener *o);
	void RemoveLanguageChangeListener(LanguageChangeListener *o);

	void BackImageChanged(int x, int y);
    void DeleteBackgroundFiles();
    void CreateBackgroundImages(int x, int y);

	void SetResolution(int x, int y);
	MM::GUI::Image *getObject(const ResKey &k );
	bool isOwner() const {return false;}
	std::vector<std::string> GetAvailSkins() {return _skins;}
	bool LoadSkin(int idx);
	bool LoadSkin(const std::string & name);
	int	GetSkinIdx(const std::string & name) const ;
	int GetLoadedSkin() {return _skin_idx;}
	std::string GetSkinName(int idx) const;

    int GetAvailableSkinID(int x, int y);
    const std::string GetCurrResPath() const { return _curr_res_id; }

protected:
	void FireLanguageChanged();

private:
	void scanSkinDlls();
    bool IsSkinVerSupported();

    void FindSkinSizes();
    void FindAvailableSkinRes(int (&res)[2] , int x, int y);

    bool LoadGUIFontStyles(const std::string &fntfile); 
    std::string ConvmmToPix(const std::string &style, float ydpi);
   
    MM::GUI::ImageOwnPtr LoadBgImage(int x, int y);

	std::vector<std::string> _skins;

    bool _bhigh_res;
    int _video_mode;

    std::map<std::string,std::string> _lang_id_map;
};

//--const wchar_t * win32_lang_find_name(LCID id);

} // namespace MMUI
} // namespace Logic
