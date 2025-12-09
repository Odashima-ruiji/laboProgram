/*
	base_struct.h : 基本的な構造体の設定
	ノード（車）/ 乗客 / 交差点について定義する
	<定義>
	SNode : Node情報に関する構造体
	SPass : 乗客に関する構造体
	STrans : 交差点に関する構造体
*/
#ifndef _base_struct_h
#define _base_struct_h

#include "config.h"

// マップ
typedef struct
{
	int info;
	int p_num;
	int info_time; // マップの最終更新時間
	int no_D;	   // すでに目的地とした座標
	int m_X;
	int m_Y;
} SMap;

// ノード
typedef struct
{

	// 共通で使うメンバ変数
	double n_X, n_Y;		  // ノードの位置
	double prog_d_surp;		  // 余剰進行距離
	int n_insec_X, n_insec_Y; // ノードが向かう交差点
	double v;				  // ノードの速度
	int p_on;				  // 乗客の人数
	int p_num;				  // 乗客の番号
	int p_num2;				  // 乗客の番号
	int n_Already_Flooding;	  // 今の時間ステップにおいて、既にフラッディングしましたというフラグ
	// int     m_Exist;              // ノードそのものが存在しているか

	int n_xD, n_yD;			// ノードの宛先地点
	int n_xD2, n_yD2;		// ノードの宛先地点
	int stack_data[ssize];	// ノードが保持する客の待つ交差点情報
	int stack_nopos[ssize]; // 不必要なデータ保管スタック
	SMap Map[Ax][Ay];
	int stack_num;	   // スタック内のデータ数
	int stack_num2;	   // スタック内のデータ数2
	int all_stack_num; // すべての得た情報数
	int area_number;
	int move_flag;
	int stack_info[ssize][2];
	int stack_num3;
	double d_length;  // 乗客１の宛先地点までの距離　乗客１の目的地がノード自体の目的地となる
	double d_length2; // 乗客２の乗客２自体の宛先地点までの距離

	int move_pattern; // ランダム移動の際のパターン決定
} SNode;

// 乗客
typedef struct
{

	// 共通で使うメンバ変数
	int p_X, p_Y; // 乗客の位置
	int p_xS, p_yS;
	int p_Exist; // 乗客がエリアに存在しているか
	int p_ride;	 // 乗客が車に乗っているか

	int p_xD, p_yD; // 乗客の目的地
	int p_wait;

} SPass;

// 交差点
typedef struct
{
	// 共通で使うメンバ変数
	int wp_Exist;		  // 待ち乗客が存在しているか waitpass
	int wp_number[qsize]; // 待ち乗客の番号を保持したキュー

} STrans;

#endif