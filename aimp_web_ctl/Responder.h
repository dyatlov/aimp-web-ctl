#pragma once
#include "AIMP2_SDK.h"
#include "AIMPCache.h"

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <fstream>
#include <streambuf>

struct ANSWER
{
	char *data;
	int size;
};

class CResponder
{
public:
	CResponder(char *buffer, int len, AIMPCache *cache);
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
	
	char *buffer;
	int buf_len;

	std::string pluginsPath;

	std::map<std::string, std::string> requestParams;

	std::string requestFile;

	AIMPCache *cache;
public:
	std::string GetBody(void);
	std::string GetPLTable(int playlistCurrent);
	void ParseParams(void);
	std::string DoInclude(void);
	std::string DoAction(void);
	void DoFilePlay(void);
	std::string GetPlaylistList(void);
	std::string GetPlaylistSongs(int playListID);
	std::string GetPlaylistCRC(int playListID);
	std::string GetCurrentSong();
	void PlayTrack(int playListID, int SongNum);
	void SetNewSongPosition(int playListID, int SongNum, int position);
	void SortPlaylist(int playlistID, std::string sortType);
	std::string GetPlayerStatus();
	void SetPlayerStatus(std::string statusType, int value);
	void AddFile(int playlistID, std::string fileName);
	void DeleteFile(int playlistID, int fileId);
};
