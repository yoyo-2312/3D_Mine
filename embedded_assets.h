#ifndef _EMBEDDED_ASSETS_H
#define _EMBEDDED_ASSETS_H

#include "Game_Mine.h"

bool PrepareEmbeddedAssets();
void CleanupEmbeddedAssets();
int LoadEmbeddedImage(IMAGE* pDstImg, const char* assetName, int nWidth = 0, int nHeight = 0, bool bResize = false);
bool OpenEmbeddedMusic(const char* alias, const char* assetName);
bool PlayOpenedMusic(const char* alias, bool repeat = false);
bool PlayEmbeddedMusic(const char* alias, const char* assetName, bool repeat = false);
bool PauseEmbeddedMusic(const char* alias);
bool ResumeEmbeddedMusic(const char* alias);
void CloseEmbeddedMusic(const char* alias);

#endif
