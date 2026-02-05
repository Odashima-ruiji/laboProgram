/*
    config.h : シミュレーション設定のマクロ定義
*/
#ifndef _config_h
#define _config_h

#define N_ALL_NUM 100 // ノードの数
#define P_ALL_NUM 400 // 人間の数
#define P_goal 400    // 目標ゴール人数
#define TRUE 1        // trueかfalseかのマクロ定義
#define FALSE 0
#define qsize 5000 // 交差点のキューの大きさ
#define ssize 5000 // ノードの持つスタックの大きさ
#define Ax 60      // サービスエリアの横幅 [m]
#define Ay 60      // サービスエリアの縦幅 [m]
#define Ax_d 15
#define Ay_d 15
#define Td 100         // 交差点間距離 [m]
#define r 100          // 通信可能距離 [m]
#define po 100         // ifにしたがって動く台数
#define p_add_p 0.0000 // それぞれの地点で乗客が生成される確率
#define RANDOM_SEED 0  // 乱数シード

#define refdistance 250 // ノードが参照する距離(格子分)
#define reftime 30      // ノードが参照する時間差
//#define EPIDEMIC
#define circle
//#define two
#define dis_center
//#define N_search

#endif
