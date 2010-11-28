REM Uncomments 2 lines below to set-up your compiler ENV vars
REM "c:\Program Files\Microsoft Visual C++ Toolkit 2003\vcvars32.bat"
REM "C:\Program Files\Microsoft Platform SDK\SetEnv.Cmd"

erase "*.obj"
erase "*.lib"
erase "*.exp"
erase "*.dll"

cl.exe /Gz /Oi /Gy /EHsc /LD /o aimp_web_ctl.dll dllmain.cpp simpleserver.cpp dealer.cpp Responder.cpp CRC32.cpp mongoose.c /link /OPT:WIN98 /OPT:REF /DEF:"aimp_web_ctl.def" kernel32.lib user32.lib ws2_32.lib

erase "*.obj"
erase "*.lib"
erase "*.exp"
