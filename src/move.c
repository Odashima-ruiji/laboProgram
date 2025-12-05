#include "globals.h"

/* -------------------------------------------------------------------------------------------- *
 *	関数名 : Move																				*
 *  機能 : ノードを動かす関数																	*
 *  仮引数 : なし													 						    *
 * -------------------------------------------------------------------------------------------- */
void Move(){
	//printf("move!!!\n");


	//すべてのノードの座標を更新
	move_update_coord();
    D_check();  // 乗客の目的地判定


	//利用要求を受ける
    //#ifdef EPIDEMIC
	detect_trans(); 

    allrideon(); // 乗客を乗せる

	//交差点にいるとき, 次に向かう交差点を指定
	move_new_direction();
	//ノードが交差点上にいる状態でフラッディング
	#ifdef EPIDEMIC
	flooding();
	//余剰進行距離を座標に加算
	move_update_coord_surplus();
	//利用要求を受ける
	//#ifdef EPIDEMIC
    detect_trans();
	#endif 
	

	//printf("Node[0].n_xD = %d,Node[0].n_yD = %d\n",Node[0].n_xD,Node[0].n_yD);
	//printf("Node[0].n_insec_X = %d,Node[0].n_insec_Y = %d\n",Node[0].n_insec_X,Node[0].n_insec_Y);

	//すべての道路の交通量を調べる
	//get_traffic_volume();

	/*printf("Node[0].n_X = %f,Node[0].n_Y = %f\n",Node[0].n_X,Node[0].n_Y);
	printf("Node[0].v = %f\n",Node[0].v);
	printf("tra_count_y_plus[9][10] = %d\n",(int)tra_count_y_plus[9][10]);
    printf("speed_counter : %f\n",free_variable);*/
}



/* -------------------------------------------------------------------------------------------- *
 *	関数名 : move_update_coord																	*
 *  機能 : 座標を更新する関数																	*
 *  仮引数 : なし													 						    *
 * -------------------------------------------------------------------------------------------- */
