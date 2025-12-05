/*
	Common_func.c : 共通関数の実装
*/

#include "globals.h"

/* allrideon関数の実装 */
void allrideon(){
	for(int l=0; l<N_ALL_NUM; l++){
		if(Node[l].n_X == (double)Node[l].n_insec_X && Node[l].n_Y == (double)Node[l].n_insec_Y){
			if(Trans[(int)Node[l].n_X][(int)Node[l].n_Y].wp_Exist == 1 && Node[l].p_on == 0 ){  

				count_ride1 += 1;

				Node[l].p_num = dequeue(Node[l].n_X,Node[l].n_Y);
				Node[l].p_on = 1;
				Pass[Node[l].p_num].p_ride = 1;
				Node[l].n_xD = Pass[Node[l].p_num].p_xD;
				Node[l].n_yD = Pass[Node[l].p_num].p_yD;
				Node[l].d_length = sqrt2(Node[l].n_xD - Node[l].n_X , Node[l].n_yD - Node[l].n_Y );
				if(Trans[(int)Node[l].n_X][(int)Node[l].n_Y].wp_Exist == 1){
					Node[l].Map[(int)Node[l].n_X][(int)Node[l].n_Y].info = 1;
				}else{
					Node[l].Map[(int)Node[l].n_X][(int)Node[l].n_Y].info = 0;
				}
				for(int i=0; i<Ax; i++){
					for(int j=0; j<Ay; j++){
						Node[l].Map[i][j].no_D = 0;
					}
				}
			}else if(Trans[(int)Node[l].n_X][(int)Node[l].n_Y].wp_Exist == 1 && Node[l].p_on == 3 ){  

				if(Node[l].n_xD == Node[l].n_X && Node[l].n_yD == Node[l].n_Y ){
					if(transmit__[l][Node[l].p_num] == 0){
						ride_transmit += 1;
					}
					count_ride3 += 1;
				}else{
					count_ride1 += 1;
				}

				Node[l].p_num = dequeue(Node[l].n_X,Node[l].n_Y);
				Node[l].p_on = 1;
				Pass[Node[l].p_num].p_ride = 1;
				Node[l].n_xD = Pass[Node[l].p_num].p_xD;
				Node[l].n_yD = Pass[Node[l].p_num].p_yD;
				Node[l].d_length = sqrt2(Node[l].n_xD - Node[l].n_X , Node[l].n_yD - Node[l].n_Y );
				if(Trans[(int)Node[l].n_X][(int)Node[l].n_Y].wp_Exist == 1){
					Node[l].Map[(int)Node[l].n_X][(int)Node[l].n_Y].info = 1;
				}else{
					Node[l].Map[(int)Node[l].n_X][(int)Node[l].n_Y].info = 0;
				}
				for(int i=0; i<Ax; i++){
					for(int j=0; j<Ay; j++){
						Node[l].Map[i][j].no_D = 0;
					}
				}
			}

			#ifdef two
			if(Trans[(int)Node[l].n_X][(int)Node[l].n_Y].wp_Exist == 1 && Node[l].p_on == 1 ){  
				count_ride2 += 1;
				Node[l].p_num2 = dequeue(Node[l].n_X,Node[l].n_Y);
				Node[l].p_on += 1;
				Pass[Node[l].p_num2].p_ride = 1;
				Node[l].n_xD2 = Pass[Node[l].p_num2].p_xD;
				Node[l].n_yD2 = Pass[Node[l].p_num2].p_yD;
				Node[l].d_length2 = sqrt2(Node[l].n_xD2 - Node[l].n_X , Node[l].n_yD2 - Node[l].n_Y );
				if(Trans[(int)Node[l].n_X][(int)Node[l].n_Y].wp_Exist == 1){
					Node[l].Map[Node[l].n_X][Node[l].n_Y].info = 1;
				}else{
					Node[l].Map[Node[l].n_X][Node[l].n_Y].info = 0;
				}
				for(int i=0; i<Ax; i++){
					for(int j=0; j<Ay; j++){
						Node[l].Map[i][j].no_D = 0;
					}
				}
			}
			#endif
		}
	}
}

