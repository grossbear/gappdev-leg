#include "stdafx.h"
#include "VoiceManager.h"
#include "../Application.h"
#include "libmap2/navi/NavigationInfo.h"
#include <stdio.h>
#include "utils/strtools.h"
#include "utils/fs/filesystem.h"
#include "utils/unicode/ctypes.h"
#include "utils/ini/ini_parser.h"
#include "utils/utils.h"
#include "MM/Logic/GpsManager.h"
#include "MM/Logic/OptionManager.h"
#include "MapDirectoryLocation.h"
#include "MM/Utils/Library.h"
#include "../../Platform/Registry.h"
#include "../../Platform/PlatformInfo.h"
#include "build/build_config.h"
#include "logger/logger_manager.h"
LOGGER(lg, "VOICE");
#ifdef __ANDROID__
#include "jni_mm.h"
#include "hardware_defs.h"
#include "../Utils/sys/SoundPlayerAndroid.h"

#include "android/log.h"

#include "MM/Logic/ResourceManager.h"

#define MANVOICE_KEYNAME "man"
#define WOMVOICE_KEYNAME "woman"
#define TWO_VOICES_LANG "pl_PL"

#endif

#ifdef __APPLE__
#include "ios_mm.h"
#include "hardware_defs.h"
#include "../Utils/sys/iPhoneSoundPlayer.h"
#endif
#if defined (_WIN32)
#include "../Utils/sys/SoundPlayerW32.h"
#endif

#if defined (_WIN32_WCE)
#include "deviceidentify.h"
using namespace DeviceIdentyfication;
#endif

#define MIN_SPEAK_SPEED 5

