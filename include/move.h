/*
    move.h : ノード移動関数の宣言
*/
#ifndef _move_h
#define _move_h

#include "config.h"
#include "base_struct.h"
#include "globals.h"
#include "base_func.h"
#include "Common_func.h"

/* 関数プロトタイプ宣言 */
void Move();                      // ノードを動かす関数
void move_update_coord();         // 座標を更新する関数
void move_update_coord_surplus(); // 交差点余剰分の座標を更新する関数
void move_new_direction();        // 方向を決定する関数
void get_traffic_volume();        // 交通量を得る関数
void speed_error(int count);      // スピードエラーを判定する関数

#endif