void move_update_coord(){
	int count = 0;
	int free_counter = 0;
	double n_error = 0;
	while(count < N_ALL_NUM){
		Node[count].prog_d_surp = 0;

		// x軸方向に移動を行っているとき
		if(ceil(Node[count].n_X) != floor(Node[count].n_X) || Node[count].n_Y == Node[count].n_insec_Y){

			/*if(count == 0 && (double)Node[count].n_insec_X - Node[count].n_X > 0.000){
				printf("Node[0]がいる道路の交通量 : %f\n",tra_count_y_plus[(int)Node[count].n_X][(int)Node[count].n_Y]);
			}else if(count == 0 && (double)Node[count].n_insec_X - Node[count].n_X < 0.000){
				printf("Node[0]がいる道路の交通量 : %f\n",tra_count_y_minus[(int)Node[count].n_X][(int)Node[count].n_Y]);
			}*/
			/*----------------------------------*/
			//右方向の移動
			if((double)Node[count].n_insec_X - Node[count].n_X > 0.0000){
				//座標を更新
				n_error = Node[count].n_X;
				if(Node[count].p_on == 2){
					if(Pass[Node[count].p_num].p_xD != Pass[Node[count].p_num2].p_xD || Pass[Node[count].p_num].p_yD != Pass[Node[count].p_num2].p_yD){
						if(sqrt2(Node[count].n_X - center_x, Node[count].n_Y  - center_y) <= area_disaster){
							if(sqrt2((Node[count].n_X + ((Node[count].v/3600)*1000)/Td) - center_x, Node[count].n_Y - center_y) > area_disaster){

									Pass[Node[count].p_num2].p_X = Node[count].n_X;  //乗客２の現在地を更新
									Pass[Node[count].p_num2].p_Y = Node[count].n_Y;
									Pass[Node[count].p_num2].p_ride = 0; //乗客の乗車情報の変更

									enqueue(Node[count].p_num2); // 更新した現在位置のキューに乗客２の情報を入力

									push(count,Ax*Ax*Twait + Ax*Node[count].n_insec_X + Node[count].n_insec_Y);//ノードに待ち客情報をpush
									get_info += 1;

									//Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の乗客１と目的地との距離
									//Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X , Node[count].n_yD2 - Node[count].n_insec_Y ); //移動した後の乗客２と目的地との距離

									Node[count].p_num2 = -1;
									Node[count].n_xD2 = -1;
									Node[count].n_yD2 = -1;

									Node[count].p_on += -1;

									count_off += 1;
							
							}
						}
					}
				}
				
				Node[count].n_X += ((Node[count].v/3600)*1000)/Td;
				if(n_error == Node[count].n_X){
					printf("座標エラー : Node[%d]\n",count);
					exit(1);
				}

				//目的の座標を超えるときは交差点座標を参照
				if((double)Node[count].n_insec_X - Node[count].n_X < 0.0000){
					Node[count].prog_d_surp = fabs((double)Node[count].n_insec_X - Node[count].n_X);
					Node[count].n_X = (double)Node[count].n_insec_X;
				}		
			//左方向の移動
			}else{
				//座標を更新
				n_error = Node[count].n_X;
				if(Node[count].p_on == 2){
					if(Pass[Node[count].p_num].p_xD != Pass[Node[count].p_num2].p_xD || Pass[Node[count].p_num].p_yD != Pass[Node[count].p_num2].p_yD){
						if(sqrt2(Node[count].n_X - center_x, Node[count].n_Y  - center_y) <= area_disaster){
							if(sqrt2((Node[count].n_X - ((Node[count].v/3600)*1000)/Td)- center_x, Node[count].n_Y - center_y) > area_disaster){

									Pass[Node[count].p_num2].p_X = Node[count].n_X;  //乗客２の現在地を更新
									Pass[Node[count].p_num2].p_Y = Node[count].n_Y;
									Pass[Node[count].p_num2].p_ride = 0; //乗客の乗車情報の変更

									enqueue(Node[count].p_num2); // 更新した現在位置のキューに乗客２の情報を入力

									push(count,Ax*Ax*Twait + Ax*Node[count].n_insec_X + Node[count].n_insec_Y);//ノードに待ち客情報をpush
									get_info += 1;

									//Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の乗客１と目的地との距離
									//Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X , Node[count].n_yD2 - Node[count].n_insec_Y ); //移動した後の乗客２と目的地との距離

									Node[count].p_num2 = -1;
									Node[count].n_xD2 = -1;
									Node[count].n_yD2 = -1;

									Node[count].p_on += -1;

									count_off += 1;
							
							}
						}
					}
				}
				Node[count].n_X -= ((Node[count].v/3600)*1000)/Td;
				if(n_error == Node[count].n_X){
					printf("座標エラー : Node[%d]\n",count);
					exit(1);
				}

				//目的の座標を超えるときは交差点座標を参照
				if((double)Node[count].n_insec_X - Node[count].n_X > 0.0000){
					Node[count].prog_d_surp = fabs((double)Node[count].n_insec_X - Node[count].n_X);
					Node[count].n_X = (double)Node[count].n_insec_X;
				}
			}

		// y軸方向に移動を行っているとき
		}else if(ceil(Node[count].n_Y) != floor(Node[count].n_Y) || Node[count].n_X == Node[count].n_insec_X){
			/*if(count == 0 && (double)Node[count].n_insec_Y - Node[count].n_Y > 0.0000){
				printf("Node[0]がいる道路の交通量 : %f\n",tra_count_x_plus[(int)Node[count].n_X][(int)Node[count].n_Y]);
			}else if(count == 0 && (double)Node[count].n_insec_Y - Node[count].n_Y < 0.0000){
				printf("Node[0]がいる道路の交通量 : %f\n",tra_count_x_minus[(int)Node[count].n_X][(int)Node[count].n_Y]);
			}*/
			/*----------------------------------*/
			//上方向の移動
			if((double)Node[count].n_insec_Y - Node[count].n_Y > 0.0000){
				n_error = Node[count].n_Y;
				if(Node[count].p_on == 2){
					if(Pass[Node[count].p_num].p_xD != Pass[Node[count].p_num2].p_xD || Pass[Node[count].p_num].p_yD != Pass[Node[count].p_num2].p_yD){
						if(sqrt2(Node[count].n_X - center_x, Node[count].n_Y  - center_y) <= area_disaster){
							if(sqrt2(Node[count].n_X - center_x, (Node[count].n_Y + ((Node[count].v/3600)*1000)/Td) - center_y) > area_disaster){

									Pass[Node[count].p_num2].p_X = Node[count].n_X;  //乗客２の現在地を更新
									Pass[Node[count].p_num2].p_Y = Node[count].n_Y;
									Pass[Node[count].p_num2].p_ride = 0; //乗客の乗車情報の変更

									enqueue(Node[count].p_num2); // 更新した現在位置のキューに乗客２の情報を入力

									push(count,Ax*Ax*Twait + Ax*Node[count].n_insec_X + Node[count].n_insec_Y);//ノードに待ち客情報をpush
									get_info += 1;

									//Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の乗客１と目的地との距離
									//Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X , Node[count].n_yD2 - Node[count].n_insec_Y ); //移動した後の乗客２と目的地との距離

									Node[count].p_num2 = -1;
									Node[count].n_xD2 = -1;
									Node[count].n_yD2 = -1;

									Node[count].p_on += -1;

									count_off += 1;
							
							}
						}
					}
				}
				Node[count].n_Y += ((Node[count].v/3600)*1000)/Td;
				if(n_error == Node[count].n_Y){
					printf("座標エラー : Node[%d]\n",count);
					exit(1);
				}

				//目的の座標を超えるときは交差点座標を参照
				if((double)Node[count].n_insec_Y - Node[count].n_Y < 0.0000){
					Node[count].prog_d_surp = fabs((double)Node[count].n_insec_Y - Node[count].n_Y);
					Node[count].n_Y = (double)Node[count].n_insec_Y;
				}
			//下方向の移動
			}else{
				//座標を更新
				n_error = Node[count].n_Y;
				if(Node[count].p_on == 2){
					if(Pass[Node[count].p_num].p_xD != Pass[Node[count].p_num2].p_xD || Pass[Node[count].p_num].p_yD != Pass[Node[count].p_num2].p_yD){
						if(sqrt2(Node[count].n_X - center_x, Node[count].n_Y  - center_y) <= area_disaster){
							if(sqrt2(Node[count].n_X - center_x, (Node[count].n_Y - ((Node[count].v/3600)*1000)/Td) - center_y) > area_disaster){

									Pass[Node[count].p_num2].p_X = Node[count].n_X;  //乗客２の現在地を更新
									Pass[Node[count].p_num2].p_Y = Node[count].n_Y;
									Pass[Node[count].p_num2].p_ride = 0; //乗客の乗車情報の変更

									enqueue(Node[count].p_num2); // 更新した現在位置のキューに乗客２の情報を入力

									push(count,Ax*Ax*Twait + Ax*Node[count].n_insec_X + Node[count].n_insec_Y);//ノードに待ち客情報をpush
									get_info += 1;

									//Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の乗客１と目的地との距離
									//Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X , Node[count].n_yD2 - Node[count].n_insec_Y ); //移動した後の乗客２と目的地との距離

									Node[count].p_num2 = -1;
									Node[count].n_xD2 = -1;
									Node[count].n_yD2 = -1;

									Node[count].p_on += -1;

									count_off += 1;
							
							}
						}
					}
				}
				Node[count].n_Y -= ((Node[count].v/3600)*1000)/Td;
				if(n_error == Node[count].n_Y){
					printf("座標エラー : Node[%d]\n",count);
					exit(1);
				}

				//目的の座標を超えるときは交差点座標を参照
				if((double)Node[count].n_insec_Y - Node[count].n_Y > 0.0000){
					Node[count].prog_d_surp = fabs((double)Node[count].n_insec_Y - Node[count].n_Y);
					Node[count].n_Y = (double)Node[count].n_insec_Y;
				}
			}
		}else{
			printf("座標更新エラー : Node[%d]\n",count);
			exit(1);
		}
		count += 1;
	}
}

/* -------------------------------------------------------------------------------------------- *
 *	関数名 : move_update_coord_surplus																	*
 *  機能 : 交差点余剰分の座標を更新する関数														*
 *  仮引数 : なし													 						    *
 * -------------------------------------------------------------------------------------------- */
void move_update_coord_surplus(){
	for(int count=0; count<N_ALL_NUM; count++){
		if(Node[count].prog_d_surp != 0){
			if((double)Node[count].n_insec_X - Node[count].n_X > 0.0000){
				//座標を更新
				Node[count].n_X += Node[count].prog_d_surp;
			}else if((double)Node[count].n_insec_X - Node[count].n_X < 0.0000){
				//座標を更新
				Node[count].n_X -= Node[count].prog_d_surp;
			}else if((double)Node[count].n_insec_Y - Node[count].n_Y > 0.0000){
				//座標を更新
				Node[count].n_Y += Node[count].prog_d_surp;
			}else if((double)Node[count].n_insec_Y - Node[count].n_Y < 0.0000){
				//座標を更新
				Node[count].n_Y -= Node[count].prog_d_surp;
			}
		}
	}
}


