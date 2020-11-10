var trackState = null;

$("#slVol").slider({
    tooltip: 'always',
    formatter: function(value) {
        return value + '%';
    }
});
$("#slVol").on("slide", function(slideEvt) {
    //console.log(slideEvt.value);
});

$("#slVol").on("change", function(slideEvt) {
    //console.log(slideEvt.value.newValue);
});

$("#slVol").on("slideStop", function(slideEvt) {
    //console.log(slideEvt.value);
    changeVolume();
});

$("#slDur").slider({
    tooltip: 'always',
    formatter: function(value) {
        return sectostr(value);
    }
});
$("#slDur").on("slideStop", function(slideEvt) {
    //console.log(slideEvt.value);
    changePosition();
});

$('#btn_play').on('click',function(){
    if(trackState==0)
    {
        playerPlay();
        trackState = 1;
    }
    else
    {
        playerPause();
        if(trackState == 2) trackState = 1;
        else trackState = 2;
    }
    statusBar(trackState);
});

$('#btn_stop').on('click',function(){
    playerStop();
    trackState = 0;
    statusBar(trackState);
    $('#slDur').slider('setValue',0);
    $('#slDur').slider({max:0});
});

$('#btn_prev').on('click',function(){
    playerPrevous();
});

$('#btn_next').on('click',function(){
    playerNext();
});

$('#btn_shuffle').on('click',function(){
    updatePlayerStatusRandom();
});

$('#btn_repeat').on('click',function(){
    updatePlayerStatusRepeat();
});

$('#vol_up').on('click',function(){
    var  val = $('#slVol').slider('getValue');
    val++;
    $('#slVol').slider('setValue',val);
    changeVolume();
});

$('#vol_dw').on('click',function(){
    var  val = $('#slVol').slider('getValue');
    val--;
    $('#slVol').slider('setValue',val);
    changeVolume();
});


function statusBar(index) {
    index = parseInt(index);
    //console.log('statusBar ' +index);
    if(index == 1)
    {
        $('#btn_play').find("span").removeClass('glyphicon-play').addClass('glyphicon-pause');
    }
    else
    {
        $('#btn_play').find("span").removeClass('glyphicon-pause').addClass('glyphicon-play');
    }

    $('#statusBar').removeClass().addClass('alert');
    $('#statusIcon').removeClass().addClass('glyphicon');
    switch(index)
    {
        case 0 :
            $('#statusBar').addClass('alert-info');
            $('#statusIcon').addClass('glyphicon-pause');
            // stopped
            break;
        case 1 :
            $('#statusBar').addClass('alert-success');
            $('#statusIcon').addClass('glyphicon-play');
            //playing
            break;
        case 2 :
            $('#statusBar').addClass('alert-warning');
            $('#statusIcon').addClass('glyphicon-pause');
            // paused
            break;
    }

}

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

function updateVolume() {
    AimpWebCtl.getVolume(function(volume) {
        $('#slVol').slider('setValue',parseInt(volume));
    });
}

function updatePlayerStatusRepeat() {
    var val = ($('#btn_repeat').hasClass('checked'))?0:1;
    AimpWebCtl.setPlayerStatusRepeat(val, getPlayerStatus);
}

function updatePlayerStatusRandom() {
    var val = ($('#btn_shuffle').hasClass('checked'))?0:1;
    AimpWebCtl.setPlayerStatusShuffle(val, getPlayerStatus);
}

function getPlayerStatus() {
    AimpWebCtl.getPlayerStatus(function(status){

        if(status.RepeatFile == 1) {
            $('#btn_repeat').addClass('checked');
        } else {
            $('#btn_repeat').removeClass('checked');
        }
        if(status.RandomFile == 1) {
            $('#btn_shuffle').addClass('checked');
        } else {
            $('#btn_shuffle').removeClass('checked');
        }
    });
}

function getTrackPosition(timer)
{
    //console.log('getTrackPosition ' + trackState + ' '+timer)
    if(trackState != 1 && timer) return;
    AimpWebCtl.getTrackPosition(
        function(obj) {
            //console.log('getTrackPosition ret');
            //console.log(obj);
            $('#slDur').slider('setValue',parseInt(obj.position));
            $('#slDur').slider({max:parseFloat(obj.length)});
            if(obj.position<2 || parseFloat(obj.length)==parseInt(obj.position)) getCurrentSong();
        }
    );
}

function changePosition() {
    AimpWebCtl.setTrackPosition($('#slDur').slider('getValue'));
}

function changeVolume() {
    AimpWebCtl.setVolume($('#slVol').slider('getValue'));
}

function getCurrentSong() {
    AimpWebCtl.getCurrentSong(
        function(obj) {
            //console.log(obj);
            $('#info_track').html(obj.PlayingFileName);
            $('#info_duration').html(sectostr(obj.length));
        }
    );
}

function getTrackState() {
    AimpWebCtl.getTrackStatus(function(obj){
        if(trackState == obj) return;
        trackState = obj;

        statusBar(trackState);
    });
}


function getCustomStatus()
{
    var val = 4;
    AimpWebCtl.getCustomStatus(val,function(obj){
        console.log('getCustomStatus '+val+ ' = '+obj);
    });
}

$(document).ready(function(){
    AimpWebCtl.getUpdateTime(function(secs){
        setInterval("init()", secs * 1000);
    });
    getTrackPosition(false);
    init();
    setInterval("getTrackPosition(true)", 1 * 1000);
});

function init()
{
    updateVolume();
    getPlayerStatus();
    getCurrentSong();
    //getCustomStatus();
    getTrackState();
    getTrackPosition(false);
}

function sectostr(value)
{
    var s = value % 60;
    var h = Math.floor(value / 3600);
    var m = Math.floor((value - (h * 3600)) / 60);
    return (h?h+':':'')+(h?(m<10?'0'+m:m):m)+':'+(s<10?'0'+s:s);
}
