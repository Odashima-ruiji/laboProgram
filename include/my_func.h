/*
    my_func.h : カスタム関数の宣言
*/
#ifndef _my_func_h
#define _my_func_h

#include "config.h"
#include "base_struct.h"
#include "globals.h"
#include "base_func.h"
#include "Common_func.h"
#include "move.h"

/* 関数プロトタイプ宣言 */
void Dist_Init_n_D();          // ノードの目的地を初期化
void initial_move();           // シミュレーション開始時の移動
void initial_D_check();        // 初期移動時の目的地判定
void initial_detect_trans();   // 初期移動時の乗客検出

#endif
