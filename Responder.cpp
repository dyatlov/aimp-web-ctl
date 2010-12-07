#include "Responder.h"
#include "simpleserver.h"

CResponder::CResponder(IAIMP2Controller *AIMP, std::map<std::string, std::string>& params)
{
	this->ctrl = AIMP;
	
	manager2 = 0;
	player = 0;
	extended = 0;
	
	AIMP->AIMP_QueryObject(IAIMP2PlaylistManager2ID, &manager2);
	AIMP->AIMP_QueryObject(IAIMP2PlayerID, &player);
	AIMP->AIMP_QueryObject(IAIMP2ExtendedID, &extended);

	WCHAR buffer[256];
        int result = extended->AIMP_GetPath(AIMP_CFG_PLUGINS, buffer, 256);
        this->pluginsPath = ToUtf8(std::wstring(buffer, result));

	requestParams = params;
}

CResponder::~CResponder(void)
{
}

std::wstring CResponder::FromLocale(const std::string& mbstring)
{
	int widesize = MultiByteToWideChar(CP_ACP, 0, mbstring.c_str(), -1, NULL, 0);
	if (widesize == 0)
	{
		return std::wstring();
	}

	std::vector<WCHAR> resultstring(widesize);

	int convresult = MultiByteToWideChar(CP_ACP, 0, mbstring.c_str(), -1, &resultstring[0], widesize);

	if (convresult != widesize)
	{
		return std::wstring();
	}

	return std::wstring(&resultstring[0]);
}

std::string CResponder::ToLocale(const std::wstring& widestring)
{
	int utf8size = WideCharToMultiByte(CP_ACP, 0, widestring.c_str(), -1, NULL, 0, NULL, NULL);
	if (utf8size == 0)
	{
		return std::string();
	}

	std::vector<char> resultstring(utf8size);

	int convresult = WideCharToMultiByte(CP_ACP, 0, widestring.c_str(), -1, &resultstring[0], utf8size, NULL, NULL);

	if (convresult != utf8size)
	{
		return std::string();
	}

	return std::string(&resultstring[0]);
}

std::wstring CResponder::FromUtf8(const std::string& mbstring)
{
	int widesize = MultiByteToWideChar(CP_UTF8, 0, mbstring.c_str(), -1, NULL, 0);
	if (widesize == 0)
	{
		return std::wstring();
	}

	std::vector<WCHAR> resultstring(widesize);

	int convresult = MultiByteToWideChar(CP_UTF8, 0, mbstring.c_str(), -1, &resultstring[0], widesize);

	if (convresult != widesize)
	{
		return std::wstring();
	}

	return std::wstring(&resultstring[0]);
}

std::string CResponder::ToUtf8(const std::wstring& widestring)
{
	int utf8size = WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, NULL, 0, NULL, NULL);
	if (utf8size == 0)
	{
		return std::string();
	}

	std::vector<char> resultstring(utf8size);

	int convresult = WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, &resultstring[0], utf8size, NULL, NULL);

	if (convresult != utf8size)
	{
		return std::string();
	}

	return std::string(&resultstring[0]);
}

int StrToInt(std::string str)
{
	return atoi(str.c_str());
}

std::string replaceAllA(
	std::string result, 
	const std::string& replaceWhat, 
	const std::string& replaceWithWhat)
{
	if(replaceWhat != replaceWithWhat && !replaceWhat.empty())
	{
		std::string::size_type pos1(0);
		std::string::size_type pos2(0);
		std::string::size_type from_len(replaceWhat.size());
		std::string::size_type to_len(replaceWithWhat.size());

		while((pos1 = result.find(replaceWhat, pos2)) != std::string::npos)
		{
			result.replace(pos1, from_len, replaceWithWhat);
			pos2 = pos1 + to_len;
		}
	}
	
	return result;
}

