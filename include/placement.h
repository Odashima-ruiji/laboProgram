/*
	placement.h : 配置に関する関数
    Dist_Init_p() : 乗客の初期配置
	Dist_Init_n() : ノードの初期配置
	Dist_add_p()　: 乗客の追加配置
*/

#ifndef _placement_h
#define _placement_h

/* メインの配置関数 */
void Dist_Init_p(); 			// 乗客の初期配置
void Dist_Init_n(); 			// ノードの初期配置
void Dist_add_p();  			// 乗客の追加配置

/* 配置関数のための補助関数 */
void Pass_Spawn();				// 乗客を存在させる
void position_p_get();			// 乗客の初期位置設定
void Set_p_Goal();				// 乗客の目的地設定
void pass_input_inter_init(); 	// 乗客情報を交差点に入力

