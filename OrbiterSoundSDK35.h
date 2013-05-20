///////////////////////////////////////////////////////////////////////////////////////////////
//       ORBITERSOUND 3.5 SDK
//       SDK to add sound to Orbiter's VESSEL (c) Daniel Polli  ORBITER ver:2006 with patch P1
///////////////////////////////////////////////////////////////////////////////////////////////
//
// info:
// -----
//
// This SDK allow loading and playing 60 wav sound for vessel.
// Sound are loaded and played throught OrbiterSound.dll so you must
// specify in your doc that the player must have OrbiterSound module installed.
// Several test are done internally to avoid problem if either OrbiterSound
// isn't loaded or the wav doesn't exist etc etc so in this case no sound is played
// and orbiter don't crash.
// You don't need to bother destroying sound they are destroyed automaticly if:
//   -you reload a wav in the same slot (WavNumber)
//   -User exit Orbiter
//
// NEW FEATURE OF 3.5
// ------------------
// - Can load dynamically another sound to one slot and play it immediattely. 
//   (ie: "RequestLoadVesselWave3" than immediattely "PlayVesselWave3" DO NOT DO THAT EACH FRAME !!!)
// -Reentry sound reworked
// -Added a "GetUserOrbiterSoundVersion()" wich will told you if orbiter 3.5 or previous is installed
//   in user's orbiter (see below for usage)
// -MFD Sound SDK so MFD can also play sound.
//
//
// Usage example:
// --------------
//
// 1. Include OrbitesSoundSDK35.h in your vessel's project: #include "OrbiterSoundSDK35.h"
// 2. Add OrbiterSoundSDK35.lib to your vessel's project
// 3. call ConnectToOrbiterSoundDLL3() in your "clbkPostCreation" (new version of ovcPostCreation wich is now obsolet)
// 4. call RequestLoadVesselWave3(MyId,1,"Sound\\AtlantisMartin\\MyFirstWave.wav",INTERNAL_ONLY); //example MyId is the id returned by connect...()
// 5. call PlayVesselWave3(1) when you want; 
// 6. SEE SHUTTLPB EXAMPLE in the orbitersoundSDK's folder.
// et voila !
//
// Function available (see below in the header how to use each function):
// -------------------
// int  ConnectToOrbiterSoundDLL3(OBJHANDLE);  //return MyId flag that you must provide to all others function
// float GetUserOrbiterSoundVersion(void);
// BOOL RequestLoadVesselWave3(int MyId,int WavNumber,char* SoundName,int extended);
// BOOL ReplaceStockSound3(int MyId,int WavNumber,char* NewWavName,int ParameterSoundToReplace);
// BOOL PlayVesselWave3(int MyId,int WavNumber,int Loop=NOLOOP,int Volume=255,int Frequency=NULL);
// BOOL StopVesselWave3(int MyId,int WavNumber);
// BOOL IsPlaying3(int MyId,int WavNumber);
// BOOL PlayVesselRadioExclusiveWave3(int MyId,int WavNumber,int Loop=NOLOOP,int Volume=255,int Frequency=NULL);
// BOOL IsRadioPlaying3(void);
// BOOL SoundOptionOnOff3(int MyId,int Option, BOOL Status);
// BOOL SetRadioFrequency3(char* Frequency);
//
// IMPORTANT NOTE:
// ---------------
//
// Sound Folder !!!  Keep it clear for player
// ------------------------------------------
// For consistency I think it's better to put your sound in the OrbiterSound's directory
// and create your specific folder here with a UNIQUE name so vessel sound add-on
// will not overlap and user will not end with dozen of sound folder trowed everywhere
// in its Orbiter's directory.
//
// Example:
//   if you add sound for Atlantis and your name is Martin
//   create a directory  "Orbiter/Sound/AtlantisMartin/"
//   put your wav in this folder and at load time call:
//   RequestLoadVesselWave(1,"Sound\\AtlantisMartin\\MyFirstWave.wav");
//
//   otherwise if you and another vessel creator choose the same common name for example
//   "atlantis" your or his wav will be erased depend on wich add-on is installed 
//   later.
//
// Sliding sound
// --------------
// if you call each game loop a PlayVesselWave function while changing its parameter
// the sound will continue to play with the new parameter
// so you can make sound that fade in , fade out or slide in frequency. 
// int this case you must set them to "LOOP" anyway
//
// Wave Format
// -----------
// Orbiter sound is locked at 22050 hz for performance care so it's useless
// to load sound that have higher frequency.
// For size's care I suggest a format of 11025 8 bit mono for radio and other minor sound
// and more until 22050 for sound that require higher quality. Stereo play well also
// but they require double size. Try to keep size as low as possible otherwise loading
// time will become very long.
// WARNING: take care that your wave doesn't have an ADPCM format the old compressed format
// this one will not play on most computer due to codec. 
// instead always Use PCM the default windows format.
//
//
////////////////////////////////////////////////////////////////////////////////
#ifndef __ORBITERSOUND35_SDK
#define __ORBITERSOUND35_SDK
// those are some definition watch below for usefull instruction for you
#pragma comment(lib,"OrbiterSoundSDK35.lib")
#define NOLOOP			0
#define LOOP			1
typedef enum{
		DEFAULT,
		INTERNAL_ONLY,
		BOTHVIEW_FADED_CLOSE,
		BOTHVIEW_FADED_MEDIUM,
		BOTHVIEW_FADED_FAR,
		EXTERNAL_ONLY_FADED_CLOSE,
		EXTERNAL_ONLY_FADED_MEDIUM,
		EXTERNAL_ONLY_FADED_FAR,
		RADIO_SOUND,
}EXTENDEDPLAY;