std::wstring replaceAllW(
	std::wstring result, 
	const std::wstring& replaceWhat, 
	const std::wstring& replaceWithWhat)
{
	if(replaceWhat != replaceWithWhat && !replaceWhat.empty())
	{
		std::wstring::size_type pos1(0);
		std::wstring::size_type pos2(0);
		std::wstring::size_type from_len(replaceWhat.size());
		std::wstring::size_type to_len(replaceWithWhat.size());

		while((pos1 = result.find(replaceWhat, pos2)) != std::wstring::npos)
		{
			result.replace(pos1, from_len, replaceWithWhat);
			pos2 = pos1 + to_len;
		}
	}
	
	return result;
}

std::string CResponder::GetResponse(void)
{
	std::stringstream outStream(std::ios::in | std::ios::out | std::ios::binary);

	if(requestParams.size() > 0) //this is parameterized request
	{
		std::map<std::string, std::string>::iterator p;
		
		std::string action = requestParams["action"];

		if(action.compare("get_playlist_list") == 0)
		{
			outStream << GetPlaylistList();
		}
		else if(action.compare("get_version_string") == 0)
		{
			outStream << "2.6.4.1";
		}
		else if(action.compare("get_version_number") == 0)
		{
			outStream << "2641";
		}
		else if(action.compare("get_update_time") == 0)
		{
			outStream << UPDATE_TIME;
		}
		else if(action.compare("get_playlist_songs") == 0)
		{
			outStream << GetPlaylistSongs(StrToInt(requestParams["id"]), /*requestParams["cache"].compare("no") == 0*/false, false);
		}
		else if(action.compare("get_playlist_crc") == 0)
		{
			outStream << GetPlaylistSongs(StrToInt(requestParams["id"]), /*requestParams["cache"].compare("no") == 0*/false, true);
		}
		else if(action.compare("get_player_status") == 0)
		{
			outStream << GetPlayerStatus();
		}
		else if(action.compare("get_song_current") == 0)
		{
			outStream << GetCurrentSong();
		}
		else if(action.compare("get_volume") == 0)
		{
			outStream << ctrl->AIMP_Status_Get(AIMP_STS_VOLUME);
		}
		else if(action.compare("set_volume") == 0)
		{
			outStream << ctrl->AIMP_Status_Set(AIMP_STS_VOLUME, StrToInt(requestParams["volume"]));
		}
		else if(action.compare("set_song_play") == 0)
		{
			PlayTrack(StrToInt(requestParams["playlist"]), StrToInt(requestParams["song"]));
		}
		else if(action.compare("set_song_position") == 0)
		{
			SetNewSongPosition(StrToInt(requestParams["playlist"]),
				StrToInt(requestParams["song"]), StrToInt(requestParams["position"]));
			GetPlaylistSongs(StrToInt(requestParams["playlist"]), true, true);
		}
		else if(action.compare("set_player_status") == 0)
		{
			SetPlayerStatus(requestParams["statusType"], StrToInt(requestParams["value"]));
		}
		else if(action.compare("player_play") == 0)
		{
			player->PlayOrResume();
		}
		else if(action.compare("player_pause") == 0)
		{
			player->Pause();
		}
		else if(action.compare("player_stop") == 0)
		{
			player->Stop();
		}
		else if(action.compare("player_prevous") == 0)
		{
			player->PrevTrack();
		}
		else if(action.compare("player_next") == 0)
		{
			player->NextTrack();
		}
		else if(action.compare("playlist_sort") == 0)
		{
			SortPlaylist(
				StrToInt(requestParams["playlist"]),
				requestParams["sort"]
				);
			GetPlaylistSongs(StrToInt(requestParams["playlist"]), true, true);
		}
		else if(action.compare("playlist_add_file") == 0)
		{
			AddFile(
				StrToInt(requestParams["playlist"]),
				requestParams["file"]
				);
			GetPlaylistSongs(StrToInt(requestParams["playlist"]), true, true);
		}
		else if(action.compare("playlist_del_file") == 0)
		{
			DeleteFile(
				StrToInt(requestParams["playlist"]),
				StrToInt(requestParams["file"])
				);
			GetPlaylistSongs(StrToInt(requestParams["playlist"]), true, true);
		}
	}

	return outStream.str();
}

