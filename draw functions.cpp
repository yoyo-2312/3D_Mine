#include "Game_Mine.h"
#include "draw_function.h" 
#include "algorithm_function.h"
#include "embedded_assets.h"

using namespace std;

static void LoadGameImage(IMAGE* image, const char* assetName, int width, int height)
{
	LoadEmbeddedImage(image, assetName, width, height);
}

//加载图片(内容详见Game_Mine.h)
void LoadIMG()
{
	LoadGameImage(&Mine.record[4], "record", 1200, 1000);
	LoadGameImage(&Mine.M_img[0], "background", 1200, 1000);
	LoadGameImage(&Mine.M_img[7], "helpboard", 1200, 1000);
	LoadGameImage(&Mine.M_img[20], "option", 1200, 1000);
	LoadGameImage(&Mine.M_img[19], "restart", 1200, 1000);
	LoadGameImage(&Mine.M_img[18], "start", 1200, 1000);
	LoadGameImage(&Mine.M_img[17], "illustration", 1200, 1000);
	LoadGameImage(&Mine.M_img[16], "illustration_tick", 1200, 1000);
	LoadGameImage(&Mine.M_img[15], "return_dot", Mine.icondot_wid, Mine.icondot_len);
	LoadGameImage(&Mine.M_img[21], "score_dot", 93, 98);
	LoadGameImage(&Mine.M_img[22], "score", 90, 90);
	LoadGameImage(&Mine.minus, "-", Mine.mine_wid, Mine.mine_len);
	LoadGameImage(&Mine.mine, "mine", Mine.mine_icon, Mine.mine_len);
	LoadGameImage(&Mine.space, "space", Mine.T_space, Mine.T_len);
	LoadGameImage(&Mine.number[20], "colon_w", Mine.num_wid, Mine.num_len);
	LoadGameImage(&Mine.number[21], "colon_b", Mine.num_wid, Mine.num_len);
	LoadGameImage(&Mine.number[22], "none", Mine.num_wid, Mine.num_len);
	LoadGameImage(&Mine.add[20], "+r", Mine.add_wid, Mine.add_len);
	LoadGameImage(&Mine.add[21], "-w", Mine.add_wid, Mine.add_len);
	char str[100] = "";
	sprintf(str, "29f");
	LoadGameImage(&Mine.L_img[31], str, Mine.large_cube, Mine.large_cube);
	LoadGameImage(&Mine.S_img[31], str, Mine.small_cube, Mine.small_cube);
	for (int i = 0; i < 4; i++)
	{
		sprintf(str, "record%d", i);
		LoadGameImage(&Mine.record[i], str, 1200, 1000);
	}
	for (int i = 1; i <= 6; i++)
	{
		sprintf(str, "%d", 60 + i);
		LoadGameImage(&Mine.M_img[i], str, Mine.Smile_a, Mine.Smile_a);
	}
	for (int i = 0; i <= 6; i++)
	{
		sprintf(str, "%s", Mine.icon[i]);
		LoadGameImage(&Mine.M_img[i + 8], str, Mine.icon_wid, Mine.icon_len);
	}
	for (int i = 0; i <= 30; i++)
	{
		sprintf(str, "%d", i);
		LoadGameImage(&Mine.L_img[i], str, Mine.large_cube, Mine.large_cube);
		LoadGameImage(&Mine.S_img[i], str, Mine.small_cube, Mine.small_cube);
	}
	for (int i = 1; i <= 10; i++)
	{
		sprintf(str, "%d", i + 30);
		LoadGameImage(&Mine.D_img[i - 1], str, Mine.large_cube, Mine.large_cube);
		sprintf(str, "%d", i + 40);
		LoadGameImage(&Mine.D_img[i + 9], str, Mine.large_cube, Mine.large_cube);
		sprintf(str, "%d", i + 50);
		LoadGameImage(&Mine.T_img[i % 10], str, Mine.T_wid, Mine.T_len);
		LoadGameImage(&Mine.T_img[i % 10 + 10], str, Mine.mine_wid, Mine.mine_len);
		LoadGameImage(&Mine.number[i % 10], str, Mine.num_wid, Mine.num_len);
		sprintf(str, "%d", i + 70);
		LoadGameImage(&Mine.number[i % 10 + 10], str, Mine.num_wid, Mine.num_len);
		sprintf(str, "%d", i + 80);
		LoadGameImage(&Mine.add[i % 10], str, Mine.add_wid, Mine.add_len);
		sprintf(str, "%d", i + 90);
		LoadGameImage(&Mine.add[i % 10 + 10], str, Mine.add_wid, Mine.add_len);
	}
}
//绘制扫雷界面背景
void DrawBack()
{
	putimage(0, 0, &Mine.M_img[0]);
	DrawLine();
	putimage(Mine.mine_left, Mine.mine_top, &Mine.mine);
	putimage(Mine.T_min1.LeftSpace + Mine.T_wid * 2, Mine.T_min1.TopSpace, &Mine.space);
	line(Mine.T_min2.LeftSpace + Mine.T_wid + 10, 69, Mine.T_min2.LeftSpace + Mine.T_wid + 10, 73);
	line(Mine.T_min2.LeftSpace + Mine.T_wid + 10, 89, Mine.T_min2.LeftSpace + Mine.T_wid + 10, 93);
	line(Mine.T_min2.LeftSpace + Mine.T_wid + 8, 71, Mine.T_min2.LeftSpace + Mine.T_wid + 12, 71);
	line(Mine.T_min2.LeftSpace + Mine.T_wid + 8, 91, Mine.T_min2.LeftSpace + Mine.T_wid + 12, 91);
	if (Mine.if_new) putimage(Mine.Icon_return.LeftSpace, Mine.Icon_return.TopSpace - Mine.icondot_len + Mine.icon_len, &Mine.M_img[15]);
	else putimage(Mine.Icon_return.LeftSpace, Mine.Icon_return.TopSpace, &Mine.M_img[14]);
	if (Mine.if_pause && !Mine.fail && !Mine.win)
		putimage(Mine.Icon_pause.LeftSpace, Mine.Icon_pause.TopSpace, &Mine.M_img[8]);
	else putimage(Mine.Icon_pause.LeftSpace, Mine.Icon_pause.TopSpace, &Mine.M_img[9]);
	putimage(Mine.Icon_help.LeftSpace, Mine.Icon_help.TopSpace, &Mine.M_img[10]);
	putimage(Mine.Icon_again.LeftSpace, Mine.Icon_again.TopSpace, &Mine.M_img[11]);
	if (Mine.if_voice)
		putimage(Mine.Icon_voice.LeftSpace, Mine.Icon_voice.TopSpace, &Mine.M_img[13]);
	else putimage(Mine.Icon_voice.LeftSpace, Mine.Icon_voice.TopSpace, &Mine.M_img[12]);
	if (Mine.fail || Mine.win) AddDraw();
}
void DrawLine()
{
	//画主界面表情计时计雷边框
	setlinecolor(0x26437E);
	setlinestyle(PS_SOLID, 8);
	//表情
	line(337, 33, 337, 129);
	line(432, 33, 432, 129);
	line(337, 33, 432, 33);
	line(337, 129, 432, 129);
	//计雷
	line(87, 44, 87, 121);
	line(296, 44, 296, 121);
	line(87, 44, 296, 44);
	line(87, 121, 296, 121);
	//计时
	line(473, 44, 473, 121);
	line(682, 44, 682, 121);
	line(473, 44, 682, 44);
	line(473, 121, 682, 121);
	//补线
	line(749, 181, 749, 861);
	line(752, 881, 752, 952);
	line(67, 882, 752, 882);
	line(65, 881, 65, 952);
	line(754, 881, 754, 952);
}
void PutIMG(int dep, int size, SPACE typ, IMAGE img[])
{
	//画层数栏
	if (typ.TopSpace == Mine.Dep.TopSpace)
		for (int i = 0; i < MAX_L; i++)
		{
			if (i != dep) putimage(typ.LeftSpace + i * size, typ.TopSpace, &img[i]);
			else putimage(typ.LeftSpace + i * size, typ.TopSpace, &img[i + 10]);
		}
	//画大小方格
	else
	{
		if (dep >= 0 && dep < MAX_L)
			for (int i = 0; i < MAX_L; i++)
				for (int j = 0; j < MAX_L; j++)
				{
					if (Mine.Map[i][j][dep] >= 0 && Mine.Map[i][j][dep] <= 26)
						putimage(typ.LeftSpace + j * size, typ.TopSpace + i * size, &img[Mine.Map[i][j][dep]]);
					else if (Mine.Map[i][j][dep] == -1)
						putimage(typ.LeftSpace + j * size, typ.TopSpace + i * size, &img[29]);
					else if (Mine.Map[i][j][dep] >= 29 && Mine.Map[i][j][dep] <= 56)
						putimage(typ.LeftSpace + j * size, typ.TopSpace + i * size, &img[30]);
					else if (Mine.Map[i][j][dep] >= 59 && Mine.Map[i][j][dep] <= 86)
						putimage(typ.LeftSpace + j * size, typ.TopSpace + i * size, &img[28]);
					else if (Mine.Map[i][j][dep] > 86)
						putimage(typ.LeftSpace + j * size, typ.TopSpace + i * size, &img[27]);
					else if (Mine.Map[i][j][dep] == -4)
						putimage(typ.LeftSpace + j * size, typ.TopSpace + i * size, &img[31]);
				}
		//大方格为顶/底层时某个小方格显示全空白格
		else for (int i = 0; i < MAX_L; i++)
			for (int j = 0; j < MAX_L; j++)
				putimage(typ.LeftSpace + i * size, typ.TopSpace + j * size, &img[0]);
	}
}
//绘制扫雷界面
void GameDraw()
{
	//计雷正数
	if (Mine.flag_cnt <= Mine.max_mine)
	{
		putimage(Mine.mine_left + Mine.mine_icon, Mine.mine_top, &Mine.T_img[(Mine.max_mine - Mine.flag_cnt) / 100 + 10]);
		putimage(Mine.mine_left + Mine.mine_icon + Mine.mine_wid, Mine.mine_top, &Mine.T_img[(Mine.max_mine - Mine.flag_cnt) / 10 % 10 + 10]);
		putimage(Mine.mine_left + Mine.mine_icon + Mine.mine_wid * 2, Mine.mine_top, &Mine.T_img[(Mine.max_mine - Mine.flag_cnt) % 10 + 10]);
	}
	//计雷负数
	else
	{
		putimage(Mine.mine_left + Mine.mine_icon, Mine.mine_top, &Mine.minus);
		putimage(Mine.mine_left + Mine.mine_icon + Mine.mine_wid, Mine.mine_top, &Mine.T_img[-(Mine.max_mine - Mine.flag_cnt) / 10 % 10 + 10]);
		putimage(Mine.mine_left + Mine.mine_icon + Mine.mine_wid * 2, Mine.mine_top, &Mine.T_img[-(Mine.max_mine - Mine.flag_cnt) % 10 + 10]);
	}
	//计时
	putimage(Mine.T_min1.LeftSpace, Mine.T_min1.TopSpace, &Mine.T_img[Mine.min1]);
	putimage(Mine.T_min2.LeftSpace, Mine.T_min2.TopSpace, &Mine.T_img[Mine.min2]);
	putimage(Mine.T_sec1.LeftSpace, Mine.T_sec1.TopSpace, &Mine.T_img[Mine.sec1]);
	putimage(Mine.T_sec2.LeftSpace, Mine.T_sec2.TopSpace, &Mine.T_img[Mine.sec2]);
	//表情
	putimage(Mine.Smile.LeftSpace, Mine.Smile.TopSpace, &Mine.M_img[Mine.turn_Smile + 1]);
	//上下小方格、大方格、层数栏
	PutIMG(Mine.depth - 1, Mine.small_cube, Mine.Small_l, Mine.S_img);
	PutIMG(Mine.depth + 1, Mine.small_cube, Mine.Small_r, Mine.S_img);
	PutIMG(Mine.depth, Mine.large_cube, Mine.Large, Mine.L_img);
	PutIMG(Mine.depth, Mine.large_cube, Mine.Dep, Mine.D_img);
}
//绘制主界面
void MainDraw()
{
	if (!Mine.if_draw)
	{
		if (Mine.fst_enter_game) putimage(0, 0, &Mine.M_img[18]);
		else putimage(0, 0, &Mine.M_img[19]);
		if (Mine.if_new) putimage(Mine.record_back_l, Mine.record_back_t - Mine.icondot_len + Mine.icon_len, &Mine.M_img[21]);
		else putimage(Mine.record_back_l, Mine.record_back_t, &Mine.M_img[22]);
	}
	Mine.if_draw = 1;
}
//绘制成绩栏
void ScoreDraw(struct RECORD* cur)
{
	GetBest(Mine.fp);
	int tmp = 10;
	if (!Mine.if_draw)
	{
		if (Mine.op == -1) putimage(0, 0, &Mine.record[4]);
		else putimage(0, 0, &Mine.record[Mine.op]);
	}
	for (int i = 0; i < 4; i++)
	{
		//当前难度的一栏显示高亮
		if (i == Mine.op)
		{
			tmp = 0;
			putimage(Mine.record_best_time[i].left + Mine.num_wid * 2, Mine.record_best_time[i].top, &Mine.number[20]);
			putimage(Mine.record_cur_time[i].left + Mine.num_wid * 2, Mine.record_cur_time[i].top, &Mine.number[20]);
			//正在扫雷时当前难度的当前正确插旗数不确定(显示问号)
			if (Mine.fst_click && !Mine.fail && !Mine.win)
			{
				putimage(Mine.record_cur_mine[i].left, Mine.record_cur_mine[i].top, &Mine.number[22]);
				putimage(Mine.record_cur_mine[i].left + Mine.num_wid, Mine.record_cur_mine[i].top, &Mine.number[22]);
			}
			else
			{
				putimage(Mine.record_cur_mine[i].left, Mine.record_cur_mine[i].top, &Mine.number[Mine.flag_cnt / 10 % 10]);
				putimage(Mine.record_cur_mine[i].left + Mine.num_wid, Mine.record_cur_mine[i].top, &Mine.number[Mine.flag_cnt % 10]);
			}
			//显示当前时间
			putimage(Mine.record_cur_time[i].left, Mine.record_cur_time[i].top, &Mine.number[Mine.min1]);
			putimage(Mine.record_cur_time[i].left + Mine.num_wid, Mine.record_cur_time[i].top, &Mine.number[Mine.min2]);
			putimage(Mine.record_cur_time[i].left + Mine.num_wid * 3, Mine.record_cur_time[i].top, &Mine.number[Mine.sec1]);
			putimage(Mine.record_cur_time[i].left + Mine.num_wid * 4, Mine.record_cur_time[i].top, &Mine.number[Mine.sec2]);
		}
		else
		{
			putimage(Mine.record_best_time[i].left + Mine.num_wid * 2, Mine.record_best_time[i].top, &Mine.number[21]);
			putimage(Mine.record_cur_time[i].left + Mine.num_wid * 2, Mine.record_cur_time[i].top, &Mine.number[21]);
			putimage(Mine.record_cur_mine[i].left, Mine.record_cur_mine[i].top, &Mine.number[10 + cur->last_num / 10 % 10]);
			putimage(Mine.record_cur_mine[i].left + Mine.num_wid, Mine.record_cur_mine[i].top, &Mine.number[10 + cur->last_num % 10]);
			putimage(Mine.record_cur_time[i].left, Mine.record_cur_time[i].top, &Mine.number[10 + cur->last_m1]);
			putimage(Mine.record_cur_time[i].left + Mine.num_wid, Mine.record_cur_time[i].top, &Mine.number[10 + cur->last_m2]);
			putimage(Mine.record_cur_time[i].left + Mine.num_wid * 3, Mine.record_cur_time[i].top, &Mine.number[10 + cur->last_s1]);
			putimage(Mine.record_cur_time[i].left + Mine.num_wid * 4, Mine.record_cur_time[i].top, &Mine.number[10 + cur->last_s2]);
		}
		putimage(Mine.record_best_mine[i].left, Mine.record_best_mine[i].top, &Mine.number[tmp + cur->num / 10 % 10]);
		putimage(Mine.record_best_mine[i].left + Mine.num_wid, Mine.record_best_mine[i].top, &Mine.number[tmp + cur->num % 10]);
		putimage(Mine.record_best_time[i].left, Mine.record_best_time[i].top, &Mine.number[tmp + cur->m1]);
		putimage(Mine.record_best_time[i].left + Mine.num_wid, Mine.record_best_time[i].top, &Mine.number[tmp + cur->m2]);
		putimage(Mine.record_best_time[i].left + Mine.num_wid * 3, Mine.record_best_time[i].top, &Mine.number[tmp + cur->s1]);
		putimage(Mine.record_best_time[i].left + Mine.num_wid * 4, Mine.record_best_time[i].top, &Mine.number[tmp + cur->s2]);
		tmp = 10;
		cur++;
	}
	Mine.if_draw = 1;
}
//绘制计雷左侧雷个数增减(游戏结束时)
void AddDraw()
{
	putimage(Mine.add_left, Mine.add_top, &Mine.add[20]);
	putimage(Mine.add_left + Mine.add_wid, Mine.add_top, &Mine.add[Mine.r_num / 10 % 10]);
	putimage(Mine.add_left + Mine.add_wid * 2, Mine.add_top, &Mine.add[Mine.r_num % 10]);
	putimage(Mine.add_left, Mine.add_top + Mine.add_len, &Mine.add[21]);
	putimage(Mine.add_left + Mine.add_wid, Mine.add_top + Mine.add_len, &Mine.add[10 + Mine.w_num / 10 % 10]);
	putimage(Mine.add_left + Mine.add_wid * 2, Mine.add_top + Mine.add_len, &Mine.add[10 + Mine.w_num % 10]);
}
