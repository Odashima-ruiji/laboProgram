/* 
    すべてのノードを動かす関数 Move
*/

#ifndef _move_h
#define _move_h

void Move(); 					// ノードを動かす関数
void move_update_coord();  		// 座標を更新する関数
void move_update_coord_surplus(); // 交差点余剰分の座標を更新する関数
void move_new_direction(); 		// 方向を決定する関数
void get_traffic_volume(); 		// 交通量を得る関数
void speed_error(int count); 	// スピードエラーを判定する関数

// グローバル変数
int last_direction[2][4];
int traffic_flag;
int count;
double d_now_length;
double d_now_length2;
double n_error; 				// 座標エラー判定用

#endif