/* D_check関数 - 移動後、乗客の目的地判定 */
void D_check(){
	for(int i=0 ; i < N_ALL_NUM ; i++){
		#ifdef two
		if(Node[i].p_on == 2){
			if(Node[i].n_X == Node[i].n_xD2 && Node[i].n_Y == Node[i].n_yD2 ){   
				if(Node[i].n_X == Node[i].n_xD && Node[i].n_Y == Node[i].n_yD ) {
					count_same += 1;
				}
				Node[i].move_flag += 1;
				Pass[Node[i].p_num2].p_Exist = 0;
				Node[i].p_on -= 1;
				Pass[Node[i].p_num2].p_ride = 0;
				Pass[Node[i].p_num2].p_X = Pass[Node[i].p_num2].p_xD;
				Pass[Node[i].p_num2].p_Y = Pass[Node[i].p_num2].p_yD;
				Pass[Node[i].p_num2].p_wait = Twait;
				Node[i].p_num2 = -1;
				Node[i].n_xD2 = -1;
				Node[i].n_yD2 = -1;
				goalstep[o] = Twait;
				o++;
				allstep += Twait;
			}

			if(Node[i].n_X == Node[i].n_xD && Node[i].n_Y == Node[i].n_yD ){    
				Pass[Node[i].p_num].p_Exist = 0;
				Node[i].p_on -= 1;
				Pass[Node[i].p_num].p_ride = 0;
				Pass[Node[i].p_num].p_X = Pass[Node[i].p_num].p_xD;
				Pass[Node[i].p_num].p_Y = Pass[Node[i].p_num].p_yD;
				Pass[Node[i].p_num].p_wait = Twait;
				Node[i].p_num = Node[i].p_num2;
				Node[i].n_xD = Node[i].n_xD2;
				Node[i].n_yD = Node[i].n_yD2;
				Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X , Node[i].n_yD - Node[i].n_Y );
				Node[i].p_num2 = -1;
				Node[i].n_xD2 = -1;
				Node[i].n_yD2 = -1;
				goalstep[o] = Twait;
				o++;
				allstep += Twait;
			}
		}
		#endif
		
		if(Node[i].p_on == 1){
			if(Node[i].n_X == Node[i].n_xD && Node[i].n_Y == Node[i].n_yD ){  
				Pass[Node[i].p_num].p_Exist = 0;
				Node[i].p_on = 0;
				Pass[Node[i].p_num].p_ride = 0;
				Pass[Node[i].p_num].p_X = Pass[Node[i].p_num].p_xD;
				Pass[Node[i].p_num].p_Y = Pass[Node[i].p_num].p_yD;
				Pass[Node[i].p_num].p_wait = Twait;
				Node[i].p_num = -1;
				Node[i].n_xD = -1;
				Node[i].n_yD = -1;
				Node[i].d_length = -1;
				Node[i].move_flag += 1;
				goalstep[o] = Twait;
				o++;
				allstep += Twait;
			}
		}

		if(Node[i].p_on == 3){
			if(Node[i].n_xD == Node[i].n_X && Node[i].n_yD == Node[i].n_Y && Trans[Node[i].n_insec_X][Node[i].n_insec_Y].wp_Exist == 0){ 	
				pushnopos(i,Ax*Node[i].n_X+Node[i].n_Y);
				Node[i].p_on = 0;
				Node[i].p_num = 0;
				Node[i].n_xD = -2;
				Node[i].n_yD = -2;
				count_p_no += 1;		
			}
		}
		if(Node[i].p_on == 5){
			int gather_area = Node[i].move_flag * 5;
			if(gather_area >= area_disaster){
				gather_area = area_disaster;
			}
			if(sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) <= gather_area){
				Node[i].p_on = 0;
				Node[i].n_xD = -2;
				Node[i].n_yD = -2;
			}
            if(Node[i].n_X == Node[i].n_xD && Node[i].n_Y == Node[i].n_yD ){  
                Node[i].p_on = 0;
            }
		}
	}
}

