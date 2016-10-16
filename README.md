AIMP Web Control plug-in
========================

What is it
----------

Plug-in for remote control music player [AIMP][] over network.
Provides very wide API support for 3rd party apps over http, provides player control over web-browser and much more!
Minimal supported version of player is 2.6 beta 2 build 485

Why you need it
---------------

Say, you are in an office. You have only one computer which can play music (one with dynamics). In the office, say, 5 employeers. You have AIMP on music-comp, AIMP is playing.. You got phone call - without this plugin you should ask others to stop music, or run yourself to this comp. Having this plug-in you can simply decrease volume, or stop/pause player. Moreover, if you dont like some song, you can simply switch song to another, you can sort songs, can add and remove songs, you can do a lot of things that AIMP can do, except that \*you do it in a web-browser and do it remotely\*!

Technical details
-----------------

 * Written on C++
 * Latest versions of plug-in use [mongoose][] server as web back-end, thanks mongoose!
 * To build plug-in from sources you should have Microsoft C++ compiler and Windows SDK installed, use _build-mscl.bat_. Alternative is to use [mingw][] - crossplatform compiler suite. In that case you should use _build-mingw.sh_
 * Although AIMP is windows-only player, AIMP Web Control can be built on linux system with mingw (use _build-mingw.sh_ for that)

How To Add Authorization
------------------------

 * In directory aimp_web_ctl create file .htpasswd
 * Write to file: vit:mydomain.com:ea91ff01a238d84ffac5f0e709b2c393
 * Authorization is done :)

vit - user name, replace it with yours

mydomain.com - just a sample domain which will be displayed in popup auth dialog, you can leave it as is

ea91ff01a238d84ffac5f0e709b2c393 - md5 from string 'vit:mydomain.com:123456', where 123456 is your password - you can write here your own md5. Good m5 generator is here: <http://www.md5.cz/>

API Documentation
-----------------

https://github.com/dyatlov/aimp-web-ctl/raw/master/AIMPWebControlRemoteAPIv1.1.pdf

Projects using AIMP Web Control
-------------------------------

 * [AIMP Remote][]
 * [AIMP Mobile Web Control][]
 * [Aimp UControl][]
 * [Aimp Remote LITE][]

Contact with author
-------------------

  * [twitter][]
  * [email][]
  * [Facebook][]
  * Skype: vdiatlov
  * https://plus.google.com/111345752845055800926/?prsrc=3

  [AIMP]: http://aimp.ru
  [mongoose]: http://code.google.com/p/mongoose
  [mingw]: http://www.mingw.org
  [AIMP Remote]: http://milksplash.de/node/10
  [AIMP Mobile Web Control]: https://github.com/cheeaun/aimp-mobile-web-ctl
  [Aimp UControl]: http://4pda.ru/forum/index.php?showtopic=239898
  [Aimp Remote LITE]: https://market.android.com/details?id=banamalon.remote.aimp.lite
  [http://twitter.com/]: http://twitter.com/
  [email]: mailto:md.xytop@gmail.com
  [Facebook]: http://www.facebook.com/profile.php?id=100000497200312
  [https://plus.google.com/11134]: https://plus.google.com/11134
  [twitter]: http://twitter.com/#!/xytop
  
========================================================================
2016-10-16 Modified by Ekilei
-----------------------------
  Adaptive Web Design for Desktop & Mobile
 
  [bootstrap]: http://getbootstrap.com
  [bootstrap-slider]: https://github.com/seiyria/bootstrap-slider
  
  Added functions:
  - setTrackPosition
  - getTrackPosition
  - getCustomStatus
  - getTrackStatus 
  
  [email]: mailto:ekilei@gmail.com 