///////////////////////////////////////////////////
// ConnectToOrbiterSoundDLL3
// -------------------------
// this must be called previous to any other OrbiterSound call
// You must call this in your "clbkPostCreation" (new version of "ovcPostCreation" wich is now obsolet)
// function of your code whichever you have.
// You must keep in your class the ID it return and provide it to other function.
//
// param:
// ------
// Obj   : the Vessel handle of your vessel using this : MySoundID=ConnectToOrbiterSoundDLL3(GetHandle());
// return : And ID that you must keep and provide to others function or -1 if it fail to connect.
///////////////////////////////////////////////////
int ConnectToOrbiterSoundDLL3(OBJHANDLE Obj);

///////////////////////////////////////////////////
// GetUserOrbiterSoundVersion()
// -------------------------
// return exact version of orbitersound installed in 
// >>>user's directory<<<< 
// version number returned by this function is in float example: 
// returned value 3.0f = version 3.0 
// returned value 3.5f = version 3.5
// returned value 4.0f = version 4.0 (not yet existing)
// previous than 3.0 version return always 0, they are 
// so outdated that you might warn the user anyway.
// this may be usefull if you want special new feature
// only existing in 3.0 or 3.5.
///////////////////////////////////////////////////
float GetUserOrbiterSoundVersion(void);