/* P_check関数 */
int P_check(){
	 check = 0;
	 check_1 = 0;
	 check_2 = 0;
	 check_3 = 0;
	 check_4 = 0;
	for(int i=0 ; i < P_ALL_NUM+p_add_count; i++){
		if(Pass[i].p_Exist == 0){
			check += 1;
			if(Pass[i].p_X == d[0][0] && Pass[i].p_Y == d[0][1]){
				check_1 += 1;
			}if(Pass[i].p_X == d[1][0] && Pass[i].p_Y == d[1][1]){
				check_2 += 1;
			}if(Pass[i].p_X == d[2][0] && Pass[i].p_Y == d[2][1]){
				check_3 += 1;
			}if(Pass[i].p_X == d[3][0] && Pass[i].p_Y == d[3][1]){
				check_4 += 1;
			}
		}
	}
	printf("到着人数 %d\n",check);
	return check;
}

/* flooding関数 */
void flooding(){
	int i;
	int flooding_end;

	while ( TRUE ){	
		flooding_end	= TRUE;
		
		for(i=0;i<N_ALL_NUM;i++){
			if ( Node[i].stack_num < 1 )
				continue;
			
			if ( transmit(i) > 0 )
				flooding_end	= FALSE;
		}
		if ( flooding_end == TRUE)
			break;
	}
}

/* transmit関数 */
int transmit(int k){
	int i;
	double x_a_iNode = Node[k].n_X;
	double y_a_iNode = Node[k].n_Y;
	double x_iNode;
	double y_iNode;
	
	int num_transmit = 0;

	for(i=0;i<N_ALL_NUM;i++){
		x_iNode	= Node[i].n_X;
		y_iNode	= Node[i].n_Y;
		
		if(search2(i,k,Twait) == 0){
			if ( (sqrt2( x_a_iNode - x_iNode, y_a_iNode - y_iNode )*Td <= r )){ 
				for(int l=0; l < Ax; l++){
					for(int m=0; m < Ay; m++){
						if(Node[i].Map[l][m].info_time < Node[k].Map[l][m].info_time){
							Node[i].Map[l][m].info = Node[k].Map[l][m].info;
							Node[i].Map[l][m].info_time = Node[k].Map[l][m].info_time;
						}
					}
				}
				if((Node[k].stack_data[0] > Node[i].stack_data[0]) && Node[k].stack_data[0] != 0 && (Node[k].n_X==Node[i].n_X || Node[k].n_Y==Node[i].n_Y)) {
					stack_sort();
					int numdata = peek(k);
					push(i,numdata);
					push2(i,k,Twait);
					stack_sort();
					num_transmit ++;
					total_transmit += num_transmit;
				}
			}
		}
		

		if((sqrt2( x_a_iNode - x_iNode, y_a_iNode - y_iNode )*Td <= r) && (Node[k].n_X==Node[i].n_X || Node[k].n_Y==Node[i].n_Y)){
			if(Node[i].p_on == 3 && Node[k].p_on == 3 && Node[i].n_xD == Node[k].n_xD && Node[i].n_yD == Node[k].n_yD){
				if(i>k){
					Node[k].p_on = 0;
					Node[k].p_num = -1;
					Node[k].n_xD = -2;
					Node[k].n_yD = -2;
				}else if(i<k){
					Node[i].p_on = 0;
					Node[i].p_num = -1;
					Node[i].n_xD = -2;
					Node[i].n_yD = -2;
				}
			}
		}
	}
	return	num_transmit;
}

