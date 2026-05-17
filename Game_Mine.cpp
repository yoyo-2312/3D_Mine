#include "Game_Mine.h"
#include "algorithm_function.h"
#include "draw_function.h"
struct GAME Mine;
void PrepareGame()
{
	//大小方格和表情左上角坐标
	Mine.Large.LeftSpace = 69;
	Mine.Large.TopSpace = 181;
	Mine.Small_l.LeftSpace = 788;
	Mine.Small_l.TopSpace = 180;
	Mine.Small_r.LeftSpace = 788;
	Mine.Small_r.TopSpace = 588;
	Mine.Dep.LeftSpace = 69;
	Mine.Dep.TopSpace = 887;
	Mine.Smile.LeftSpace = 339;
	Mine.Smile.TopSpace = 35;
	//图标长宽
	Mine.icon_wid = 86, Mine.icon_len = 86;
	Mine.icondot_wid = 90, Mine.icondot_len = 93;
	//帮助/重新开始/暂停/声音/返回图标左上角
	Mine.Icon_help.LeftSpace = Mine.Small_l.LeftSpace;
	Mine.Icon_again.LeftSpace = Mine.Icon_help.LeftSpace + Mine.icon_wid + 10;
	Mine.Icon_pause.LeftSpace = Mine.Icon_again.LeftSpace + Mine.icon_wid + 10;
	Mine.Icon_voice.LeftSpace = Mine.Icon_pause.LeftSpace + Mine.icon_wid + 10;
	Mine.Icon_return.LeftSpace = Mine.Icon_help.LeftSpace - Mine.icon_wid - 10;
	Mine.Icon_pause.TopSpace = Mine.Icon_help.TopSpace = Mine.Icon_again.TopSpace = Mine.Icon_voice.TopSpace = Mine.Icon_return.TopSpace = 41;
	//计时器分秒左上角坐标/长宽
	Mine.T_wid = 46, Mine.T_len = 73, Mine.T_space = 22;
	Mine.T_min1.LeftSpace = 475;
	Mine.T_min2.LeftSpace = Mine.T_min1.LeftSpace + Mine.T_wid;
	Mine.T_sec1.LeftSpace = Mine.T_min2.LeftSpace + Mine.T_space + Mine.T_wid;
	Mine.T_sec2.LeftSpace = Mine.T_sec1.LeftSpace + Mine.T_wid;
	Mine.T_min1.TopSpace = Mine.T_min2.TopSpace = Mine.T_sec1.TopSpace = Mine.T_sec2.TopSpace = 46;
	//表情边长/大小方格边长
	Mine.Smile_a = 91, Mine.large_cube = 68, Mine.small_cube = 37;
	//计雷3张图片分别的左侧坐标以及共同的上下坐标
	Mine.mine_left = 89, Mine.mine_top = 46, Mine.mine_icon = 85, Mine.mine_wid = 40, Mine.mine_len = 73;
	//帮助选项框的左右上下
	Mine.ifhelp_l = 422, Mine.ifhelp_r = 457, Mine.ifhelp_t = 917, Mine.ifhelp_b = 952;
	//难度选择的左右上下
	Mine.choice[0].l = Mine.choice[2].l = 247, Mine.choice[0].r = Mine.choice[2].r = 525;
	Mine.choice[1].l = Mine.choice[3].l = 666, Mine.choice[1].r = Mine.choice[3].r = 950;
	Mine.choice[0].t = Mine.choice[1].t = 331, Mine.choice[0].b = Mine.choice[1].b = 526;
	Mine.choice[2].t = Mine.choice[3].t = 627, Mine.choice[2].b = Mine.choice[3].b = 855;
	//主界面开始游戏的左右上下
	Mine.start.l = 423, Mine.start.r = 785, Mine.start.t = 616, Mine.start.b = 848;
	// 主界面重新开始的左右上下
	Mine.restart.l = 638, Mine.restart.r = 906, Mine.restart.t = 624, Mine.restart.b = 818;
	// 主界面继续游戏的左右上下
	Mine.back.l = 318, Mine.back.r = 543, Mine.back.t = 624, Mine.back.b = 818;
	//雷增减数字长宽
	Mine.add_wid = 21, Mine.add_len = 34;
	//雷增减数字上下
	Mine.add_left = 21, Mine.add_top = 34, Mine.add_bot = 53;
	//记录单返回的左右上下
	Mine.record_back_l = 106, Mine.record_back_r = 196, Mine.record_back_t = 73, Mine.record_back_b = 164;
	//记录单数字的长宽
	Mine.num_wid = 18, Mine.num_len = 63;
	//记录单数字的左上坐标
	Mine.record_cur_mine[0].left = Mine.record_cur_mine[1].left = Mine.record_cur_mine[2].left = Mine.record_cur_mine[3].left = 371;
	Mine.record_cur_time[0].left = Mine.record_cur_time[1].left = Mine.record_cur_time[2].left = Mine.record_cur_time[3].left = 491;
	Mine.record_best_mine[0].left = Mine.record_best_mine[1].left = Mine.record_best_mine[2].left = Mine.record_best_mine[3].left = 713;
	Mine.record_best_time[0].left = Mine.record_best_time[1].left = Mine.record_best_time[2].left = Mine.record_best_time[3].left = 834;
	Mine.record_cur_mine[0].top = Mine.record_cur_time[0].top = Mine.record_best_mine[0].top = Mine.record_best_time[0].top = 294;
	Mine.record_cur_mine[1].top = Mine.record_cur_time[1].top = Mine.record_best_mine[1].top = Mine.record_best_time[1].top = 454;
	Mine.record_cur_mine[2].top = Mine.record_cur_time[2].top = Mine.record_best_mine[2].top = Mine.record_best_time[2].top = 624;
	Mine.record_cur_mine[3].top = Mine.record_cur_time[3].top = Mine.record_best_mine[3].top = Mine.record_best_time[3].top = 800;
	//返回的左右上下
	Mine.return_l = 952, Mine.return_r = 1128, Mine.return_t = 778, Mine.return_b = 922;
	//图标名称
	Mine.icon[0] = "pause", Mine.icon[1] = "continue", Mine.icon[2] = "help", Mine.icon[3] = "again", Mine.icon[4] = "play", Mine.icon[5] = "quiet", Mine.icon[6] = "return", Mine.icon[7] = "return_dot";
	//音乐名称
	Mine.mp3[0] = "win", Mine.mp3[1] = "Loss", Mine.mp3[2] = "Err", Mine.mp3[3] = "Click", Mine.mp3[4] = "Flag", Mine.mp3[5] = "Back", Mine.mp3[6] = "Other", Mine.mp3[7] = "Deflag";
	//打开背景音乐
	char str[50] = "";
	sprintf(str, "close ./Sweep_MP3/%s.MP3", Mine.mp3[5]);
	mciSendString(str, NULL, 0, NULL);
	sprintf(str, "open ./Sweep_MP3/%s.MP3", Mine.mp3[5]);
	mciSendString(str, NULL, 0, NULL);
	//画窗口
	initgraph(1200, 1080, EX_DBLCLKS);
	LoadIMG();
}
void PreOperation()
{
	//播放背景音乐
	char str[50] = "";
	sprintf(str, "play ./Sweep_MP3/%s.MP3 repeat", Mine.mp3[5]);
	mciSendString(str, NULL, 0, NULL);
	//首次进入游戏
	Mine.op = -1;
	Mine.fst_enter_game = Mine.if_main = 1;
	Mine.if_draw = Mine.if_score = Mine.if_choice = Mine.if_voice = Mine.if_tick = Mine.if_new = 0;
}
void RenderMap()
{
	//首次操作前未布置雷，先显示未打开空格图片
	for (int r = 0; r < MAX_L; r++)
		for (int c = 0; c < MAX_L; c++)
			for (int d = 0; d < MAX_L; d++)
				Mine.Map[r][c][d] = 30;
	//清零时间分秒/当前层/暂停时间段/打开格子数/插旗数/首次点击/表情变化/失败/胜利,表情设为正常
	Mine.time_all = Mine.Min = Mine.Sec = Mine.min1 = Mine.min2 = Mine.sec1 = Mine.sec2 = Mine.depth = Mine.pau_t = Mine.cnt = Mine.flag_cnt = Mine.r_num = Mine.w_num = Mine.fst_click = Mine.if_pause = Mine.if_wait = Mine.fail = Mine.win = Mine.turn_Smile = 0;
	//绘制背景
	DrawBack();
	//首次进入游戏显示帮助
	if (Mine.fst_enter_game && !Mine.if_tick)
	{
		Mine.if_help = 1;
		putimage(0, 0, &Mine.M_img[17]);
	}
	else Mine.if_help = 0;
}
void Mine3DLoop()
{
	while (1)
	{
		if (WindowClosed())
			ExitGame();

		//第一次点击后开始计时
		if (Mine.fst_click)Time();
		//显示帮助时其他图片不显示
		if (Mine.if_main)
			MainDraw();
		else if (Mine.if_score)
			ScoreDraw(Mine.Best);
		else if (Mine.if_choice)
			putimage(0, 0, &Mine.M_img[20]);
		else if (!Mine.if_help)
			GameDraw();
		MouseEvent();
	}
}
