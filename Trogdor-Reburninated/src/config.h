// Save file config

#ifndef CONFIG_H
#define CONFIG_H

#include "include.h"
#include "window.h"

struct SoundSettings {
	Sint8 musicIndex;
	Sint8 bgmVolume;
	Sint8 sfxVolume;
};

struct VideoSettings {
	Sint8 resolutionIndex;
	Sint8 aspectRatioIndex;
	Uint16 widthSetting;
	Uint16 heightSetting;
};

/* Save File */
extern SDL_RWops *saveFile;
extern SoundSettings soundSettings;
extern VideoSettings videoSettings;

/* System-Specific Variables */
#if defined(VITA)
const string rootDir = "ux0:data/TrogdorRB/";
#elif defined(WII) || defined(GAMECUBE)
const string rootDir = "sd:/apps/TrogdorRB/";
#else
const string rootDir = "";
#endif

#define SAVE_FILE (rootDir+"save.bin").c_str()

extern void loadSaveFile();
extern void initializeSaveFileNew();
//extern void SAVE_CURRENT_SAVE();

#endif