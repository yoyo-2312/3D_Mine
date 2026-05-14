#include "Game_Mine.h"
using namespace std;

int main()
{
	//int screenWidth = GetSystemMetrics(SM_CXSCREEN);  // 屏幕宽度，仅调试时使用
	//int screenHeight = GetSystemMetrics(SM_CYSCREEN); // 屏幕高度，仅调试时使用
	//printf("%d %d", screenWidth, screenHeight);
	PrepareGame();
	PreOperation();
	Mine3DLoop();
	return 0;
}
