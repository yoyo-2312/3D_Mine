#include "Game_Mine.h"
#include "algorithm_function.h"
#include "draw_function.h" 
using namespace std;

static FILE* OpenRecordFile()
{
	FILE* fp = fopen("GameRecord.txt", "r+");
	if (fp) return fp;

	fp = fopen("GameRecord.txt", "w+");
	if (!fp)
	{
		printf("Open error\n");
		exit(1);
	}

	for (int i = 0; i < 4; i++)
		fprintf(fp, "000 00:00 000 00:00\r\n");

	rewind(fp);
	return fp;
}
void ExitGame()
{
	mciSendString("close all", NULL, 0, NULL);
	closegraph();
	exit(0);
}

bool WindowClosed()
{
	HWND hwnd = GetHWnd();
	return hwnd == NULL || !IsWindow(hwnd) || !IsWindowVisible(hwnd);
}

//获取文件数据
void GetBest(FILE* fp)
{
	fp = OpenRecordFile();
	rewind(fp);
	for (int i = 0; i < 4; i++)
		fscanf(fp, "%3d %1d%1d:%1d%1d %3d %1d%1d:%1d%1d\r\n", &Mine.Best[i].num, &Mine.Best[i].m1, &Mine.Best[i].m2, &Mine.Best[i].s1, &Mine.Best[i].s2, &Mine.Best[i].last_num, &Mine.Best[i].last_m1, &Mine.Best[i].last_m2, &Mine.Best[i].last_s1, &Mine.Best[i].last_s2);
	fclose(fp);
}
//更新文件数据
void UpdataBest(FILE* fp)
{
	GetBest(fp);
	int t = (Mine.Best[Mine.op].m1 * 10 + Mine.Best[Mine.op].m2) * 60 + Mine.Best[Mine.op].s1 * 10 + Mine.Best[Mine.op].s2;
	fp = OpenRecordFile();
	rewind(fp);
	for (int i = 0; i < 4; i++)
	{
		if (i == Mine.op)
		{
			if (Mine.flag_cnt > Mine.Best[Mine.op].num || Mine.flag_cnt == Mine.Best[Mine.op].num && Mine.time_all < t)
			{
				Mine.if_new = 1;
				putimage(Mine.Icon_return.LeftSpace, Mine.Icon_return.TopSpace - Mine.icondot_len + Mine.icon_len, &Mine.M_img[15]);
				fprintf(fp, "%03d %d%d:%d%d %03d %d%d:%d%d\r\n", Mine.flag_cnt, Mine.min1, Mine.min2, Mine.sec1, Mine.sec2, Mine.flag_cnt, Mine.min1, Mine.min2, Mine.sec1, Mine.sec2);
			}
			else fprintf(fp, "%03d %d%d:%d%d %03d %d%d:%d%d\r\n", Mine.Best[Mine.op].num, Mine.Best[Mine.op].m1, Mine.Best[Mine.op].m2, Mine.Best[Mine.op].s1, Mine.Best[Mine.op].s2, Mine.flag_cnt, Mine.min1, Mine.min2, Mine.sec1, Mine.sec2);
		}
		else fprintf(fp, "%03d %d%d:%d%d %03d %d%d:%d%d\r\n", Mine.Best[i].num, Mine.Best[i].m1, Mine.Best[i].m2, Mine.Best[i].s1, Mine.Best[i].s2, Mine.Best[i].last_num, Mine.Best[i].last_m1, Mine.Best[i].last_m2, Mine.Best[i].last_s1, Mine.Best[i].last_s2);
	}
	fclose(fp);
}
//音效
void PlayMusic(const char* mp3)
{
	char str[50] = "";
	sprintf(str, "close ./Sweep_MP3/%s.MP3", mp3);
	mciSendString(str, NULL, 0, NULL);
	sprintf(str, "open ./Sweep_MP3/%s.MP3", mp3);
	mciSendString(str, NULL, 0, NULL);
	sprintf(str, "play ./Sweep_MP3/%s.MP3", mp3);
	mciSendString(str, NULL, 0, NULL);
}
//计时
void Time()
{
	Mine.cur_t = time(NULL);
	//表情变化一次停留1秒
	if (Mine.if_wait && Mine.cur_t - Mine.wait_t >= 1)
	{
		Mine.if_wait = 0;
		Mine.turn_Smile = 0;
	}
	//未暂停则计时
	if (!Mine.if_pause && Mine.cur_t - Mine.last_t >= 1)
	{
		Mine.time_all = (int)(Mine.cur_t - Mine.sta_t - Mine.pau_t);
		Mine.Sec = Mine.time_all % 60;
		Mine.Min = Mine.time_all / 60;
		Mine.min1 = Mine.Min / 10 % 10, Mine.min2 = Mine.Min % 10, Mine.sec1 = Mine.Sec / 10 % 10, Mine.sec2 = Mine.Sec % 10;
		Mine.last_t = Mine.cur_t;
	}
	//无操作超过180秒变成待机表情
	if (Mine.cur_t - Mine.rest_t >= 180 && !Mine.fail && !Mine.win) Mine.turn_Smile = 180;
}
//判断输赢
void Judge()
{
	if (Mine.fail)
	{
		for (int r = 0; r < MAX_L; r++)
			for (int c = 0; c < MAX_L; c++)
				for (int d = 0; d < MAX_L; d++)
				{
					//统计有效排雷
					if (Mine.Map[r][c][d] == 29)
					{
						int flag = 1;
						for (int i = r - 1; i <= r + 1; i++)
							for (int j = c - 1; j <= c + 1; j++)
								for (int l = d - 1; l <= d + 1; l++)
									if (0 <= i && i < MAX_L && 0 <= j && j < MAX_L && 0 <= l && l < MAX_L)
										if (Mine.Map[i][j][l] != 29 && Mine.Map[i][j][l] != 59 && (Mine.Map[i][j][l] > 26 || Mine.Map[i][j][l] < 0)) flag = 0;
						if (flag)
						{
							Mine.r_num++;
							Mine.flag_cnt++;
							Mine.Map[r][c][d] += 30;
						}
					}
					//标记错旗
					if (Mine.Map[r][c][d] > 59 && Mine.Map[r][c][d] <= 86)
					{
						Mine.Map[r][c][d] += 30;
						Mine.flag_cnt--;
						Mine.w_num++;
					}
					//标记炸开雷
					if (Mine.Map[r][c][d] == 29) Mine.Map[r][c][d] = -4;
				}
		//仅执行一次更新成绩单和播放音乐
		if (!Mine.if_pause)
		{
			UpdataBest(Mine.fp);
			PlayMusic(Mine.mp3[1]);
			AddDraw();
		}
		Mine.turn_Smile = 4;
		Mine.if_pause = 1;
		Mine.if_wait = 0;
	}
	//判断是否赢
	else if (Mine.cnt == (MAX_L * MAX_L * MAX_L - Mine.max_mine))
	{
		int flag;
		for (int r = 0; r < MAX_L; r++)
			for (int c = 0; c < MAX_L; c++)
				for (int d = 0; d < MAX_L; d++)
				{
					//统计有效排雷
					if (Mine.Map[r][c][d] == 29)
					{
						int flag = 1;
						for (int i = r - 1; i <= r + 1; i++)
							for (int j = c - 1; j <= c + 1; j++)
								for (int l = d - 1; l <= d + 1; l++)
									if (0 <= i && i < MAX_L && 0 <= j && j < MAX_L && 0 <= l && l < MAX_L)
										if (Mine.Map[i][j][l] != 29 && Mine.Map[i][j][l] != 59 && (Mine.Map[i][j][l] > 26 || Mine.Map[i][j][l] < 0)) flag = 0;
						if (flag)
						{
							Mine.r_num++;
							Mine.flag_cnt++;
							Mine.Map[r][c][d] += 30;
						}
					}
				}
		//仅执行一次更新成绩单和播放音乐
		if (!Mine.if_pause)
		{
			UpdataBest(Mine.fp);
			PlayMusic(Mine.mp3[0]);
			AddDraw();
		}
		Mine.turn_Smile = 3;
		Mine.win = 1;
		Mine.if_pause = 1;
		Mine.if_wait = 0;
	}
}
//打开空白格子时炸开一片
void OpenNull(int r, int c, int d)
{
	for (int i = r - 1; i <= r + 1; i++)
		for (int j = c - 1; j <= c + 1; j++)
			for (int l = d - 1; l <= d + 1; l++)
				if (0 <= i && i < MAX_L && 0 <= j && j < MAX_L && 0 <= l && l < MAX_L)
					if (29 <= Mine.Map[i][j][l] && Mine.Map[i][j][l] <= 56)
					{
						Mine.Map[i][j][l] -= 30;
						Mine.cnt++;
						if (Mine.Map[i][j][l] == 0) OpenNull(i, j, l);
						if (Mine.Map[i][j][l] == -1) Mine.fail = 1;
					}
}
//初始化雷位置(首次点击后)
void GameInit(int x, int y, int z)
{
	Mine.sta_t = Mine.cur_t = Mine.last_t = Mine.rest_t = time(NULL);
	memset(Mine.Map, 0, sizeof(Mine.Map));
	srand((unsigned)time(NULL));
	for (int cnt = 0; cnt < Mine.max_mine;)
	{
		int row = rand() % MAX_L;
		int col = rand() % MAX_L;
		int dep = rand() % MAX_L;
		if (Mine.Map[row][col][dep] == 0 && !(x - 1 <= row && row <= x + 1 && y - 1 <= col && col <= y + 1 && z - 1 <= dep && dep <= z + 1))
		{
			Mine.Map[row][col][dep] = -1;
			cnt++;
		}
	}
	//统计每个格子旁雷的个数，初始化方格数字
	for (int r = 0; r < MAX_L; r++)
		for (int c = 0; c < MAX_L; c++)
			for (int d = 0; d < MAX_L; d++)
				if (Mine.Map[r][c][d] == -1)
					for (int i = r - 1; i <= r + 1; i++)
						for (int j = c - 1; j <= c + 1; j++)
							for (int l = d - 1; l <= d + 1; l++)
								if ((0 <= i && i < MAX_L && 0 <= j && j < MAX_L && 0 <= l && l < MAX_L) && Mine.Map[i][j][l] != -1)
									Mine.Map[i][j][l]++;
	//将格子用未打开图片覆盖
	for (int r = 0; r < MAX_L; r++)
		for (int c = 0; c < MAX_L; c++)
			for (int d = 0; d < MAX_L; d++)
				Mine.Map[r][c][d] += 30;
}
//鼠标操作
void MouseEvent()
{
	ExMessage msg;
	while (peekmessage(&msg, EM_MOUSE | EM_WINDOW))
	{
		if (msg.message == WM_CLOSE || msg.message == WM_DESTROY)
		{
			ExitGame();
		}

		if (msg.message < WM_MOUSEFIRST || msg.message > WM_MOUSELAST)
			continue;

		//获取鼠标在大方格以及层数栏的行列数
		int posx = msg.x, posy = msg.y;
		int row = (posy - Mine.Large.TopSpace) / Mine.large_cube;
		int col = (posx - Mine.Large.LeftSpace) / Mine.large_cube;
		int row_d = (posy - Mine.Dep.TopSpace) / Mine.large_cube;
		int col_d = (posx - Mine.Dep.LeftSpace) / Mine.large_cube;
		//是否在主界面
		if (Mine.if_main)
		{
			if ((msg.message == WM_LBUTTONDOWN || msg.message == WM_LBUTTONDBLCLK))
			{
				//点击成绩栏
				if (posx >= Mine.record_back_l && posx <= Mine.record_back_r && posy >= Mine.record_back_t && posy <= Mine.record_back_b)
				{
					PlayMusic(Mine.mp3[6]);
					Mine.if_main = Mine.if_draw = 0;
					Mine.if_score = 1;
					Mine.if_new = 0;
				}
				//判断是否首次打开程序,显示不同操作界面
				if (Mine.fst_enter_game)
				{
					if (posx >= Mine.start.l && posx <= Mine.start.r && posy >= Mine.start.t && posy <= Mine.start.b)
					{
						PlayMusic(Mine.mp3[6]);
						Mine.if_main = Mine.if_draw = 0;
						Mine.if_choice = 1;
					}
				}
				else
				{
					//重新开始
					if (posx >= Mine.restart.l && posx <= Mine.restart.r && posy >= Mine.restart.t && posy <= Mine.restart.b)
					{
						PlayMusic(Mine.mp3[6]);
						Mine.if_main = Mine.if_draw = 0;
						Mine.fst_enter_game = Mine.if_choice = 1;
					}
					//继续游戏
					else if (posx >= Mine.back.l && posx <= Mine.back.r && posy >= Mine.back.t && posy <= Mine.back.b)
					{
						PlayMusic(Mine.mp3[6]);
						Mine.if_main = Mine.if_draw = 0;
						DrawBack();
					}
				}
			}
		}
		//是否在难度选择界面
		else if (Mine.if_choice)
		{
			Mine.if_main = 0;
			if ((msg.message == WM_LBUTTONDOWN || msg.message == WM_LBUTTONDBLCLK))
			{
				for (int i = 0; i < 4; i++)
				{
					if (posx >= Mine.choice[i].l && posx <= Mine.choice[i].r && posy >= Mine.choice[i].t && posy <= Mine.choice[i].b)
					{
						PlayMusic(Mine.mp3[6]);
						Mine.op = i;
						Mine.max_mine = Mine.option[i];
						Mine.if_choice = Mine.if_draw = 0;
						RenderMap();
					}
				}
			}
		}
		//是否在成绩栏界面
		else if (Mine.if_score)
		{
			Mine.if_main = 0;
			if (posx >= Mine.record_back_l && posx <= Mine.record_back_r && posy >= Mine.record_back_t && posy <= Mine.record_back_b && (msg.message == WM_LBUTTONDOWN || msg.message == WM_LBUTTONDBLCLK))
			{
				PlayMusic(Mine.mp3[6]);
				Mine.if_score = Mine.if_draw = 0;
				Mine.if_main = 1;
			}
		}
		//是否在帮助界面
		else if (Mine.if_help)
		{
			//关闭帮助界面
			if (posx >= Mine.return_l && posx <= Mine.return_r && posy >= Mine.return_t && posy <= Mine.return_b && (msg.message == WM_LBUTTONDOWN || msg.message == WM_LBUTTONDBLCLK))
			{
				PlayMusic(Mine.mp3[6]);
				Mine.if_help = 0;
				DrawBack();
			}
			//操作帮助界面的选项框
			else if (Mine.fst_enter_game && posx >= Mine.ifhelp_l && posx <= Mine.ifhelp_r && posy >= Mine.ifhelp_t && posy <= Mine.ifhelp_b && (msg.message == WM_LBUTTONDOWN || msg.message == WM_LBUTTONDBLCLK))
			{
				PlayMusic(Mine.mp3[6]);
				if (Mine.if_tick)
				{
					Mine.if_tick = 0;
					putimage(0, 0, &Mine.M_img[17]);
				}
				else
				{
					Mine.if_tick = 1;
					putimage(0, 0, &Mine.M_img[16]);
				}
			}
		}
		//在扫雷界面
		else
		{
			if (Mine.fst_enter_game)
				Mine.fst_enter_game = 0;
			if (!Mine.if_pause)
			{
				//点击大方格
				if (posx >= Mine.Large.LeftSpace && posx <= Mine.Large.LeftSpace + MAX_L * Mine.large_cube && posy >= Mine.Large.TopSpace && posy <= Mine.Large.TopSpace + MAX_L * Mine.large_cube)
				{
					//统计所点击方格周围插旗数
					int cnt = 0;
					for (int i = row - 1; i <= row + 1; i++)
						for (int j = col - 1; j <= col + 1; j++)
							for (int l = Mine.depth - 1; l <= Mine.depth + 1; l++)
								if ((0 <= i && i < MAX_L && 0 <= j && j < MAX_L && 0 <= l && l < MAX_L) && Mine.Map[i][j][l] > 56)cnt++;
					//更新无操作初始时间
					if (msg.message == WM_LBUTTONDOWN || msg.message == WM_LBUTTONDBLCLK || msg.message == WM_RBUTTONDOWN) Mine.rest_t = time(NULL);
					//左键单击或双击未打开格子
					if ((msg.message == WM_LBUTTONDOWN || msg.message == WM_LBUTTONDBLCLK) && 29 <= Mine.Map[row][col][Mine.depth] && Mine.Map[row][col][Mine.depth] <= 56)
					{
						PlayMusic(Mine.mp3[3]);
						Mine.turn_Smile = 1;
						Mine.if_wait = 1;
						Mine.wait_t = time(NULL);
						//首次点击后初始化，避免第一次点到雷
						if (!Mine.fst_click)
						{
							GameInit(row, col, Mine.depth);
							Mine.fst_click = 1;
						}
						Mine.Map[row][col][Mine.depth] -= 30;
						Mine.cnt++;
						if (Mine.Map[row][col][Mine.depth] == 0) OpenNull(row, col, Mine.depth);
						if (Mine.Map[row][col][Mine.depth] == -1) Mine.fail = 1;
					}
					//左键双击已打开数字,炸开周围格子
					else if ((msg.message == WM_LBUTTONDOWN || msg.message == WM_LBUTTONDBLCLK) && 1 <= Mine.Map[row][col][Mine.depth] && Mine.Map[row][col][Mine.depth] <= 26 && cnt == Mine.Map[row][col][Mine.depth])
					{
						if (msg.message == WM_LBUTTONDBLCLK)
						{
							PlayMusic(Mine.mp3[3]);
							Mine.turn_Smile = 1;
							Mine.if_wait = 1;
							Mine.wait_t = time(NULL);
							OpenNull(row, col, Mine.depth);
						}
					}
					//右键单击未打开格子，插旗（非首次操作）
					else if (msg.message == WM_RBUTTONDOWN && Mine.fst_click && Mine.Map[row][col][Mine.depth] >= 29)
					{
						Mine.turn_Smile = 1;
						Mine.if_wait = 1;
						Mine.wait_t = time(NULL);
						//插旗
						if (29 <= Mine.Map[row][col][Mine.depth] && Mine.Map[row][col][Mine.depth] <= 56)
						{
							PlayMusic(Mine.mp3[4]);
							Mine.Map[row][col][Mine.depth] += 30;
							Mine.flag_cnt++;
						}
						//拔旗
						else if (Mine.Map[row][col][Mine.depth] > 56)
						{
							PlayMusic(Mine.mp3[7]);
							Mine.Map[row][col][Mine.depth] -= 30;
							Mine.flag_cnt--;
						}
					}
					//无效操作
					else if (msg.message == WM_LBUTTONDOWN || msg.message == WM_LBUTTONDBLCLK || msg.message == WM_RBUTTONDOWN)
					{
						PlayMusic(Mine.mp3[2]);
						Mine.turn_Smile = 2;
						Mine.if_wait = 1;
						Mine.wait_t = time(NULL);
					}
				}
			}
			//重新开始
			if (posx >= Mine.Icon_again.LeftSpace && posx <= Mine.Icon_again.LeftSpace + Mine.icon_wid && posy >= Mine.Icon_again.TopSpace && posy <= Mine.Icon_again.TopSpace + Mine.icon_len && (msg.message == WM_LBUTTONDOWN || msg.message == WM_LBUTTONDBLCLK))
			{
				PlayMusic(Mine.mp3[6]);
				RenderMap();
			}
			//帮助
			if (posx >= Mine.Icon_help.LeftSpace && posx <= Mine.Icon_help.LeftSpace + Mine.icon_wid && posy >= Mine.Icon_help.TopSpace && posy <= Mine.Icon_help.TopSpace + Mine.icon_len && (msg.message == WM_LBUTTONDOWN || msg.message == WM_LBUTTONDBLCLK))
			{
				PlayMusic(Mine.mp3[6]);
				Mine.if_help = 1;
				putimage(0, 0, &Mine.M_img[7]);
			}
			//暂停
			if (posx >= Mine.Icon_pause.LeftSpace && posx <= Mine.Icon_pause.LeftSpace + Mine.icon_wid && posy >= Mine.Icon_pause.TopSpace && posy <= Mine.Icon_pause.TopSpace + Mine.icon_len && (msg.message == WM_LBUTTONDOWN || msg.message == WM_LBUTTONDBLCLK))
			{
				PlayMusic(Mine.mp3[6]);
				if (!Mine.if_pause)
				{
					Mine.turn_Smile = 5;
					Mine.if_pause = 1;
					putimage(Mine.Icon_pause.LeftSpace, Mine.Icon_pause.TopSpace, &Mine.M_img[8]);
				}
				//继续游戏（在游戏结束后暂停键无效）
				else if (!Mine.fail && !Mine.win)
				{
					Mine.rest_t = time(NULL);
					Mine.pau_t += Mine.cur_t - Mine.last_t;
					Mine.turn_Smile = 0;
					Mine.if_pause = 0;
					putimage(Mine.Icon_pause.LeftSpace, Mine.Icon_pause.TopSpace, &Mine.M_img[9]);
				}
			}
			//声音
			if (posx >= Mine.Icon_voice.LeftSpace && posx <= Mine.Icon_voice.LeftSpace + Mine.icon_wid && posy >= Mine.Icon_voice.TopSpace && posy <= Mine.Icon_voice.TopSpace + Mine.icon_len && (msg.message == WM_LBUTTONDOWN || msg.message == WM_LBUTTONDBLCLK))
			{
				PlayMusic(Mine.mp3[6]);
				if (!Mine.if_voice)
				{
					Mine.if_voice = 1;
					char str[50] = "";
					sprintf(str, "pause ./Sweep_MP3/%s.MP3", Mine.mp3[5]);
					mciSendString(str, NULL, 0, NULL);
					putimage(Mine.Icon_voice.LeftSpace, Mine.Icon_voice.TopSpace, &Mine.M_img[13]);
				}
				else
				{
					Mine.if_voice = 0;
					char str[50] = "";
					sprintf(str, "play ./Sweep_MP3/%s.MP3 repeat", Mine.mp3[5]);
					mciSendString(str, NULL, 0, NULL);
					putimage(Mine.Icon_voice.LeftSpace, Mine.Icon_voice.TopSpace, &Mine.M_img[12]);
				}
			}
			//返回主界面
			if (posx >= Mine.Icon_return.LeftSpace && posx <= Mine.Icon_return.LeftSpace + Mine.icon_wid && posy >= Mine.Icon_return.TopSpace && posy <= Mine.Icon_return.TopSpace + Mine.icon_len && (msg.message == WM_LBUTTONDOWN || msg.message == WM_LBUTTONDBLCLK))
			{
				PlayMusic(Mine.mp3[6]);
				Mine.if_main = 1;
			}
			//层数切换
			if (posx >= Mine.Dep.LeftSpace && posx <= Mine.Dep.LeftSpace + MAX_L * Mine.large_cube && posy >= Mine.Dep.TopSpace && posy <= Mine.Dep.TopSpace + Mine.large_cube && (msg.message == WM_LBUTTONDOWN || msg.message == WM_LBUTTONDBLCLK))
			{
				Mine.depth = col_d;
				PlayMusic(Mine.mp3[3]);
			}
			Judge();
		}
	}
}
void ShowConsole()
{
	//第几次控制台打印Map(调试时使用,与游戏无关)
	static int time = 0;
	for (int d = 0; d < MAX_L; d++)
	{
		printf("%02d:", d + 1);
		for (int r = 0; r < MAX_L; r++)
		{
			if (r) printf("   ");
			for (int c = 0; c < MAX_L; c++)
				printf("%02d ", Mine.Map[r][c][d]);
			printf("\n");
		}
	}
	printf("time:%02d*******************************\n\n", time++);
}
