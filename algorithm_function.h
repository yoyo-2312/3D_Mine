#ifndef _A_FUNC
#define _A_FUNC
void OpenNull(int r, int c, int d);
void GameInit(int x, int y, int z);
void MouseEvent();
void PlayMusic(const char* mp3);
void Time();
void Judge();
void ShowConsole();
void UpdataBest(FILE* fp);
void GetBest(FILE* fp);
void ExitGame();
bool WindowClosed();
#endif
