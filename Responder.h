#ifndef RESPONDER_H
#define RESPONDER_H

#include "AIMP2_SDK.h"

#include "MultiReaderSingleWriter.hpp"

#include "CRC32.h"

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <fstream>
#include <streambuf>

static std::map<int /*playlistID*/, std::pair<unsigned int /*time*/, std::pair<int /*crc32*/, std::string /*cached playlist*/> > > PLAYLIST_CACHE;

class CResponder
{
public:
	CResponder(IAIMP2Controller *AIMP, std::map<std::string, std::string>& params);
	~CResponder(void);
	std::string GetResponse(void);
private:
	std::wstring FromLocale(const std::string& mbstring);
	std::string ToLocale(const std::wstring& widestring);
	std::wstring FromUtf8(const std::string& mbstring);
	std::string ToUtf8(const std::wstring& widestring);
	IAIMP2Controller *ctrl;

	IAIMP2PlaylistManager2 *manager2;
	IAIMP2Player *player;
	IAIMP2Extended *extended;

	std::string pluginsPath;

	std::map<std::string, std::string> requestParams;
public:
	std::string DoAction(void);
	void DoFilePlay(void);
	std::string GetPlaylistList(void);
	std::string GetPlaylistSongs(int playListID, bool ignoreCache, bool returnCRC);
	std::string GetCurrentSong();
	void PlayTrack(int playListID, int SongNum);
	void SetNewSongPosition(int playListID, int SongNum, int position);
	void SortPlaylist(int playlistID, std::string sortType);
	std::string GetPlayerStatus();
	void SetPlayerStatus(std::string statusType, int value);
	void AddFile(int playlistID, std::string fileName);
	void DeleteFile(int playlistID, int fileId);
};

#endif //RESPONDER_H