std::string CResponder::GetPlaylistSongs(int playListID, bool ignoreCache, bool returnCRC)
{
	bool cacheFound = false;
        std::stringstream answer;
        
        unsigned int tickCount = GetTickCount();
        
        std::map<int /*playlistID*/,
			std::pair<unsigned int /*time*/,
				std::pair<int /*crc32*/,
					std::string /*cached playlist*/> > >::iterator it;

	if(!ignoreCache)
	{
		concurencyInstance.EnterReader();
		// check last time
		it = PLAYLIST_CACHE.find(playListID);
		if((it != PLAYLIST_CACHE.end()) && (tickCount - it->second.first < static_cast<unsigned int>(CACHE_TIME) * 1000) )
		{
			cacheFound = true;
			if(!returnCRC)
				answer << it->second.second.second;
			else
				answer << it->second.second.first;
		}
	
		concurencyInstance.LeaveReader();
	}
	
	if(cacheFound)
		return answer.str();
	
	concurencyInstance.EnterWriter();

        answer << "{\"status\":";
        int fileCount = manager2->AIMP_PLS_GetFilesCount(playListID);
        if(fileCount < 0)
        {
                answer << "\"ERROR\"";
        }
        else
        {
                answer << "\"OK\",\"songs\":[";
                std::wstring pBuf(256, 0);
                AIMP2FileInfo fileInfo;
                memset(&fileInfo, 0, sizeof(fileInfo));
                fileInfo.cbSizeOf = sizeof(fileInfo);
                for(int i= 0; i < fileCount; i++)
                {
                        manager2->AIMP_PLS_Entry_GetTitle(playListID, i, const_cast<const PWCHAR>(pBuf.c_str()), pBuf.capacity());
                        manager2->AIMP_PLS_Entry_InfoGet(playListID, i, &fileInfo);
                        pBuf = replaceAllW(pBuf, L"\"", L"\\\"");
                        answer << "{\"name\":\"" << ToUtf8(pBuf) << "\",\"length\":\"" << fileInfo.nDuration << "\"},";
                }
                if(fileCount > 0)
                        answer.seekp((int)(answer.tellp()) - 1);
                answer << "]";
        }
        answer << "}";
        
        //removing old cache
        for (it=PLAYLIST_CACHE.begin(); it != PLAYLIST_CACHE.end();)
	{
		if(tickCount - it->second.first > 60000 * 60) //delete playlists which where accessed last time hour or more ago
		{
			PLAYLIST_CACHE.erase(it++);
		}
		else
			++it;
	}
        
        CRC32 crc;
        unsigned int hash = crc.GetHash(answer.str().c_str());
        
        PLAYLIST_CACHE[playListID] = std::pair<int, std::pair<int, std::string> >(tickCount,
        			std::pair<int, std::string>(hash,
        			answer.str()));        
        
        if(returnCRC)
        {
        	answer.str(std::string());
        	answer << hash;
        }
        
        concurencyInstance.LeaveWriter();
        
        return answer.str();
}

std::string CResponder::GetPlaylistList()
{
	std::stringstream answer;
	answer << "[";
	short playlistCount = ctrl->AIMP_PLS_Count();
	for(int i = 0; i < playlistCount; i++)
	{
		int playlistID;
		INT64 ADuration, ASize;
		std::wstring ABuffer(256, 0);
		manager2->AIMP_PLS_ID_By_Index(i, &playlistID);
		manager2->AIMP_PLS_GetInfo(playlistID, &ADuration, &ASize);
		manager2->AIMP_PLS_GetName(playlistID, const_cast<const PWCHAR>(ABuffer.c_str()), ABuffer.length());
		ABuffer = replaceAllW(ABuffer, L"\"", L"\\\"");
		answer << "{\"id\":" << playlistID << ",\"duration\":" << ADuration << ",\"size\":" << ASize << ",\"name\":\"" << ToUtf8(ABuffer) << "\"},";
	}
	if(playlistCount > 0)
		answer.seekp((int)(answer.tellp()) - 1);
	answer << "]";
	return answer.str();
}

