#include "StdAfx.h"
#include "Responder.h"

#include <boost/regex.hpp>
#include <boost/crc.hpp>

CResponder::CResponder(char *buf, int len, AIMPCache *cache)
{
	this->cache = cache;
	this->buffer = buf;
	this->buf_len = len;

	this->ctrl = cache->getAIMP();
	this->manager2 = cache->getManager2();
	this->player = cache->getPlayer();
	this->extended = cache->getExtended();
	this->pluginsPath = cache->getPluginsPath();

	ParseParams();
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
	while(1)
	{
		const int pos = result.find(replaceWhat);
		if (pos==-1) break;
		result.replace(pos,replaceWhat.size(),replaceWithWhat);
	}
	return result;
}

std::wstring replaceAllW(
	std::wstring result, 
	const std::wstring& replaceWhat, 
	const std::wstring& replaceWithWhat)
{
	while(1)
	{
		const int pos = result.find(replaceWhat);
		if (pos==-1) break;
		result.replace(pos,replaceWhat.size(),replaceWithWhat);
	}
	return result;
}

void CResponder::ParseParams(void)
{
	/* Added/Fixed by Kirill Gorbunov */
	if (buf_len == -1)
	{
		return;
	}
	/**/
	std::string request = std::string(buffer, buf_len);	
	boost::regex global("GET\\s([^\\?\\s]+)(\\?(\\S*))?");
	boost::match_results<std::string::const_iterator> results;
	if(boost::regex_search(request, results, global, boost::regex_constants::match_stop))
	{
		requestFile = std::string(results[1].first, results[1].second);
		std::string params = std::string(results[3].first, results[3].second);

		if(params.length() > 0)
		{
			std::string::const_iterator begin = params.begin(), end = params.end();

			boost::regex pars("^([^=]+)=([^&]*)(&|$)");
			boost::match_results<std::string::const_iterator> m;
			while(boost::regex_search(begin, end, m, pars, boost::regex_constants::match_default))
			{
				requestParams[std::string(m[1].first, m[1].second)] = std::string(m[2].first, m[2].second);
				begin = m[0].second;
			}
		}
	}
}

std::string CResponder::GetResponse(void)
{
	std::stringstream outStream(std::ios::in | std::ios::out | std::ios::binary);
	// Headers
	outStream << "HTTP/1.1 200 OK\nServer: AIMP2 Web Control © Dyatlov Vitaliy\nCache-Control: no-cache, must-revalidate\nPragma: no-cache\n\n";
	
	if(requestFile != "")
	{
		if(requestFile == "/" && requestParams.size() > 0) //this is parameterized request
		{
			std::map<std::string, std::string>::iterator p;
			for(p = requestParams.begin(); p != requestParams.end(); ++p)
			{
				if(p->first.compare("action") == 0)
				{
					if(p->second.compare("get_playlist_list") == 0)
					{
						outStream << GetPlaylistList();
					}
					else if(p->second.compare("get_playlist_songs") == 0)
					{
						outStream << cache->getPlaylist(StrToInt(requestParams["id"]));
					}
					else if(p->second.compare("get_playlist_crc") == 0)
					{
						outStream << cache->getCRC32(StrToInt(requestParams["id"]));
					}
					else if(p->second.compare("get_player_status") == 0)
					{
						outStream << GetPlayerStatus();
					}
					else if(p->second.compare("get_song_current") == 0)
					{
						outStream << GetCurrentSong();
					}
					else if(p->second.compare("get_volume") == 0)
					{
						outStream << ctrl->AIMP_Status_Get(AIMP_STS_VOLUME);
					}
					else if(p->second.compare("set_volume") == 0)
					{
						outStream << ctrl->AIMP_Status_Set(AIMP_STS_VOLUME, StrToInt(requestParams["volume"]));
					}
					else if(p->second.compare("set_song_play") == 0)
					{
						PlayTrack(StrToInt(requestParams["playlist"]), StrToInt(requestParams["song"]));
					}
					else if(p->second.compare("set_song_position") == 0)
					{
						SetNewSongPosition(StrToInt(requestParams["playlist"]),
							StrToInt(requestParams["song"]), StrToInt(requestParams["position"]));
					}
					else if(p->second.compare("set_player_status") == 0)
					{
						SetPlayerStatus(requestParams["statusType"], StrToInt(requestParams["value"]));
					}
					else if(p->second.compare("player_play") == 0)
					{
						player->PlayOrResume();
					}
					else if(p->second.compare("player_pause") == 0)
					{
						player->Pause();
					}
					else if(p->second.compare("player_stop") == 0)
					{
						player->Stop();
					}
					else if(p->second.compare("player_prevous") == 0)
					{
						player->PrevTrack();
					}
					else if(p->second.compare("player_next") == 0)
					{
						player->NextTrack();
					}
					else if(p->second.compare("playlist_sort") == 0)
					{
						SortPlaylist(
							StrToInt(requestParams["playlist"]),
							requestParams["sort"]
							);
					}
					else if(p->second.compare("playlist_add_file") == 0)
					{
						AddFile(
							StrToInt(requestParams["playlist"]),
							requestParams["file"]
							);
					}
					else if(p->second.compare("playlist_del_file") == 0)
					{
						DeleteFile(
							StrToInt(requestParams["playlist"]),
							StrToInt(requestParams["file"])
							);
					}
				}
			}
		}
		else //this is just request a file
		{
			requestFile = replaceAllA(requestFile, "/../", "/");
			if(requestFile == "/")
			{
				requestFile = "/index.htm";
			}
			std::ifstream in((pluginsPath + "/aimp_web_ctl" + requestFile).c_str(), std::ios::in | std::ios::binary);
			if(!in.is_open())
			{
				outStream.str("");
				outStream.clear();
				outStream << "HTTP/1.1 404 Not found\nServer: AIMP2 Web Control © Dyatlov Vitaliy\nCache-Control: no-cache, must-revalidate\nPragma: no-cache\n\n";
			}
			else
			{
				outStream << in.rdbuf();
				in.close();
			}
		}
	}

	return outStream.str();
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
		manager2->AIMP_PLS_GetName(playlistID, (PWCHAR)(ABuffer.c_str()), ABuffer.length());
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
	manager2->AIMP_PLS_Entry_GetTitle(playing_list, playing_file, (PWCHAR)pBuf.c_str(), pBuf.capacity());
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
	Strings->AddFile((PWCHAR)resultFileName.c_str(), NULL);
	ctrl->AIMP_PLS_AddFiles(playlistID, Strings);
}

void CResponder::DeleteFile(int playlistID, int fileId)
{
	manager2->AIMP_PLS_Entry_Delete(playlistID, fileId);
}