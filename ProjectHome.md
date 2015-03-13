# AIMP Web Control plug-in #

## Donate ##
[If you like this plugin - PLEASE DONATE](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=Z5LPQWABFXRX6&lc=US&item_name=AIMP%20Web%20Control&currency_code=USD&bn=PP%2dDonationsBF%3abtn_donateCC_LG%2egif%3aNonHosted)

## What is it ##
Plug-in for remote control music player [AIMP](http://aimp.ru) over network.<br>
Provides very wide API support for 3rd party apps over http, provides player control over web-browser and much more!<br>
Minimal supported version of player is 2.6 beta 2 build 485<br>
<br>
<h2>Why you need it</h2>
Say, you are in an office. You have only one computer which can play music (one with dynamics). In the office, say, 5 employeers. You have AIMP on music-comp, AIMP is playing..<br>
You got phone call - without this plugin you should ask others to stop music, or run yourself to this comp. Having this plug-in you can simply decrease volume, or stop/pause player. Moreover, if you dont like some song, you can simply switch song to another, you can sort songs, can add and remove songs, you can do a lot of things that AIMP can do, except that <b>you do it in a web-browser and do it remotely</b>!<br>
<br>
<h2>Technical details</h2>
<ul><li>Written on C++<br>
</li><li>Latest versions of plug-in use <a href='http://code.google.com/p/mongoose'>mongoose</a> server as web back-end, thanks mongoose!<br>
</li><li>To build plug-in from sources you should have Microsoft C++ compiler and Windows SDK installed, use <i>build-mscl.bat</i>. Alternative is to use <a href='http://www.mingw.org'>mingw</a> - crossplatform compiler suite. In that case you should use <i>build-mingw.sh</i>
</li><li>Although AIMP is windows-only player, AIMP Web Control can be built on linux system with mingw (sources include <i>build-mingw.sh</i> for that)</li></ul>

<h2>How To Add Authorization</h2>
<ul><li>In directory aimp_web_ctl create file .htpasswd<br>
</li><li>Write to file: vit:mydomain.com:ea91ff01a238d84ffac5f0e709b2c393<br>
</li><li>Authorization is done :)<br>
vit - user name, replace it with yours</li></ul>

mydomain.com - just a sample domain which will be displayed in popup auth dialog, you can leave it as is<br>
<br>
ea91ff01a238d84ffac5f0e709b2c393 - md5 from string 'vit:mydomain.com:123456', where 123456 is your password - you can write here your own md5. Good m5 generator is here: <a href='http://www.md5.cz/'>http://www.md5.cz/</a>

<h2>API Documentation</h2>
<a href='http://code.google.com/p/aimp-web-ctl/downloads/detail?name=AIMPWebControlRemoteAPI.pdf&can=2&q='>AIMP Web Control Remote API</a>

<h2>Projects using AIMP Web Control</h2>
<ul><li><a href='http://milksplash.de/node/10'>AIMP Remote</a>
</li><li><a href='https://github.com/cheeaun/aimp-mobile-web-ctl'>AIMP Mobile Web Control</a>
</li><li><a href='http://4pda.ru/forum/index.php?showtopic=239898'>Aimp UControl</a>
</li><li><a href='https://market.android.com/details?id=banamalon.remote.aimp.lite'>Aimp Remote LITE</a></li></ul>

<h2>Contact with author</h2>
<ul><li><a href='http://twitter.com/#!/xytop'>twitter</a>
</li><li><a href='mailto:md.xytop@gmail.com'>email</a>
</li><li><a href='http://www.facebook.com/profile.php?id=100000497200312'>Facebook</a>
</li><li>Skype: vdiatlov<br>
</li><li>Jabber: dyatlov@jabber.org<br>
</li><li>ICQ/AOL: 488880111<br>
</li><li><a href='https://plus.google.com/111345752845055800926/?prsrc=3'><div><div><img src='https://ssl.gstatic.com/images/icons/gplus-64.png' width='64' height='64'></img></div><div>AIMP Web Control</div><div> on Google+ </div></div></a>