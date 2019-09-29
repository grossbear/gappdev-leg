#include "stdafx.h"
#include "../Application.h"
#include "../Utils/Library.h"
#include "../GUI/Image.h"

#include "ResourceManager.h"
#include <algorithm>
#include <cassert>
#include "OptionManager.h"
#include "VideoManager.h"
#include "MM/GUI/AppWindow.h"
#include "utils/mem/memory_allocator_pool.h"

#include "utils/fs/filesystem.h"
#include "utils/platform.h"
#include "utils/utils.h"
#include "utils/unicode/ctypes.h"
#include "utils/pak.h"
#include "utils/stringpak.h"

#if defined (__ANDROID__)
#include "jni_mm.h"
#include "hardware_defs.h"
#endif

#if defined (__APPLE__)
#include "hardware_defs.h"
#endif

#include "build/build_config.h"
#if defined SYNC_LANG_WITH_SYSTEM
#include "winuser.h"
#include "Winnls.h"
#endif

#include "logger/logger_helper.h"

#if defined _LOGGER_ENABLED
#	define DO_LOG 1
#else
#	define DO_LOG 0
#endif


LOGGER_IF(lg, "GUI", DO_LOG);

//define message sent to shell when language changes (so shel can update its own language) 
#define WM_LANGUAGE_CHANGE    32771

#define SUPPORTED_SKIN_MAJOR_VER 1
#define SUPPORTED_SKIN_MINOR_VER 1

#define DEFAULT_LOWRES_SKIN_STR "PNG320X240"
#define DEFAULT_HIGHRES_SKIN_STR "PNG640X480"
#define DEFAULT_HD720P_SKIN_STR "PNG1280X720"
#define DEFAULT_HD1080P_SKIN_STR "PNG1920X1080"
#define DEFAULT_ULTRAHD2K_SKIN_STR "PNG2560X1440"
#define DEFAULT_ULTRAHD4K_SKIN_STR "PNG3840X2160"
#define DEFAULT_ULTRAHD8K_SKIN_STR "PNG7680X4320"


#define BACKGROUND_IMAGE "bckg.png"
#define IMAGES_FOLDER "pics"

#define BACKGROUND_IMAGE_VERT   "backgroundV.png"
#define BACKGROUND_IMAGE_HORI   "backgroundH.png"


//--#define NO_HD_IMG


