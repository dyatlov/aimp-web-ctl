C_COMPILER=i586-mingw32msvc-gcc
CC_COMPILER=i586-mingw32msvc-g++

MG_HEADERS=mongoose.h
MG_SOURCES=mongoose.c

AWC_HEADERS="AIMP2_SDK.h CRC32.h dealer.h Responder.h simpleserver.h ConfigFile.h"
AWC_SOURCES="dllmain.cpp AIMP2WebControlHeader.hpp simpleserver.cpp dealer.cpp Responder.cpp CRC32.cpp MultiReaderSingleWriter.hpp ConfigFile.cpp"

clean_obj() {
	echo "cleaning files"
	rm -f *.o
	rm -f *.gch
}

clean_dll() {
	echo "Removing dll files"
	rm -f *.dll
}

clean_obj
clean_dll

echo "Building mongoose server\n"
$C_COMPILER -c $MG_SOURCES -std=c99 

if [ ! -f mongoose.o ]; then
	echo "Error building mongoose. Stoping\n"
	clean_obj
	exit
fi

echo "\nBuilding aimp_web_ctl plugin\n"

$CC_COMPILER -o aimp_web_ctl.dll -mdll -mms-bitfields -mwin32 -Wl,--enable-runtime-pseudo-reloc,-s,--no-demangle,--kill-at -W -Wall -Wcast-align -Wcast-qual -Wconversion -Werror -Wfloat-equal -Wformat -Winline -Wmissing-braces -Woverloaded-virtual -Wparentheses -Wpointer-arith -Wredundant-decls -Wreorder -Wreturn-type -Wshadow -Wsign-compare -Wswitch -Wunused -Wwrite-strings -Wchar-subscripts -Wformat-security -Wunknown-pragmas -Wundef -Wformat-nonliteral -Wno-unused-parameter -Wno-conversion $AWC_HEADERS $AWC_SOURCES mongoose.o -lws2_32 -lshlwapi

if [ -f aimp_web_ctl.dll ]; then
	echo "\nPlug-in was built successfully!\n"
fi

clean_obj
