/* 
	base_func.h : プログラム簡略化のための基礎関数の宣言
*/	

#ifndef _base_func_h
#define _base_func_h

/* 関数宣言 */
double sqrt2(double a_x,double a_y); //二乗和の平方根を返す
double randAxAy(); 					//0~Axの整数をランダムで返す
double randAxAy_d();				//Ax_dからAx-Ax_dの範囲でランダムな値を返す
void enqueue(int p_number); 		//乗客番号をキューに入力
int dequeue(int x,int y); 			//乗客番号をキューから出力
int peek(int i);					//キューの先頭を参照する
void stack_sort(); 					//スタック中のデータを大きい順に並べる
void push(int i,int push_data); 	//スタックにpushする
void push2(int i,int push_data_1, int push_data_2); //スタックに2つのデータをpushする
void pushnopos(int i,int push_data2); //訪れていない位置をpushする
int search(int count,int number); 	//一度通った道を記憶し避ける関数
int search2(int count,int number_1, int number_2); //2つのデータで検索
int pop(int i); 					//スタックからpopする関数

#endif