void CResponder::PlayTrack(int playListID, int SongNum)
{
	player->PlayTrack(playListID, SongNum);
}
void CResponder::SetNewSongPosition(int playListID, int SongNum, int position)
{
	manager2->AIMP_PLS_Entry_SetPosition(playListID, SongNum, position);
}
std::string CResponder::GetCurrentSong()
{
	int playing_list = manager2->AIMP_PLS_ID_PlayingGet();
	int playing_file = manager2->AIMP_PLS_ID_PlayingGetTrackIndex(playing_list);
	std::wstring pBuf(256, 0);
	manager2->AIMP_PLS_Entry_GetTitle(playing_list, playing_file, const_cast<const PWCHAR>(pBuf.c_str()), pBuf.capacity());
	pBuf = replaceAllW(pBuf, L"\"", L"\\\"");
	std::stringstream answer;
	answer << "{\"status\": \"OK\", \"PlayingList\": \"";
	answer << playing_list;
	answer << "\", \"PlayingFile\": \"";
	answer << playing_file;
	answer << "\", \"PlayingFileName\": \"";
	answer << ToUtf8(pBuf);
	answer << "\"}";
	return answer.str();
}

void CResponder::SortPlaylist(int playlistID, std::string sortType)
{
	if(sortType.compare("title") == 0)
	{
		manager2->AIMP_PLS_Sort(playlistID, AIMP_PLS_SORT_TYPE_TITLE);
	}
	else if(sortType.compare("filename") == 0)
	{
		manager2->AIMP_PLS_Sort(playlistID, AIMP_PLS_SORT_TYPE_FILENAME);
	}
	else if(sortType.compare("duration") == 0)
	{
		manager2->AIMP_PLS_Sort(playlistID, AIMP_PLS_SORT_TYPE_DURATION);
	}
	else if(sortType.compare("artist") == 0)
	{
		manager2->AIMP_PLS_Sort(playlistID, AIMP_PLS_SORT_TYPE_ARTIST);
	}
	else if(sortType.compare("inverse") == 0)
	{
		manager2->AIMP_PLS_Sort(playlistID, AIMP_PLS_SORT_TYPE_INVERSE);
	}
	else if(sortType.compare("randomize") == 0)
	{
		manager2->AIMP_PLS_Sort(playlistID, AIMP_PLS_SORT_TYPE_RANDOMIZE);
	}
}

std::string CResponder::GetPlayerStatus()
{
	std::stringstream answer;
	answer << "{\"status\": \"OK\", \"RepeatFile\": \"";
	answer << ctrl->AIMP_Status_Get(AIMP_STS_REPEAT);
	answer << "\", \"RandomFile\": \"";
	answer << ctrl->AIMP_Status_Get(AIMP_STS_SHUFFLE);
	answer << "\"}";
	return answer.str();
}

void CResponder::SetPlayerStatus(std::string statusType, int value)
{
	if(statusType.compare("repeat") == 0)
	{
		ctrl->AIMP_Status_Set(AIMP_STS_REPEAT, value);
	}
	else if(statusType.compare("shuffle") == 0)
	{
		ctrl->AIMP_Status_Set(AIMP_STS_SHUFFLE, value);
	}
}

std::string urldecode( const std::string& url_src )
{
	std::string url_ret = "";
	int len = (int)url_src.length();
	char tmpstr[5] = { '0', 'x', '_', '_', '\0' };

	for (int i = 0; i < len; i++)
	{
		char ch = url_src.at( i );
		if ( ch == '%' )
		{
			if ( i+2 >= len )
			return "";
			tmpstr[2] = url_src.at( ++i );
			tmpstr[3] = url_src.at( ++i );
			ch = (char)strtol( tmpstr, NULL, 16 );
		}
		url_ret += ch;
	}
	return url_ret;
}

void CResponder::AddFile(int playlistID, std::string fileName)
{
	IPLSStrings *Strings;
	ctrl->AIMP_NewStrings(&Strings);
	std::wstring resultFileName = FromUtf8(urldecode(fileName));
	Strings->AddFile(const_cast<const PWCHAR>(resultFileName.c_str()), NULL);
	ctrl->AIMP_PLS_AddFiles(playlistID, Strings);
}

void CResponder::DeleteFile(int playlistID, int fileId)
{
	manager2->AIMP_PLS_Entry_Delete(playlistID, fileId);
}
