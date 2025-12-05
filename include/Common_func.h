
/*
	Common_func.h : 共通関数の宣言
*/

#ifndef _Common_func_h
#define _Common_func_h

/* 関数宣言 */
void allrideon(); 		// ノード(車)に乗客を乗せる
void D_check();			// 移動後、乗客の目的地判定を行う関数
int P_check();			// 乗客が残って存在しているかの判定する関数 
void flooding();		// フラッディングする関数
int transmit(int k);	// 情報伝播
void detect_trans();	// ノードのいる交差点に待ち行列があるのかを判定
int syokika();			// ノードの保持データを初期化する
int check_ride();		// 乗客の車待ち状況を調べる

#endif