///////////////////////////////////////////////////
// ReplaceStockSound3
// ------------------------
// If you need only one function it's this one, it will instruct
// to permanently replace one or more OrbiterSound's stock sound by 
// your sound for your vessel.
// You might call this just after ConnectToOrbiterSoundDLL3
// and forget all the others sound function, your vessel will have
// unique sound that will play automatically.
//
// parameter:
// ---------
// MyID					: The number returned by ConnectToOrbiterSoundDLL3
// MyCustomWavName		: Your wav's name  (exemple: "Sound\\mysound\\mymainthrust.wav")
// WhichSoundToReplace	: a keyword as defined below:
// REPLACE_MAIN_THRUST			 // This sound will replace the OrbiterSound main thrust and keep all it's behaviour
// REPLACE_HOVER_THRUST			 // This sound will replace the OrbiterSound hover thrust and keep all it's behaviour
// REPLACE_RCS_THRUST_ATTACK	 // This sound will replace the OrbiterSound rcs attack thrust and keep all it's behaviour
// REPLACE_RCS_THRUST_SUSTAIN	 // This sound will replace the OrbiterSound rcs sustain thrust and keep all it's behaviour
// REPLACE_AIR_CONDITIONNING	 // This sound will replace the OrbiterSound air conditionning sound
// REPLACE_COCKPIT_AMBIENCE_1	 // This sound will replace the wave 1 used for cockpit ambience, it will play automatically.
// REPLACE_COCKPIT_AMBIENCE_2	 // This sound will replace the wave 2 used for cockpit ambience, it will play automatically.
// REPLACE_COCKPIT_AMBIENCE_3	 // This sound will replace the wave 3 used for cockpit ambience, it will play automatically.
// REPLACE_COCKPIT_AMBIENCE_4	 // This sound will replace the wave 4 used for cockpit ambience, it will play automatically.
// REPLACE_COCKPIT_AMBIENCE_5	 // This sound will replace the wave 5 used for cockpit ambience, it will play automatically.
// REPLACE_COCKPIT_AMBIENCE_6	 // This sound will replace the wave 6 used for cockpit ambience, it will play automatically.
// REPLACE_COCKPIT_AMBIENCE_7	 // This sound will replace the wave 7 used for cockpit ambience, it will play automatically.
// REPLACE_COCKPIT_AMBIENCE_8	 // This sound will replace the wave 8 used for cockpit ambience, it will play automatically.
// REPLACE_COCKPIT_AMBIENCE_9	 // This sound will replace the wave 9 used for cockpit ambience, it will play automatically.
//
// return:
//  FALSE if request not taken in account.
//  if a wave wasn't found a log will be created in Orbiter's directory with the name of the 
//  missing wave.
///////////////////////////////////////////////////
BOOL ReplaceStockSound3(int MyId,char *MyCustomWavName,int WhichSoundToReplace);


///////////////////////////////////////////////////////////////////////////////////////
// RequestLoadVesselWave3
// ----------------------
// Request to Load one sound in slot "WavNumber"
//
// param:
// ------
// MyID     : The number returned by ConnectToOrbiterSoundDLL3
// WavNumber: from 1 to 60 or #defined name from 1 to 60 
// SoundName: your wav's name and path. example: "Sound\\atlantisMartin\\separate.wav"
// Extended : This parameter will define how your sound will play, in cockpit only in external view etc etc.
//
// This is the keyword used for the extended parameter:
//	DEFAULT					    DEFAULT NOT RECOMMENDED !  sound will always play in all view and will not be faded by distance,pressure or view change
//  INTERNAL_ONLY			    Sound will play in internal view only (usefull for internal cockpit sound)
//  BOTHVIEW_FADED_CLOSE	    Sound will play as the RCS thrust,faded by close distance and pressure in external view and full volume in internal view
//  BOTHVIEW_FADED_MEDIUM	    Sound will play as the airspeed wind,faded by medium distance and pressure in external view and full volume in internal view
//  BOTHVIEW_FADED_FAR		    Sound will play as the main thrust,faded by far distance and pressure in external view and full volume in internal view
//  EXTERNAL_ONLY_FADED_CLOSE   Sound will play in external view only (faded with close distance and pressure)
//  EXTERNAL_ONLY_FADED_MEDIUM  Sound will play in external view only (faded with medium distance and pressure)
//  EXTERNAL_ONLY_FADED_FAR     Sound will play in external view only (faded with far distance and pressure)
//  RADIO_SOUND					Sound is a radio sound that will be played by PlayVesselRadioExclusiveWave3()
//
// IMPORTANT NOTICE: FOR SOUND IN EXTERNAL VIEW OR BOTHVIEW YOU MUST CALL THE SOUND EACH FRAME AND "LOOPED" OTHERWISE 
// IT WILL NOT BE FADED PROPERLY BY DISTANCE AND PRESSURE. SEE "SHUTLEPB" EXAMPLE.
//
// return:
//  FALSE if request not taken in account.
//  if a wave wasn't found a log will be created in Orbiter's directory with the name of the 
//  missing wave
//
// Note:
// -----
// When you call this function THE WAV ISN'T LOADED DIRECTLY
// due to the fact that the vessel is initialised before directsound when orbiter start,
// this function SUBMIT the loading. As soon as directsound is ready it will load
// the sound. The delay is due while Orbiter open its screen at last when all
// is initialised and DirectSound need the screen's handle previous to be initialised.
// So the sound will be loaded at least when Orbiter screen open Else it's almost 
// instantly. the best place to load wave is "ovcinit" function just after 
// ConnectToOrbiterSoundDLL3() function.
//
// You can submit a new loading at same "slot" any time during simulation the new
// one will erase the old one. Take care not to ask a loading every frame otherwise
// the framerate will suffer. Take car that if the user want to save it's situation
// and come back later you may define some flag and record the time so when the player
// come back you reload the appropriate wav for the time it is.
// 
// you can use plain WavNumber from 1 to 60 or use the name
// defined above for more readability.
// ie: it's more clear to call:
// LoadVesselWave3(MyID,SEPARATESRB,"Sound\\atlantisMartin\\separate.wav");
// PlayVesseWave3(MyID,SEPARATESRB);
//   than
// LoadVesselWave3(MyID,18,"Sound\\atlantisMartin\\separate.wav");
// PlayVesseWave3(MyID,18);
// when you end with dozen line of code you will wonder what playvesselwave(18) will do.
//
// if you plan to reload some sound at the same slot during simulation you can also
// declare another set of name.
// Ie:
// #define LAUNCHONEARTHSOUND    0   // set number one in slot 0
// ...
// #define LAUNCHONMOONSOUND     0   // set number two in slot 0 (erase the previous one if loaded)
//
////////////////////////////////////////////////////////////////////////////////////////
BOOL RequestLoadVesselWave3(int MyID,int WavNumber,char* SoundName,EXTENDEDPLAY extended);

