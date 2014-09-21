SMTube for AmigaOS 4.1


SMTube for AmigaOS4 is a port of SMTube for AmigaOS 4 computers (aka Amiga-NG, successor of popular Classic Commodore Amiga) 


A) Building 

Prerequisites : 

- AmigaOS4 SDK (Hyperion website) 

- Qt port from Alfkil Thorbjørn Wennermark (OS4Depot) 


1- Download the current svn sources : 

svn co http://subversion.assembla.com/svn/smplayer/smtube/trunk/ smtube 

2- in the SMTube svn drawer, in a CLI window, type : 

sh 

./get_svn_revision.sh to build the svn version file used in version.cpp 

./get_amiga_date.sh to build the amiga date file for AmiUpdate used in version.cpp 

cd src 

qmake *to build the Qt project file*

gmake *to build SMTube*

lrelease smtube.pro *to build the translations files. Cancel Requesters if needed*

delete translations/*.ts

cd Amiga 

rx Populate_info.rexx *to copy the .info file for SMTube executable, Amiga and Translations drawers*


B) Usage ? 

Before launching SMTube, review Amiga/PlayVideo.rexx file to 

1- specify the location of your MPlayer binary (stream support needed, MUIMplayer or SDLMplayer) 
example: MPLAYER = "APPDIR:MPlayer" 

2- optionally, add options to MPlayer 
For example: MPLAYER_OPTIONS = "-vo cgx_wpa -cache 8192"  for MUIMplayer on NON overlay compatible system. 

3- optionally, specify the position of the output console 
For example: MPLAYER_CONSOLE = "1450/975/465/99"  to get a log console in the bottom right corner on a 1920x1080 screen 

4- optionally, specify the number of seconds that the Console will be kept opened after MPlayer has started. 
Could be useful to debug if the streaming failed. 
SECONDS = 10 

5- when downloading an "audio stream" an extra step is required as M4A DASH is not supported by AmigaAmp or TuneNet
	
	APPDIR:ffmpeg -i "input.m4a" -b:a 128k "output.MP3"

Many thanks to : 

Ricardo 'rvm' Villalba for his SMTube program and for having included AmigaOS4 modifications 

Alfkil Thorbjørn Wennermark for the AmigaOS 4.1 port of Qt


  
Guillaume 'zzd10h' Boesel 
2014/09/20 