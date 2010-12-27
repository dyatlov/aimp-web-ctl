erase "*.obj"
erase "*.lib"
erase "*.exp"
erase "*.dll"

cl.exe /Gz /Oi /Gy /EHsc /LD /o aimp_web_ctl.dll dllmain.cpp simpleserver.cpp dealer.cpp Responder.cpp CRC32.cpp ConfigFile.cpp mongoose.c /link /OPT:REF /DEF:"aimp_web_ctl.def" kernel32.lib user32.lib ws2_32.lib shlwapi.lib

erase "*.obj"
erase "*.lib"
erase "*.exp"