///////////////////////////////////////////////
// PlayVesselWave3
// --------------
// Play a wave loaded before
//
// param:
// ------
// MyID     : The number returned by ConnectToOrbiterSoundDLL3
// WavNumber: from 1 to 60 or #defined name from 1 to 60
// Loop		: NOLOOP= sound not looped LOOP= sound looped.
// Volume	: from 0 to 255
// Frequency: from 3000 to 44100  (OrbiterSound's play anyway at 22050 so your sound will be resampled at run time)
//
// return:
//  FALSE if wave not played/loaded OR the connection to orbitersound's dll failed.
//
// example:
// --------
//  PlayVesselWave3(MyID,SEPARATESRB);			// play with no loop full volume and wav's default frequency
//  PlayVesselWave3(MyID,SEPARATESRB,LOOP,210);	// play a looped sound with specified volume and wav's default frequency
//  PlayVesselWave3(MyID,SEPARATESRB,NOLOOP,128,22050);// play no loop with specified volume and frequency
///////////////////////////////////////////////
BOOL PlayVesselWave3(int MyID,int WavNumber,int Loop=NOLOOP,int Volume=255,int Frequency=NULL);

///////////////////////////////////////////////
// StopVesselWave3
// --------------
// Stop a wave that play
//
// param:
// ------
// MyID  : The number returned by ConnectToOrbiterSoundDLL3
// WavNumber: from 1 to 60 or #defined name from 1 to 60
//
// return:
//  FALSE if connection to orbitersound's dll failed or sound not loaded.
//
// example:
// --------
//  StopVesselWave3(MyID,SEPARATESRB);		
///////////////////////////////////////////////
BOOL StopVesselWave3(int MyID,int WavNumber);

///////////////////////////////////////////////
// IsPlaying3
// ---------
// Return TRUE if the sound specified in WaveNumber
// is currently playing otherwise it return FALSE.
//
// param:
// ------
// MyID  : The number returned by ConnectToOrbiterSoundDLL3
// WavNumber: from 1 to 60 or #defined name from 1 to 60
//
// return:
// -------
//  FALSE if wave does not play OR the connection to orbitersound's dll failed.
//
// this is useful to avoid playing two sound in same time
// example:
// --------
//  if(IsPlaying3(MyID,SEPARATESRB)==FALSE)
//  {
//     PlayVesselWave3(MyID,AFTERSRBSEPARATE);		
//  }
///////////////////////////////////////////////
BOOL IsPlaying3(int MyID,int WavNumber);