namespace MM {
namespace Logic {
using MM::Application;
using Logic::VideoManager;


///////////////////////////////////////////////////////////////////////////////////////////////
ResourceManager::ResourceManager()
: _skin_idx(-1),
  m_brescale_font(false),
  _curr_x(0),
  _curr_y(0),
  textPak(NULL),
  resPak(NULL), 
  _bhigh_res(false),
  _video_mode(0)
{
	PAKtextManager = new StringPAKSystemManager(),
	PAKresourceManager = new PAKfileSystemManager();
	_cacher = new SharedCache<ResKey, MM::GUI::Image>(this, false);
	InitLangMap();
}	

ResourceManager::~ResourceManager()
{
    styleMap.clear();
	if (PAKtextManager && textPak) {
		PAKtextManager->closePak(textPak);
	}
	if (PAKresourceManager && resPak) {
		PAKresourceManager->closePak(resPak);
	}
 	delete PAKtextManager; 
	delete PAKresourceManager;

	//cache hack, release back image
	ResKey key;
	key.id = 1000000;
	if(_cacher->isId(key))
		_cacher->freeId(key);

	delete _cacher;
}

std::string ResourceManager::LoadString(int StringID) {
    assert(textPak != NULL);
    return PAKtextManager->extractText(StringID, textPak);
}

std::string ResourceManager::LoadStyle(int StringID) {
	std::string retval;	

    style_map_t::const_iterator it = styleMap.find(StringID);
	if (it != styleMap.end()) 
	{
		retval = it->second;
		
        if(Application::Instance().GetVideoManager()->IsHires() && m_brescale_font) 
        {
            const float *lscltab = Application::Instance().GetResTypeCoeff(RES_FONTSCALE);
            float sclval = lscltab[_video_mode];
            std::string str = retval;
            size_t pos1,pos2;
            pos1 = str.find(",");
            pos1 = str.find(",",pos1+1);
            pos1 = str.find(",",pos1+1);
            pos2 = str.find(",",pos1+1);

            std::string substr = str.substr(pos1+1,pos2-pos1-1);
            int val = ::Utils::str2Int<int>(substr); 
            val = (int)(float(val)*sclval);
            val = (val%2)?(val-1):(val);
            std::string cval = ::Utils::int2Str(val);

            retval = str.substr(0,pos1+1) + cval + str.substr(pos2);
        }
	}

    return retval; 
}

MM::GUI::ImageOwnPtr ResourceManager::LoadImage(int ImageID) {
	ResKey _key;

	if(ImageID == IDI_BG_DIALOG)
	{
		MM::Logic::OptionManager *optmgr = Application::Instance().GetOptionManager();
		if(optmgr->GetBoolValue(MM::Logic::BackgroundCategory,MM::Logic::BackgroundImageOpt))
		{
			_key.id = 1000000;
			if(!_cacher->isId(_key))
			{
				MM::Logic::VideoManager *vm = Application::Instance().GetVideoManager();
				BackImageChanged(vm->GetScreenWidth(), vm->GetScreenHeight());
			}

			if(_cacher->isId(_key))
			{
				MM::GUI::ImageOwnPtr bgimg = _cacher->getResource(_key);
				if(bgimg.ptr())
				{
					return bgimg;
				}
			}
		}
	}
	else if(ImageID == IDI_SKIN_BG_DIALOG)
	{
		ImageID = IDI_BG_DIALOG;
	}

	_key.id = ImageID;
	_key.dll = _skins[_skin_idx];
	_key.curr_res_id = _curr_res_id; //res_finder->GetResID();
	
	return _cacher->getResource(_key);
}


const std::vector<std::string> &ResourceManager::GetLanguages() {
	return lang_ids;
}

std::string ResourceManager::GetSkinName(int idx) const {
	if(idx < 0 || idx >= (int)_skins.size()) return "";
	return _skins[idx];
}
int	ResourceManager::GetSkinIdx(const std::string & name) const {
	std::string lcname = unicode::to_lower(name);
	for (int i = 0; i < (int)_skins.size(); i++) {
		if (unicode::to_lower(_skins[i]) == lcname) {
			return i;
		}
	}
	return -1;
}

bool ResourceManager::LoadSkin(const std::string & name)
{
	int idx = GetSkinIdx(name);
	if (idx == -1) {
		if(_skins.size() > 0) 
		{
			idx = 0;
		} else return false;
	}
	return LoadSkin(idx);
}

bool ResourceManager::LoadSkin(int idx)
{
	Application &app = Application::Instance();
	std::string appDir = app.GetDirectory();

	if(_skin_idx == idx) return true;
	if(resPak != NULL)
	{
		PAKresourceManager->closePak(resPak);
	}
	
	delete PAKresourceManager;
	resPak = NULL;
	
	if(_skins.size() == 0 || (int)_skins.size() - 1 < idx) return false;
	PAKresourceManager = new PAKfileSystemManager();
	filesystem::path pakfile = filesystem::path(appDir) / "skins" / _skins[idx];
	resPak = PAKresourceManager->openPak(pakfile.string().c_str());
	
	assert (resPak != NULL);
    if(resPak == NULL)
        return false;

    if(!IsSkinVerSupported())
        return false;

	_skin_idx = idx;
    if(resPak != NULL) 
    {
        FindSkinSizes();
        return true;
    }
	/*
	if(resPak != NULL)
	{
		// ************** put styles into map ********************
		unsigned int size = 0;
		char* tok = NULL;
//				wchar_t* tok2 = NULL;
//				wchar_t* tmp = NULL;
		size_t found = 0;
		size_t len = 0;
		std::string id_s;
		std::string style_s;
		int id_m = 0;
		
        styleMap.clear();
		unsigned char *styles_buf = PAKresourceManager->extractMem("\\styles",resPak,&size);
		char* m_styles = reinterpret_cast<char *>(styles_buf);

		for(tok = strtok(m_styles,"\n"); tok; tok = strtok(0,"\n"))
		{
			std::string temp(tok);
			len = temp.length();
			found = temp.find_first_of(":");
			
			if(found > 256)
            {
				continue;
            }
			else
            {
				id_s.assign(temp, 0, found);
				style_s.assign(temp, found+1, len - found);
				id_m = strtol(id_s.c_str(),NULL,10);
				styleMap.insert(make_pair( id_m, style_s ) );			
			}

		}
		free(m_styles);
        FindSkinSizes();
		// *****************************************************
		return true;
	}
    */

    return false;
}

bool ResourceManager::LoadGUIFontStyles(const std::string &fntfile)
{
    if(resPak == NULL)
        return false;

    unsigned int size = 0;

    char* tok = NULL;
	size_t found = 0;
	size_t len = 0;
	std::string id_s;
	std::string style_s;
	int id_m = 0;

    unsigned char *styles_buf = PAKresourceManager->extractMem(fntfile.c_str(),resPak,&size);
    char* cstyles = reinterpret_cast<char *>(styles_buf);
    if(!size)
        return false;

    float dpix,dpiy;
    VideoManager *vm = Application::Instance().GetVideoManager();
    vm->GetDPI(dpix,dpiy);

    styleMap.clear();
    for(tok = strtok(cstyles,"\n"); tok; tok = strtok(0,"\n"))
	{
		std::string temp(tok);
		len = temp.length();
		found = temp.find_first_of(":");
		
		if(found > 256)
			continue;

		id_s.assign(temp, 0, found);
		style_s.assign(temp, found+1, len - found);

        if(style_s.find_first_of("Mm")!=std::string::npos)
            style_s = ConvmmToPix(style_s,dpiy);

		id_m = strtol(id_s.c_str(),NULL,10);
		styleMap.insert(make_pair( id_m, style_s ) );			
	}
    free(styles_buf);

    return true;
}

std::string ResourceManager::ConvmmToPix(const std::string &style, float ydpi)
{
    size_t mpos = style.find_first_of("Mm");
    if(mpos == std::string::npos)
        return style;

    size_t cpos = style.rfind(',',mpos);
    if(cpos == std::string::npos)
        return style;
    size_t cpos2 = style.find(',',mpos);

    std::string strval = style.substr(cpos+1,mpos-cpos-1);
    float fvalmm = ::Utils::str2Float(strval);
    const float cl = 25.4f;
    float fpix = fvalmm*ydpi/cl;
    fpix += 0.5f;
    int pix = (int)fpix;
    std::string strpix = ::Utils::int2Str(pix);

    std::string outstyle = style.substr(0,cpos+1);
    outstyle += strpix;
    outstyle += style.substr(cpos2);

    return outstyle;
}

void ResourceManager::scanSkinDlls()
{
	Application &app = Application::Instance();
	filesystem::path appDir = filesystem::path(app.GetDirectory()) / "skins";
	filesystem::directory_list dl;
	filesystem::find(appDir, "*.pak", dl);
	for (filesystem::directory_list::const_iterator it = dl.begin(); it != dl.end(); ++it) {
		filesystem::path fname = appDir / (*it).name;
		resPak = PAKresourceManager->openPak(fname.string().c_str());
		// TODO: is strcmp valid here?
		if (resPak && strcmp(resPak->header.head,"PACK") && IsSkinVerSupported()) {
			_skins.push_back((*it).name);
		}
        PAKresourceManager->closePak(resPak);
	}
	resPak = 0;
}

bool ResourceManager::IsSkinVerSupported()
{
    assert(PAKresourceManager!=NULL);
    assert(resPak != NULL);

    //Check skin version
    unsigned int major_ver = PAKresourceManager->getMajorVersion(resPak);
    unsigned int minor_ver = PAKresourceManager->getMinorVersion(resPak);

    bool supported = (major_ver == SUPPORTED_SKIN_MAJOR_VER) && (minor_ver == SUPPORTED_SKIN_MINOR_VER);

    return supported;
}


void ResourceManager::InitLangMap()
{
    _lang_id_map["pl"] = "pl_PL";
    _lang_id_map["en"] = "en_GB";
    _lang_id_map["us"] = "en_GB";
    _lang_id_map["ru"] = "ru_RU";
    _lang_id_map["de"] = "de_DE";
    _lang_id_map["sk"] = "sk_SK";
    _lang_id_map["cz"] = "cs_CZ";
}


bool ResourceManager::SetLanguage(const std::string LangID) {
	if (LangID == curr_lang_id) {
		// no lang change
		return true;
	}

    std::string strlang = LangID;

#if defined (__ANDROID__) || defined (__APPLE__)
    if(strlang == "AUTO")
    {
        strlang = "en_GB";
#if defined (__ANDROID__)
        std::string key = hardware_defs.language;
#else
        std::string key = hd_defs.language;
#endif
        if(_lang_id_map.find(key) != _lang_id_map.end())
        {
            std::string lang = _lang_id_map[key];
            for(size_t i = 0; i < lang_ids.size(); i++)
            {
                if(lang_ids[i] == lang)
                {
                    strlang = lang;
                    break;
                }
            }
        }
    }
#else
    if(strlang == "AUTO")
        strlang = "en_GB";
#endif


    for(size_t i = 0; i < lang_ids.size(); i++)
    {
        if(lang_ids[i] == strlang)
        {
            if(textPak != NULL)
            {
                PAKtextManager->closePak(textPak);        
            }
            delete PAKtextManager;
			textPak = NULL;           

            PAKtextManager = new StringPAKSystemManager();
            textPak = PAKtextManager->openPak(lang_infos[i].lang_dll.c_str());

            if (textPak != NULL) 
            {
				curr_lang_id = strlang;
				// notify listeners
				FireLanguageChanged();
				Application::Instance().GetOptionManager()->SetValue(MM::Logic::LangCategory, MM::Logic::UILanguageOpt, strlang);
                Application::Instance().GetAppWindow()->ReloadResources();
            } 
            else 
            {
			    curr_lang_id = "";
			    return false;
            }
			
            return true;  

        }
    }

    return false; // lang unknown

    
}


const LangInfo &ResourceManager::GetLanguageInfo(const std::string &LangID) {
	const static LangInfo empty;
	for (int i = 0; i < (int)lang_ids.size(); i++) {
		if (lang_ids[i] == LangID) {
			return lang_infos[i];
		}
	}
	return empty;
}

bool ResourceManager::Init() {
	filesystem::path appdir(Application::Instance().GetDirectory());
	filesystem::directory_list dl;
	filesystem::find(appdir, "*.txp", dl);
	for (filesystem::directory_list::const_iterator it = dl.begin(); it != dl.end(); ++it) {
		filesystem::path fname = appdir / (*it).name;
		stringpakfile *textPak = PAKtextManager->openPak(fname.string().c_str());
		if (textPak) {
			std::string lang_id = PAKtextManager->pakLangID(textPak);	
			std::string lang_name = PAKtextManager->pakLangName(textPak);
			if (!lang_id.empty() && !lang_name.empty()) {
				LangInfo li;
				li.lang_dll = fname.string();
				li.lang_id = lang_id;
				li.lang_name = lang_name;
				lang_ids.push_back(li.lang_id);
				lang_infos.push_back(li);
			}
			PAKtextManager->closePak(textPak);
		}
	}
	if (lang_ids.size() == 0)
		return false;
	scanSkinDlls();
	return true;
}

void ResourceManager::AddLanguageChangeListener(LanguageChangeListener *o) {
	LangListeners::iterator i =
		std::find(_listeners.begin(), _listeners.end(), o);
	if (i == _listeners.end()) {
		_listeners.push_back(o);
	}
}

void ResourceManager::RemoveLanguageChangeListener(LanguageChangeListener *o) {
	LangListeners::iterator i =
		std::find(_listeners.begin(), _listeners.end(), o);
	if (i != _listeners.end()) {
		_listeners.erase(i);
	}
}

void ResourceManager::FireLanguageChanged() {
	for (LangListeners::iterator i = _listeners.begin(); i != _listeners.end(); i++) {
		(*i)->LanguageChanged(curr_lang_id);
	}
}

void ResourceManager::BackImageChanged(int x, int y)
{
    MM::GUI::ImageOwnPtr p = LoadBgImage(x,y);

	if(p) 
    {
		ResKey key;
		key.id = 1000000;	
		_cacher->insertResource(key, p.take());
	}
    else
    {
        //--OptionManager *opt = Application::Instance().GetOptionManager();
		//--opt->SetBoolValue(MM::Logic::BackgroundCategory,MM::Logic::BackgroundImageOpt,false);
        DeleteBackgroundFiles();
    }
}

MM::GUI::ImageOwnPtr ResourceManager::LoadBgImage(int x, int y) 
{
    std::string fp(MM::Application::Instance().GetCacheAppDirectory());
	if (x < y)
		fp.append(BACKGROUND_IMAGE_VERT);
	else
		fp.append(BACKGROUND_IMAGE_HORI);
    MM::GUI::Point imgres(x,y);
	MM::GUI::ImageOwnPtr bgimg(MM::GUI::Image::Load(fp,&imgres));

    return bgimg;
}


void ResourceManager::CreateBackgroundImages(int x, int y)
{
    int width = (x > y) ? (x) : (y);
    int height = (x > y) ? (y) : (x);

    std::string orig_filename(BACKGROUND_IMAGE);
    const char *dest_filename[][2] = {{BACKGROUND_IMAGE_HORI},{BACKGROUND_IMAGE_VERT}};

    std::string cdir(MM::Application::Instance().GetCacheAppDirectory());
    std::string cdirIn(MM::Application::Instance().GetDirectory());

    /////////////////////
    bool bexist1 = filesystem::exists(filesystem::path(cdir+BACKGROUND_IMAGE_HORI));
    bool bexist2 = filesystem::exists(filesystem::path(cdir+BACKGROUND_IMAGE_VERT));
    bool bexist3 = filesystem::exists(filesystem::path(cdirIn) / IMAGES_FOLDER / BACKGROUND_IMAGE);

    if(bexist1 && bexist2) return ;
    if(!bexist3) return ;
    /////////////////////

    for(int i = 0; i < 2; i++) 
    {
        filesystem::path fpath = filesystem::path(IMAGES_FOLDER) / BACKGROUND_IMAGE;
        std::string filename = cdirIn + fpath.string();

        MM::GUI::ImageLoadStruct ld;
        ld.dstSize = MM::GUI::Point(width, height);
        ld.higherRes = true;

        MM::GUI::Image::LoadInterImg(filename,ld);
        if (!ld.interImage) {
            break;
        }

        //Scaling Image
        MM::GUI::ImageOwnPtr & interImg = ld.interImage;
		unsigned *interbits = interImg->ImageBits();
		int srcW = interImg->GetWidth();
		int srcH = interImg->GetHeight();

        MM::GUI::Point rsmpSize = MM::GUI::Image::CalcFitImgSize(srcW,srcH,ld.dstSize.x,ld.dstSize.y, true);

        int bs = rsmpSize.x * rsmpSize.y * sizeof(unsigned);
		memory::buffer mb(bs, memory::allocator_pool::instance().for_size(bs));
		unsigned *imgbits = static_cast<unsigned *>(mb.ptr());

        MM::GUI::Image::LinearScaleFixed(srcW,srcH,interbits,rsmpSize.x,rsmpSize.y,imgbits);

		MM::GUI::ImageOwnPtr fimage(new MM::GUI::Image(width,height,MM::GUI::NO_TRANSPARENCY),true);

		unsigned *fbits = fimage->ImageBits();
        memset(fbits,0,width*height*sizeof(unsigned int));

        int ptroffx = (rsmpSize.x >= width) ? (rsmpSize.x - width) : (0);
        int ptroffy = (rsmpSize.y >= height) ? (rsmpSize.y - height) : (0);

        unsigned *imgdata = imgbits + (ptroffy>>1)*rsmpSize.x + (ptroffx>>1);
        unsigned *dstdata = fbits;
        for(int x = 0; x < height; x++)
        {
            memcpy(dstdata,imgdata,width*sizeof(unsigned));
            dstdata += width;
            imgdata += rsmpSize.x;
        }

        std::string bgfilename = cdir;
        bgfilename.append(*dest_filename[i]);
        fimage->Save(bgfilename,"");

		// swap dimensions to generate next image
        int tmp = width;
		width = height;
        height = tmp;

		MM::GUI::Image::ReleaseInfoStruct(filename,ld);
    }

    return ;
}

void ResourceManager::DeleteBackgroundFiles()
{
	filesystem::path dir(MM::Application::Instance().GetCacheAppDirectory());
	filesystem::remove(dir / BACKGROUND_IMAGE_HORI);
    filesystem::remove(dir / BACKGROUND_IMAGE_VERT);
}

MM::GUI::Image *ResourceManager::getObject(const ResKey &k)
{
	assert(resPak != NULL);
	char tempID[512];
	unsigned int size = 0;
    int rescale = 0;
	
    std::string kid = ::Utils::int2Str(k.id);
    std::string resid = _curr_res_id + "\\" + kid + ".PNG"; 

    PAKresourceManager->fileCheckout(resid.c_str(), resPak, &size); 
    

    static const char *def_res_skin[][RES_COUNT] = 
    {
        {DEFAULT_LOWRES_SKIN_STR},
        {DEFAULT_HIGHRES_SKIN_STR},
        {DEFAULT_HD720P_SKIN_STR},
        {DEFAULT_HD1080P_SKIN_STR},
        {DEFAULT_ULTRAHD2K_SKIN_STR},
        {DEFAULT_ULTRAHD4K_SKIN_STR},
        {DEFAULT_ULTRAHD8K_SKIN_STR},
    };

    if(size == 0)
    {
        int rescnt = RES_COUNT;
#ifdef NO_HD_IMG
        if(_video_mode != VM_HD720P) {//na razie hack, zeby dla HD nie szukal w wyzszych rozdzielczosciach
#endif //NO_HD_IMG
        for(int i = _video_mode; i < rescnt; i++) 
        {
            std::string resid = *def_res_skin[i];
            resid = resid + "\\" + kid + ".PNG";
            PAKresourceManager->fileCheckout(resid.c_str(), resPak, &size);
            if(size!=0) break;
            rescale -= 1;
        }
#ifdef NO_HD_IMG
        }
#endif //NO_HD_IMG
    }
    if(size == 0)
    {
        rescale = 0;
        for(int i = _video_mode; i >= 0; i--)
        {
            std::string resid = *def_res_skin[i];
            resid = resid + "\\" + kid + ".PNG";
            PAKresourceManager->fileCheckout(resid.c_str(), resPak, &size);
            if(size!=0) break;
            rescale += 1;
        }
    }

    //Couldn't find proper image
    if(size==0) 
        return NULL;
    
    memory::buffer buffer(size, memory::allocator_pool::instance().for_size(size));
	if(buffer.ptr() == NULL)
		return NULL;
	bool res = PAKresourceManager->extractMemToBuffer(tempID, resPak, (byte*)buffer.ptr());
    //Couldn't extract data from pak file
    if(!res)
       return NULL;

    MM::GUI::ImageOwnPtr imgptr = MM::GUI::Image::Load(buffer.ptr(), size);
	//Failed load image
    if(imgptr.ptr() == NULL)
		return NULL;

    if(rescale>=0) //!rescale
        return imgptr.take();

    buffer.release();

    int width = imgptr->GetWidth();
    int height = imgptr->GetHeight();
  
    MM::GUI::Image *image = NULL;
    if(rescale < 0)
    {
        rescale = -rescale;

        int tsize = (width>>1)*height*sizeof(unsigned);
        memory::buffer membuf(tsize, memory::allocator_pool::instance().for_size(tsize));
        unsigned *bufptr1 = (unsigned*)membuf.ptr();
        unsigned *bufptr2 = (unsigned*)membuf.ptr();
        bufptr2 += (height>>1);

        //first pass
        MM::GUI::Image::ScaleMinifyByTwo(width,height,imgptr->ImageBits(),bufptr1);
        imgptr.release();
        int sclwidth = width >> 1;
        int sclheight = height >> 1;
        if(sclwidth==0 || sclheight==0) 
            return NULL;
        //next passes
        for(int i = 1; i < rescale; i++) 
        {
            MM::GUI::Image::ScaleMinifyByTwo(sclwidth,sclheight,bufptr1,bufptr2);
            sclwidth>>=1;
            sclheight>>=1;
            if(sclwidth==0 || sclheight==0) 
                return NULL;
            memcpy(bufptr1,bufptr2,sclwidth*sclheight*sizeof(unsigned));
        }
        //creating image
        image = new MM::GUI::Image(sclwidth,sclheight);
        unsigned int *data = image->ImageBits();
        memcpy(data,bufptr1,sclwidth*sclheight*sizeof(unsigned int));
        membuf.release();
    }

    return image;
}

void ResourceManager::SetResolution(int x, int y)
{
    int tab[2]={0};
    FindAvailableSkinRes(tab,x,y);

    std::string svalx = ::Utils::int2Str(tab[0]);
    std::string svaly = ::Utils::int2Str(tab[1]);
    _curr_res_id = "PNG" + svalx + "X" + svaly;

    _curr_x = x;
    _curr_y = y;

    _bhigh_res = Application::Instance().GetVideoManager()->IsHires();
    ////////
    VideoMode vidmode = Application::Instance().GetVideoManager()->GetVideoMode();
    _video_mode = vidmode;
    ////////
    BackImageChanged(x,y);

    m_brescale_font = false;
    std::string fntstylefile = "\\fontstyles_" + svalx + "x" + svaly;
    bool bres = LoadGUIFontStyles(fntstylefile);
    m_brescale_font = (tab[0] < 100);
    if(!bres) {
        LoadGUIFontStyles("\\styles");
        m_brescale_font = true;
    }
    
}


bool ResourceManager::IsLayoutAvailable(int ID)
{
    if(resPak==NULL)
        return false;

    char fileid[128]={0};
    sprintf(fileid,"Layout\\%d",ID);
    unsigned int size = 0;
    int res = PAKresourceManager->fileCheckout(fileid, resPak, &size);

    return res == 1;
}


MM::GUI::LayoutReader * ResourceManager::LoadLayout(int ID)
{
    if(resPak==NULL)
        return 0;

	//assert(resPak != NULL);
	char tmpid[128];	
	sprintf(tmpid,"Layout\\%d",ID);
	
	unsigned int size = 0;
	unsigned char* buffer = PAKresourceManager->extractMem(tmpid, resPak, &size);
	
	if(buffer != NULL)
	{
		MM::GUI::LayoutReader *pRet = new MM::GUI::LayoutReader(buffer,size);
		free(buffer);
		return pRet;
	}
	
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////
void ResourceManager::FindSkinSizes()
{
    resMap.clear();
    resMap[0][0] = -1;
    
    if(IsLayoutAvailable(IDR_240x320))
        resMap[240][320] = IDR_240x320;
    if(IsLayoutAvailable(IDR_320x240))
        resMap[320][240] = IDR_320x240;

    if(IsLayoutAvailable(IDR_400x240))
        resMap[400][240] = IDR_400x240;
    if(IsLayoutAvailable(IDR_240x400))
        resMap[240][400] = IDR_240x400;

    if(IsLayoutAvailable(IDR_480x234))
        resMap[480][234] = IDR_480x234;


    if(IsLayoutAvailable(IDR_640x480))
        resMap[640][480] = IDR_640x480;
    if(IsLayoutAvailable(IDR_480x640))
        resMap[480][640] = IDR_480x640;
    
    if(IsLayoutAvailable(IDR_800x400))
        resMap[800][400] = IDR_800x400;
    if(IsLayoutAvailable(IDR_480x764))
        resMap[480][764] = IDR_480x764;

    if(IsLayoutAvailable(IDR_800x480))
        resMap[800][480] = IDR_800x480;
    if(IsLayoutAvailable(IDR_480x800))
        resMap[480][800] = IDR_480x800;

    if(IsLayoutAvailable(IDR_960x640))
        resMap[960][640] = IDR_960x640;
    if(IsLayoutAvailable(IDR_640x960))
        resMap[640][960] = IDR_640x960;
    
    if(IsLayoutAvailable(IDR_1024x768))
        resMap[1024][768] = IDR_1024x768;
    if(IsLayoutAvailable(IDR_768x1024))
        resMap[768][1024] = IDR_768x1024;

    if(IsLayoutAvailable(IDR_1136x640))
        resMap[1136][640] = IDR_1136x640;
    if(IsLayoutAvailable(IDR_640x1136))
        resMap[640][1136] = IDR_640x1136;

    if(IsLayoutAvailable(IDR_1200x700))
        resMap[1200][700] = IDR_1200x700;
    if(IsLayoutAvailable(IDR_700x1200))
        resMap[700][1200] = IDR_700x1200;


    if(IsLayoutAvailable(IDR_1600x900))
        resMap[1600][900] = IDR_1600x900;
    if(IsLayoutAvailable(IDR_900x1600))
        resMap[900][1600] = IDR_900x1600;

    if(IsLayoutAvailable(IDR_1334x750))
        resMap[1334][750] = IDR_1334x750;
    if(IsLayoutAvailable(IDR_750x1334))
        resMap[750][1334] = IDR_750x1334;

    if(IsLayoutAvailable(IDR_2048x1536))
        resMap[2048][1536] = IDR_2048x1536;
    if(IsLayoutAvailable(IDR_1536x2048))
        resMap[1536][2048] = IDR_1536x2048;

    if(IsLayoutAvailable(IDR_2560x1440))
        resMap[2560][1440] = IDR_2560x1440;
    if(IsLayoutAvailable(IDR_1440x2560))
        resMap[1440][2560] = IDR_1440x2560;

    //screen ratio layouts
    if(IsLayoutAvailable(IDR_4x3))
        resMap[4][3] = IDR_4x3;
    if(IsLayoutAvailable(IDR_3x4))
        resMap[3][4] = IDR_3x4;

    if(IsLayoutAvailable(IDR_16x9))
        resMap[16][9] = IDR_16x9;
    if(IsLayoutAvailable(IDR_9x16))
        resMap[9][16] = IDR_9x16;

    if(IsLayoutAvailable(IDR_16x10))
        resMap[16][10] = IDR_16x10;
    if(IsLayoutAvailable(IDR_10x16))
        resMap[10][16] = IDR_10x16;
}

////////////////////////////////////////////////////////////////////////////////////////////
int ResourceManager::GetAvailableSkinID(int x, int y)
{
    if(resMap.empty())
        return -1;

    int tab[2]={0};
    FindAvailableSkinRes(tab,x,y);
    int newx = tab[0];
    int newy = tab[1];

    int id = resMap[newx][newy];

    return id;
}

////////////////////////////////////////////////////////////////////////////////////////////
void ResourceManager::FindAvailableSkinRes(int (&res)[2], int x, int y)
{
    bool vert_res = y > x;

    if(x<1 || y<1) return;

    //find the nearest resolution in map
    int newx = 0;
    int newy = 0;

    res_map_t::const_iterator ix = resMap.begin();
    while(ix != resMap.end())
    {
        int_map_t::const_iterator iy = ix->second.begin();
        while(iy != ix->second.end())
        {
            bool bvert = (*iy).first > (*ix).first;
            if(vert_res == bvert)
            {
                if( ((*ix).first >= newx) && ((*ix).first <= x)  )
                {
                    if( ((*iy).first >= newy) && ((*iy).first <= y) )
                    {
                        newx = (*ix).first;
                        newy = (*iy).first;
                    }
                }
            }
            iy++;
        }
        ix++;
    }

    res[0] = newx;
    res[1] = newy;

    //searching aspect ratio
    if(res[0] != x && res[1] != y)
    {
        //find aspect ratio
        float fx = (float)x;
        float fy = (float)y;
        float aspect = (vert_res) ? (fy/fx) : (fx/fy);
        const float ratio_tab[] = {4.f/3.f, 16.f/9.f, 16.f/10.f};

        int idx = 0;
        float min_delta = 2.f;
        for(int i = 0; i < 3; i++) {
            float delta = aspect - ratio_tab[i];
            delta = (delta < 0.f) ? (-delta) : (delta);
            if(delta < min_delta) {
                min_delta = delta;
                idx = i;
            }
        }

        const int aspect_vals_tab[3][2] = {{4,3},{16,9},{16,10}};
        int xvalue = aspect_vals_tab[idx][0];
        int yvalue = aspect_vals_tab[idx][1];

        if(vert_res) {
            xvalue = aspect_vals_tab[idx][1];
            yvalue = aspect_vals_tab[idx][0];
        }
        
        res_map_t::const_iterator ix = resMap.find(xvalue);
        if(ix != resMap.end()) {
            res_map_t::const_iterator iy = resMap.find(yvalue);
            if(iy != resMap.end()) {
                res[0] = xvalue;
                res[1] = yvalue;
            }
        }
    }

}


#if defined SYNC_LANG_WITH_SYSTEM

struct win32_lang_entry {
const wchar_t * name;
LCID			id;
};

const win32_lang_entry win32_lang_map[] = {
	{L"pl_PL", 0x0415},
	{L"en_GB", 0x0409},	// US de facto
	{L"en_GB", 0x0809},	// wlasciwe en_GB
	{L"de_DE", 0x0407},
	{L"ru_RU", 0x0419}
};

const wchar_t * win32_lang_find_name(LCID id) {
	for (int i = 0; i < sizeof(win32_lang_map) / sizeof(win32_lang_entry); ++i) {
		if (win32_lang_map[i].id == id) {
			return win32_lang_map[i].name;
		}
	}
	return 0;
}

LCID win32_lang_find_id(const wchar_t *name) {
	for (int i = 0; i < sizeof(win32_lang_map) / sizeof(win32_lang_entry); ++i) {
		if (wcscmp(win32_lang_map[i].name, name) == 0) {
			return win32_lang_map[i].id;
		}
	}
	return 0;
}
#endif


} // namespace Logic
} // namespace MM