/* -------------------------------------------------------------------------------------------- *
 *	関数名 : move_new_direction																	*
 *  機能 : 交差点にいるとき, 次に向かう交差点を指定する関数										*
 *  仮引数 : なし													 						    *
 * -------------------------------------------------------------------------------------------- */
void move_new_direction(){
	int count = 0;
	double d_now_length = 0;
	double d_now_length2 = 0;
	int direction = 0;
	int last_direction[2][4];

	// //allrideon(); // 乗客を乗せる

	// /*while(count < N_ALL_NUM ){

	// 	if(count == 1){
	// 		printf("(Node[1].n_Y=%f) == ((double)Node[1].n_insec_Y=%f)\n",Node[1].n_Y,(double)Node[1].n_insec_Y);
	// 	}

	// 	//交差点にいるとき, 次に向かう交差点を指定
	// 	if((Node[count].n_X == (double)Node[count].n_insec_X) && (Node[count].n_Y == (double)Node[count].n_insec_Y)){

	// 		allrideon(); // 乗客を乗せる
	// 	}
	// }
	// count = 0;*/

	while(count < N_ALL_NUM ){

		/*if(count == 1){
			printf("(Node[1].n_Y=%f) == ((double)Node[1].n_insec_Y=%f)\n",Node[1].n_Y,(double)Node[1].n_insec_Y);
		}*/

		//交差点にいるとき, 次に向かう交差点を指定
		if((Node[count].n_X == (double)Node[count].n_insec_X) && (Node[count].n_Y == (double)Node[count].n_insec_Y)){
			d_now_length = sqrt2(center_x - Node[count].n_insec_X , center_y - Node[count].n_insec_Y );

			//int pattern = rand() % 100000; //変更！！！！！！！！！！！！！！！！

			//allrideon(); // 乗客を乗せる

			int direction = rand() % 4;  // 0~3までの4つの整数をランダムに発生
			int direction2 = rand() % 2;  // 0~1までの2つの整数をランダムに発生

			/*if(count < N_HUB && (Node[count].p_on != 1 && Node[count].p_on != 2 && Node[count].p_on != 10)){
				count += 1;
				continue;
			}*/

			if(Node[count].p_on == 0){ // ノードが乗客を乗せておらず、目的地設定がない場合　ランダム移動
			
					#ifdef circle
					if (sqrt2(Node[count].n_X - center_x, Node[count].n_Y - center_y) <= area_disaster){
					#else
					if (Node[count].n_X >= Ax_d && Node[count].n_X <= Ax-Ax_d && Node[count].n_Y >= Ay_d && Node[count].n_Y <= Ay-Ay_d){
					#endif
						if(Node[count].move_pattern ==0 ){ // パターン0  右0上1						if(direction2 == 0){ //右

							Node[count].n_insec_X += 1;
							Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X , center_y - Node[count].n_insec_Y );
							
							//if(pattern >= 1){
								#ifdef circle
								if(sqrt2(Node[count].n_insec_X - center_x, Node[count].n_Y - center_y) > area_disaster)// エリアから右に出た場合while文の先頭に戻る   //変更！！！！！！！
								#else
								if(Node[count].n_insec_X > Ax-Ax_d-1 )
								#endif
								{
									Node[count].n_insec_X += -1; //元に戻す
									direction2 = rand() % 2;
									if(direction2==0){
										Node[count].move_pattern = 2;
									}else{
										Node[count].move_pattern = 3;
									}
									continue;
								}
							// }else if(pattern == 0 && d_now_length >= 5){
							// 	#ifdef circle
							// 	if(sqrt2(Node[count].n_insec_X - center_x, Node[count].n_Y - center_y) > area_disaster || d_now_length < Node[count].d_length)// エリアから右に出た場合while文の先頭に戻る   //変更！！！！！！！
							// 	#else
							// 	if(Node[count].n_insec_X > Ax-Ax_d-1 )
							// 	{
							// 		Node[count].n_insec_X += -1; //元に戻す
							// 		direction2 = rand() % 2;
							// 		if(direction2==0){
							// 			Node[count].move_pattern = 2;
							// 		}else{
							// 			Node[count].move_pattern = 3;
							// 		}
							// 		continue;
							// 	}
							// }
							
						}else if(direction2 == 1){ //上

							Node[count].n_insec_Y += -1;
							Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X , center_y - Node[count].n_insec_Y );
							
							//if(pattern >= 1){
								#ifdef circle
								if(sqrt2(Node[count].n_X - center_x, Node[count].n_insec_Y - center_y) > area_disaster) // エリアから上に出た場合while文の先頭に戻る
								#else
								if(Node[count].n_insec_Y < Ay_d )
								#endif
								{
									
									Node[count].n_insec_Y += 1; //元に戻す
									direction2 = rand() % 2;
									if(direction2==0){
										Node[count].move_pattern = 1;
									}else{
										Node[count].move_pattern = 3;
									}
									continue;
								}
							// }else if(pattern == 0 && d_now_length >= 5){
							// 	#ifdef circle
							// 	if(sqrt2(Node[count].n_X - center_x, Node[count].n_insec_Y - center_y) > area_disaster || d_now_length < Node[count].d_length) // エリアから上に出た場合while文の先頭に戻る
							// 	#else
							// 	if(Node[count].n_insec_Y < Ay_d )
							// 	{
									
							// 		Node[count].n_insec_Y += 1; //元に戻す
							// 		direction2 = rand() % 2;
							// 		if(direction2==0){
							// 			Node[count].move_pattern = 1;
							// 		}else{
							// 			Node[count].move_pattern = 3;
							// 		}
							// 		continue;
							// 	}
							// }
							
						}


					}else if(Node[count].move_pattern == 1 ){ // パターン1  右0下1


						if(direction2 == 1){ //右

							Node[count].n_insec_X += 1;
							Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X , center_y - Node[count].n_insec_Y );
						
						//if(pattern >= 1){
							#ifdef circle
							if(sqrt2(Node[count].n_insec_X - center_x, Node[count].n_Y - center_y) > area_disaster)// エリアから右に出た場合while文の先頭に戻る   //変更！！！！！！！
							#else
							if(Node[count].n_insec_X > Ax-Ax_d-1 )
							#endif
							{									Node[count].n_insec_X += -1; //元に戻す
									direction2 = rand() % 2;
									if(direction2==0){
										Node[count].move_pattern = 2;
									}else{
										Node[count].move_pattern = 3;
									}
									continue;
								}
							// }else if(pattern == 0 && d_now_length >= 5){
							// 	#ifdef circle
							// 	if(sqrt2(Node[count].n_insec_X - center_x, Node[count].n_Y - center_y) > area_disaster || d_now_length < Node[count].d_length)// エリアから右に出た場合while文の先頭に戻る   //変更！！！！！！！
							// 	#else
							// 	if(Node[count].n_insec_X > Ax-Ax_d-1 )
							// 	{
									
							// 		Node[count].n_insec_X += -1; //元に戻す
							// 		direction2 = rand() % 2;
							// 		if(direction2==0){
							// 			Node[count].move_pattern = 2;
							// 		}else{
							// 			Node[count].move_pattern = 3;
							// 		}
							// 		continue;
							// 	}
							// }
							

						}else if(direction2 == 1){ //下

							Node[count].n_insec_Y += 1;
							Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X , center_y - Node[count].n_insec_Y );
						
							//if(pattern >= 1){
								#ifdef circle
								if(sqrt2(Node[count].n_X - center_x, Node[count].n_insec_Y - center_y) > area_disaster) // エリアから上に出た場合while文の先頭に戻る
								#else
								if(Node[count].n_insec_Y > Ay-Ay_d-1 )
						#endif
						{
									
									Node[count].n_insec_Y += -1; //元に戻す
									direction2 = rand() % 2;
									if(direction2==0){
										Node[count].move_pattern = 0;
									}else{
										Node[count].move_pattern = 2;
									}
									continue;
								}
							// }else if(pattern == 0 && d_now_length >= 5){
							// 	#ifdef circle
							// 	if(sqrt2(Node[count].n_X - center_x, Node[count].n_insec_Y - center_y) > area_disaster || d_now_length < Node[count].d_length) // エリアから上に出た場合while文の先頭に戻る
							// 	#else
							// 	if(Node[count].n_insec_Y > Ay-Ay_d-1 )
							// 	{
									
							// 		Node[count].n_insec_Y += -1; //元に戻す
							// 		direction2 = rand() % 2;
							// 		if(direction2==0){
							// 			Node[count].move_pattern = 0;
							// 		}else{
							// 			Node[count].move_pattern = 2;
							// 		}
							// 		continue;
							// 	}
							// }
							

						}

					}else if(Node[count].move_pattern == 2 ){ // パターン2  左0上1

						if(direction2 == 0){ //左

							Node[count].n_insec_X += -1;
							Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X , center_y - Node[count].n_insec_Y );
						
							//if(pattern >= 1){
								#ifdef circle
								if(sqrt2(Node[count].n_insec_X - center_x, Node[count].n_Y - center_y) > area_disaster)// エリアから右に出た場合while文の先頭に戻る   //変更！！！！！！！
								#else
								if(Node[count].n_insec_X < Ax_d )
						#endif
						{
									
									Node[count].n_insec_X += 1; //元に戻す
									direction2 = rand() % 2;
									if(direction2==0){
										Node[count].move_pattern = 0;
									}else{
										Node[count].move_pattern = 1;
									}
									continue;
								}
							// }else if(pattern == 0 && d_now_length >= 5){
							// 	#ifdef circle
							// 	if(sqrt2(Node[count].n_insec_X - center_x, Node[count].n_Y - center_y) > area_disaster || d_now_length < Node[count].d_length)// エリアから右に出た場合while文の先頭に戻る   //変更！！！！！！！
							// 	#else
							// 	if(Node[count].n_insec_X < Ax_d )
							// 	{
									
							// 		Node[count].n_insec_X += 1; //元に戻す
							// 		direction2 = rand() % 2;
							// 		if(direction2==0){
							// 			Node[count].move_pattern = 0;
							// 		}else{
							// 			Node[count].move_pattern = 1;
							// 		}
							// 		continue;
							// 	}
							// }
							
						}else if(direction2== 1){ //上

							Node[count].n_insec_Y += -1;
							Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X , center_y - Node[count].n_insec_Y );
						
														//if(pattern >= 1){
															#ifdef circle
															if(sqrt2(Node[count].n_X - center_x, Node[count].n_insec_Y - center_y) > area_disaster) // エリアから上に出た場合while文の先頭に戻る
															#else
															if(Node[count].n_insec_Y < Ay_d ) 
															#endif
															{									Node[count].n_insec_Y += 1; //元に戻す
									direction2 = rand() % 2;
									if(direction2==0){
										Node[count].move_pattern = 1;
									}else{
										Node[count].move_pattern = 3;
									}
									continue;
								}
							// }else if(pattern == 0 && d_now_length >= 5){
							// 	#ifdef circle
							// 	if(sqrt2(Node[count].n_X - center_x, Node[count].n_insec_Y - center_y) > area_disaster || d_now_length < Node[count].d_length) // エリアから上に出た場合while文の先頭に戻る
							// 	#else
							// 	if(Node[count].n_insec_Y < Ay_d ) 
							// 	{
									
							// 		Node[count].n_insec_Y += 1; //元に戻す
							// 		direction2 = rand() % 2;
							// 		if(direction2==0){
							// 			Node[count].move_pattern = 1;
							// 		}else{
							// 			Node[count].move_pattern = 3;
							// 		}
							// 		continue;
							// 	}
							// }
							
						}

					}else if(Node[count].move_pattern == 3 ){ // パターン3  左0下1

						if(direction2 == 0){ //左

							Node[count].n_insec_X += -1;
							Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X , center_y - Node[count].n_insec_Y );

							//if(pattern >= 1){
								#ifdef circle
								if(sqrt2(Node[count].n_insec_X - center_x, Node[count].n_Y - center_y) > area_disaster)// エリアから右に出た場合while文の先頭に戻る   //変更！！！！！！！
								#else
								if(Node[count].n_insec_X < Ax_d )
								#endif
								{
									
									Node[count].n_insec_X += 1; //元に戻す
									direction2 = rand() % 2;
									if(direction2==0){
										Node[count].move_pattern = 0;
									}else{
										Node[count].move_pattern = 1;
									}
									continue;
								}
							// }else if(pattern == 0 && d_now_length >= 5){
							// 	#ifdef circle
							// 	if(sqrt2(Node[count].n_insec_X - center_x, Node[count].n_Y - center_y) > area_disaster || d_now_length < Node[count].d_length)// エリアから右に出た場合while文の先頭に戻る   //変更！！！！！！！
							// 	#else
							// 	if(Node[count].n_insec_X < Ax_d )
							// 	{
									
							// 		Node[count].n_insec_X += 1; //元に戻す
							// 		direction2 = rand() % 2;
							// 		if(direction2==0){
							// 			Node[count].move_pattern = 0;
							// 		}else{
							// 			Node[count].move_pattern = 1;
							// 		}
							// 		continue;
							// 	}
							// }

						}else if(direction2 == 1){ //下

							Node[count].n_insec_Y += 1;
							Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X , center_y - Node[count].n_insec_Y );
						
							//if(pattern >= 1){
								#ifdef circle
								if(sqrt2(Node[count].n_X - center_x, Node[count].n_insec_Y - center_y) > area_disaster) // エリアから上に出た場合while文の先頭に戻る
								#else
								if(Node[count].n_insec_Y > Ay-Ay_d-1 )
						#endif
						{
									
									Node[count].n_insec_Y += -1; //元に戻す
									direction2 = rand() % 2;
									if(direction2==0){
										Node[count].move_pattern = 0;
									}else{
										Node[count].move_pattern = 2;
									}
									continue;
								}
							// }else if(pattern == 0 && d_now_length >= 5){
							// 	#ifdef circle
							// 	if(sqrt2(Node[count].n_X - center_x, Node[count].n_insec_Y - center_y) > area_disaster || d_now_length < Node[count].d_length) // エリアから上に出た場合while文の先頭に戻る
							// 	#else
							// 	if(Node[count].n_insec_Y > Ay-Ay_d-1 )
							// 	{
									
							// 		Node[count].n_insec_Y += -1; //元に戻す
							// 		direction2 = rand() % 2;
							// 		if(direction2==0){
							// 			Node[count].move_pattern = 0;
							// 		}else{
							// 			Node[count].move_pattern = 2;
							// 		}
							// 		continue;
							// 	}
							// }
							
						}
						
					}

					count += 1 ; // 調べるノード番号を増加
				}else{     //中心へ向かって移動
					Node[count].d_length = sqrt2(center_x - Node[count].n_X , center_y - Node[count].n_Y); 
					d_now_length = Node[count].d_length;
					if( direction == 0 && d_now_length != 0){
					
						Node[count].n_insec_Y += -1;
						Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X , center_y - Node[count].n_insec_Y ); //移動した後の目的地との距離
						
						

						if(Node[count].d_length - d_now_length > 0.0 ) // 目的地との距離が長くなったらwhile文の先頭に戻る
						{
							
							Node[count].n_insec_Y += 1; //元に戻す
							Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X , center_y - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す
							
							continue;
						}

						if(Node[count].n_insec_Y < 0 ) // エリアから南に出た場合while文の先頭に戻る
						{
							
							Node[count].n_insec_Y += 1; //元に戻す
							Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X , center_y - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す
							continue;
						}
					}

						
						
					else if( direction == 1 && d_now_length != 0 ){
						
						Node[count].n_insec_Y += 1;
						Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X , center_y - Node[count].n_insec_Y ); //移動した後の目的地との距離
						
						

						if(Node[count].d_length -  d_now_length > 0.0 ) // 目的地との距離が長くなったらwhile文の先頭に戻る
						{
							Node[count].n_insec_Y += -1; //元に戻す
							Node[count].d_length = sqrt2(center_x- Node[count].n_insec_X , center_y - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す

							continue;
						}

						if(Node[count].n_insec_Y > Ay-1 ) // エリアから北に出た場合while文の先頭に戻る
						{
							
							Node[count].n_insec_Y += -1; //元に戻す
							Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X , center_y - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す
							
							continue;
						}

						
						
					}
					
					else if( direction == 2 && d_now_length != 0){
						
						Node[count].n_insec_X += -1;
						
						Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X , center_y - Node[count].n_insec_Y ); //移動した後の目的地との距離
						
						if(Node[count].d_length - d_now_length > 0.0 ) // 目的地との距離が長くなったらwhile文の先頭に戻る
						{
							
							Node[count].n_insec_X += 1; //元に戻す
							Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X , center_y - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す
							

							continue;
						}
						
						if(Node[count].n_insec_X < 0 ) // エリアから西に出た場合while文の先頭に戻る
						{
							Node[count].n_insec_X += 1; //元に戻す
							Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X , center_y - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す
					
							continue;
						}

						

					}
					
					else if( direction == 3 && d_now_length != 0){
						
						Node[count].n_insec_X += 1;

						Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X , center_y - Node[count].n_insec_Y ); //移動した後の目的地との距離

						
						if(Node[count].d_length - d_now_length > 0.0 ) // 目的地との距離が長くなったらwhile文の先頭に戻る
						{
							
							Node[count].n_insec_X += -1; //元に戻す
							Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X , center_y - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す

							continue;
						}

						if(Node[count].n_insec_X > Ax-1 ) // エリアから東に出た場合while文の先頭に戻る
						{
							
							Node[count].n_insec_X += -1; //元に戻す
							Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X , center_y - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す

							continue;
						}

					}


					count += 1 ; // 調べるノード番号を増加

				}
			}else if(Node[count].p_on == 1 || Node[count].p_on == 3 || Node[count].p_on == 5){   // ノードが乗客を乗せており、目的地設定がある場合　目的地へ移動  またifより情報を得ている場合は目的地へ移動
			
				/*if(Node[count].p_on == 3 && Node[count].n_X == Node[count].n_xD && Node[count].n_Y == Node[count].n_yD){// すでに目的地に到着している場合
					Node[count].p_on = 0;
					continue;
				}*/

				d_now_length = Node[count].d_length; // 移動する前の目的地との距離 

				//printf("direction %d\n",direction);

				/*
				
				乗客を乗せている場合、directionの値によって目的地に近づくようにランダムに移動する
				0 ... 北    1... 南
				2 ... 西    3... 東
				
				*/
				
				//サービスエリアから出たらもう一回経路決定する +  目的地への距離が遠くなったらもう一回経路決定する
				if( direction == 0 && d_now_length != 0){
					
					Node[count].n_insec_Y += -1;
					Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の目的地との距離
					
					

					if(Node[count].d_length - d_now_length > 0.0 ) // 目的地との距離が長くなったらwhile文の先頭に戻る
					{
						
						Node[count].n_insec_Y += 1; //元に戻す
						Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す
						
						continue;
					}

					if(Node[count].n_insec_Y < 0 ) // エリアから南に出た場合while文の先頭に戻る
					{
						
						Node[count].n_insec_Y += 1; //元に戻す
						Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す
						continue;
					}
				}

					
					
				else if( direction == 1 && d_now_length != 0 ){
					
					Node[count].n_insec_Y += 1;
					Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の目的地との距離
					
					

					if(Node[count].d_length -  d_now_length > 0.0 ) // 目的地との距離が長くなったらwhile文の先頭に戻る
					{
						Node[count].n_insec_Y += -1; //元に戻す
						Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す

						continue;
					}

					if(Node[count].n_insec_Y > Ay-1 ) // エリアから北に出た場合while文の先頭に戻る
					{
						
						Node[count].n_insec_Y += -1; //元に戻す
						Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す
						
						continue;
					}

					
					
				}
				
				else if( direction == 2 && d_now_length != 0){
					
					Node[count].n_insec_X += -1;
					
					Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の目的地との距離
					
					if(Node[count].d_length - d_now_length > 0.0 ) // 目的地との距離が長くなったらwhile文の先頭に戻る
					{
						
						Node[count].n_insec_X += 1; //元に戻す
						Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す
						

						continue;
					}
					
					if(Node[count].n_insec_X < 0 ) // エリアから西に出た場合while文の先頭に戻る
					{
						Node[count].n_insec_X += 1; //元に戻す
						Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す
				
						continue;
					}

					

				}
				
				else if( direction == 3 && d_now_length != 0){
					
					Node[count].n_insec_X += 1;

					Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の目的地との距離

					
					if(Node[count].d_length - d_now_length > 0.0 ) // 目的地との距離が長くなったらwhile文の先頭に戻る
					{
						
						Node[count].n_insec_X += -1; //元に戻す
						Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す

						continue;
					}

					if(Node[count].n_insec_X > Ax-1 ) // エリアから東に出た場合while文の先頭に戻る
					{
						
						Node[count].n_insec_X += -1; //元に戻す
						Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す

						continue;
					}

				}


				count += 1 ; // 調べるノード番号を増加
			}else if(Node[count].p_on == 2){ // ノードが乗客を二人乗せており、乗客１の目的地に向かって移動、なお乗客２の目的地から離れる場合、乗客２を下車させる
			
				// if(Node[count].p_on == 2 && Node[count].n_X == Node[count].n_xD && Node[count].n_Y == Node[count].n_yD){// すでに目的地に到着している場合
				// 	Node[count].p_on = 0;
				// 	continue;
				// }

				for(int i =0;i<2;i++){
					for(int k = 0 ;k<4;k++){
						last_direction[i][k] = 0;
					}
				}

				/*
				
				乗客二人がもっとも目的地に近づく経路を選択する　しかし経路は完全にランダムではない(要改善)
				0 ... 北    1... 南
				2 ... 西    3... 東
				
				*/
				/*乗客1,2ともに目的地に近づくかを判定 --------------------------------------------------------------------------*/
				for(direction = 0;direction < 4 ;direction++){

					d_now_length = Node[count].d_length; // 移動する前の乗客１と目的地との距離
					d_now_length2 = Node[count].d_length2; // 移動する前の乗客2と目的地との距離

					if( direction == 0 ){
						Node[count].n_insec_Y += 1;
				
						
						Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の乗客１と目的地との距離
						Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X , Node[count].n_yD2 - Node[count].n_insec_Y ); //移動した後の乗客２と目的地との距離
						
						if(d_now_length - Node[count].d_length > 0 ) // 目的地との距離が短くなったら
						{
							last_direction[0][0] = 1;
						}
						
						if(d_now_length2 - Node[count].d_length2 > 0 ) // 乗客２と乗客２の目的地も近づいたら
						{
							last_direction[1][0] = 1;
						}else if(sqrt2(Node[count].n_X - center_x, Node[count].n_Y - center_y) >= area_disaster){
							last_direction[1][0] = 1;
						}

						Node[count].n_insec_Y += -1;
						Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の乗客１と目的地との距離
						Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X , Node[count].n_yD2 - Node[count].n_insec_Y ); //移動した後の乗客２と目的地との距離

						

					}else if( direction == 1 ){
						Node[count].n_insec_Y += -1;
						
						Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の乗客１と目的地との距離
						Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X , Node[count].n_yD2 - Node[count].n_insec_Y ); //移動した後の乗客２と目的地との距離
						
						if(d_now_length - Node[count].d_length > 0 ) // 目的地との距離が短くなったら
						{
							last_direction[0][1] = 1;
						}
						
						if(d_now_length2 - Node[count].d_length2 > 0 ) // 乗客２と乗客２の目的地も近づいたら
						{
							last_direction[1][1] = 1;
						}else if(sqrt2(Node[count].n_X - center_x, Node[count].n_Y - center_y) >= area_disaster){
							last_direction[1][1] = 1;
						}

						Node[count].n_insec_Y += 1;
						Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の乗客１と目的地との距離
						Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X , Node[count].n_yD2 - Node[count].n_insec_Y ); //移動した後の乗客２と目的地との距離
						

					}else if( direction == 2 ){
						Node[count].n_insec_X += -1;
						
						Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の乗客１と目的地との距離
						Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X , Node[count].n_yD2 - Node[count].n_insec_Y ); //移動した後の乗客２と目的地との距離
						
						if(d_now_length - Node[count].d_length > 0 ) // 目的地との距離が短くなったら
						{
							last_direction[0][2] = 1;
						}
						
						if(d_now_length2 - Node[count].d_length2 > 0 ) // 乗客２と乗客２の目的地も近づいたら
						{
							last_direction[1][2] = 1;
						}else if(sqrt2(Node[count].n_X - center_x, Node[count].n_Y - center_y) >= area_disaster){
							last_direction[1][2] = 1;
						}

						Node[count].n_insec_X += 1;
						Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の乗客１と目的地との距離
						Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X , Node[count].n_yD2 - Node[count].n_insec_Y ); //移動した後の乗客２と目的地との距離
						
					}else if( direction == 3 ){
						Node[count].n_insec_X += 1;
						
						Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の乗客１と目的地との距離
						Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X , Node[count].n_yD2 - Node[count].n_insec_Y ); //移動した後の乗客２と目的地との距離
						
						if(d_now_length - Node[count].d_length > 0 ) // 目的地との距離が短くなったら
						{
							last_direction[0][3] = 1;
						}
						
						if(d_now_length2 - Node[count].d_length2 > 0 ) // 乗客２と乗客２の目的地も近づいたら
						{
							last_direction[1][3] = 1;
						}else if(sqrt2(Node[count].n_X - center_x, Node[count].n_Y - center_y) >= area_disaster){
							last_direction[1][3] = 1;
						}

						Node[count].n_insec_X += -1;
						Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の乗客１と目的地との距離
						Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X , Node[count].n_yD2 - Node[count].n_insec_Y ); //移動した後の乗客２と目的地との距離
						
					}
				}

				/* 判定に基づいて次に向かう交差点を決定 ---------------------------------------------------------------------------------*/
				if(last_direction[0][0]== 1 && last_direction[1][0] == 1) //乗客1,2ともに、目的地に近づき、北に移動
				{
					Node[count].n_insec_Y += 1;
						
					Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の乗客１と目的地との距離
					Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X , Node[count].n_yD2 - Node[count].n_insec_Y ); //移動した後の乗客２と目的地との距離
					
				}else if(last_direction[0][1]== 1 && last_direction[1][1] == 1) //乗客1,2ともに、目的地に近づき、南に移動
				{
					Node[count].n_insec_Y += -1;
						
					Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); 
					Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X , Node[count].n_yD2 - Node[count].n_insec_Y ); 

				}else if(last_direction[0][2]== 1 && last_direction[1][2] == 1) //乗客1,2ともに、目的地に近づき、西に移動
				{
					Node[count].n_insec_X += -1;
					Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); 
					Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X , Node[count].n_yD2 - Node[count].n_insec_Y ); 

				}else if(last_direction[0][3]== 1 && last_direction[1][3] == 1)  //乗客1,2ともに、目的地に近づき、東に移動
				{
					Node[count].n_insec_X += 1;
						
					Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); 
					Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X , Node[count].n_yD2 - Node[count].n_insec_Y ); 
				}				
				
				// 乗客1,2が共に目的地に近づかない場合, 乗客２を移動前に下車させる
				else if(last_direction[0][0]== 1 && last_direction[1][0] == 0) //乗客1だけ、目的地に近づき、北に移動
				{

					//printf("北に移動せずに降りた!\n");
						
					Pass[Node[count].p_num2].p_X = Node[count].n_insec_X;  //乗客２の現在地を更新
					Pass[Node[count].p_num2].p_Y = Node[count].n_insec_Y;
					Pass[Node[count].p_num2].p_ride = 0; //乗客の乗車情報の変更

					enqueue(Node[count].p_num2); // 更新した現在位置のキューに乗客２の情報を入力
					push(count,Ax*Ax*Twait + Ax*Node[count].n_insec_X + Node[count].n_insec_Y); //ノードに待ち客情報をpush
					Node[count].Map[Node[count].n_insec_X][Node[count].n_insec_Y].info = 1;
					Node[count].Map[Node[count].n_insec_X][Node[count].n_insec_Y].info_time = Twait;
					get_info += 1;
					

					Node[count].n_insec_Y += 1; //北に移動

					Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //距離測定
					Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X , Node[count].n_yD2 - Node[count].n_insec_Y );


					
					Node[count].p_num2 = -1;
					Node[count].n_xD2 = -1;
					Node[count].n_yD2 = -1;

					Node[count].p_on += -1;

					count_getoff += 1;
					


				}else if(last_direction[0][1]== 1 && last_direction[1][1] == 0) //乗客1だけ、目的地に近づき、南に移動
				{
					//if(count==11){printf("南に移動せずに降りた!\n");}
						
					Pass[Node[count].p_num2].p_X = Node[count].n_insec_X;  //乗客２の現在地を更新
					Pass[Node[count].p_num2].p_Y = Node[count].n_insec_Y;
					Pass[Node[count].p_num2].p_ride = 0; //乗客の乗車情報の変更

					enqueue(Node[count].p_num2); // 更新した現在位置のキューに乗客２の情報を入力

					push(count,Ax*Ax*Twait + Ax*Node[count].n_insec_X + Node[count].n_insec_Y);//ノードに待ち客情報をpush
					Node[count].Map[Node[count].n_insec_X][Node[count].n_insec_Y].info = 1;
					Node[count].Map[Node[count].n_insec_X][Node[count].n_insec_Y].info_time = Twait;
					get_info += 1;


					Node[count].n_insec_Y += -1; //南に移動

					Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の乗客１と目的地との距離
					Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X , Node[count].n_yD2 - Node[count].n_insec_Y ); //移動した後の乗客２と目的地との距離

					Node[count].p_num2 = -1;
					Node[count].n_xD2 = -1;
					Node[count].n_yD2 = -1;

					Node[count].p_on += -1;

					count_getoff += 1;
					

				}else if(last_direction[0][2]== 1 && last_direction[1][2] == 0)  //乗客1だけ、目的地に近づき、西に移動
				{
					//printf("西に移動せずに降りた!\n");
				
					Pass[Node[count].p_num2].p_X = Node[count].n_insec_X;  //乗客２の現在地を更新
					Pass[Node[count].p_num2].p_Y = Node[count].n_insec_Y;
					Pass[Node[count].p_num2].p_ride = 0; //乗客の乗車情報の変更

					enqueue(Node[count].p_num2); // 更新した現在位置のキューに乗客２の情報を入力

					push(count,Ax*Ax*Twait + Ax*Node[count].n_insec_X + Node[count].n_insec_Y); //ノードに待ち客情報をpush
					Node[count].Map[Node[count].n_insec_X][Node[count].n_insec_Y].info = 1;
					Node[count].Map[Node[count].n_insec_X][Node[count].n_insec_Y].info_time = Twait;
					get_info += 1;


					Node[count].n_insec_X += -1;// 西に移動
						
					Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の乗客１と目的地との距離
					Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X , Node[count].n_yD2 - Node[count].n_insec_Y ); //移動した後の乗客２と目的地との距離

					Node[count].p_num2 = -1;
					Node[count].n_xD2 = -1;
					Node[count].n_yD2 = -1;

					Node[count].p_on += -1;

					count_getoff += 1;
					

				}else if(last_direction[0][3]== 1 && last_direction[1][3] == 0) //乗客1だけ、目的地に近づき、東に移動
				{
					//printf("東に移動せずに降りた!\n");
					
					Pass[Node[count].p_num2].p_X = Node[count].n_insec_X;  //乗客２の現在地を更新
					Pass[Node[count].p_num2].p_Y = Node[count].n_insec_Y;
					Pass[Node[count].p_num2].p_ride = 0; //乗客の乗車情報の変更

					enqueue(Node[count].p_num2); // 更新した現在位置のキューに乗客２の情報を入力

					push(count,Ax*Ax*Twait + Ax*Node[count].n_insec_X + Node[count].n_insec_Y); //ノードに待ち客情報をpush
					Node[count].Map[Node[count].n_insec_X][Node[count].n_insec_Y].info = 1;
					Node[count].Map[Node[count].n_insec_X][Node[count].n_insec_Y].info_time = Twait;
					get_info += 1;


					Node[count].n_insec_X += 1; //東に移動
						
					Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の乗客１と目的地との距離
					Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X , Node[count].n_yD2 - Node[count].n_insec_Y ); //移動した後の乗客２と目的地との距離


					Node[count].p_num2 = -1;
					Node[count].n_xD2 = -1;
					Node[count].n_yD2 = -1;

					Node[count].p_on += -1;

					count_getoff += 1;
					

				}
				count += 1 ; // 調べるノード番号を増加
			
			}
			// #ifdef dis_center
			// else if(Node[count].p_on == 5){
			// 	d_now_length = Node[count].d_length; // 移動する前の目的地との距離 

			// 	//printf("direction %d\n",direction);

			// 	/*
				
			// 	乗客を乗せている場合、directionの値によって目的地に近づくようにランダムに移動する
			// 	0 ... 北    1... 南
			// 	2 ... 西    3... 東
				
			// 	*/
			// 	int gather_area = Node[count].move_flag * 2;
			// 	if(gather_area > area_disaster){
			// 		gather_area = area_disaster;
			// 	}
			// 	if(Node[count].move_flag > 1 && sqrt2(Node[count].n_X - center_x, Node[count].n_Y - center_y) <= gather_area){
			// 		Node[count].p_on = 0;
			// 	}
				
			// 	//サービスエリアから出たらもう一回経路決定する +  目的地への距離が遠くなったらもう一回経路決定する
			// 	if( direction == 0 && d_now_length != 0){
					
			// 		Node[count].n_insec_Y += -1;
			// 		Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の目的地との距離
					
					

			// 		if(Node[count].d_length - d_now_length > 0.0 ) // 目的地との距離が長くなったらwhile文の先頭に戻る
			// 		{
						
			// 			Node[count].n_insec_Y += 1; //元に戻す
			// 			Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す
						
			// 			continue;
			// 		}

			// 		if(Node[count].n_insec_Y < 0 ) // エリアから南に出た場合while文の先頭に戻る
			// 		{
						
			// 			Node[count].n_insec_Y += 1; //元に戻す
			// 			Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す
			// 			continue;
			// 		}
			// 	}

					
					
			// 	else if( direction == 1 && d_now_length != 0 ){
					
			// 		Node[count].n_insec_Y += 1;
			// 		Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の目的地との距離
					
					

			// 		if(Node[count].d_length -  d_now_length > 0.0 ) // 目的地との距離が長くなったらwhile文の先頭に戻る
			// 		{
			// 			Node[count].n_insec_Y += -1; //元に戻す
			// 			Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す

			// 			continue;
			// 		}

			// 		if(Node[count].n_insec_Y > Ay-1 ) // エリアから北に出た場合while文の先頭に戻る
			// 		{
						
			// 			Node[count].n_insec_Y += -1; //元に戻す
			// 			Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す
						
			// 			continue;
			// 		}

					
					
			// 	}
				
			// 	else if( direction == 2 && d_now_length != 0){
					
			// 		Node[count].n_insec_X += -1;
					
			// 		Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の目的地との距離
					
			// 		if(Node[count].d_length - d_now_length > 0.0 ) // 目的地との距離が長くなったらwhile文の先頭に戻る
			// 		{
						
			// 			Node[count].n_insec_X += 1; //元に戻す
			// 			Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す
						

			// 			continue;
			// 		}
					
			// 		if(Node[count].n_insec_X < 0 ) // エリアから西に出た場合while文の先頭に戻る
			// 		{
			// 			Node[count].n_insec_X += 1; //元に戻す
			// 			Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す
				
			// 			continue;
			// 		}

					

			// 	}
				
			// 	else if( direction == 3 && d_now_length != 0){
					
			// 		Node[count].n_insec_X += 1;

			// 		Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動した後の目的地との距離

					
			// 		if(Node[count].d_length - d_now_length > 0.0 ) // 目的地との距離が長くなったらwhile文の先頭に戻る
			// 		{
						
			// 			Node[count].n_insec_X += -1; //元に戻す
			// 			Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す

			// 			continue;
			// 		}

			// 		if(Node[count].n_insec_X > Ax-1 ) // エリアから東に出た場合while文の先頭に戻る
			// 		{
						
			// 			Node[count].n_insec_X += -1; //元に戻す
			// 			Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X , Node[count].n_yD - Node[count].n_insec_Y ); //移動する前の現在地と目的地との距離に戻す

			// 			continue;
			// 		}

			// 	}
				
				




	// 	count += 1 ; // 調べるノード番号を増加
	// }
	}

	/*if(Node[count].n_X==9.0 && Node[count].n_Y==10.0 && Node[count].n_insec_X==10 && Node[count].n_insec_Y==10){
		traffic_counter += 1;
	}*/
}

