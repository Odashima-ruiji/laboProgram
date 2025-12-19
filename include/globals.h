/*
    globals.h : グローバル変数の宣言
*/
#ifndef _globals_h
#define _globals_h

#include "config.h"
#include "base_struct.h"

/* 構造体配列のextern宣言 */
extern STrans Trans[Ax][Ay];
extern SNode Node[N_ALL_NUM];
extern SPass Pass[5000];

/* main.cで定義されていたグローバル変数 */
extern int queue_head[Ax][Ay];
extern int queue_num[Ax][Ay];
extern int p_add_count;
extern int all10count[10000];
extern int all_count[10000];
extern double range[15];
extern int range_count[15];
extern int sum_all10count[500];
extern double ave_all10count[500];
extern int traffic_counter2[10000];
extern int transmit__[N_ALL_NUM][P_ALL_NUM];
extern double speed_conter2[10000];
extern double n;

extern int stack_push[5000];
extern int stack_pop[5000];
extern int push_num;
extern int pop_num;

extern int check;
extern int check_1;
extern int check_2;
extern int check_3;
extern int check_4;
extern int goalstep[P_ALL_NUM];
extern int o;
extern int Twait;
extern int allstep;

extern int count_ifgo;
extern int count_getoff;
extern int count_ride1;
extern int count_ride2;
extern int total_transmit;
extern int count_ride3;
extern int ride_transmit;
extern int count_p_no;
extern int count_p_on_3;
extern int count_map;
extern int ridecount;
extern int waitcount;
extern int distancemass;
extern int get_info;
extern int traffic_counter;
extern double speed_counter;
extern double v_f;
extern double d_j;
extern double free_variable;
extern double sum_Twait;
extern double sum_P_Twait;
extern int d[4][2];

extern double area_disaster;
extern double area_evacuation;
extern double center_x;
extern double center_y;
extern int count_same;
extern int count_off;

/* P_map関数用のグローバル変数 */
extern int smoothed_count[20][20];
extern int cell_width;
extern int cell_height;

#endif
