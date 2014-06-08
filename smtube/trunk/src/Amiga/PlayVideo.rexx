/* REXX */
parse arg type url

/* UPDATE THESE PARAMETERS IF NEEDED */

/* TAKE CARE TO SPECIFY THE PATH OF MUI-MPlayer OR SDL MPlayer SVN-r32561 TO ALLOW STREAMING IN OPTION MPLAYER = "" */  
/* Others players or, even, other versions of MPlayer are NOT supported */  


MPLAYER_OPTIONS = ""					
DEBUG = ON
WAIT = OFF
SECONDS = 3
MPLAYER = "APPDIR:MPlayer" 
MPLAYER_CONSOLE = "0/400/640/400" 
/*MPLAYER_CONSOLE = "1450/975/465/99" */	/* console in lower right corner ONLY for 1920x1080 screen */
/*MPLAYER_CONSOLE = "9999/9999/400/100" */	/* console in lower right corner ONLY for 1920x1080 screen */

/* END OF UPDATABLES PARAMETERS */

/* examples */

/* MPLAYER = "APPDIR:MPlayer_SDL_1.1_Altivec_cache" */
/* optimisations from K-L (compatible with MUI-MPlayer Overlay and SDL-MPlayer) */ 
/*MPLAYER_OPTIONS = "-cache 8192 -cache-min 20 -autoq 100 -autosync 30" */

/* optimisations from K-L (compatible with MUI-MPlayer NON-Overlay) */ 
/*MPLAYER_OPTIONS = "-vo cgx_wpa -cache-min 20 -autoq 100 -autosync 30" */ 

/* DON'T MODIFY BELOW THESE LINES */
/* DON'T MODIFY BELOW THESE LINES */
/* DON'T MODIFY BELOW THESE LINES */
/* DON'T MODIFY BELOW THESE LINES */


/* for SDL-MPlayer 4.2.4 complaining about a HTTP: assign at startup */
ADDRESS COMMAND "ASSIGN http: RAM:"


if type = "file",		/* pour lire les vidéos déjà téléchargées */
then do
	url = strip(url)
	url = right(url,length(url)-1)
	A = pos("/",url)
	if A > 0,
	then do
 	 url = OVERLAY(':',url,A)
	end

	url = INSERT('"',url)
	url = INSERT('"',url,length(url))

	type = substr(url,lastpos(".",url)+1,3)

end

if DEBUG = ON, 
then do
	/* console open for debug log */
	call close STDOUT		
	/*open(STDOUT, 'con:0/400/640/400/smTube AmigaOS4 console/CLOSE',W) */
	open(STDOUT, 'con:'MPLAYER_CONSOLE'/smTube AmigaOS4 console/CLOSE',W) 

	say "URL = "url

	say "Mplayer options ="MPLAYER_OPTIONS 

	ADDRESS COMMAND MPLAYER MPLAYER_OPTIONS url

	SAY " "	
	SAY "-------------------------  smTube AmigaOS4  -------------------------"
	SAY " "
	SAY "If you encounter playback problems, check MPlayer option (MPLAYER_OPTIONS = " ") in Amiga\PlayVideo.rexx."
	SAY " "
	SAY "Streaming seems to work only with MUI-Mplayer (MPlayer UNKNOWN-4.4.3 (C) 2000-2010 MPlayer Team)"
	SAY "or SDL MPlayer SVN-r32561-snapshot-4.2.4"
	SAY " "
	SAY "This DEBUG console can be turned OFF in Amiga\PlayVideo.rexx (DEBUG = OFF)."
	SAY " "	

   
	if wait = ON, 
	then do
		SAY "Waiting "SECONDS" seconds before to close this debug window..."
		SAY " "
		SAY "Wait seconds can be modified in Amiga\PlayVideo.rexx (SECONDS = x)"
		SAY " "
		SAY "Wait time can be turned OFF in Amiga\PlayVideo.rexx (WAIT = OFF)"
		SAY " "
		SAY "----------------------------------------------------------------------"
		ADDRESS COMMAND "WAIT "SECONDS	
	end	
	else do
		SAY "----------------------------------------------------------------------"
	end
	call close STDOUT
end
else do	
	/* no debug output */
	ADDRESS COMMAND MPLAYER MPLAYER_OPTIONS url ">NIL: *>NIL:"
end


EXIT


	



