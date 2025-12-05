/* 
	base_func.c : プログラム簡略化のための基礎関数の実装
*/	

#include "globals.h"

/* ===== グローバル変数定義 ===== */
STrans Trans[Ax][Ay];
SNode Node[N_ALL_NUM];
SPass Pass[5000];

/* キュー関連 */
int queue_head[Ax][Ay];
int queue_num[Ax][Ay];

/* カウント関連 */
int p_add_count = 0;
int all10count[10000];
int all_count[10000];
int range_count[15];
int traffic_counter2[10000];
int transmit__[N_ALL_NUM][P_ALL_NUM];
int stack_push[5000];
int stack_pop[5000];
int push_num = 0;
int pop_num = 0;
int check = 0;
int check_1 = 0;
int check_2 = 0;
int check_3 = 0;
int check_4 = 0;
int goalstep[P_ALL_NUM];
int o = 0;
int Twait = 0;
int allstep = 0;
int count_ifgo = 0;
int count_getoff = 0;
int count_ride1 = 0;
int count_ride2 = 0;
int total_transmit = 0;
int count_ride3 = 0;
int ride_transmit = 0;
int count_p_no = 0;
int count_p_on_3 = 0;
int ridecount = 0;
int waitcount = 0;
int distancemass = 0;
int get_info = 0;
int traffic_counter = 0;

/* 浮動小数点数 */
double range[15];
double ave_all10count[500];
double speed_conter2[10000];
int sum_all10count[500];
double n = 0.0;
double speed_counter = 0.0;
double v_f = 80.0;
double d_j = 0.0;
double free_variable = 0.0;
double sum = 0.0;

/* 避難区域 */
double area_disaster = 0.0;
double area_evacuation = 0.0;
double center_x = 30.0;
double center_y = 30.0;
int count_same = 0;
int count_off = 0;

/* 目的地配列 */
int d[4][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}};

/* --------------------------------------------------------- *
 *	関数名 : sqrt2											 *
 *  機能 : 二乗和の平方根を返す								 *
 *  仮引数 : a_x : x座標（目的地-現在地）					 *
 * 		　 : a_y : y座標（目的地-現在地）					 *
 * --------------------------------------------------------- */
double sqrt2(double a_x, double a_y){
	return	sqrt( a_x * a_x + a_y * a_y );
}


/* --------------------------------------------------------- *
 *	関数名 : randAxAy										 *
 *  機能 : 0~Axの整数をランダムで返す						 *
 *  仮引数 : なし											 *
 * --------------------------------------------------------- */
double randAxAy(){
	double ran;
	//srand(1);
	ran = rand() % Ax;
	return ran;
}


/* --------------------------------------------------------- *
 *	関数名 : randAxAy_d										 *
 *  機能 : 0~Axの整数をランダムで返す						 *
 *  仮引数 : なし											 *
 * --------------------------------------------------------- */
double randAxAy_d(){
	double ran_d;
	//srand(1);
	ran_d = rand() % (Ax-2*Ax_d) + Ax_d;
	return ran_d;
}


/* --------------------------------------------------------- *
 *	関数名 : enqueue										 *
 *  機能 : 交差点配列の構造体に, 待っている乗客の番号を入力	 *
 *  仮引数 : p_number : 乗客番号を取得						 *
 *  メモ : 途中で乗客を降ろす場合改良が必要					 *
 * --------------------------------------------------------- */
void enqueue(int p_number ){ //	乗客番号の現在位置の交差点のキューに入力
	//printf("AAA\n");
	//printf("%f\n",(/*queue_head[Pass[p_number].p_X][Pass[p_number]] + */queue_num[Pass[p_number].p_X][Pass[p_number]]) % qsize);
	
	//利用客番号を保持
	Trans[Pass[p_number].p_X][Pass[p_number].p_Y].wp_number[(queue_head[Pass[p_number].p_X][Pass[p_number].p_Y] + queue_num[Pass[p_number].p_X][Pass[p_number].p_Y]) % qsize] = p_number;
	//交差点の待ち客数を保持
	queue_num[Pass[p_number].p_X][Pass[p_number].p_Y] += 1;
	//交差点に待ち客が存在するかの判定
	Trans[Pass[p_number].p_X][Pass[p_number].p_Y].wp_Exist = 1;
}


/* --------------------------------------------------------- *
 *	関数名 : dequeue										 *
 *  機能 : 仮引数で交差点の座標を受け取り, その構造体の		 *
 * 	　 　　キューから乗客番号を取り出す.					 *
 *  仮引数 : x : x座標										 *
 * 		　 : y : y座標										 *
 * --------------------------------------------------------- */
int dequeue(int x, int y){ 
	int answer = -1;
	//printf("AAA\n");

	if(queue_num[x][y] > 0){
		answer = Trans[x][y].wp_number[queue_head[x][y]];
		queue_head[x][y] += 1;
		queue_num[x][y] += -1;

		//printf("BBB\n");

		if(queue_num[x][y] == 0){ // 待ち行列が存在しない場合、交差点の待ち行列情報を変更

			Trans[x][y].wp_Exist = 0;
			//printf("CCC\n");
		}
	}
	return answer;
}

