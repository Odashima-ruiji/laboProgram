/*
    placement_80_20.h : 配置に関する関数をまとめたヘッダーファイル (宣言のみ)
    Dist_Init_p() : 乗客の初期配置
    Dist_Init_n() : ノードの初期配置
    Dist_add_p()  : 乗客の追加配置
*/
#ifndef _placement_80_20_h
#define _placement_80_20_h

#include "config.h"
#include "base_struct.h"
#include "globals.h"
#include "base_func.h"

/* メインの配置関数 */
void Dist_Init_p(void);
void Dist_Init_n(void);
void Dist_add_p(void);
void Init_W_map(void);

/* 配置関数のための補助関数 */
void Pass_Spawn(void);
void position_p_get(void);
void Set_p_Goal(void);
void pass_input_inter_init(void);

#endif