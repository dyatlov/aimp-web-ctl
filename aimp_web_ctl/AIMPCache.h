#pragma once

#include "AIMP2_SDK.h"

#include <string>
#include <sstream>
#include <vector>
#include <map>

#include <boost/crc.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/shared_mutex.hpp>

struct CacheObj
{
	DWORD cacheTime;
	std::string crc32;
	std::string playlist;
	CacheObj()
	{
		cacheTime = 0;
		crc32 = "";
		playlist = "";
	}
};

class AIMPCache
{
private:
	std::map<int, CacheObj> cacheList;

	std::string pluginsPath;

	IAIMP2Controller *AIMP;
	IAIMP2PlaylistManager2 *manager2;
	IAIMP2Player *player;
	IAIMP2Extended *extended;

	DWORD sleepTime;

	boost::shared_mutex m_mutex;

	std::string ToUtf8(const std::wstring& widestring);
	std::string GetPlaylistSongs(int playListID);
	std::string GetPlaylistCRC(const std::string& playlistSongs);
	std::wstring replaceAllW(std::wstring result, const std::wstring& replaceWhat, const std::wstring& replaceWithWhat);
	void UpdatePlaylist(int playlistID);
public:
	std::string getCRC32(int playlistID);
	std::string getPlaylist(int playlistID);
	std::string getPluginsPath();
	
	IAIMP2Controller *getAIMP();
	IAIMP2PlaylistManager2 *getManager2();
	IAIMP2Player *getPlayer();
	IAIMP2Extended *getExtended();

	AIMPCache(IAIMP2Controller *AIMP, DWORD sleepTime);
	~AIMPCache(void);
};