/* detect_trans関数 */
void detect_trans(){
	int data =0;
	for(int i=0 ;i<N_ALL_NUM ;i++){
		
		if((double)Node[i].n_insec_X==Node[i].n_X && (double)Node[i].n_insec_Y==Node[i].n_Y){
			for(int j=0; j<P_ALL_NUM; j++){
				if(transmit__[i][j] == 0){
					if(Node[i].n_Y == Pass[j].p_Y && (fabs(Node[i].n_X - Pass[j].p_X)*Td <= r)){		
						if(Pass[j].p_Exist == 1){
							Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info = 1;
							Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info_time = Twait;
							data = Ax*Ax*Twait + Ax*(Pass[j].p_X) + Pass[j].p_Y;  
							push(i,data);
							get_info += 1;
							transmit__[i][j] = Twait;
						}
					}
					else if(Node[i].n_X == Pass[j].p_X && (fabs(Node[i].n_Y - Pass[j].p_Y)*Td <= r)){
						if(Pass[j].p_Exist == 1){
							Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info = 1;
							Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info_time = Twait;
							data = Ax*Ax*Twait + Ax*(Pass[j].p_X) + Pass[j].p_Y;
							push(i,data);
							get_info += 1;
							transmit__[i][j] = Twait;
						}
					}
				}
			}
		}else{
			if(ceil(Node[i].n_X) != floor(Node[i].n_X) || Node[i].n_Y == Node[i].n_insec_Y){
				for(int j=0; j<P_ALL_NUM; j++){
					if(transmit__[i][j] == 0){
						if(Node[i].n_Y == Pass[j].p_Y && (fabs(Node[i].n_X - Pass[j].p_X)*Td <= r)){		
							if(Pass[j].p_Exist == 1){
								Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info = 1;
								Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info_time = Twait;
								data = Ax*Ax*Twait + Ax*(Pass[j].p_X) + Pass[j].p_Y;
								push(i,data);
								get_info += 1;
								transmit__[i][j] = Twait;
							}
						}
					}
				}
			}else if(ceil(Node[i].n_Y) != floor(Node[i].n_Y) || Node[i].n_X == Node[i].n_insec_X){
				for(int j=0; j<P_ALL_NUM; j++){
					if(transmit__[i][j] == 0){
						if(Node[i].n_X == Pass[j].p_X && (fabs(Node[i].n_Y - Pass[j].p_Y)*Td <= r)){
							if(Pass[j].p_Exist == 1){
								Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info = 1;
								Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info_time = Twait;
								data = Ax*Ax*Twait + Ax*(Pass[j].p_X) + Pass[j].p_Y;
								push(i,data);
								get_info += 1;
								transmit__[i][j] = Twait;
							}
						}
					}
				}
			}
		}
	}
}

/* syokika関数 */
int syokika(){ 
	for(int count=0;count<N_ALL_NUM;count++){
		for(int i=0;i<ssize;i++){
			Node[count].stack_data[i] = 0;
			Node[count].stack_nopos[i] = 0;
		}
		for(int j=0; j<P_ALL_NUM; j++){
			transmit__[count][j] = 0;
		}
		Node[count].prog_d_surp = 0;
		Node[count].stack_num = 0;
		Node[count].stack_num2 = 0;
		Node[count].all_stack_num = 0;	
		Node[count].move_flag = 0;
		for(int i=0; i<Ax; i++){
			for(int j=0; j<Ay; j++){
				Node[count].Map[i][j].info = 0;
				Node[count].Map[i][j].no_D = 0;
			}
		}
	}
	for(int i=0; i<Ax; i++){
		for(int j=0; j<Ay; j++){
			queue_num[i][j]=0;
			queue_head[i][j]=0;
			Trans[i][j].wp_Exist=0;
		}
	}
	o = 0;
}

/* check_ride関数 */
int check_ride(){ 
	for(int y=0;y<P_ALL_NUM+p_add_count;y++){
		if(Pass[y].p_Exist == 1 && Pass[y].p_ride == 1){
			ridecount++;
		}else if(Pass[y].p_Exist == 1 && Pass[y].p_ride == 0){
			waitcount++;
		}
	}
}
