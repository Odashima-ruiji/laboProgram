#include "globals.h"

//メインの配置関数 -------------------------------------------------------------------------------------------------------------------------
/* --------------------------------------------------------- *
 *	関数名 : Dist_Init_p									 *
 *  機能 : 乗客の初期配置を行う. 							 *
 *  仮引数 : なし											 *
 * --------------------------------------------------------- */
void Dist_Init_p(){
	Pass_Spawn();     //乗客を存在させる
	position_p_get(); //乗客の初期位置設定
	Set_p_Goal();     //乗客の目的地設定
	pass_input_inter_init(); //乗客情報を交差点に入力
}


/* --------------------------------------------------------- *
 *	関数名 : Dist_Init_n									 *
 *  機能 : ノードの初期配置を行う. 							 *
 *  仮引数 : なし											 *
 * --------------------------------------------------------- */
void Dist_Init_n(){
	//すべてのノードの初期位置の設定
	for(int i=0; i<8; i++ ){
		do{
			Node[i].n_X   = randAxAy() ;
			Node[i].n_Y   = center_y -2*i;
		}while(((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X > center_x || Node[i].n_Y > center_y));
	}
	for(int i=8; i<10; i++ ){
		do{
			Node[i].n_X   = randAxAy() ;
			Node[i].n_Y   = randAxAy() ;
		}while(((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X > center_x || Node[i].n_Y > center_y));
	}
	for(int i=10; i<18; i++ ){
		do{
			Node[i].n_X   = randAxAy() ;
			Node[i].n_Y   = center_y + 2*(i-10) + 1;
		}while(((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X > center_x || Node[i].n_Y < center_y));
	}
	for(int i=18; i<20; i++ ){
		do{
			Node[i].n_X   = randAxAy() ;
			Node[i].n_Y   = randAxAy() ;
		}while(((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X > center_x || Node[i].n_Y < center_y));
	}
	for(int i=20; i<28; i++ ){
		do{
			Node[i].n_X   = randAxAy() ;
			Node[i].n_Y   = center_y + 2*(i-20) ;
		}while(((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X < center_x || Node[i].n_Y < center_y));
	}
	for(int i=28; i<30; i++ ){
		do{
			Node[i].n_X   = randAxAy() ;
			Node[i].n_Y   = randAxAy() ;
		}while(((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X < center_x || Node[i].n_Y < center_y));
	}
	for(int i=30; i<38; i++ ){
		do{
			Node[i].n_X   = randAxAy() ;
			Node[i].n_Y   = center_y -2*(i-30) - 1 ;
		}while(((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X < center_x || Node[i].n_Y > center_y));
	}
	for(int i=38; i<40; i++ ){
		do{
			Node[i].n_X   = randAxAy() ;
			Node[i].n_Y   = randAxAy() ;
		}while(((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X < center_x || Node[i].n_Y > center_y));
	}
	for(int i = 40; i<N_ALL_NUM; i++){

		// Node[i].n_X   = randAxAy_d() ;
	    // Node[i].n_Y   = randAxAy_d() ;
		do{
			Node[i].n_X   = randAxAy() ;
			Node[i].n_Y   = randAxAy() ;
			Node[i].move_flag = 10;
		}while((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2));
	}
	for(int i=0; i < N_ALL_NUM; i++){
		Node[i].n_insec_X = (int)Node[i].n_X;
		Node[i].n_insec_Y = (int)Node[i].n_Y;
		Node[i].p_num = -1;
		Node[i].p_num2 = -1;
		Node[i].n_xD = -1;
		Node[i].n_xD2 = -1;
		Node[i].n_yD = -1;
		Node[i].n_yD2 = -1;
		Node[i].p_on = 0;
		Node[i].v = v_f;

		int rand4 = rand() % 4;  // 0~3までの4つの整数をランダムに発生

		Node[i].move_pattern = rand4;
	}
		
}


//配置関数のための補助関数 ------------------------------------------------------------------------------------------------------------------
/* --------------------------------------------------------- *
 *	関数名 : Pass_Spawn										 *
 *  機能 : 乗客を存在させる.	 							 *
 *  仮引数 : なし											 *
 * --------------------------------------------------------- */
void Pass_Spawn(){
	//すべての客の生成
	for(int i=0; i<P_ALL_NUM; i++){
		Pass[i].p_Exist = 1;  //乗客が存在している
		Pass[i].p_ride  = 0; //乗客が車に乗っていない
	}
}


/* --------------------------------------------------------- *
 *	関数名 : position_p_get									 *
 *  機能 : 乗客の現在地の決定.	 							 *
 *  仮引数 : なし											 *
 * --------------------------------------------------------- */
void position_p_get(){
	//すべての客の現在地の生成(円)
	#ifdef circle
	for( int i=0; i<P_ALL_NUM; i++ ){
		do {
			//ランダムな座標を生成
			Pass[i].p_X   = randAxAy() ;
	    	Pass[i].p_Y   = randAxAy() ;
			Pass[i].p_xS = Pass[i].p_X;
			Pass[i].p_yS = Pass[i].p_Y;
		} while ((sqrt2(Pass[i].p_X - center_x, Pass[i].p_Y - center_y) > area_disaster) /*|| (Pass[i].p_X > center_x)/*&&(Pass[i].p_Y > center_y)*/);
		
	}
	#else
	//すべての客の現在地の生成(四角)
	for( int i=0; i<P_ALL_NUM; i++ ){
		Pass[i].p_X   = randAxAy_d() ;
		Pass[i].p_Y   = randAxAy_d() ;
	}
	#endif

}


/* --------------------------------------------------------- *
 *	関数名 : Set_p_Goal										 *
 *  機能 : 乗客の目的地の決定.	 							 *
 *  仮引数 : なし											 *
 * --------------------------------------------------------- */
void Set_p_Goal(){
	int i =0;
	int flag = 0;
	//すべての客の目的地の生成
	while(i < P_ALL_NUM){
        // int p_D  = rand() % 4;  // 0~3までの4つの整数をランダムに発生
        // if(p_D == 0){
        //     Pass[i].p_xD   =  1;  //避難場所の座標
	    //     Pass[i].p_yD   =  1;
        // }else if(p_D == 1){
        //     Pass[i].p_xD   =  1;
	    //     Pass[i].p_yD   =  49;
        // }else if(p_D == 2){
        //     Pass[i].p_xD   =  49;
	    //     Pass[i].p_yD   =  49;
        // }else if(p_D == 3){
        //     Pass[i].p_xD   =  49;
	    //     Pass[i].p_yD   =  1;
        // }



		//目的地をランダムに４つ配置
		int p_D = rand()%4;
		
		// if(flag == 0){
		// 	int j = 0;
		// 	while(j < 4){
		// 		do {
		// 				//ランダムな座標を生成
		// 			d[j][0]  = randAxAy() ;
		// 			d[j][1]  = randAxAy() ;
		// 		} while ((d[j][0] >= Ax_d && d[j][0] <= Ax - Ax_d) &&(d[j][1] >= Ay_d && d[j][1] <= Ay - Ay_d));
		// 		for (int k = 0; k < j; k++) {
		// 			if (d[k][0] == d[j][0] && d[k][1] == d[j][1]){
		// 				break;
		// 			}
		// 		}
		// 		j++;
		// 	}
		// 	flag += 1;
		// }

		
		
		
		// if(p_D == 0){
		// 	Pass[i].p_xD   =  d[0][0];  //避難場所の座標
	    //     Pass[i].p_yD   =  d[0][1];
		// }else if(p_D == 1){
        //     Pass[i].p_xD   =  d[1][0];
	    //     Pass[i].p_yD   =  d[1][1];
        // }else if(p_D == 2){
        //     Pass[i].p_xD   =  d[2][0];
	    //     Pass[i].p_yD   =  d[2][1];
        // }else if(p_D == 3){
        //     Pass[i].p_xD   =  d[3][0];
	    //     Pass[i].p_yD   =  d[3][1];
        // }


		// #ifdef circle
		// //目的地をランダムに４つ配置(円形の避難区域)
		// int p_D = rand()%4;
		
		// if(flag == 0){
		// 	while(1){
		// 			//ランダムな座標を生成
		// 		d[0][0]  = randAxAy() ;
		// 		d[0][1]  = randAxAy() ;
		// 		if((sqrt2(d[0][0] - center_x, d[0][1] - center_y) >= area_disaster + 10)&&(sqrt2(d[0][0] - center_x, d[0][1] - center_y) <= area_evacuation)){
		// 			if((d[0][0] > center_x)&&(d[0][1]>center_y)){
		// 				break;
		// 			}
		// 		}
		// 	} 
		// 	while(1){
		// 			//ランダムな座標を生成
		// 		d[1][0]  = randAxAy() ;
		// 		d[1][1]  = randAxAy() ;
		// 		if((sqrt2(d[1][0] - center_x, d[1][1] - center_y) >= area_disaster + 10)&&(sqrt2(d[1][0] - center_x, d[1][1] - center_y) <= area_evacuation)){
		// 			if((d[1][0] < center_x)&&(d[1][1]>center_y)){
		// 				break;
		// 			}
		// 		}
		// 	}
		// 	while(1){
		// 			//ランダムな座標を生成
		// 		d[2][0]  = randAxAy() ;
		// 		d[2][1]  = randAxAy() ;
		// 		if((sqrt2(d[2][0] - center_x, d[2][1] - center_y) >= area_disaster + 10)&&(sqrt2(d[2][0] - center_x, d[2][1] - center_y) <= area_evacuation)){
		// 			if((d[2][0] < center_x)&&(d[2][1]<center_y)){
		// 				break;
		// 			}
		// 		}
		// 	} 
		// 	while(1){
		// 			//ランダムな座標を生成
		// 		d[3][0]  = randAxAy() ;
		// 		d[3][1]  = randAxAy() ;
		// 		if((sqrt2(d[3][0] - center_x, d[3][1] - center_y) >= area_disaster + 10)&&(sqrt2(d[3][0] - center_x, d[3][1] - center_y) <= area_evacuation)){
		// 			if((d[3][0] > center_x)&&(d[3][1]<center_y)){
		// 				break;
		// 			}
		// 		}
		// 	}
		// 	flag += 1;
		// }
		// #else
		// //目的地をランダムに４つ配置
		// int p_D = rand()%4;
		
		// if(flag == 0){
		// 	int j = 0;
		// 	while(j < 4){
		// 		do {
		// 				//ランダムな座標を生成
		// 			d[j][0]  = randAxAy() ;
		// 			d[j][1]  = randAxAy() ;
		// 		} while ((d[j][0] >= Ax_d && d[j][0] <= Ax - Ax_d) &&(d[j][1] >= Ay_d && d[j][1] <= Ay - Ay_d));
		// 		for (int k = 0; k < j; k++) {
		// 			if (d[k][0] == d[j][0] && d[k][1] == d[j][1]){
		// 				break;
		// 			}
		// 		}
		// 		j++;
		// 	}
		// 	flag += 1;
		// }
			
	

		
		
		//ランダムに目的地を設定
		// if(p_D == 0){
		// 	Pass[i].p_xD   =  d[0][0];  //避難場所の座標
	    //     Pass[i].p_yD   =  d[0][1];
		// }else if(p_D == 1){
        //     Pass[i].p_xD   =  d[1][0];
	    //     Pass[i].p_yD   =  d[1][1];
        // }else if(p_D == 2){
        //     Pass[i].p_xD   =  d[2][0];
	    //     Pass[i].p_yD   =  d[2][1];
        // }else if(p_D == 3){
        //     Pass[i].p_xD   =  d[3][0];
	    //     Pass[i].p_yD   =  d[3][1];
        // }
        

		//1番近い避難場所を目的地に設定
		double min = sqrt2(d[0][0] - Pass[i].p_X, d[0][1] - Pass[i].p_Y);
		Pass[i].p_xD   =  d[0][0];  //避難場所の座標
	    Pass[i].p_yD   =  d[0][1];
		for(int k = 1; k < 4; k++){
			double tmp = sqrt2(d[k][0] - Pass[i].p_X, d[k][1] - Pass[i].p_Y);
			if(min > tmp){
				Pass[i].p_xD   =  d[k][0];  //避難場所の座標
	        	Pass[i].p_yD   =  d[k][1];
				min = tmp;
			}
		}


		/* 目的地を一箇所に設定 */
		/*Pass[i].p_xD   = 10 ;
	    Pass[i].p_yD   = 10 ;*/


		/*if(Pass[i].p_xD == Pass[i].p_X && Pass[i].p_yD == Pass[i].p_Y ){ //現在地と目的地が一致しないようにする
			continue ;
		}*/

		/*int a,b;
		a = Pass[i].p_xD-Pass[i].p_X;
		b = Pass[i].p_yD-Pass[i].p_Y;
		if(a<0){
			a = -1*a;
		}
		if(b<0){
			b = -1*b;
		}
		distancemass += (a+ b);*/

		i += 1;
	}	
}


/* --------------------------------------------------------- *
 *	関数名 : pass_input_inter_init							 *
 *  機能 : 交差点配列に待っている乗客情報を初期入力.		 *
 *  仮引数 : なし											 *
 * --------------------------------------------------------- */
void pass_input_inter_init(){
	for ( int i = 0; i < P_ALL_NUM; i++){
		enqueue(i);
	}
}
