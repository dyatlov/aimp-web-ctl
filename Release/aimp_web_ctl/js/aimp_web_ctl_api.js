var AimpWebCtl = {

    /**
     * Add file to the specific playlist
     *
     * @param playlistID - ID of playlist
     * @param filePath - full path to the file, relative to host machine
     * @param onCompleteFunc - execute, when file added
     */
    addFile: function(playlistID, filePath, onCompleteFunc) {
        $.ajax({
            type: "GET",
            url: "/",
            data: "action=playlist_add_file&playlist=" + playlistID + "&file=" + filePath,
            success: function(){
                onCompleteFunc();
            }
          });
    },

    /**
     * Delete file from the specific playlist
     *
     * @param playlistID - ID of playlist
     * @param fileId - ID of the file (number in playlist)
     * @param onCompleteFunc - execute, when file deleted
     */
    deleteFile: function(playlistID, fileId, onCompleteFunc) {
        $.ajax({
            type: "GET",
            url: "/",
            data: "action=playlist_del_file&playlist=" + playlistID + "&file=" + fileId,
            success: function(){
                onCompleteFunc();
            }
          });
    },

    /**
     * Set new volume
     *
     *  @param newVolume: 0..100
     */
    setVolume: function(newVolume) {
        $.ajax({
            type: "GET",
            url: "/",
            data: "action=set_volume&volume=" + newVolume
          });
    },

    /**
     * Get current volume
     *
     * @param onCompleteFunc function, where will be current volume
     */
    getVolume: function(onCompleteFunc) {
        $.ajax({
            type: "GET",
            url: "/",
            data: "action=get_volume",
            dataType: "text",
            success: function(volume){
                onCompleteFunc(volume);
            }
          });
    },

    /**
     * Get current player status
     * 
     * @param onCompleteFunc function, where will be current status
     * Status is object with these fields:
     *   RepeatFile - 1 if repeat file, 0 otherwise
     *   RandomFile - 1 if random file, 0 otherwise
     */
    getPlayerStatus: function(onCompleteFunc) {
        $.ajax({
            type: "GET",
            url: "/",
            data: "action=get_player_status",
            dataType: "json",
            success: function(status){
                onCompleteFunc(status);
            }
          });
    },

    /**
     * Set repeating song status, 1 - repeat, 0 no repeat
     */
    setPlayerStatusRepeat: function(value, onCompleteFunc) {
        $.ajax({
            type: "GET",
            url: "/",
            data: "action=set_player_status&statusType=repeat&value=" + value,
            success: onCompleteFunc
          });
    },

    /**
     * Set random song status, 1 - random, 0 no random
     */
    setPlayerStatusShuffle: function(value, onCompleteFunc) {
        $.ajax({
            type: "GET",
            url: "/",
            data: "action=set_player_status&statusType=shuffle&value=" + value,
            success: onCompleteFunc
          });
    },

    /**
     * Sort playlist
     *
     * @param playlistID - identifier of playlist
     * @param sortType one of: title, filename, duration, artist, inverse, randomize
     * @param onCompleteFunc function to execute when sorting completed
     */
    sortPlaylist: function(playlistID, sortType, onCompleteFunc) {
        $.ajax({
            type: "GET",
            url: "/",
            data: "action=playlist_sort&playlist=" + playlistID + "&sort=" + sortType,
            dataType: "text",
            success: function(){
                onCompleteFunc();
            }
          });
    },

    /**
     * Get current playing playlist id, playing song number and song name
     *
     * Object fields:
     *   PlayingList - playing list id
     *   PlayingFile - playing file number
     *   PlayingFileName - name of playing file
     *
     * @return object
     */
    getCurrentSong: function(onCompleteFunc) {
        //get_song_current
        $.ajax({
            type: "GET",
            url: "/",
            data: "action=get_song_current",
            dataType: "json",
            success: function(obj){
                onCompleteFunc(obj);
            }
          });
    },

    /**
     * Sends specified action to plugin
     * @param action - action name to send
     */
    sendAction: function(action) {
        $.ajax({
            type: "GET",
            url: "/",
            data: "action=" + action
          });
    },

    /**
     * Starts playing
     */
    playerPlay: function() {
        AimpWebCtl.sendAction("player_play");
    },

    /**
     * Stops playing
     */
    playerStop: function() {
        AimpWebCtl.sendAction("player_stop");
    },

    /**
     * Pausing playing
     */
    playerPause: function() {
        AimpWebCtl.sendAction("player_pause");
    },

    /**
     * Prevous playing
     */
    playerPrevous: function() {
        AimpWebCtl.sendAction("player_prevous");
    },

    /**
     * Next playing
     */
    playerNext: function() {
        AimpWebCtl.sendAction("player_next");
    },

    /**
     * @name setNewSongPosition(playListID, songNum, songPos)
     * @description Changes position of the song
     *
     * @param playListID - ID of requested playlist
     * @param songNum - number of Song for playing
     * @param songPos - new position of song
     */
    setNewSongPosition: function(playListID, songNum, songPos) {
        if(playListID == null)
            playListID = 0;
        $.ajax({
            type: "GET",
            url: "/",
            data: "action=set_song_position&playlist=" + playListID + "&song=" + songNum + "&position=" + songPos
          });
    },
    /**
     * @name playSound(playListID, songNum)
     * @description Plays selected sound
     *
     * @param playListID - ID of requested playlist
     * @param songNum - number of Song for playing
     */
    playSound: function(playListID, songNum) {
        if(playListID == null)
            playListID = 0;
        $.ajax({
            type: "GET",
            url: "/",
            data: "action=set_song_play&playlist=" + playListID + "&song=" + songNum
          });
    },
    /**
     * @name getPlayListCRC(playListID)
     * @description Get CRC32 of requested playlist.
     * Its usefull for check playlist as new or old.
     * 
     * @param playListID - ID of requested playlist
     * @param onCompleteFunc - function for accepting completed result
     * @return CRC32 of playlist (example: 76E3E705)
     */
    getPlayListCRC: function(playListID, onCompleteFunc) {
        if(playListID == null)
            playListID = 0;
        $.ajax({
            type: "GET",
            url: "/",
            data: "action=get_playlist_crc&id=" + playListID,
            dataType: "text",
            success: function(crc){
                onCompleteFunc(crc);
            }
          });
    },
    /**
     * @name getPlayListSongs(playListID)
     * @description Get array of songs.
     *
     * @param playListID - ID of requested playlist
     * @param onCompleteFunc - function for accepting completed result
     * @return Array of songs. Every item is object with these fields:
     *  name (name of song), length (length of song in miliseconds )
     */
    getPlayListSongs: function(playListID, onCompleteFunc) {
        if(playListID == null)
            playListID = 0;
        $.ajax({
            type: "GET",
            url: "/",
            data: "action=get_playlist_songs&id=" + playListID,
            dataType: "json",
            success: function(obj){
                if(obj.status == "OK") {
                    onCompleteFunc(obj.songs);
                } else {
                    onCompleteFunc(null);
                }
            }
          });
    },
    /**
     * @name getPlayListList()
     * @description Returns array of current playlists
     *
     * @param onCompleteFunc - function for accepting completed result
     * @return Array of playlists. Every item consists these fields:
     *  id - identifier of the playlist,
     *  duration - summary duration in milliseconds of songs in playlist,
     *  size - summary size in bytes of songs in playlist,
     *  name - name of playlist
     */
    getPlayListList: function(onCompleteFunc) {
        $.ajax({
            type: "GET",
            url: "/",
            data: "action=get_playlist_list",
            dataType: "json",
            success: function(ar){
                onCompleteFunc(ar);
            }
          });
    }
}