///////////////////////////////////////////////
// PlayVesselRadioExclusiveWave3
// ----------------------------
// This function is used to play your radio sound it:
//
// 1- STOP the ATC RADIO sound that OrbiterSound is currently playing
// 2- FORBID OrbiterSound to play radio atc while this sound is playing. (auto test)
// as soon as your sound end OrbiterSound will continue to play random ATC radio Wave
// this is perfect to play your radio sound or important sound without being disturbed by
// OrbiterSound's atc sound.
//
// For parameter see PlayVesselWave3
//
// Note:
// it is perhaps better to test with IsRadioPlaying3() if an atc wav is currently playing
// before calling this or the Orbiter's atc sound will stop in the middle and that may
// look strange.
//
///////////////////////////////////////////////
BOOL PlayVesselRadioExclusiveWave3(int MyID,int WavNumber,int Volume=255);

///////////////////////////////////////////////
// SoundOptionOnOff3
// -----------------
// With this function you can put ON/OFF every sound
// in OrbiterSound the same way SOUNDCONFIG.EXE does
// but in a temporary manner. 
// (start soundconfig.exe to test the sound related to each option so you have an idea what it does)
// 
// param:
// ------
// MyID  : The number returned by ConnectToOrbiterSoundDLL3
// Option: Keyword as defined below:
//
// PLAYCOUNTDOWNWHENTAKEOFF			// the countdown of apollo11 when you take off
// PLAYWHENATTITUDEMODECHANGE		// play "rotation" "linear" sound when you change mode
// PLAYGPWS							// the GPWS sound
// PLAYMAINTHRUST					// main thrust sound
// PLAYHOVERTHRUST					// the hover thrust sound
// PLAYATTITUDETHRUST				// the attitude thrust sound
// PLAYDOCKINGSOUND					// the docking sound and radio
// PLAYRADARBIP						// the radar bip when near another vessel
// PLAYWINDAIRSPEED					// the wind airspeed when atmosphere
// PLAYDOCKLANDCLEARANCE			// the landing clearance you can hear bellow 3000m or near a station
// PLAYLANDINGANDGROUNDSOUND		// Rolling , landing, speedbrake, crash sound
// PLAYCABINAIRCONDITIONING			// play the air conditionning sound
// PLAYCABINRANDOMAMBIANCE			// play the random pump and rumble sound
// PLAYWINDAMBIANCEWHENLANDED		// play the wind sound when landed
// PLAYRADIOATC						// play the atc radio sound
// DISPLAYTIMER						// display the timer text at the bottom of the screen when you take-off
//
// Status: TRUE if you want to allow the sound FALSE if you want to disable it (sound stop immediately when you call it)
//
// return:
//  FALSE if the connection to orbitersound's dll failed
//  or the keyword used is false
//
// example:
// --------
//  SoundOptionOnOff3(MyID,PLAYMAINTHRUST,FALSE)		// Disable the main thrust sound in orbitersound
//  SoundOptionOnOff3(MyID,PLAYGPWS,TRUE)				// Enable the GPWS sound in OrbiterSound
///////////////////////////////////////////////
BOOL SoundOptionOnOff3(int MyID,int Option, BOOL Status=TRUE);


///////////////////////////////////////////////
// IsRadioPlaying3
// --------------
// Return TRUE if an OrbiterSound's atc radio wav is currently playing
// otherwise it return FALSE.
// This is usefull to avoid playing YOUR radio wav during
// and OrbiterSound's atc one.
//
// return:
//  FALSE if radio atc does not play OR the connection to orbitersound's dll failed.
//
// example:
// --------
//  if(IsRadioPlaying3()==FALSE)
//  {
//     PlayVesselWave3(MyID,MYRADIODOCKED);		
//  }
///////////////////////////////////////////////
BOOL IsRadioPlaying3(void);
///////////////////////////////////////////////
// SetRadioFrequency
// -----------------
// With this function you can set the main radio frequency 
// used in OrbiterSound.
// 
// param:
// ------
// frequency: a char string in the form "nnn.nnn"
// a directory with the same name must exist in the sound/radio folder
//
// return:
//  FALSE if the connection to orbitersound's dll failed
//
// example:
// --------
//  SetRadioFrequency3("296.800");		// Set the frequency 296.800. radio will play the sound contained in this folder
///////////////////////////////////////////////
BOOL SetRadioFrequency3(char* Frequency);



