#include "StdAfx.h"
#include "AIMPCache.h"

AIMPCache::AIMPCache(IAIMP2Controller *AIMP, DWORD sleepTime)
{
	this->sleepTime = sleepTime;
	this->AIMP = NULL;
	this->manager2 = NULL;
	this->player = NULL;
	this->extended = NULL;

	this->AIMP = AIMP;
	AIMP->AIMP_QueryObject(IAIMP2PlaylistManager2ID, &manager2);
	AIMP->AIMP_QueryObject(IAIMP2PlayerID, &player);
	AIMP->AIMP_QueryObject(IAIMP2ExtendedID, &extended);

	WCHAR buffer[256];
	int result = extended->AIMP_GetPath(AIMP_CFG_PLUGINS, buffer, 256);
	this->pluginsPath = ToUtf8(std::wstring(buffer, result));
}

AIMPCache::~AIMPCache(void)
{
}

void AIMPCache::UpdatePlaylist(int playlistID)
{//TODO: here must be upgradable lock
	{
		boost::unique_lock<boost::shared_mutex> lock(m_mutex);

		CacheObj source = cacheList[playlistID];

		if(GetTickCount() - source.cacheTime > this->sleepTime)
		{
			if(cacheList.size() > 20)
			{
				cacheList.empty();
			}

			CacheObj obj;
			obj.playlist = GetPlaylistSongs(playlistID);
			obj.crc32 = GetPlaylistCRC(source.playlist);
			obj.cacheTime = GetTickCount();
			cacheList[playlistID] = obj;
		}
	}
}

std::string AIMPCache::GetPlaylistSongs(int playListID)
{
	std::stringstream answer;
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
			manager2->AIMP_PLS_Entry_GetTitle(playListID, i, (PWCHAR)pBuf.c_str(), pBuf.capacity());
			manager2->AIMP_PLS_Entry_InfoGet(playListID, i, &fileInfo);
			pBuf = replaceAllW(pBuf, L"\"", L"\\\"");
			answer << "{\"name\":\"" << ToUtf8(pBuf) << "\",\"length\":\"" << fileInfo.nDuration << "\"},";
		}
		if(fileCount > 0)
			answer.seekp((int)(answer.tellp()) - 1);
		answer << "]";
	}
	answer << "}";
	return answer.str();
}

std::string AIMPCache::GetPlaylistCRC(const std::string& playlistSongs)
{
	boost::crc_32_type crc_result;
	crc_result.process_bytes(playlistSongs.c_str(), playlistSongs.length());
	std::stringstream result;
	result << std::hex << std::uppercase << crc_result.checksum();
	return result.str();
}

std::wstring AIMPCache::replaceAllW(
	std::wstring result, 
	const std::wstring& replaceWhat, 
	const std::wstring& replaceWithWhat)
{
	int pos = 0;
	while(1)
	{
		pos = result.find(replaceWhat, pos);
		if (pos==-1) break;
		result.replace(pos,replaceWhat.size(),replaceWithWhat);
	}
	return result;
}

std::string AIMPCache::ToUtf8(const std::wstring& widestring)
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

std::string AIMPCache::getCRC32(int playlistID)
{
	std::string result;
	if(GetTickCount() - cacheList[playlistID].cacheTime > this->sleepTime)
		UpdatePlaylist(playlistID);
	
	{
		boost::shared_lock<boost::shared_mutex> lock(m_mutex);
		result = cacheList[playlistID].crc32;
	}

	return result;
}

std::string AIMPCache::getPlaylist(int playlistID)
{
	std::string result;
	
	{
		boost::shared_lock<boost::shared_mutex> lock(m_mutex);
		result = cacheList[playlistID].playlist;
	}

	return result;
}

std::string AIMPCache::getPluginsPath()
{
	return pluginsPath;
}

IAIMP2Controller *AIMPCache::getAIMP()
{
	return AIMP;
}

IAIMP2PlaylistManager2 *AIMPCache::getManager2()
{
	return manager2;
}

IAIMP2Player *AIMPCache::getPlayer()
{
	return player;
}

IAIMP2Extended *AIMPCache::getExtended()
{
	return extended;
}