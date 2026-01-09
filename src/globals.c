/*
    globals.c : グローバル変数の実体定義
*/
#include "globals.h"

/* 構造体配列の実体定義 */
STrans Trans[Ax][Ay];
SNode Node[N_ALL_NUM];
SPass Pass[5000];

/* main.cで定義されていたグローバル変数の実体 */
int queue_head[Ax][Ay];
int queue_num[Ax][Ay];
int p_add_count;
int all10count[10000];
int all_count[10000];
double range[15];
int range_count[15];
int sum_all10count[500];
double ave_all10count[500];
int traffic_counter2[10000];
int transmit__[N_ALL_NUM][P_ALL_NUM];
double speed_conter2[10000];
double n = 0;

int stack_push[5000];
int stack_pop[5000];
int push_num = 0;
int pop_num = 0;

int check;
int check_1;
int check_2;
int check_3;
int check_4;
int goalstep[P_ALL_NUM];
int o = 0;
int Twait = 0;
int allstep;

int count_ifgo = 0;
int count_getoff = 0;
int count_ride1 = 0;
int count_ride2 = 0;
int total_transmit = 0;
int count_ride3 = 0;
int ride_transmit = 0;
int count_p_no = 0;
int count_p_on_3 = 0;
int count_map = 0;
int ridecount = 0;
int waitcount = 0;
int distancemass = 0;
int get_info = 0;
int traffic_counter = 0;
double speed_counter = 0;
double v_f = 40;
double d_j = 0.12;
double free_variable;
double sum_Twait = 0;
double sum_P_Twait = 0;
int d[4][2] = {{10, 10}, {10, 50}, {50, 50}, {50, 10}};

double area_disaster = (Ax - 2*Ax_d)/2; //避難区域
double area_evacuation = Ax/2;
double center_x  = Ax/2;
double center_y  = Ay/2;
int count_same = 0;
int count_off= 0;

/* P_map関数用のグローバル変数 */
int grid_count[grid_size][grid_size];
int smoothed_count[grid_size][grid_size];
int cell_width = Ax / 20;
int cell_height = Ay / 20;

/* スコア計算用の重みパラメータ */
double W_dens = 3.0;  // 混雑度の重み
double W_dist = 8.0;  // 距離の重み（大きくして距離を重視）
