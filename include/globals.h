/*
	globals.h : グローバルヘッダファイル
	マクロ定義、構造体定義、グローバル変数、全関数宣言を集約
*/

#ifndef _globals_h
#define _globals_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

/* ===== マクロ定義 ===== */
#define N_ALL_NUM    100 // ノードの数
#define P_ALL_NUM    400  // 人間の数
#define P_goal     400 //目標ゴール人数
#define TRUE       1    //trueかfalseかのマクロ定義
#define FALSE      0
#define qsize     5000 // 交差点のキューの大きさ
#define ssize     5000 //ノードの持つスタックの大きさ
#define Ax        60   // サービスエリアの横幅 [m]
#define	Ay		  60   // サービスエリアの縦幅 [m]
#define Ax_d      15
#define Ay_d      15
#define Td        100  //交差点間距離 [m]
#define r         100  //通信可能距離 [m]
#define po        100  // ifにしたがって動く台数
#define p_add_p    0.0000//それぞれの地点で乗客が生成される確率
#define random    0
#define refdistance 250 //ノードが参照する距離(格子分)
#define reftime 30 //ノードが参照する時間差
#define EPIDEMIC
#define circle
#define dis_center

/* ===== 構造体定義 ===== */
//マップ
typedef struct{
	int info;
	int info_time;  //マップの最終更新時間
	int no_D;  //すでに目的地とした座標
}SMap;

// ノード
typedef struct{
	//共通で使うメンバ変数
	double n_X, n_Y;	 // ノードの位置
	double prog_d_surp;  //余剰進行距離
	int n_insec_X, n_insec_Y; //ノードが向かう交差点
	double v;			 //ノードの速度
	int p_on  ;			 // 乗客の人数
	int p_num;           // 乗客の番号
	int p_num2;           // 乗客の番号
	int	n_Already_Flooding;	 // 今の時間ステップにおいて、既にフラッディングしましたというフラグ
	int  n_xD,n_yD;               // ノードの宛先地点
	int  n_xD2,n_yD2;               // ノードの宛先地点
	int  stack_data[ssize];       //ノードが保持する客の待つ交差点情報
	int  stack_nopos[ssize];    //不必要なデータ保管スタック
	SMap  Map[Ax][Ay];
	int  stack_num;               //スタック内のデータ数
	int  stack_num2;              //スタック内のデータ数2
	int  all_stack_num;			//すべての得た情報数
	int  area_number;           
	int  move_flag;
	int stack_info[ssize][2];
	int stack_num3;
	double  d_length;                  // 乗客１の宛先地点までの距離
	double  d_length2;                  // 乗客２の乗客２自体の宛先地点までの距離
	int move_pattern;   //ランダム移動の際のパターン決定
}SNode;

//乗客
typedef struct{
	//共通で使うメンバ変数
	int	p_X, p_Y;		 // 乗客の位置
	int p_xS, p_yS;
	int p_Exist;         // 乗客がエリアに存在しているか
	int p_ride;          // 乗客が車に乗っているか
	int  p_xD,p_yD;               // 乗客の目的地
	int p_wait;
}SPass;

//交差点
typedef struct{
	//共通で使うメンバ変数
	int  wp_Exist;         // 待ち乗客が存在しているか waitpass
	int  wp_number[qsize];          // 待ち乗客の番号を保持したキュー
}STrans;

/* ===== グローバル変数宣言 ===== */
extern int queue_head[Ax][Ay];  //交差点のキューの初期値設定
extern int queue_num[Ax][Ay];   //交差点のキューのデータ数
extern int p_add_count;   //追加した乗客の数
extern int all10count[10000];         //10秒ごとのすべての到達人数
extern int all_count[10000];
extern double range[15];
extern int range_count[15];
extern int sum_all10count[500];
extern double ave_all10count[500];
extern int traffic_counter2[10000]; //10秒ごとの交通量
extern int transmit__[N_ALL_NUM][P_ALL_NUM]; //通信した利用客を記憶する配列
extern double speed_conter2[10000]; //特定区間の速度を保持する関数
extern double n; //実行回数

//確認用20231013
extern int stack_push[5000]; //スタックにpushされたデータを確認
extern int stack_pop[5000]; //スタックからpopされたデータを確認
extern int push_num;
extern int pop_num;

extern int check;
extern int check_1;
extern int check_2;
extern int check_3;
extern int check_4;
extern int goalstep[P_ALL_NUM]; //全員の到着ステップ格納
extern int o;
extern int Twait; //待ち時間
extern int allstep; //ステップ数の総和

extern int count_ifgo;  // 情報に従うノードがデータをもとに目的地を設定した回数
extern int count_getoff; //二人目の乗客をおろした回数
extern int count_ride1; //乗客が一人目を乗せた回数
extern int count_ride2; //乗客が二人目を乗せた回数
extern int total_transmit; //すべての情報交換回数
extern int count_ride3; //情報フローティングで得た情報をもとに乗客を拾った回数
extern int ride_transmit; //エピデミック通信で他の車から共有された待ち客情報によって乗客を拾った回数
extern int count_p_no; //情報に従うノードが目的地に到着したがいなかった回数
extern int count_p_on_3; //p_on = 3になる回数
extern int ridecount; //乗客が乗っていたステップ
extern int waitcount; //乗客が待っていたステップ
extern int distancemass;
extern int get_info; //情報を取得したカウント
extern int traffic_counter; //交通量をカウント
extern double speed_counter; //速度を保持
extern double v_f; //自由走行速度 [km/h]
extern double d_j; //道路の飽和密度
extern double free_variable; 
extern double sum;
extern int d[4][2];

extern double area_disaster; //避難区域半径
extern double area_evacuation;
extern double center_x;
extern double center_y;
extern int count_same;
extern int count_off;

// グローバル構造体配列
extern STrans Trans[Ax][Ay];
extern SNode Node[N_ALL_NUM];
extern SPass Pass[5000];

/* ===== 関数宣言 ===== */
// base_func.c
double sqrt2(double a_x, double a_y);
double randAxAy();
double randAxAy_d();
void enqueue(int p_number);
int dequeue(int x, int y);
int peek(int i);
void stack_sort();
void push(int i, int push_data);
void push2(int i, int push_data_1, int push_data_2);
void pushnopos(int i, int push_data2);
int search(int count, int number);
int search2(int count, int number_1, int number_2);
int pop(int i);

// Common_func.c
void allrideon();
void D_check();
int P_check();
void flooding();
int transmit(int k);
void detect_trans();
int syokika();
int check_ride();

// move.c
void Move();
void move_update_coord();
void move_update_coord_surplus();
void move_new_direction();
void get_traffic_volume();
void speed_error(int count);

// my_func.c
void Dist_Init_n_D();
void initial_move();
void initial_D_check();
void initial_detect_trans();

// placement.c
void Dist_Init_p();
void Dist_Init_n();
void Dist_add_p();
void Pass_Spawn();
void position_p_get();
void Set_p_Goal();
void pass_input_inter_init();

#endif
