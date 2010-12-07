var AWC_DVY_PLAYLIST_CRC = '0';
var AWC_DVY_PLAYLIST_CURRENT = -1;
var AWC_DVY_PLAYLIST_PLAYING = -1;
var AWC_DVY_SONG_PLAYING = -1;

function playerPlay() {
    AimpWebCtl.playerPlay();
}

function playerPause() {
    AimpWebCtl.playerPause();
}

function playerStop() {
    AimpWebCtl.playerStop();
}

function playerPrevous() {
    AimpWebCtl.playerPrevous();
    getCurrentSong();
}

function playerNext() {
    AimpWebCtl.playerNext();
    getCurrentSong();
}

function addFile() {
    val = prompt("Enter full path to the file:");
    if((val == null) || (val == ""))
        return;
    AimpWebCtl.addFile(AWC_DVY_PLAYLIST_CURRENT, encodeURIComponent(val), function(){
        updatePlayList(AWC_DVY_PLAYLIST_CURRENT);
        hoverSong();
    });
}

function delFile(id) {
    if(!confirm('Confirm delete please'))
        return;
    AimpWebCtl.deleteFile(AWC_DVY_PLAYLIST_CURRENT, id, function(){
        updatePlayList(AWC_DVY_PLAYLIST_CURRENT);
        hoverSong();
    });
}

function niceTime(len) {
  min = (len / (1000 * 60)) | 0;
  sec = ((len / 1000) % 60) | 0;
  return ((min/10)|0) + '' + ((min%10)|0)+':'+((sec/10)|0) + '' + ((sec%10)|0);
}

function playSound(playListID, songNum) {
    AimpWebCtl.playSound(playListID, songNum);
    getCurrentSong();
}

function sortPlaylist() {
    AimpWebCtl.sortPlaylist(AWC_DVY_PLAYLIST_CURRENT, $('#sort-type').val(), function(){
        updatePlayList(AWC_DVY_PLAYLIST_CURRENT);
        hoverSong();
    });
}

function goToSong(id) {
    if(id > 11) {
        if((id -= 11) < 2)
            id = 2;
    } else {
        id = -1;
    }
    jQuery.scrollTo( 'a[name=song'+id+']', 1000);
}

function hoverSong() {
    if(AWC_DVY_PLAYLIST_PLAYING == AWC_DVY_PLAYLIST_CURRENT) {
        $('.song-current-playing').attr('class', '');
        $('#playlist-table_'+AWC_DVY_SONG_PLAYING).attr('class', 'song-current-playing');
    }
}

function moveToCurrentSong() {
    if(AWC_DVY_PLAYLIST_CURRENT != AWC_DVY_PLAYLIST_PLAYING) {
        songPlaying = AWC_DVY_SONG_PLAYING;
        updatePlayList(AWC_DVY_PLAYLIST_PLAYING, function(){
           goToSong(songPlaying);
           hoverSong();
        });
    }
    else {
        goToSong(AWC_DVY_SONG_PLAYING);
    }
}

function getCurrentSong() {
    AimpWebCtl.getCurrentSong(
        function(obj) {
            if(obj == null)
                return;
            AWC_DVY_PLAYLIST_PLAYING = obj.PlayingList;
            AWC_DVY_SONG_PLAYING = obj.PlayingFile;
            $('.playlist-list-name.playing').attr('class', 'playlist-list-name');
            $('#playlist'+obj.PlayingList+':not(.selected)').attr('class', 'playlist-list-name playing');

            hoverSong();

            $('#song-current-playing').html(obj.PlayingFileName);
        }
    );
}

function updatePlayListSongs(playListID, OnUpdateFunc, nocache) {
    AimpWebCtl.getPlayListSongs(playListID, function(songs) {
        if(songs == null) return;
        var playListContent = '<ul id="playlist-table">';
        for(i = 0; i < songs.length; i++) {
          playListContent += '<li id="playlist-table_'+i+'"><table cellpadding="0" cellspacing="0"><tr>'
              +'<td class="playlist-song-delete"><a href="#delete" onclick="delFile('+i+');return false;">X</a></td>'
              +'<td class="playlist-song-num">'
              +(i+1)+'</td><td class="playlist-song-name">'+
              '<a name="song'+i+'"></a><a href="#play" onclick="playSound('+playListID+','+i+');return false;">'+
              songs[i].name+
              '</a>'+
              '</td><td class="playlist-song-length">'+niceTime(songs[i].length)+
              '</td></tr></table></li>';
        }
        playListContent += '</ul>';
        $('#playlist-place').html(playListContent);

        $('#playlist-table').sortable({
            axis: 'y',
            cursor: 'crosshair',
            delay: 100,
            opacity: 0.6,
            scroll: true,
            update: function(event, ui) {
                source_N = /_(\d+)/.exec($(ui.item).attr('id'))[1];
                dest_N = /_(\d+)/.exec(
                    $('#'+$(ui.item).attr('id')+' + '+'li').attr('id')
                )[1];
                AimpWebCtl.setNewSongPosition(AWC_DVY_PLAYLIST_CURRENT, source_N, dest_N)
                updatePlayList(AWC_DVY_PLAYLIST_CURRENT);
            }
        }, nocache);
        if(OnUpdateFunc != null)
            OnUpdateFunc();
    });
}