///////////// END OF USEFULL CODE FOR YOU /////


////////////////////////////////////////////
// KEYWORD
////////////////////////////////////////////

// this is the keyword used by SoundOptionOnOff3()
#define PLAYCOUNTDOWNWHENTAKEOFF	 1		// the countdown of apollo11 when you take off
#define PLAYWHENATTITUDEMODECHANGE	 3		// play "rotation" "linear" sound when you change mode
#define PLAYGPWS					 4		// the GPWS sound
#define PLAYMAINTHRUST				 5		// main thrust sound
#define PLAYHOVERTHRUST				 6		// the hover thrust sound
#define PLAYATTITUDETHRUST			 7		// the attitude thrust sound
#define PLAYDOCKINGSOUND			 8		// the docking sound and radio
#define PLAYRADARBIP				 9		// the radar bip when near another vessel
#define PLAYWINDAIRSPEED			 10		// the wind airspeed when atmosphere
#define PLAYDOCKLANDCLEARANCE		 11		// the landing clearance you can hear bellow 3000m or near a station
#define PLAYLANDINGANDGROUNDSOUND	 12		// Rolling , landing, speedbrake, crash sound
#define PLAYCABINAIRCONDITIONING	 13		// play the air conditionning sound
#define PLAYCABINRANDOMAMBIANCE		 14		// play the random pump and rumble sound
#define PLAYWINDAMBIANCEWHENLANDED	 15		// play the wind sound when landed
#define PLAYRADIOATC				 16		// play the atc radio sound THIS DOESNT AFFECT YOUR RADIO SOUND played with PlayVesselRadioExclusiveWave3()
#define DISPLAYTIMER				 17		// display the timer text at the bottom of the screen when you take-off

//This is the keyword used by ReplaceStockSound3()
#define REPLACE_MAIN_THRUST			 10		// This sound will replace the OrbiterSound main thrust and keep all it's behaviour
#define REPLACE_HOVER_THRUST		 11		// This sound will replace the OrbiterSound hover thrust and keep all it's behaviour
#define REPLACE_RCS_THRUST_ATTACK	 12		// This sound will replace the OrbiterSound rcs attack thrust and keep all it's behaviour
#define REPLACE_RCS_THRUST_SUSTAIN	 13		// This sound will replace the OrbiterSound rcs sustain thrust and keep all it's behaviour
#define REPLACE_AIR_CONDITIONNING	 14		// This sound will replace the OrbiterSound air conditionning sound
#define REPLACE_COCKPIT_AMBIENCE_1	 15		// This sound will replace the wave 1 used for cockpit ambience (rumble, pump etc etc)
#define REPLACE_COCKPIT_AMBIENCE_2	 16		// This sound will replace the wave 1 used for cockpit ambience (rumble, pump etc etc)
#define REPLACE_COCKPIT_AMBIENCE_3	 17		// This sound will replace the wave 1 used for cockpit ambience (rumble, pump etc etc)
#define REPLACE_COCKPIT_AMBIENCE_4	 18		// This sound will replace the wave 1 used for cockpit ambience (rumble, pump etc etc)
#define REPLACE_COCKPIT_AMBIENCE_5	 19		// This sound will replace the wave 1 used for cockpit ambience (rumble, pump etc etc)
#define REPLACE_COCKPIT_AMBIENCE_6	 20		// This sound will replace the wave 1 used for cockpit ambience (rumble, pump etc etc)
#define REPLACE_COCKPIT_AMBIENCE_7	 21		// This sound will replace the wave 1 used for cockpit ambience (rumble, pump etc etc)
#define REPLACE_COCKPIT_AMBIENCE_8	 22		// This sound will replace the wave 1 used for cockpit ambience (rumble, pump etc etc)
#define REPLACE_COCKPIT_AMBIENCE_9	 23		// This sound will replace the wave 1 used for cockpit ambience (rumble, pump etc etc)


#endif //__ORBITERSOUND35_SDK