/* --------------------------------------------------------- *
 *	関数名 : peek										 *
 *  機能 : 仮引数で交差点の座標を受け取り, その構造体の		 *
 * 	　 　　キューの先頭の乗客番号を返す.					 *
 *  仮引数 : x : x座標										 *
 * 		　 : y : y座標										 *
 * --------------------------------------------------------- */
int peek(int i) {
    if(Node[i].stack_num > 0){
		return Node[i].stack_data[0];
    }else{
		printf("エラー: キューは空です。\n");
        return -1;
	}
    // frontインデックスにある値を返すだけ
    
}


/* --------------------------------------------------------- *
 *	関数名 : stack_sort										 *
 *  機能 : すべてのノードに格納されているデータスタックを	 *
 * 　　　　降順（大きい順）に並べ替える.					 *
 *  仮引数 : なし											 *
 * --------------------------------------------------------- */
void stack_sort(){
	int i,j,tmp;

	for(int k=0;k<N_ALL_NUM;k++){
		for (i=0; i<Node[k].stack_num; ++i) {
			for (j=i+1; j<Node[k].stack_num; ++j) {
				if (Node[k].stack_data[i] < Node[k].stack_data[j]) {
					tmp =  Node[k].stack_data[i];
					Node[k].stack_data[i] = Node[k].stack_data[j];
					Node[k].stack_data[j] = tmp;
				} 
			}
		}
	}
}


/* --------------------------------------------------------- *
 *	関数名 : push											 *
 *  機能 : ノードの情報スタックにデータをpushする関数		 *
 *  仮引数 : i : ノード番号									 *
 * 		　 : push_data : pushするデータ						 *
 * --------------------------------------------------------- */
void push(int i,int push_data){
	if(Node[i].stack_num < ssize){
		Node[i].stack_data[Node[i].stack_num] = push_data;
		stack_sort();
		Node[i].stack_num++;
		Node[i].all_stack_num++;
	}else{
		printf("スタックがいっぱいです\n");
	}
}

/* --------------------------------------------------------- *
 *	関数名 : push2											 *
 *  機能 : ノードの情報スタックにデータをpushする関数		 *
 *  仮引数 : i : ノード番号									 *
 * 		　 : push_data : pushするデータ						 *
 * --------------------------------------------------------- */
void push2(int i,int push_data_1, int push_data_2){
	if(Node[i].stack_num3< ssize){
		Node[i].stack_info[Node[i].stack_num3][0] = push_data_1;
		Node[i].stack_info[Node[i].stack_num3][1] = push_data_2;
		Node[i].stack_num3++;
	}else{
		printf("スタックがいっぱいです\n");
	}
}


/* --------------------------------------------------------- *
 *	関数名 : pushnopos										 *
 *  機能 : 乗客が存在していない位置をpushする関数			 *
 *  仮引数 : i : ノード番号									 *
 * 		　 : push_data2 : pushするデータ					 *
 * --------------------------------------------------------- */
void pushnopos(int i ,int push_data2){
	if (Node[i].stack_num2 < ssize) {
        Node[i].stack_nopos[Node[i].stack_num2] = push_data2;
        Node[i].stack_num2 ++;   
    }else{
		printf("スタックがいっぱいです\n");
	}
}


/* --------------------------------------------------------- *
 *	関数名 : search											 *
 *  機能 : 一度訪れた地点を記憶し, 再び行かないように避ける. *
 *  仮引数 : count : ノード番号								 *
 * 		　 : number : 										 *
 * --------------------------------------------------------- */
int search(int count,int number){
    for (int i = 0; i < Node[count].stack_num2+1; i++){
        /* 文字列を先頭から順に１つずつ比較 */
        if (Node[count].stack_nopos[i] == number) {
            /* 一致したら探索終了 */
            return 1;
        }
    }
    /* 見つからなかった場合 */
    return 0;
}

/* --------------------------------------------------------- *
 *	関数名 : search2											 *
 *  機能 : 一度訪れた地点を記憶し, 再び行かないように避ける. *
 *  仮引数 : count : ノード番号								 *
 * 		　 : number : 										 *
 * --------------------------------------------------------- */
int search2(int count,int number_1, int number_2){
    for (int i = 0; i < Node[count].stack_num3+1; i++){
        /* 文字列を先頭から順に１つずつ比較 */
        if (Node[count].stack_info[i][0] == number_1) {
			if(number_2 - Node[count].stack_info[i][1] < 60){
				/* 一致したら探索終了 */
				return 1;
			}
        }
    }
    /* 見つからなかった場合 */
    return 0;
}

/* --------------------------------------------------------- *
 *	関数名 : pop											 *
 *  機能 : ノードの情報スタックからデータをpopする.			 *
 *  仮引数 : i : ノード番号									 *
 * --------------------------------------------------------- */
int pop(int i){
	int answer;
	if(Node[i].stack_num > 0){
		Node[i].stack_num--;
		answer = Node[i].stack_data[0];
		Node[i].stack_data[0] = 0;
		if(i==0){
			stack_pop[pop_num] = answer;
			pop_num ++;
		}
		stack_sort();
		return answer;
	}else{
		return -1;
	}
}