/* -------------------------------------------------------------------------------------------- *
 *	関数名 : get_traffic_volume																	*
 *  機能 : すべての交差点の交通量を取得する関数													*
 *  仮引数 : なし													 						    *
 * -------------------------------------------------------------------------------------------- */
// void get_traffic_volume(){
// 	int count = 0;
// 	while(count < N_ALL_NUM){

// 		// x軸方向に移動しているノードについて
// 		if(ceil(Node[count].n_X) != floor(Node[count].n_X) || Node[count].n_Y == Node[count].n_insec_Y){

// 			if((double)Node[count].n_insec_X - Node[count].n_X > 0.0){
// 				// x軸プラス方向に移動しているノードについて
// 				tra_count_y_plus[(int)Node[count].n_X][(int)Node[count].n_Y] += 1;
// 			}else{
// 				// x軸マイナス方向に移動しているノードについて
// 				tra_count_y_minus[(int)Node[count].n_X][(int)Node[count].n_Y] += 1;
// 			}
// 		// y軸方向に移動しているノードについて
// 		}else if(ceil(Node[count].n_Y) != floor(Node[count].n_Y) || Node[count].n_X == Node[count].n_insec_X){

// 			if((double)Node[count].n_insec_Y - Node[count].n_Y > 0.0){
// 				// y軸プラス方向に移動しているノードについて
// 				tra_count_x_plus[(int)Node[count].n_X][(int)Node[count].n_Y] += 1;
// 			}else{
// 				// y軸マイナス方向に移動しているノードについて
// 				tra_count_x_minus[(int)Node[count].n_X][(int)Node[count].n_Y] += 1;
// 			}
// 		}
// 		count += 1;
// 	}
// }


/* -------------------------------------------------------------------------------------------- *
 *	関数名 : speed_error       																	*
 *  機能 : 速度エラーを表示する関数							            						*
 *  仮引数 : なし													 						    *
 * -------------------------------------------------------------------------------------------- */
void speed_error(int count){
	if(Node[count].v < 10){
		printf("speed_error = Node[%d]\n",count);
		exit(1);
	}
}