namespace MM {
namespace Logic {

// helper struct that defines static manouver definition
// turn2 encoding determines manouver type:
// NONE - single turn manouver
// PAST_END - not a manouver, indicates special voice prompt
// otherwise - joined turn manouver
struct snd_entry_t {
	const char *	file;
	int				turn1;
	int				turn2;
	int				dist;
};

extern const snd_entry_t snd_entries[];

VoiceManager::VoiceManager()
:	_voice_id_filename("voice-id.txt"),
	_prev_accept(0),
	_prev_d(0),
	_prev_l(0),
	_init_vol(0),
#ifdef __ANDROID__
	_curr_vol(hardware_defs.currvol)
#else
	_curr_vol(0)
#endif

{
#if defined (_WIN32_WCE)
	_devident=NULL;
	_rvolume=NULL;
	InitVol();
	if (_devident!=NULL && _rvolume!=NULL){
		int volume = _rvolume->Get();
		if(volume>-1) _init_vol = (unsigned int) volume; 
	}
#elif defined (__APPLE__)
    _curr_vol = ios_get_current_volume();
#endif	
	OptionManager *om = Application::Instance().GetOptionManager();
	
#if !defined( __ANDROID__) && !defined (__APPLE__)
		SetVolume(om->GetIntValue(SoundVideoCategory, VoiceVolumeOpt));
#endif
	_params.dist_treshold = om->GetIntValue(NavigationCategory, VoiceDistanceThresholdOpt);
	_params.join_msg_dist = om->GetIntValue(NavigationCategory, VoiceJoinMessageDistanceOpt);
	_params.join_now_msg_dist = om->GetIntValue(NavigationCategory, VoiceJoinMessageDistanceSmallOpt);

#if defined (_WIN32_WCE) || defined (_WIN32)
	_soundPlayer = new SoundPlayerW32();
#elif defined (__ANDROID__)
	_soundPlayer = new MM::SoundPlayerAndroid();
#elif defined (__APPLE__)
	_soundPlayer = new MM::iPhoneSoundPlayer();
#endif		

	if(!om->GetBoolValue(MM::Logic::SoundVideoCategory, MM::Logic::VoiceScreenClickEnabledOpt))
	{
			_init_click_vol = GetScreenClickVolume();
			SetScreenClickVolume(0);
	}
	Init();
}

VoiceManager::~VoiceManager() {
	//::waveOutSetVolume(NULL, _init_vol);
#if defined (_WIN32_WCE)
	if (_devident!=NULL && _rvolume!=NULL){
		_rvolume->Set((int)_init_vol);
	}
	if(!Application::Instance().GetOptionManager()->GetBoolValue(MM::Logic::SoundVideoCategory, MM::Logic::VoiceScreenClickEnabledOpt))
	{
		SetScreenClickVolume(_init_click_vol);
	}
#endif
	if(_soundPlayer)
		delete _soundPlayer;

}	

std::string VoiceManager::GetVoiceName(int num) const {
	string ret;
	if (num >= 0 && num < GetVoiceCount()) {
		ret.append(_voice_names[num]);
	}
	return ret;
}

bool VoiceManager::SelectVoice(const std::string &voicename) {
	// none was selected?
	if (voicename.empty()) {
		_selected_dir = _selected = voicename;
		return true;
	}
	// one of exisiting voices?
	for (int i = 0; i < GetVoiceCount(); i++) {
		if (voicename == _voice_names[i]) {
			_selected = voicename;
			_selected_dir = _voice_dirs[i];
			return true;
		}
	}
	return false;
}

bool VoiceManager::SelectVoiceByDir(const std::string &voicedir) {
	// none was selected?
	if (voicedir.empty()) {
		_selected_dir = _selected = voicedir;
		return true;
	}
	// one of exisiting voices?
	for (int i = 0; i < GetVoiceCount(); i++) {
		if (voicedir == _voice_dirs[i]) {
			_selected = _voice_names[i];
			_selected_dir = voicedir;
			return true;
		}
	}
	return false;
}

void VoiceManager::Add(const std::string &voice_name, int Turn, int Dist) {
	_single[Turn][Dist] = voice_name;
}

void VoiceManager::Add(const std::string &voice_name, int Turn, int NextTurn, int Dist){
	_joined[Turn][NextTurn][Dist] = voice_name;
}

void VoiceManager::Add(const std::string &voice_name, SpecialVoice special) {
	_special[special] = voice_name;
}

std::string VoiceManager::GetVoice(int Turn, int Dist) const {
	string ret;
	TurnMap::const_iterator turn = _single.find(Turn);
	if (turn != _single.end()) {
		DistMap::const_iterator dist = turn->second.find(Dist);
		if (dist != turn->second.end()) {
			// got msg
			filesystem::path p = filesystem::path(MainVoiceDirectory()) / _selected_dir / dist->second;
			ret.append(p.string());
		}
	}
	return ret;
}

std::string VoiceManager::GetVoice(int Turn, int NextTurn, int Dist) const {
	string ret;
	JoinTurnMap::const_iterator turn = _joined.find(Turn);
	if (turn != _joined.end()) {
		TurnMap::const_iterator nturn = turn->second.find(NextTurn);
		if (nturn != turn->second.end()) {
			DistMap::const_iterator dist = nturn->second.find(Dist);
			if (dist != nturn->second.end()) {
				// got msg
				filesystem::path p = filesystem::path(MainVoiceDirectory()) / _selected_dir / dist->second;
				ret.assign(p.string());
			}
		}
	}
	return ret;
}

std::string VoiceManager::GetVoice(SpecialVoice special) const {
	std::string ret;
	SpecialMap::const_iterator i = _special.find(special);
	if (i != _special.end()) {
		// got msg
		filesystem::path p = filesystem::path(MainVoiceDirectory()) / _selected_dir / i->second;
		ret.append(p.string());
	}
	return ret;
}

std::string VoiceManager::GetDistanceVoice(int Dist) const
{
	const int max_dist = 2050;
	const int mul = 100;
	const int dx = 49;
	if(Dist > max_dist)
		return "";
	std::string ret  = "in_" + ::Utils::int2Str(((Dist+dx)/mul)*mul);	
	filesystem::path p = filesystem::path(MainVoiceDirectory()) / _selected_dir / ret;
	ret = p.string();
	return ret;
}

std::string VoiceManager::GetPoiVoice(int ico_id, int voice_id) const
{
	std::string ret;
	return ret;
}

void VoiceManager::InitPoiSounds()
{
	
}

bool VoiceManager::SelectVoiceLanguage(const std::string &lang_id) {

#if defined ( __ANDROID__)
	return SelectVoiceLanguageAndroid(lang_id);
#endif

	_voice_names.clear();
	_voice_dirs.clear();
	bool found = false;
	// locate main voice directory
	std::string lang_dir(lang_id.substr(0, 5));
	filesystem::directory_list dl;
	
#if defined ( __APPLE__)
    filesystem::find( filesystem::path(hd_defs.iosappPath), "*", dl );
#else
	filesystem::find(filesystem::root(), "*", dl);
#endif
	for (filesystem::directory_iterator it = dl.begin(); it != dl.end(); ++it) {

#if defined ( __APPLE__)
		filesystem::path rdir;
        if( (*it).name != "." && (*it).name != "..")
//            rdir = filesystem::path("/User/Media/Downloads") / (*it).name;
            rdir = filesystem::path(hd_defs.iosappPath) / (*it).name;
        else continue;
#else
		filesystem::path rdir = filesystem::root() / (*it).name;
#endif
#if defined(__WINPC__)
		if (filesystem::is_directory(rdir) && (*it).name.compare(0, 2, "SD") == 0)
#else
		if (filesystem::is_directory(rdir))
#endif
		{
#if defined (__APPLE__)
            filesystem::path ldir = rdir / "komunikaty" / lang_dir / "/";
#else
            filesystem::path ldir = rdir / MAP_DIRECTORY_BASE / "komunikaty" / lang_dir / "/";
#endif

			if (filesystem::is_directory(ldir)) {
				_main_dir = ldir.string();
#if defined ( __APPLE__)
                _base_dir = (rdir / "komunikaty").string();
#else                
				_base_dir = (rdir / MAP_DIRECTORY_BASE / "komunikaty").string();
#endif
				// locate voices
                typedef std::map<std::string, std::string> dir_map_type;
				dir_map_type e_dirs;

				filesystem::directory_list dl2;
				filesystem::find(ldir, "*", dl2);
				for (filesystem::directory_iterator it2 = dl2.begin(); it2 != dl2.end(); ++it2) {
					if (filesystem::is_directory((*it2).status) && (*it2).name[0] != '.') {
                        // to lowercase
						std::string fname = unicode::to_lower((*it2).name);
                        e_dirs.insert(std::make_pair(fname, fname));
					}
				}
				if (e_dirs.size() > 0) found = true;
				// scan for voice id
                ini::ini_types::option_list_type list_type;
                ini::ini_parser parser(true);
				filesystem::path rstr = filesystem::path(ldir) / _voice_id_filename;

                int ret = parser.read(rstr.string().c_str());
				if (ret == ini::ipOk) {
					// got base voice id definition
                    typedef ini::ini_types::option_list_type list_type;
                    const list_type & l = parser.get_options();

					for (list_type::const_iterator it = l.begin(); it != l.end(); ++it) {
						std::string fval(it->first);
						dir_map_type::iterator it2 = e_dirs.find(fval);
						if (it2 != e_dirs.end()) {
							it2->second = it->second.value;
						}
					}
				}
				// scan for in-directory voice id file
				for (dir_map_type::iterator it = e_dirs.begin(); it != e_dirs.end(); ++it) {
					filesystem::path rstr = ldir / it->first / _voice_id_filename;
                    ret = parser.read(rstr.string().c_str());
					if (ret == ini::ipOk) {
                        ini::ini_types::option_list_type::const_iterator it2 = parser.get_options().begin();
                        if (it2 != parser.get_options().end())
							it->second = it2->second.value;
					}
				}

				// setup app list
				for (dir_map_type::iterator it = e_dirs.begin(); it != e_dirs.end(); ++it) {
					_voice_dirs.push_back(it->first);
					_voice_names.push_back(it->second);
				}
			}
		}
    }

	SetValidVoice();
	return found;
}
#if defined ( __ANDROID__)
bool VoiceManager::SelectVoiceLanguageAndroid(const std::string &lang_id) {
	_voice_names.clear();
	_voice_dirs.clear();
	// locate main voice directory

	std::string lang_dir(lang_id.substr(0, 5));

    filesystem::path ldir = filesystem::path(MAP_DIRECTORY_BASE) / "komunikaty" / lang_dir / "/";

	_main_dir = ldir.string();
    _base_dir = (filesystem::path(MAP_DIRECTORY_BASE) / "komunikaty").string();

	// locate voices
    typedef std::map<std::string, std::string> dir_map_type;
	dir_map_type e_dirs;

    ResourceManager *rm = Application::Instance().GetResourceManager();

    if(std::string(TWO_VOICES_LANG).find(lang_id) != std::string::npos)
    {
        std::string manstr = rm->LoadString(IDS_1980);
        std::string womstr = rm->LoadString(IDS_1981);

        e_dirs.insert(std::make_pair(MANVOICE_KEYNAME, manstr));
        e_dirs.insert(std::make_pair(WOMVOICE_KEYNAME, womstr));
    }
    else 
    {
        std::string womstr = rm->LoadString(IDS_1981);
        e_dirs.insert(std::make_pair(WOMVOICE_KEYNAME, womstr));
    }


	// setup app list
	for (dir_map_type::iterator it = e_dirs.begin(); it != e_dirs.end(); ++it) {
		_voice_dirs.push_back(it->first);
		_voice_names.push_back(it->second);
	}

	SetValidVoice();
	return true;
}
#endif

void VoiceManager::SetValidVoice(){
	// check if currently selected voice is still valid
	bool valid = false;
	for (int i = 0; i < GetVoiceCount(); i++) {
		if (_selected_dir == _voice_dirs[i]) {
			_selected = _voice_names[i];
			valid = true;
			break;
		}
	}
	if (!valid) {
		// if not, fallback to first
		if (GetVoiceCount() > 0) {
			_selected_dir = _voice_dirs[0];
			_selected = _voice_names[0];
		} else {
			// no voice avaliable for this language
			_selected_dir = _selected = EmptyVoice();
		}
	}

}

void VoiceManager::Init() {
	using namespace libmap2::navi;
	const snd_entry_t * p = snd_entries;
	while (p->file) {
		switch (p->turn2) {
		case PAST_END:
			Add(p->file, static_cast<SpecialVoice>(p->turn1));
			break;
		case NONE:
			Add(p->file, p->turn1, p->dist);
			break;
		default:
			Add(p->file, p->turn1, p->turn2, p->dist);
			break;
		}
		++p;
	}
}
#if defined (_WIN32_WCE)
void VoiceManager::InitVol(){
	_devident = Application::Instance().GetDevIdent();
	if(_devident!=NULL){ 
		DeviceIdentyfication::Device *dev = _devident->GetDevice(); 
		_rvolume = dev->GetRegulation(DeviceRegulation::rtVolume);
	}
}
#endif
void VoiceManager::Speak(const std::string &voice_file_path,MM::PriorityVoice pv) const {
	OptionManager *om = Application::Instance().GetOptionManager();
	// do not speak if voice is disabled
	if(om->GetIntValue(Logic::SoundVideoCategory,Logic::VoiceEnabledOpt) == 0) return;
    if (!voice_file_path.empty()) {
		filesystem::path p(voice_file_path);
		p.replace_ext("ogg");
#if !defined(__ANDROID__)
		if (!filesystem::exists(p)) {
			p.replace_ext("aac");
		}
        if (!filesystem::exists(p)) {
			p.replace_ext("wav");
		}
#endif //__ANDROID__
		VoicePaths voices;
		voices.push_back(p.string());
        if(_soundPlayer)
            _soundPlayer->Speak(voices,pv);
	}
}

void VoiceManager::Speak(const VoicePaths &voices,PriorityVoice pv) const {
	OptionManager *om = Application::Instance().GetOptionManager();
	// do not speak if voice is disabled
	if(om->GetIntValue(Logic::SoundVideoCategory,Logic::VoiceEnabledOpt) == 0) return;

	VoicePaths voi;
	for(size_t i=0; i<voices.size(); i++)
	{
        if (!voices[i].empty()) {
			filesystem::path p(voices[i]);
			p.replace_ext("ogg");
#if !defined (__ANDROID__)
            if (!filesystem::exists(p)) {
                p.replace_ext("aac");
            }
			if (!filesystem::exists(p))
				p.replace_ext("wav");
			if (!filesystem::exists(p)) 
				continue;
#endif //__ANDROID__
			voi.push_back(p.string());
		}
	}
	if(_soundPlayer && !voi.empty())
		_soundPlayer->Speak(voi,pv);	
}

void VoiceManager::SpeakTest() const {
    std::string basevoicedir = BaseVoiceDirectory();
    filesystem::path _test = filesystem::path(basevoicedir) / "default" / "poi" / "volume";
	_test.replace_ext("ogg");
#if !defined(__ANDROID__)
    if (!filesystem::exists(_test)) _test.replace_ext("aac");
	if (!filesystem::exists(_test)) _test.replace_ext("wav");
#endif //__ANDROID__
	VoicePaths voices;
	voices.push_back(_test.string());
    if(_soundPlayer)
        _soundPlayer->Speak(voices,MM::PvOthers);
}

MM::PriorityVoice VoiceManager::GetManouverPriorityFromDist(int d)
{
	if(d > 600)
		return PvVeryFarManouver;
	else if(d > 400)
		return PvFarManouver;
	else 
		return PvManouver;
}

int VoiceManager::interpVal(int val, int valRangeBegin, int valRangeEnd, int rangeBegin, int rangeEnd )
{
	if( val <= rangeBegin) return valRangeBegin;
	if( val >= rangeEnd) return valRangeEnd;

	float y = float(valRangeEnd - valRangeBegin) / float(rangeEnd - rangeBegin) ;
	float res = valRangeBegin + float(val - rangeBegin) * y;
	return ((int)res);
}

void VoiceManager::Update(const libmap2::navi::NavigationInfoState *ni) {

	using namespace libmap2::navi;
	
	if (Application::Instance().GetGpsManager()->GetSpeed() <= MIN_SPEAK_SPEED) return;
	if (ni == NULL) return;
	
#if defined (_WIN32)
	MM::Logic::OptionManager *om = Application::Instance().GetOptionManager();
	int volume = om->GetIntValue(MM::Logic::SoundVideoCategory, MM::Logic::
	VoiceVolumeOpt); 
	if(om->GetBoolValue(MM::Logic::SoundVideoCategory, MM::Logic::VoiceVolInterpEnabledOpt))
	{
		int speed = Application::Instance().GetGpsManager()->GetSpeed();
		int speedRangeBegin = om->GetIntValue(MM::Logic::SoundVideoCategory, MM::Logic::
	VoiceVolInterpSpeedRangeBeginOpt);
		int speedRangeEnd = om->GetIntValue(MM::Logic::SoundVideoCategory, MM::Logic::
	VoiceVolInterpSpeedRangeEndOpt);
		int volRangeEnd = om->GetIntValue(MM::Logic::SoundVideoCategory, MM::Logic::
	VoiceVolInterpRangeEndOpt);
		int volRangeBegin = volume;
		volume = interpVal(speed, volRangeBegin, volRangeEnd, speedRangeBegin, speedRangeEnd);
	} 
	SetVolume(volume);
#endif

	ManouverInfo mc, mn;
	ni->GetCurrManouver(&mc);
	ni->GetNextManouver(&mn);
	int curr = mc.GetDistToManouver();
	int l = mc.GetManouverLength();
	int turn1 = mc.GetTurn();
	int l2 = mn.GetManouverLength();
	int turn2 = mn.GetTurn();

	int fix_600 = (l > 600) ? 200 : 0;
	if (l < 200 && l > 140) {
		if (turn1 < LEAVE_ROUNDABOUT_1 || turn1 > LEAVE_ROUNDABOUT_8) {
			if (abs(l - 20 - curr) < _params.dist_treshold) {
				//nasty rondo hack
				if (turn1 == ENTER_ROUNDABOUT && (int)turn2 != PAST_END) {
					Speak(Accept(turn2, 150, l),PvManouver);
					return;
				} else {
					Speak(Accept(turn1, 150, l),PvManouver);
					return;
				}
			}
		}
	} else if (l > 2000 && turn1 != UTURN_IF_POSSIBLE1) {
		if (abs(l - 150 - curr) < _params.dist_treshold) {
			Speak(Accept(STRAIGHT_AHEAD, 700, l),GetManouverPriorityFromDist(700));
			return;
		}
	} else if (turn1 == UTURN_IF_POSSIBLE1) {
		if (abs(l - curr) < _params.dist_treshold)
			Speak(Accept(turn1, 700, l),GetManouverPriorityFromDist(700));
			return;
	}
	const int dists_size = 4;
	int dists[dists_size] = { 2000, 400 + fix_600, 200, 0 };
	// for exta long arcs check also 2000m. TODO: check intersecion with STRAIGHT_AHEAD,700
	int j = (l > 2000 && mc.OnHighway()) ? 0 : 1;	
	std::string retval;
	for (; j < dists_size; j++) {
		if (curr > dists[j]) {
			if (abs(curr - dists[j]) >= _params.dist_treshold)
				return;
			if (turn2 != PAST_END && (l2 < _params.join_msg_dist)) {
				// rozroznij now od prepare
				if (l2 > _params.join_now_msg_dist) {
					retval = Accept(turn1, turn2, dists[j] + 5, l);
				} else {
					retval = Accept(turn1, turn2, dists[j], l);
				}
			}
			// albo nastepny manewr to zjazd z ronda
			if (retval.empty() && turn2 != PAST_END && turn2 >= LEAVE_ROUNDABOUT_1
				&& turn2 <= LEAVE_ROUNDABOUT_8) {
				retval = Accept(turn1, turn2, dists[j], l);
			}
			if (retval.empty()) {
				//nasty rondo hack
				if (dists[j] == 200 && turn1 == ENTER_ROUNDABOUT && turn2 != PAST_END) {
					retval = Accept(turn2, 150, l);
				}
			}
			if (retval.empty()) {
				retval = Accept(turn1, dists[j], l);
			}
			if (!retval.empty()) {
				Speak(retval,GetManouverPriorityFromDist(dists[j]));
				break;
			}
		}
	}
}

unsigned VoiceManager::GetVolume() const {
	int ret = 0;
#if defined (_WIN32_WCE)
	if (_devident!=NULL && _rvolume!=NULL){
		ret = _rvolume->Get();
	}
#elif defined (__ANDROID__)
	ret = get_current_volume();
#elif defined (__APPLE__)
    ret = ios_get_current_volume();
#endif
	ret = interpVal(ret, 0, 100, 0, GetLimiterVolume());
	return ret;
}

void VoiceManager::SetVolume(unsigned vol, int sysui) {
	vol = interpVal(vol, 0, GetLimiterVolume(), 0, 100);
#if defined (_WIN32_WCE)
	if (_devident!=NULL && _rvolume!=NULL){
		_rvolume->Set(vol);
	}
#elif defined (__ANDROID__)
	_curr_vol = vol;
	jni_set_volume_audio((int)vol, sysui);
#elif defined (__APPLE__)
    _curr_vol = vol;
    ios_set_volume_audio(vol);
#endif

}

// FB8567, FB8567(20140327), FB8567 (20140526)
int VoiceManager::GetLimiterVolume() const {
#if defined (_WIN32_WCE) && defined(__COMMON_PNA__)
	return 100;
	//default.opt controlled variant
	//MM::Logic::OptionManager *om = Application::Instance().GetOptionManager();
	//int limit = om->GetIntValue(MM::Logic::SoundVideoCategory, MM::Logic::VoiceVolLimitOpt);
	//if (limit < 0 || limit > 100) limit = 100;
	//return limit;
#else
	return 100;
#endif
}

std::string VoiceManager::Accept(int t, int l, int d) {
	std::string ret = GetVoice(t, l);
	if (!ret.empty()) {
		int accept = t * 100000 + l + d;
		if ((accept == _prev_accept) || (abs(_prev_l - l) <= 5 && _prev_d == d)) {
			ret = EmptyVoice();
		} else {
			_prev_accept = accept;
			_prev_d = d;
			_prev_l = l;
		}
	}
	return ret;
}

std::string VoiceManager::Accept(int t1, int t2, int l, int d) {
	std::string ret = GetVoice(t1, t2, l);
	if (!ret.empty()) {
		int accept = t1 * 100000 + t2 * 100000 + l + d;
		if ((accept == _prev_accept) || (abs(_prev_l - l) <= 5 && _prev_d == d)) {
			ret = EmptyVoice();
		} else {
			_prev_accept = accept;
			_prev_d = d;
			_prev_l = l;
		}
	}
	return ret;
}

unsigned VoiceManager::GetScreenClickVolume() const
{
	int res = -1;
#if defined (_WIN32_WCE) && defined (NEVER_USE_THIS_CODE)
	Platform::Registry reg(Platform::Registry::CUser + "\\ControlPanel\\Volume");
	bool ok = reg.GetValue("Screen",res);
#endif
	return res;
}

void VoiceManager::SetScreenClickVolume(unsigned vol) const
{
#if defined (_WIN32_WCE) && defined (NEVER_USE_THIS_CODE)
	Platform::Registry reg(Platform::Registry::CUser + "\\ControlPanel\\Volume");
	bool res = reg.SetValue("Screen",vol);
	::DeviceRegulation::AudioUpdate();
#endif
}

using namespace libmap2::navi;	 // for manouver names

const snd_entry_t snd_entries[] = {
	/* single  */ { "end_400", DESTINATION_AHEAD, NONE, 400 },
	/* single  */ { "end_200", DESTINATION_AHEAD, NONE, 200 },
	/* single  */ { "end_0", DESTINATION_AHEAD, NONE, 0 },
	/* single  */ { "via_400", VIA_AHEAD, NONE, 400 },
	/* single  */ { "via_200", VIA_AHEAD, NONE, 200 },
	/* single  */ { "via_0", VIA_AHEAD, NONE, 0 },
	/* joined  */ { "round_right_0", LEAVE_ROUNDABOUT_1, TURN_RIGHT, 0 },
	/* joined  */ { "round_right_0", LEAVE_ROUNDABOUT_2, TURN_RIGHT, 0 },
	/* joined  */ { "round_right_0", LEAVE_ROUNDABOUT_3, TURN_RIGHT, 0 },
	/* joined  */ { "round_right_0", LEAVE_ROUNDABOUT_4, TURN_RIGHT, 0 },
	/* joined  */ { "round_right_0", LEAVE_ROUNDABOUT_5, TURN_RIGHT, 0 },
	/* joined  */ { "round_right_0", LEAVE_ROUNDABOUT_6, TURN_RIGHT, 0 },
	/* joined  */ { "round_right_0", LEAVE_ROUNDABOUT_7, TURN_RIGHT, 0 },
	/* joined  */ { "round_right_0", LEAVE_ROUNDABOUT_8, TURN_RIGHT, 0 },
	/* joined  */ { "round_right_prepare_0", LEAVE_ROUNDABOUT_1, TURN_RIGHT, 5 },
	/* joined  */ { "round_right_prepare_0", LEAVE_ROUNDABOUT_2, TURN_RIGHT, 5 },
	/* joined  */ { "round_right_prepare_0", LEAVE_ROUNDABOUT_3, TURN_RIGHT, 5 },
	/* joined  */ { "round_right_prepare_0", LEAVE_ROUNDABOUT_4, TURN_RIGHT, 5 },
	/* joined  */ { "round_right_prepare_0", LEAVE_ROUNDABOUT_5, TURN_RIGHT, 5 },
	/* joined  */ { "round_right_prepare_0", LEAVE_ROUNDABOUT_6, TURN_RIGHT, 5 },
	/* joined  */ { "round_right_prepare_0", LEAVE_ROUNDABOUT_7, TURN_RIGHT, 5 },
	/* joined  */ { "round_right_prepare_0", LEAVE_ROUNDABOUT_8, TURN_RIGHT, 5 },
	/* joined  */ { "round_left_0", LEAVE_ROUNDABOUT_1, TURN_LEFT, 0 },
	/* joined  */ { "round_left_0", LEAVE_ROUNDABOUT_2, TURN_LEFT, 0 },
	/* joined  */ { "round_left_0", LEAVE_ROUNDABOUT_3, TURN_LEFT, 0 },
	/* joined  */ { "round_left_0", LEAVE_ROUNDABOUT_4, TURN_LEFT, 0 },
	/* joined  */ { "round_left_0", LEAVE_ROUNDABOUT_5, TURN_LEFT, 0 },
	/* joined  */ { "round_left_0", LEAVE_ROUNDABOUT_6, TURN_LEFT, 0 },
	/* joined  */ { "round_left_0", LEAVE_ROUNDABOUT_7, TURN_LEFT, 0 },
	/* joined  */ { "round_left_0", LEAVE_ROUNDABOUT_8, TURN_LEFT, 0 },
	/* joined  */ { "round_left_prepare_0", LEAVE_ROUNDABOUT_1, TURN_LEFT, 5 },
	/* joined  */ { "round_left_prepare_0", LEAVE_ROUNDABOUT_2, TURN_LEFT, 5 },
	/* joined  */ { "round_left_prepare_0", LEAVE_ROUNDABOUT_3, TURN_LEFT, 5 },
	/* joined  */ { "round_left_prepare_0", LEAVE_ROUNDABOUT_4, TURN_LEFT, 5 },
	/* joined  */ { "round_left_prepare_0", LEAVE_ROUNDABOUT_5, TURN_LEFT, 5 },
	/* joined  */ { "round_left_prepare_0", LEAVE_ROUNDABOUT_6, TURN_LEFT, 5 },
	/* joined  */ { "round_left_prepare_0", LEAVE_ROUNDABOUT_7, TURN_LEFT, 5 },
	/* joined  */ { "round_left_prepare_0", LEAVE_ROUNDABOUT_8, TURN_LEFT, 5 },
	/* joined  */ { "round_keep_right_0", LEAVE_ROUNDABOUT_1, KEEP_RIGHT, 0 },
	/* joined  */ { "round_keep_right_0", LEAVE_ROUNDABOUT_2, KEEP_RIGHT, 0 },
	/* joined  */ { "round_keep_right_0", LEAVE_ROUNDABOUT_3, KEEP_RIGHT, 0 },
	/* joined  */ { "round_keep_right_0", LEAVE_ROUNDABOUT_4, KEEP_RIGHT, 0 },
	/* joined  */ { "round_keep_right_0", LEAVE_ROUNDABOUT_5, KEEP_RIGHT, 0 },
	/* joined  */ { "round_keep_right_0", LEAVE_ROUNDABOUT_6, KEEP_RIGHT, 0 },
	/* joined  */ { "round_keep_right_0", LEAVE_ROUNDABOUT_7, KEEP_RIGHT, 0 },
	/* joined  */ { "round_keep_right_0", LEAVE_ROUNDABOUT_8, KEEP_RIGHT, 0 },
	/* joined  */ { "round_keep_right_prepare_0", LEAVE_ROUNDABOUT_1, KEEP_RIGHT, 5 },
	/* joined  */ { "round_keep_right_prepare_0", LEAVE_ROUNDABOUT_2, KEEP_RIGHT, 5 },
	/* joined  */ { "round_keep_right_prepare_0", LEAVE_ROUNDABOUT_3, KEEP_RIGHT, 5 },
	/* joined  */ { "round_keep_right_prepare_0", LEAVE_ROUNDABOUT_4, KEEP_RIGHT, 5 },
	/* joined  */ { "round_keep_right_prepare_0", LEAVE_ROUNDABOUT_5, KEEP_RIGHT, 5 },
	/* joined  */ { "round_keep_right_prepare_0", LEAVE_ROUNDABOUT_6, KEEP_RIGHT, 5 },
	/* joined  */ { "round_keep_right_prepare_0", LEAVE_ROUNDABOUT_7, KEEP_RIGHT, 5 },
	/* joined  */ { "round_keep_right_prepare_0", LEAVE_ROUNDABOUT_8, KEEP_RIGHT, 5 },
	/* joined  */ { "round_keep_left_0", LEAVE_ROUNDABOUT_1, KEEP_LEFT, 0 },
	/* joined  */ { "round_keep_left_0", LEAVE_ROUNDABOUT_2, KEEP_LEFT, 0 },
	/* joined  */ { "round_keep_left_0", LEAVE_ROUNDABOUT_3, KEEP_LEFT, 0 },
	/* joined  */ { "round_keep_left_0", LEAVE_ROUNDABOUT_4, KEEP_LEFT, 0 },
	/* joined  */ { "round_keep_left_0", LEAVE_ROUNDABOUT_5, KEEP_LEFT, 0 },
	/* joined  */ { "round_keep_left_0", LEAVE_ROUNDABOUT_6, KEEP_LEFT, 0 },
	/* joined  */ { "round_keep_left_0", LEAVE_ROUNDABOUT_7, KEEP_LEFT, 0 },
	/* joined  */ { "round_keep_left_0", LEAVE_ROUNDABOUT_8, KEEP_LEFT, 0 },
	/* joined  */ { "round_keep_left_prepare_0", LEAVE_ROUNDABOUT_1, KEEP_LEFT, 5 },
	/* joined  */ { "round_keep_left_prepare_0", LEAVE_ROUNDABOUT_2, KEEP_LEFT, 5 },
	/* joined  */ { "round_keep_left_prepare_0", LEAVE_ROUNDABOUT_3, KEEP_LEFT, 5 },
	/* joined  */ { "round_keep_left_prepare_0", LEAVE_ROUNDABOUT_4, KEEP_LEFT, 5 },
	/* joined  */ { "round_keep_left_prepare_0", LEAVE_ROUNDABOUT_5, KEEP_LEFT, 5 },
	/* joined  */ { "round_keep_left_prepare_0", LEAVE_ROUNDABOUT_6, KEEP_LEFT, 5 },
	/* joined  */ { "round_keep_left_prepare_0", LEAVE_ROUNDABOUT_7, KEEP_LEFT, 5 },
	/* joined  */ { "round_keep_left_prepare_0", LEAVE_ROUNDABOUT_8, KEEP_LEFT, 5 },
	/* joined  */ { "round_turn_back_0", LEAVE_ROUNDABOUT_1, UTURN_IF_POSSIBLE, 0 },
	/* joined  */ { "round_turn_back_0", LEAVE_ROUNDABOUT_2, UTURN_IF_POSSIBLE, 0 },
	/* joined  */ { "round_turn_back_0", LEAVE_ROUNDABOUT_3, UTURN_IF_POSSIBLE, 0 },
	/* joined  */ { "round_turn_back_0", LEAVE_ROUNDABOUT_4, UTURN_IF_POSSIBLE, 0 },
	/* joined  */ { "round_turn_back_0", LEAVE_ROUNDABOUT_5, UTURN_IF_POSSIBLE, 0 },
	/* joined  */ { "round_turn_back_0", LEAVE_ROUNDABOUT_6, UTURN_IF_POSSIBLE, 0 },
	/* joined  */ { "round_turn_back_0", LEAVE_ROUNDABOUT_7, UTURN_IF_POSSIBLE, 0 },
	/* joined  */ { "round_turn_back_0", LEAVE_ROUNDABOUT_8, UTURN_IF_POSSIBLE, 0 },
	/* joined  */ { "round_turn_back_prepare_0", LEAVE_ROUNDABOUT_1, UTURN_IF_POSSIBLE, 5 },
	/* joined  */ { "round_turn_back_prepare_0", LEAVE_ROUNDABOUT_2, UTURN_IF_POSSIBLE, 5 },
	/* joined  */ { "round_turn_back_prepare_0", LEAVE_ROUNDABOUT_3, UTURN_IF_POSSIBLE, 5 },
	/* joined  */ { "round_turn_back_prepare_0", LEAVE_ROUNDABOUT_4, UTURN_IF_POSSIBLE, 5 },
	/* joined  */ { "round_turn_back_prepare_0", LEAVE_ROUNDABOUT_5, UTURN_IF_POSSIBLE, 5 },
	/* joined  */ { "round_turn_back_prepare_0", LEAVE_ROUNDABOUT_6, UTURN_IF_POSSIBLE, 5 },
	/* joined  */ { "round_turn_back_prepare_0", LEAVE_ROUNDABOUT_7, UTURN_IF_POSSIBLE, 5 },
	/* joined  */ { "round_turn_back_prepare_0", LEAVE_ROUNDABOUT_8, UTURN_IF_POSSIBLE, 5 },
	/* joined  */ { "round_200_exit_1", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_1, 200 },
	/* joined  */ { "round_200_exit_2", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_2, 200 },
	/* joined  */ { "round_200_exit_3", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_3, 200 },
	/* joined  */ { "round_200_exit_4", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_4, 200 },
	/* joined  */ { "round_200_exit_5", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_5, 200 },
	/* joined  */ { "round_200_exit_6", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_6, 200 },
	/* joined  */ { "round_200_exit_7", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_7, 200 },
	/* joined  */ { "round_200_exit_8", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_8, 200 },
	/* joined  */ { "round_exit_1", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_1, 0 },
	/* joined  */ { "round_exit_2", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_2, 0 },
	/* joined  */ { "round_exit_3", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_3, 0 },
	/* joined  */ { "round_exit_4", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_4, 0 },
	/* joined  */ { "round_exit_5", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_5, 0 },
	/* joined  */ { "round_exit_6", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_6, 0 },
	/* joined  */ { "round_exit_7", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_7, 0 },
	/* joined  */ { "round_exit_8", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_8, 0 },
	/* joined  */ { "round_200_exit_1", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_1, 150 },
	/* joined  */ { "round_200_exit_2", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_2, 150 },
	/* joined  */ { "round_200_exit_3", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_3, 150 },
	/* joined  */ { "round_200_exit_4", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_4, 150 },
	/* joined  */ { "round_200_exit_5", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_5, 150 },
	/* joined  */ { "round_200_exit_6", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_6, 150 },
	/* joined  */ { "round_200_exit_7", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_7, 150 },
	/* joined  */ { "round_200_exit_8", ENTER_ROUNDABOUT, LEAVE_ROUNDABOUT_8, 150 },
	/* single  */ { "left_400", TURN_LEFT, NONE, 400 },
	/* single  */ { "left_200", TURN_LEFT, NONE, 200 },
	/* single  */ { "left_prepare", TURN_LEFT, NONE, 150 },
	/* single  */ { "left_0", TURN_LEFT, NONE, 0 },
	/* single  */ { "right_400", TURN_RIGHT, NONE, 400 },
	/* single  */ { "right_200", TURN_RIGHT, NONE, 200 },
	/* single  */ { "right_prepare", TURN_RIGHT, NONE, 150 },
	/* single  */ { "right_0", TURN_RIGHT, NONE, 0 },
	/* joined  */ { "left_left_200", TURN_LEFT, TURN_LEFT, 200 },
	/* joined  */ { "left_left_prepare_200", TURN_LEFT, TURN_LEFT, 205 },
	/* joined  */ { "left_right_200", TURN_LEFT, TURN_RIGHT, 200 },
	/* joined  */ { "left_right_prepare_200", TURN_LEFT, TURN_RIGHT, 205 },
	/* joined  */ { "left_keep_right_200", TURN_LEFT, KEEP_RIGHT, 200 },
	/* joined  */ { "left_keep_left_200", TURN_LEFT, KEEP_LEFT, 200 },
	/* joined  */ { "right_right_200", TURN_RIGHT, TURN_RIGHT, 200 },
	/* joined  */ { "right_right_prepare_200", TURN_RIGHT, TURN_RIGHT, 205 },
	/* joined  */ { "right_left_200", TURN_RIGHT, TURN_LEFT, 200 },
	/* joined  */ { "right_left_prepare_200", TURN_RIGHT, TURN_LEFT, 205 },
	/* joined  */ { "right_keep_right_200", TURN_RIGHT, KEEP_RIGHT, 200 },
	/* joined  */ { "right_keep_left_200", TURN_RIGHT, KEEP_LEFT, 200 },
	/* single  */ { "keep_left_400", KEEP_LEFT, NONE, 400 },
	/* single  */ { "keep_left_200", KEEP_LEFT, NONE, 200 },
	/* single  */ { "keep_left_0", KEEP_LEFT, NONE, 150 },
	/* joined  */ { "keep_left_right_200", KEEP_LEFT, KEEP_RIGHT, 200 },
	/* joined  */ { "keep_left_left_200", KEEP_LEFT, KEEP_LEFT, 200 },
	/* joined  */ { "keep_left_turn_right_200", KEEP_LEFT, TURN_RIGHT, 200 },
	/* joined  */ { "keep_left_turn_left_200", KEEP_LEFT, TURN_LEFT, 200 },
	/* joined  */ { "keep_left_turn_right_prepare_200", KEEP_LEFT, TURN_RIGHT, 205 },
	/* joined  */ { "keep_left_turn_left_prepare_200", KEEP_LEFT, TURN_LEFT, 205 },
	/* single  */ { "keep_right_400", KEEP_RIGHT, NONE, 400 },
	/* single  */ { "keep_right_200", KEEP_RIGHT, NONE, 200 },
	/* single  */ { "keep_right_0", KEEP_RIGHT, NONE, 150 },
	/* joined  */ { "keep_right_left_200", KEEP_RIGHT, KEEP_LEFT, 200 },
	/* joined  */ { "keep_right_right_200", KEEP_RIGHT, KEEP_RIGHT, 200 },
	/* joined  */ { "keep_right_turn_left_200", KEEP_RIGHT, TURN_LEFT, 200 },
	/* joined  */ { "keep_right_turn_right_200", KEEP_RIGHT, TURN_RIGHT, 200 },
	/* joined  */ { "keep_right_turn_left_prepare_200", KEEP_RIGHT, TURN_LEFT, 205 },
	/* joined  */ { "keep_right_turn_right_prepare_200", KEEP_RIGHT, TURN_RIGHT, 205 },
	/* single  */ { "turn_back", UTURN_IF_POSSIBLE, NONE, 0 },
	/* single  */ { "turn_back_prepare", UTURN_IF_POSSIBLE, NONE, 150 },
	/* single  */ { "turn_back_200", UTURN_IF_POSSIBLE, NONE, 200 },
	/* single  */ { "turn_back_400", UTURN_IF_POSSIBLE, NONE, 400 },
	/* single  */ { "turn_back_600", UTURN_IF_POSSIBLE, NONE, 600 },
	/* joined  */ { "turn_back_right_200", UTURN_IF_POSSIBLE, TURN_RIGHT, 200 },
	/* joined  */ { "turn_back_left_200", UTURN_IF_POSSIBLE, TURN_LEFT, 200 },
	/* joined  */ { "turn_back_right_prepare_200", UTURN_IF_POSSIBLE, TURN_RIGHT, 205 },
	/* joined  */ { "turn_back_left_prepare_200", UTURN_IF_POSSIBLE, TURN_LEFT, 205 },
	/* single  */ { "left_600", TURN_LEFT, NONE, 600 },
	/* single  */ { "right_600", TURN_RIGHT, NONE, 600 },
	/* single  */ { "keep_left_600", KEEP_LEFT, NONE, 600 },
	/* single  */ { "keep_right_600", KEEP_RIGHT, NONE, 600 },
	/* single  */ { "end_600", DESTINATION_AHEAD, NONE, 600 },
	/* single  */ { "via_600", VIA_AHEAD, NONE, 600 },
	/* single  */ { "left_2000", TURN_LEFT, NONE, 2000 },
	/* single  */ { "right_2000", TURN_RIGHT, NONE, 2000 },
	/* single  */ { "keep_left_2000", KEEP_LEFT, NONE, 2000 },
	/* single  */ { "keep_right_2000", KEEP_RIGHT, NONE, 2000 },
	/* single  */ { "via_2000", VIA_AHEAD, NONE, 2000 },
	/* single  */ { "turn_back", UTURN_IF_POSSIBLE1, NONE, 700 },
	/* single  */ { "stright_100", STRAIGHT_AHEAD, NONE, 700 },
	/* single  */ { "ramp_2000_exit_1", RAMP_EXIT_1, NONE, 2000 },
	/* single  */ { "ramp_600_exit_1", RAMP_EXIT_1, NONE, 600 },	
	/* single  */ { "ramp_200_exit_1", RAMP_EXIT_1, NONE, 200 },
	/* single  */ { "ramp_150_exit_1", RAMP_EXIT_1, NONE, 150 },
	/* single  */ { "ramp_exit_1", RAMP_EXIT_1, NONE, 0 },
	/* joined  */ { "ramp_exit_1_right_200", RAMP_EXIT_1, KEEP_RIGHT, 200 },
	/* joined  */ { "ramp_exit_1_left_200", RAMP_EXIT_1, KEEP_LEFT, 200 },
	/* joined  */ { "ramp_exit_1_turn_right_200", RAMP_EXIT_1, TURN_RIGHT, 200 },
	/* joined  */ { "ramp_exit_1_turn_left_200", RAMP_EXIT_1, TURN_LEFT, 200 },
	/* joined  */ { "ramp_exit_1_turn_right_prepare_200", RAMP_EXIT_1, TURN_RIGHT, 205 },
	/* joined  */ { "ramp_exit_1_turn_left_prepare_200", RAMP_EXIT_1, TURN_LEFT, 205 },
	/* single  */ { "ramp_2000_exit_2", RAMP_EXIT_2, NONE, 2000 },
	/* single  */ { "ramp_600_exit_2", RAMP_EXIT_2, NONE, 600 },	
	/* single  */ { "ramp_200_exit_2", RAMP_EXIT_2, NONE, 200 },
	/* single  */ { "ramp_150_exit_2", RAMP_EXIT_2, NONE, 150 },
	/* single  */ { "ramp_exit_2", RAMP_EXIT_2, NONE, 0 },
	/* joined  */ { "ramp_exit_2_right_200", RAMP_EXIT_2, KEEP_RIGHT, 200 },
	/* joined  */ { "ramp_exit_2_left_200", RAMP_EXIT_2, KEEP_LEFT, 200 },
	/* joined  */ { "ramp_exit_2_turn_right_200", RAMP_EXIT_2, TURN_RIGHT, 200 },
	/* joined  */ { "ramp_exit_2_turn_left_200", RAMP_EXIT_2, TURN_LEFT, 200 },
	/* joined  */ { "ramp_exit_2_turn_right_prepare_200", RAMP_EXIT_2, TURN_RIGHT, 205 },
	/* joined  */ { "ramp_exit_2_turn_left_prepare_200", RAMP_EXIT_2, TURN_LEFT, 205 },
	/* single  */ { "ramp_2000_exit_3", RAMP_EXIT_3, NONE, 2000 },
	/* single  */ { "ramp_600_exit_3", RAMP_EXIT_3, NONE, 600 },	
	/* single  */ { "ramp_200_exit_3", RAMP_EXIT_3, NONE, 200 },
	/* single  */ { "ramp_150_exit_3", RAMP_EXIT_3, NONE, 150 },
	/* single  */ { "ramp_exit_3", RAMP_EXIT_3, NONE, 0 },
	/* joined  */ { "ramp_exit_3_right_200", RAMP_EXIT_3, KEEP_RIGHT, 200 },
	/* joined  */ { "ramp_exit_3_left_200", RAMP_EXIT_3, KEEP_LEFT, 200 },
	/* joined  */ { "ramp_exit_3_turn_right_200", RAMP_EXIT_3, TURN_RIGHT, 200 },
	/* joined  */ { "ramp_exit_3_turn_left_200", RAMP_EXIT_3, TURN_LEFT, 200 },
	/* joined  */ { "ramp_exit_3_turn_right_prepare_200", RAMP_EXIT_3, TURN_RIGHT, 205 },
	/* joined  */ { "ramp_exit_3_turn_left_prepare_200", RAMP_EXIT_3, TURN_LEFT, 205 },
	/* special */ { "offroad", OffRoad, PAST_END, 0 },
	/* special */ { "satellite_off", LostStatellites, PAST_END, 0 },
	/* special */ { "satellite_on", GotSatellites, PAST_END, 0 },
	/* special */ { "poi_available", PoiAvailable, PAST_END, 0 },
	/* special */ { "noroad", NoRoad, PAST_END, 0 },
	/* special */ { "welcome", Welcome, PAST_END, 0 },
	/* special */ { "byebye", Byebye, PAST_END, 0 },
	/* no more */ { 0, NONE, NONE, 0 },
};


} // namespace MM
} // namespace Logic
