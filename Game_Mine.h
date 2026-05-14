#ifndef _GAME
#define _GAME
#define _CRT_SECURE_NO_WARNINGS
#define MAX_L 10
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <graphics.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
struct SPACE {
	int TopSpace, LeftSpace;
};
struct BUTTON {
	int l, r, b, t;
};
struct POS {
	int left, top;
};
struct RECORD {
	int num, m1, m2, s1, s2, last_num, last_m1, last_m2, last_s1, last_s2;
};
struct GAME {
	int cnt, flag_cnt, max_mine, r_num, w_num;//打开格子数,插旗格子数,雷总数,有效排雷数,无效插旗数
	int time_all, Min, Sec, min1, min2, sec1, sec2;//总时/s,分,秒,分十个位,秒十个位
	const char* mp3[10], * icon[10];
	int option[4] = { 10,30,50,100 };
	bool if_tick, if_new;//点击帮助选项框,查看成绩更新
	bool fst_enter_game, fst_click, fail, win, if_pause, if_wait, if_help, if_voice, if_main, if_score, if_choice, if_draw;//首次进入游戏,每局游戏首次点击,失败/成功,暂停,表情变化,帮助,声音,主界面,成绩栏,难度选择,绘制界面
	int mine_left, mine_top, mine_wid, mine_len, mine_icon;//计雷3张图片分别的左侧坐标以及共同的上下坐标
	int num_wid, num_len;//成绩栏数字长宽
	int add_wid, add_len;//雷增减数字长宽
	int add_left, add_top, add_bot;//雷增减数字上下
	int return_l, return_r, return_t, return_b;//返回的左右上下


	int record_back_l, record_back_r, record_back_t, record_back_b;//记录单返回的左右上下
	int ifhelp_l, ifhelp_r, ifhelp_t, ifhelp_b;//帮助选项框的左右上下
	int op;//0:简单/10 Mines,1:中等/30 Mines,2:困难/50 Mines,3:地狱/100 Mines
	int turn_Smile;//0:正常/闭嘴,1:有效/张嘴,2:无效/耷耳,3:胜利/皇冠,4:失败/死掉,5:待机/睡觉
	int depth;//当前层
	int large_cube, small_cube;//大小方格边长
	int T_wid, T_len, T_space, icon_wid, icon_len, icondot_wid, icondot_len, Smile_a;
	int Map[MAX_L][MAX_L][MAX_L];//-4:其他雷,-1:雷,0:空白,1-26:数字,29-56:未打开,59-86:插旗,90-116:错旗
	struct SPACE Large, Small_l, Small_r, Dep, Smile, T_min1, T_min2, T_sec1, T_sec2, Icon_pause, Icon_help, Icon_again, Icon_voice, Icon_return;
	//大方格,左上小方格,左下小方格,层数栏,表情,时间分秒十个位,三个图标
	struct BUTTON choice[4], back, start, restart;//难度选择的左右上下
	struct POS record_cur_mine[4], record_cur_time[4], record_best_mine[4], record_best_time[4];//记录单数字左上坐标
	IMAGE M_img[23];//0:background,1-6:smile,7:helpboard,8:pause,9:continue,10:help,11:again,12:play,13:quiet,14:return,15:return_dot,16:illustration_tick,17:illustration,18:start,19:restart,20:option,21:score_dot,22:score
	IMAGE L_img[32];//大图标
	IMAGE S_img[32];//小图标
	//27:错旗,28:旗,29:炸开雷,30:未点开,31:剩余雷
	IMAGE D_img[20];//0-9:其他层,10-19:当前层
	IMAGE T_img[20];//0-9:计时,10-19:计雷
	IMAGE minus, mine, space;//计时器负号,雷图标,计时器冒号
	IMAGE record[5];//记录单背景 0-3:四个难度 4:初始
	IMAGE number[23];//记录单数字 0-9:white,10-19:black,20:white colon,21:black colon,22:none
	IMAGE add[22];//雷增减数
	time_t sta_t, pau_t, cur_t, last_t, wait_t, rest_t;//起始时间点,暂停时间段,当前、下一刻、表情变化、无操作时间点
	FILE* fp;
	struct RECORD Best[4];//最高成绩记录
};
extern struct GAME Mine;
void PreOperation();
void RenderMap();
void Mine3DLoop();
void PrepareGame();
#endif