function updatePlayList(playListID, OnUpdateFunc, cache) {
    if(OnUpdateFunc == null)
        OnUpdateFunc = getCurrentSong;
        
    var locNoCache = cache != true;

    AimpWebCtl.getPlayListCRC(playListID, function(crc) {
        if(crc != AWC_DVY_PLAYLIST_CRC) {
            AWC_DVY_PLAYLIST_CRC = crc;
            $('.playlist-list-name.selected').attr('class', 'playlist-list-name');
            $('#playlist'+playListID).attr('class', 'playlist-list-name selected');
            $('#playlist-list-table').val(playListID);

            AWC_DVY_PLAYLIST_CURRENT = playListID;
            updatePlayListSongs(playListID, OnUpdateFunc, locNoCache);
        }
    }, locNoCache);
}

function updateVolume() {
    AimpWebCtl.getVolume(function(volume) {
        $('#volume-slider').slider('option', 'value', volume);
    });
}

function updatePlayerStatusRepeat() {
    val = ($('#repeat-file-status').attr('src') == '/img/file-repeat.png')?0:1;
    AimpWebCtl.setPlayerStatusRepeat(val, getPlayerStatus);
}

function updatePlayerStatusRandom() {
    val = ($('#random-file-status').attr('src') == '/img/file-random.png')?0:1;
    AimpWebCtl.setPlayerStatusShuffle(val, getPlayerStatus);
}

function getPlayerStatus() {
    AimpWebCtl.getPlayerStatus(function(status){
        if(status.RepeatFile == 1) {
            $('#repeat-file-status').attr('src', '/img/file-repeat.png');
            $('#repeat-file-status').attr('title', 'Song is repeating');
        } else {
            $('#repeat-file-status').attr('src', '/img/file-norepeat.png');
            $('#repeat-file-status').attr('title', 'Song is not repeating');
        }
        if(status.RandomFile == 1) {
            $('#random-file-status').attr('src', '/img/file-random.png');
            $('#random-file-status').attr('title', 'Song is random');
        } else {
            $('#random-file-status').attr('src', '/img/file-norandom.png');
            $('#random-file-status').attr('title', 'Song is not random');
        }
    });
}

function updatePlayListList(useCache) {
    var locUseCache = useCache;

    AimpWebCtl.getPlayListList(
        function(ar) {
            if(ar == null) return;
            var currentPlayListExist = 0;
            var playListListContent = '';
            for(i = 0; i < ar.length; i++) {
                
              if((AWC_DVY_PLAYLIST_CURRENT == -1) && (i == 0))
                  AWC_DVY_PLAYLIST_CURRENT = ar[i].id;

              if(ar[i].id == AWC_DVY_PLAYLIST_CURRENT)
                  currentPlayListExist = 1;

              if(((AWC_DVY_PLAYLIST_CURRENT == -1) && (0 == i)) || (AWC_DVY_PLAYLIST_CURRENT == ar[i].id))
                  playListListContent += '<option id="playlist'+ar[i].id+'" value="'+ar[i].id+'" class="playlist-list-name selected" selected="selected">';
              else
                playListListContent += '<option id="playlist'+ar[i].id+'" value="'+ar[i].id+'" class="playlist-list-name">';

              playListListContent += ar[i].name + '</option>';
            }

            if(playListListContent != $('#playlist-list-table').html().replace(' playing', ''))
            {
	            $('#playlist-list-table').html(playListListContent);
	    }

            if(currentPlayListExist == 0) {
                if(ar.length > 0)
                    AWC_DVY_PLAYLIST_CURRENT = ar[0].id;
                else
                    AWC_DVY_PLAYLIST_CURRENT = -1;
            }

            if(AWC_DVY_PLAYLIST_CURRENT != -1)
                updatePlayList(AWC_DVY_PLAYLIST_CURRENT, function() { getCurrentSong(); }, locUseCache);
        }
    );

    getCurrentSong();
    updateVolume();
    getPlayerStatus();
}

function changeVolume() {
    AimpWebCtl.setVolume($('#volume-slider').slider('option', 'value'));
}

$(
  function() {
    $("#volume-slider").slider({
        min: 0,
        max: 100,
        step: 1,
        stop: function(event, ui) {
            changeVolume();
        }
    });
    updatePlayListList(true);
    
    AimpWebCtl.getUpdateTime(function(secs){
    	setInterval("updatePlayListList(true)", secs * 1000);
    });
  }
);
