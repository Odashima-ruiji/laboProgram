/*
    Common_func.c : 共通関数の実装
*/

#include "config.h"
#include "base_struct.h"
#include "globals.h"
#include "base_func.h"
#include "Common_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ------------------------------------------------------------------------------------ *
 *	関数名 : allrideon										 							*
 *  機能 : ノードに乗客を乗せる. すべての交差点の待ち乗客がいるかいないか判定し,		*
 * 		客のいる交差点に車がいる, かつ車が満車でない場合に乗せる. 一つの交差点に		*
 * 		複数人が待っていても,車が複数台あれば対応する.									*
 *  仮引数 : なし											 							*
 * ------------------------------------------------------------------------------------ */
void allrideon()
{
    /*新しい方法*/
    for (int l = 0; l < N_ALL_NUM; l++)
    {
        if (Node[l].n_X == (double)Node[l].n_insec_X && Node[l].n_Y == (double)Node[l].n_insec_Y)
        {
            /* 乗客が一人も乗っていない状態 */
            if (Trans[(int)Node[l].n_X][(int)Node[l].n_Y].wp_Exist == 1 && Node[l].p_on == 0)
            {

                count_ride1 += 1;

                Node[l].p_num = dequeue(Node[l].n_X, Node[l].n_Y);                                // 乗客の番号をノード情報に入力
                Node[l].p_on = 1;                                                                 // ノードの乗客状況を変更　複数人の乗れる場合はここを変更
                Pass[Node[l].p_num].p_ride = 1;                                                   // 乗客の乗車状態の変更
                Node[l].n_xD = Pass[Node[l].p_num].p_xD;                                          // 乗客の目的地情報をノードの目的地情報に入力(x座標)
                Node[l].n_yD = Pass[Node[l].p_num].p_yD;                                          // 乗客の目的地情報をノードの目的地情報に入力(y座標)
                Node[l].d_length = sqrt2(Node[l].n_xD - Node[l].n_X, Node[l].n_yD - Node[l].n_Y); // 目的地との距離を入力
                // 乗客を乗せた交差点の待ち客がいなくなった場合、Map情報を更新
                if (Trans[(int)Node[l].n_X][(int)Node[l].n_Y].wp_Exist == 0)
                {
                    Node[l].Map[(int)Node[l].n_X][(int)Node[l].n_Y].info = 0;
                    Node[l].Map[(int)Node[l].n_X][(int)Node[l].n_Y].p_num = -2;
                    Node[l].Map[(int)Node[l].n_X][(int)Node[l].n_Y].info_time = Twait + 1; // info=0 を優先させるため +1
                }else{
                    Node[l].Map[(int)Node[l].n_X][(int)Node[l].n_Y].info = 1;
                    Node[l].Map[(int)Node[l].n_X][(int)Node[l].n_Y].info_time = Twait;
                }
                
                for (int i = 0; i < Ax; i++)
                {
                    for (int j = 0; j < Ay; j++)
                    {
                        Node[l].Map[i][j].no_D = 0;
                    }
                }
            }
            else if (Trans[(int)Node[l].n_X][(int)Node[l].n_Y].wp_Exist == 1 && Node[l].p_on == 3)
            {
                /* 乗客が一人も乗っていない状態　かつ　ifより得た情報がある場合 */

                if (Node[l].n_xD == Node[l].n_X && Node[l].n_yD == Node[l].n_Y)
                { // 情報フローティングで得た情報をもとに乗客を載せた場合
                    if (transmit__[l][Node[l].p_num] != 0)
                    {
                        ride_transmit += 1;
                    }
                    count_ride3 += 1;
                }
                else
                {
                    count_ride1 += 1;
                }

                Node[l].p_num = dequeue(Node[l].n_X, Node[l].n_Y);                                // 乗客の番号をノード情報に入力
                Node[l].p_on = 1;                                                                 // ノードの乗客状況を変更　複数人の乗れる場合はここを変更
                Pass[Node[l].p_num].p_ride = 1;                                                   // 乗客の乗車状態の変更
                Node[l].n_xD = Pass[Node[l].p_num].p_xD;                                          // 乗客の目的地情報をノードの目的地情報に入力(x座標)
                Node[l].n_yD = Pass[Node[l].p_num].p_yD;                                          // 乗客の目的地情報をノードの目的地情報に入力(y座標)
                Node[l].d_length = sqrt2(Node[l].n_xD - Node[l].n_X, Node[l].n_yD - Node[l].n_Y); // 目的地との距離を入力
                
                // 乗客を乗せた交差点の待ち客がいなくなった場合、Map情報を更新
                if (Trans[(int)Node[l].n_X][(int)Node[l].n_Y].wp_Exist == 0)
                {
                    Node[l].Map[(int)Node[l].n_X][(int)Node[l].n_Y].info = 0;
                    Node[l].Map[(int)Node[l].n_X][(int)Node[l].n_Y].p_num = -2;
                    Node[l].Map[(int)Node[l].n_X][(int)Node[l].n_Y].info_time = Twait + 1; // info=0 を優先させるため +1
                }else{
                    Node[l].Map[(int)Node[l].n_X][(int)Node[l].n_Y].info = 1;
                    Node[l].Map[(int)Node[l].n_X][(int)Node[l].n_Y].info_time = Twait;
                }
                
                for (int i = 0; i < Ax; i++)
                {
                    for (int j = 0; j < Ay; j++)
                    {
                        Node[l].Map[i][j].no_D = 0;
                    }
                }
            }

#ifdef two
            /* 乗客が一人乗っている状態 */
            if (Trans[(int)Node[l].n_X][(int)Node[l].n_Y].wp_Exist == 1 && Node[l].p_on == 1)
            {
                count_ride2 += 1;
                Node[l].p_num2 = dequeue(Node[l].n_X, Node[l].n_Y);                                  // 乗客の番号をノード情報に入力
                Node[l].p_on += 1;                                                                   // ノードの乗客状況を変更　複数人の乗れる場合はここを変更
                Pass[Node[l].p_num2].p_ride = 1;                                                     // 乗客の乗車状態の変更
                Node[l].n_xD2 = Pass[Node[l].p_num2].p_xD;                                           // 乗客の目的地情報をノードの目的地情報2に入力(x座標)
                Node[l].n_yD2 = Pass[Node[l].p_num2].p_yD;                                           // 乗客の目的地情報をノードの目的地情報2に入力(y座標)
                Node[l].d_length2 = sqrt2(Node[l].n_xD2 - Node[l].n_X, Node[l].n_yD2 - Node[l].n_Y); // 目的地との距離を入力
                
                // 乗客を乗せた交差点の待ち客がいなくなった場合、Map情報を更新
                if (Trans[(int)Node[l].n_X][(int)Node[l].n_Y].wp_Exist == 0)
                {
                    Node[l].Map[(int)Node[l].n_X][(int)Node[l].n_Y].info = 0;
                    Node[l].Map[(int)Node[l].n_X][(int)Node[l].n_Y].p_num = -2;
                    Node[l].Map[(int)Node[l].n_X][(int)Node[l].n_Y].info_time = Twait + 1; // info=0 を優先させるため +1
                }else{
                    Node[l].Map[(int)Node[l].n_X][(int)Node[l].n_Y].info = 1;
                    Node[l].Map[(int)Node[l].n_X][(int)Node[l].n_Y].info_time = Twait;
                }
                
                }
                for (int i = 0; i < Ax; i++)
                {
                    for (int j = 0; j < Ay; j++)
                    {
                        Node[l].Map[i][j].no_D = 0;
                    }
                }
            }
#endif
        }
    }
}

/* -------------------------------------------- *
 *	関数名 : D_check 							*
 *  機能 : 移動後, 乗客の目的地判定を行う関数	*
 *  仮引数 : なし								*
 * -------------------------------------------- */
void D_check()
{
    for (int i = 0; i < N_ALL_NUM; i++)
    {
#ifdef two
        if (Node[i].p_on == 2)
        { // ノードに乗客が二人いる場合

            // 乗客２の目的地とノードの現在位置が一致する場合
            if (Node[i].n_X == Node[i].n_xD2 && Node[i].n_Y == Node[i].n_yD2)
            {
                if (Node[i].n_X == Node[i].n_xD && Node[i].n_Y == Node[i].n_yD)
                {
                    count_same += 1;
                }
                Node[i].move_flag += 1;
                // printf("3:  乗客が2人乗っている車から乗客2の %d 番目のお客様が到着されました!!\n",Node[i].p_num2);
                Pass[Node[i].p_num2].p_Exist = 0;                     // 乗客の存在を消去
                Node[i].p_on -= 1;                                    // ノードの乗車情報を変更
                Pass[Node[i].p_num2].p_ride = 0;                      // 乗客の乗車情報を変更
                Pass[Node[i].p_num2].p_X = Pass[Node[i].p_num2].p_xD; // 乗客の現在位置を更新
                Pass[Node[i].p_num2].p_Y = Pass[Node[i].p_num2].p_yD;
                Pass[Node[i].p_num2].p_wait = Twait;
                Node[i].p_num2 = -1; // ノードの乗客番号情報を -1に変更
                Node[i].n_xD2 = -1;  // ノードの目的地(x座標)情報を -1に変更
                Node[i].n_yD2 = -1;  // ノードの目的地情報(y座標)情報を -1に変更
                goalstep[o] = Twait;
                o++;
                allstep += Twait;
            }

            // 乗客1の目的地とノードの現在位置が一致する場合
            if (Node[i].n_X == Node[i].n_xD && Node[i].n_Y == Node[i].n_yD)
            {
                // printf("2:  乗客が2人乗っている車から乗客1の %d 番目のお客様が到着されました!!\n",Node[i].p_num);
                Pass[Node[i].p_num].p_Exist = 0;                    // 乗客の存在を消去
                Node[i].p_on -= 1;                                  // ノードの乗車情報を変更
                Pass[Node[i].p_num].p_ride = 0;                     // 乗客の乗車情報を変更
                Pass[Node[i].p_num].p_X = Pass[Node[i].p_num].p_xD; // 乗客の現在位置を更新
                Pass[Node[i].p_num].p_Y = Pass[Node[i].p_num].p_yD;
                Pass[Node[i].p_num].p_wait = Twait;
                Node[i].p_num = Node[i].p_num2; // ノードの乗客番号情報を -1に変更
                Node[i].n_xD = Node[i].n_xD2;   // ノードの目的地(x座標)情報を -1に変更
                Node[i].n_yD = Node[i].n_yD2;
                Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                Node[i].p_num2 = -1; // ノードの乗客番号情報を -1に変更
                Node[i].n_xD2 = -1;  // ノードの目的地(x座標)情報を -1に変更
                Node[i].n_yD2 = -1;  // ノードの目的地情報(y座標)情報を -1に変更
                goalstep[o] = Twait;
                o++;
                allstep += Twait;

                if (Node[i].stack_num > 0 && Node[i].p_on == 0 && i < po)
                {
                    int count = 0;
                    do
                    {
                        count += 1;
                        int numdata = peek(i);
                        // stack_sort();
                        if (search(i, numdata) == 0)
                        { // 同じデータがなければ

                            int numdata_x = (numdata % (Ax * Ax)) / Ax;
                            int numdata_y = numdata % Ax;
                            if ((Twait * Ax * Ax - numdata > reftime * Ax * Ax) || (numdata_x - Node[i].n_X > refdistance || Node[i].n_X - numdata_x > refdistance) || (numdata_y - Node[i].n_Y > refdistance || Node[i].n_Y - numdata_y > refdistance) || Node[i].Map[numdata_x][numdata_y].no_D == 1)
                            { // 長い時間と距離経過しているとき参照しない

                                pop(i);
                                stack_sort();
                                Node[i].p_on = 0;
                                // printf("現在時刻 %d %d番ノード: %ds前\n",Twait,i,(Twait*10000 - numdata)/10000);
                                // printf("本来目指す目的地(%d,%d)\n\n",Node[i].n_xD,Node[i].n_yD);

                                Node[i].n_xD = -1;
                                Node[i].n_yD = -1;

                                int pattern_1 = rand() % 4 + 1;

#ifdef dis_center
                                pattern_1 = 0;
#endif
                                if (Node[i].n_X < center_x && Node[i].n_Y < center_y)
                                {
                                    if (pattern_1 == 0)
                                    {
                                        // Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x;
                                        Node[i].n_yD = center_y;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                    else if (pattern_1 == 1 || pattern_1 == 2)
                                    {
                                        Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x - 12;
                                        Node[i].n_yD = center_y;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                    else if (pattern_1 == 3 || pattern_1 == 4)
                                    {
                                        Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x;
                                        Node[i].n_yD = center_y - 12;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                }
                                else if (Node[i].n_X < center_x && Node[i].n_Y > center_y)
                                {
                                    if (pattern_1 == 0)
                                    {
                                        // Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x;
                                        Node[i].n_yD = center_y;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                    else if (pattern_1 == 1 || pattern_1 == 2)
                                    {
                                        Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x - 12;
                                        Node[i].n_yD = center_y;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                    else if (pattern_1 == 3 || pattern_1 == 4)
                                    {
                                        Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x;
                                        Node[i].n_yD = center_y + 12;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                }
                                else if (Node[i].n_X > center_x && Node[i].n_Y > center_y)
                                {
                                    if (pattern_1 == 0)
                                    {
                                        // Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x;
                                        Node[i].n_yD = center_y;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                    else if (pattern_1 == 1 || pattern_1 == 2)
                                    {
                                        Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x + 12;
                                        Node[i].n_yD = center_y;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                    else if (pattern_1 == 3 || pattern_1 == 4)
                                    {
                                        Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x;
                                        Node[i].n_yD = center_y + 12;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                }
                                else if (Node[i].n_X > center_x && Node[i].n_Y < center_y)
                                {
                                    if (pattern_1 == 0)
                                    {
                                        // Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x;
                                        Node[i].n_yD = center_y;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                    else if (pattern_1 == 1 || pattern_1 == 2)
                                    {
                                        Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x + 12;
                                        Node[i].n_yD = center_y;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                    else if (pattern_1 == 3 || pattern_1 == 4)
                                    {
                                        Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x;
                                        Node[i].n_yD = center_y - 12;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                }

                                pushnopos(i, numdata);
                                continue;
                            }
                            else
                            {
                                pop(i);
                                stack_sort();
                                Node[i].p_on = 3;
                                Node[i].n_xD = (numdata % (Ax * Ax)) / Ax;
                                Node[i].n_yD = numdata % Ax;
                                Node[i].Map[Node[i].n_xD][Node[i].n_yD].no_D = 1;
                                count_p_on_3 += 1;
                            }

                            // count_ifgo += 1;
                            pushnopos(i, numdata);

                            count = 0;
                            break;
                        }

                        // if(search(i,Ax*Node[i].n_xD+Node[i].n_yD)>0){
                        // printf("BB\n");
                        //}
                        // 客が生成されるために一度訪れたところでも行く
                    } while (/*search(i,Ax*Node[i].n_yD+Node[i].n_yD)>0 &&*/ count < Node[i].stack_num + 1);

                    if (Node[i].n_yD < 0)
                    {
                        Node[i].p_on = 0;
                    }
                    if (Node[i].stack_num == 0)
                    {
                        Node[i].n_xD = center_x;
                        Node[i].n_yD = center_y;
                        Node[i].p_on = 5;
                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                    }

                    // printf("目的地は(%d.%d)\n",Node[i].n_xD,Node[i].n_yD);
                    Node[i].p_num = -1;
                    Node[i].p_num2 = -1;
                    Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y); // 目的地との距離を入力
                                                                                                      // printf("ABBBBBB\n");
                }
            }
        }
#endif

        if (Node[i].p_on == 1)
        { // ノードに乗客がいる場合の乗客１の判定

            if (Node[i].n_X == Node[i].n_xD && Node[i].n_Y == Node[i].n_yD)
            { // ノードの目的地とノードの現在位置が一致する場合

                int count1 = 0;
                // printf("1:  乗客が1人乗っている車から乗客1の %d 番目のお客様が到着されました!!\n",Node[i].p_num);
                Pass[Node[i].p_num].p_Exist = 0;                    // 乗客の存在を消去
                Node[i].p_on = 0;                                   // ノードの乗車情報を変更
                Pass[Node[i].p_num].p_ride = 0;                     // 乗客の乗車情報を変更
                Pass[Node[i].p_num].p_X = Pass[Node[i].p_num].p_xD; // 乗客の現在位置を更新
                Pass[Node[i].p_num].p_Y = Pass[Node[i].p_num].p_yD;
                Pass[Node[i].p_num].p_wait = Twait;
                Node[i].p_num = -1; // ノードの乗客番号情報を -1に変更
                Node[i].n_xD = -1;  // ノードの目的地(x座標)情報を -1に変更
                Node[i].n_yD = -1;  // ノードの目的地情報(y座標)情報を -1に変更
                Node[i].d_length = -1;
                Node[i].move_flag += 1;
                goalstep[o] = Twait;
                o++;
                allstep += Twait;

                if (Node[i].stack_num > 0 && Node[i].p_on == 0 && i <= po)
                {
                    int count1 = 0;
                    do
                    {
                        count1 += 1;
                        int numdata = peek(i);
                        // stack_sort();
                        if (search(i, numdata) == 0)
                        { // 同じデータがなければ

                            int numdata_x = (numdata % (Ax * Ax)) / Ax;
                            int numdata_y = numdata % Ax;
                            if ((Twait * Ax * Ax - numdata > reftime * Ax * Ax) || (numdata_x - Node[i].n_X > refdistance || Node[i].n_X - numdata_x > refdistance) || (numdata_y - Node[i].n_Y > refdistance || Node[i].n_Y - numdata_y > refdistance) || Node[i].Map[numdata_x][numdata_y].no_D == 1)
                            { // 長い時間と距離経過しているとき参照しない

                                pop(i);
                                stack_sort();
                                Node[i].p_on = 0;
                                // printf("現在時刻 %d %d番ノード: %ds前\n",Twait,i,(Twait*10000 - numdata)/10000);
                                // printf("本来目指す目的地(%d,%d)\n\n",Node[i].n_xD,Node[i].n_yD);

                                Node[i].n_xD = -1;
                                Node[i].n_yD = -1;

#ifdef dis_center
                                int pattern_1 = 0;
#else
                                int pattern_1 = rand() % 4 + 1;
#endif
                                if (Node[i].n_X < center_x && Node[i].n_Y <= center_y)
                                {
                                    if (pattern_1 == 0)
                                    {
                                        // Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x;
                                        Node[i].n_yD = center_y;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                    else if (pattern_1 == 1 || pattern_1 == 2)
                                    {
                                        // Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x - 5;
                                        Node[i].n_yD = center_y;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                    else if (pattern_1 == 3 || pattern_1 == 4)
                                    {
                                        // Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x;
                                        Node[i].n_yD = center_y - 5;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                }
                                else if (Node[i].n_X <= center_x && Node[i].n_Y > center_y)
                                {
                                    if (pattern_1 == 0)
                                    {
                                        // Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x;
                                        Node[i].n_yD = center_y;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                    else if (pattern_1 == 1 || pattern_1 == 2)
                                    {
                                        // Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x - 5;
                                        Node[i].n_yD = center_y;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                    else if (pattern_1 == 3 || pattern_1 == 4)
                                    {
                                        // Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x;
                                        Node[i].n_yD = center_y + 5;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                }
                                else if (Node[i].n_X > center_x && Node[i].n_Y >= center_y)
                                {
                                    if (pattern_1 == 0)
                                    {
                                        // Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x;
                                        Node[i].n_yD = center_y;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                    else if (pattern_1 == 1 || pattern_1 == 2)
                                    {
                                        // Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x + 5;
                                        Node[i].n_yD = center_y;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                    else if (pattern_1 == 3 || pattern_1 == 4)
                                    {
                                        // Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x;
                                        Node[i].n_yD = center_y + 5;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                }
                                else if (Node[i].n_X >= center_x && Node[i].n_Y < center_y)
                                {
                                    if (pattern_1 == 0)
                                    {
                                        // Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x;
                                        Node[i].n_yD = center_y;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                    else if (pattern_1 == 1 || pattern_1 == 2)
                                    {
                                        // Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x + 5;
                                        Node[i].n_yD = center_y;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                    else if (pattern_1 == 3 || pattern_1 == 4)
                                    {
                                        // Node[i].move_flag += 1;
                                        Node[i].n_xD = center_x;
                                        Node[i].n_yD = center_y - 5;
                                        Node[i].p_on = 5;
                                        Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                                    }
                                }

                                pushnopos(i, numdata);
                                continue;
                            }
                            else
                            {
                                pop(i);
                                stack_sort();
                                Node[i].p_on = 3;
                                Node[i].n_xD = (numdata % (Ax * Ax)) / Ax;
                                Node[i].n_yD = numdata % Ax;
                                Node[i].Map[Node[i].n_xD][Node[i].n_yD].no_D = 1;
                                count_p_on_3 += 1;
                            }

                            // count_ifgo += 1;
                            pushnopos(i, numdata);

                            count1 = 0;
                            break;
                        }

                        // if(search(i,Ax*Node[i].n_xD+Node[i].n_yD)>0){
                        // printf("BBBB\n");
                        //}
                        // 客が生成されるため
                    } while (/*search(i,Ax*Node[i].n_xD+Node[i].n_yD)>0 && */ count1 < Node[i].stack_num);

                    if (Node[i].n_yD < 0)
                    {
                        Node[i].p_on = 0;
                    }

                    // printf("目的地は(%d.%d)\n",Node[i].n_xD,Node[i].n_yD);
                    Node[i].p_num = -1;
                    Node[i].p_num2 = -1;
                    Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y); // 目的地との距離を入力
                                                                                                      // printf("ABBBBBB\n");
                }
                if (Node[i].p_on == 0)
                {
                    Node[i].n_xD = center_x;
                    Node[i].n_yD = center_y;
                    Node[i].p_on = 5;
                    Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
                }
            }
        }

        if (Node[i].p_on == 3)
        {
            // 移動後に情報に従うノードが目的地に到着しかし待ち客はいなかった
            if (Node[i].n_xD == Node[i].n_X && Node[i].n_yD == Node[i].n_Y && Trans[Node[i].n_insec_X][Node[i].n_insec_Y].wp_Exist == 0)
            {

                pushnopos(i, Ax * Node[i].n_X + Node[i].n_Y);

                Node[i].p_on = 0;
                Node[i].p_num = 0;
                Node[i].n_xD = -2;
                Node[i].n_yD = -2;
                count_p_no += 1;
            }
        }
        if (Node[i].p_on == 5)
        {
            int gather_area = Node[i].move_flag * 5;
            if (gather_area >= area_disaster)
            {
                gather_area = area_disaster;
            }
            if (/*Node[i].move_flag >= 1 && */ sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) <= gather_area)
            {
                Node[i].p_on = 0;
                Node[i].n_xD = -2;
                Node[i].n_yD = -2;
                // printf("flag %d\n", Node[i].move_flag);
            }
            // 移動後、中心に到着したかの確認と、目的とする乗客の設定
            if (Node[i].n_X == Node[i].n_xD && Node[i].n_Y == Node[i].n_yD)
            {
                // Node[i].move_flag = 1;
                Node[i].p_on = 0; // ノードの乗車情報を変更

                if (Node[i].stack_num > 0 && Node[i].p_on == 0 && i <= po)
                {
                    int count = 0;
                    do
                    {
                        count += 1;
                        int numdata = peek(i);
                        // stack_sort();
                        if (search(i, numdata) == 0)
                        { // 同じデータがなければ

                            int numdata_x = (numdata % (Ax * Ax)) / Ax;
                            int numdata_y = numdata % Ax;
                            if ((Twait * Ax * Ax - numdata > reftime * Ax * Ax) || (numdata_x - Node[i].n_X > refdistance || Node[i].n_X - numdata_x > refdistance) || (numdata_y - Node[i].n_Y > refdistance || Node[i].n_Y - numdata_y > refdistance) || Node[i].Map[numdata_x][numdata_y].no_D == 1)
                            { // 長い時間と距離経過しているとき参照しない

                                pop(i);
                                stack_sort();
                                Node[i].p_on = 0;
                                // printf("現在時刻 %d %d番ノード: %ds前\n",Twait,i,(Twait*10000 - numdata)/10000);
                                // printf("本来目指す目的地(%d,%d)\n\n",Node[i].n_xD,Node[i].n_yD);

                                Node[i].n_xD = -1;
                                Node[i].n_yD = -1;

                                pushnopos(i, numdata);
                                continue;
                            }
                            else
                            {
                                pop(i);
                                stack_sort();
                                Node[i].p_on = 3;
                                Node[i].n_xD = (numdata % (Ax * Ax)) / Ax;
                                Node[i].n_yD = numdata % Ax;
                                Node[i].Map[Node[i].n_xD][Node[i].n_yD].no_D = 1;
                                count_p_on_3 += 1;
                            }

                            // count_ifgo += 1;
                            pushnopos(i, numdata);

                            count = 0;
                            break;
                        }

                        // if(search(i,Ax*Node[i].n_xD+Node[i].n_yD)>0){
                        // printf("BB\n");
                        //}
                        // 客が生成されるために一度訪れたところでも行く
                    } while (/*search(i,Ax*Node[i].n_yD+Node[i].n_yD)>0 &&*/ count < Node[i].stack_num + 1);

                    if (Node[i].n_yD < 0)
                    {
                        Node[i].p_on = 0;
                    }

                    // printf("目的地は(%d.%d)\n",Node[i].n_xD,Node[i].n_yD);
                    Node[i].p_num = -1;
                    Node[i].p_num2 = -1;
                    Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y); // 目的地との距離を入力
                    // printf("ABBBBBB\n");
                }
            }
        }

        // 変更の余地あり

        if (Node[i].stack_num > 0 && Node[i].p_on == 0 && i <= po)
        {
            int count2 = 0;
            do
            {
                count2 += 1;
                int numdata = peek(i);
                // stack_sort();
                if (search(i, numdata) == 0)
                { // 同じデータがなければ

                    int numdata_x = (numdata % (Ax * Ax)) / Ax;
                    int numdata_y = numdata % Ax;
                    if ((Twait * Ax * Ax - numdata > reftime * Ax * Ax) || (numdata_x - Node[i].n_X > refdistance || Node[i].n_X - numdata_x > refdistance) || (numdata_y - Node[i].n_Y > refdistance || Node[i].n_Y - numdata_y > refdistance) || Node[i].Map[numdata_x][numdata_y].no_D == 1)
                    { // 長い時間と距離経過しているとき参照しない

                        pop(i);
                        stack_sort();
                        Node[i].p_on = 0;
                        // printf("現在時刻 %d %d番ノード: %ds前\n",Twait,i,(Twait*10000 - numdata)/10000);
                        // printf("本来目指す目的地(%d,%d)\n\n",Node[i].n_xD,Node[i].n_yD);

                        Node[i].n_xD = -1;
                        Node[i].n_yD = -1;

                        pushnopos(i, numdata);
                        continue;
                    }
                    else
                    {
                        pop(i);
                        stack_sort();
                        Node[i].p_on = 3;
                        Node[i].n_xD = (numdata % (Ax * Ax)) / Ax;
                        Node[i].n_yD = numdata % Ax;
                        Node[i].Map[Node[i].n_xD][Node[i].n_yD].no_D = 1;
                        count_p_on_3 += 1;
                    }

                    // count_ifgo += 1;
                    pushnopos(i, numdata);

                    count2 = 0;
                    break;
                }

                // if(search(i,Ax*Node[i].n_xD+Node[i].n_yD)>0){
                // printf("BBB\n");
                // }
                // 客が生成されるため
            } while (/*search(i,Ax*Node[i].n_xD+Node[i].n_yD)>0&&*/ count2 < Node[i].stack_num);

            if (Node[i].n_yD < 0)
            {
                Node[i].p_on = 0;
            }

            // printf("目的地は(%d.%d)\n",Node[i].n_xD,Node[i].n_yD);
            Node[i].p_num = -1;
            Node[i].p_num2 = -1;
            Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y); // 目的地との距離を入力
                                                                                              // printf("ABBBBBB\n");
        }
    }
}

/* --------------------------------------------------------- *
 *	関数名 : P_check										 *
 *  機能 : 到着した人数をカウントする関数					 *
 *  仮引数 : なし					 						 *
 * --------------------------------------------------------- */
int P_check()
{
    check = 0;
    check_1 = 0;
    check_2 = 0;
    check_3 = 0;
    check_4 = 0;
    for (int i = 0; i < P_ALL_NUM + p_add_count; i++)
    {
        if (Pass[i].p_Exist == 0)
        {
            check += 1;
            if (Pass[i].p_X == d[0][0] && Pass[i].p_Y == d[0][1])
            {
                check_1 += 1;
            }
            if (Pass[i].p_X == d[1][0] && Pass[i].p_Y == d[1][1])
            {
                check_2 += 1;
            }
            if (Pass[i].p_X == d[2][0] && Pass[i].p_Y == d[2][1])
            {
                check_3 += 1;
            }
            if (Pass[i].p_X == d[3][0] && Pass[i].p_Y == d[3][1])
            {
                check_4 += 1;
            }
        }
    }
    printf("到着人数 %d\n", check);
    return check;
}

/* --------------------------------------------------------- *
 *	関数名 : flooding										 *
 *  機能 : フラッディングする関数							 *
 *  仮引数 : なし					 						 *
 * --------------------------------------------------------- */
void flooding()
{
    int i;
    int flooding_end;

    // 無限ループ
    while (TRUE)
    {
        flooding_end = TRUE;

        // 全てのノードを辿る
        for (i = 0; i < N_ALL_NUM; i++)
        {
            // iNodeが情報を持っていなかったら、continue
            if (Node[i].stack_num < 1)
                continue;

            // 1回でも情報を送信したら、フラッディングを終了しない
            // printf("%d\n",h);
            if (transmit(i) > 0)
                flooding_end = FALSE;
        }
        if (flooding_end == TRUE)
            break;
    }
}

/* --------------------------------------------------------- *
 *	関数名 : transmit										 *
 *  機能 : 輸送した回数を返す関数							 *
 *  仮引数 : なし					 						 *
 * --------------------------------------------------------- */
int transmit(int k)
{ // kは送信側のノード番号

    int i;
    double x_a_iNode = Node[k].n_X; // 送信側Nodeの位置
    double y_a_iNode = Node[k].n_Y;
    double x_iNode; // iNodeの位置
    double y_iNode;

    int num_transmit = 0; // 送った回数

    // 全てのノードを辿る
    for (i = 0; i < N_ALL_NUM; i++)
    {

        // iNodeの位置
        x_iNode = Node[i].n_X;
        y_iNode = Node[i].n_Y;

        /* 直線上通信可能距離による通信 */
        if (search2(i, k, Twait) == 0)
        {
            if ((sqrt2(x_a_iNode - x_iNode, y_a_iNode - y_iNode) * Td <= r))
            {
                for (int l = 0; l < Ax; l++)
                {
                    for (int m = 0; m < Ay; m++)
                    {
                        //if(Node[i].Map[l][m].info == 1 && Node[k].Map[l][m].info == 1)
                        {
                            if (Node[i].Map[l][m].info_time < Node[k].Map[l][m].info_time)
                            {
                                Node[i].Map[l][m].info = Node[k].Map[l][m].info;
                                Node[i].Map[l][m].p_num = Node[k].Map[l][m].p_num;
                                Node[i].Map[l][m].info_time = Node[k].Map[l][m].info_time;
                            }
                        }
                        // else if (Node[i].Map[l][m].info == 0 && Node[k].Map[l][m].info == 1)
                        // {
                        //     if ((Node[i].Map[l][m].info_time < Node[k].Map[l][m].info_time))
                        //     {
                        //         Node[i].Map[l][m].info = Node[k].Map[l][m].info;
                        //         Node[i].Map[l][m].info_time = Node[k].Map[l][m].info_time;
                        //     }
                        // }
                        
                        
                    }
                }
                if ((Node[k].stack_data[0] > Node[i].stack_data[0]) && Node[k].stack_data[0] != 0 && (Node[k].n_X == Node[i].n_X || Node[k].n_Y == Node[i].n_Y))
                {
                    // printf("ノード%d番からノード%d番へ情報が送られました!!\n",k,i);
                    stack_sort();
                    int numdata = peek(k);
                    push(i, numdata);
                    push2(i, k, Twait);
                    stack_sort();
                    // printf("        %d == %d      \n\n",Node[i].stack_data[0],Node[k].stack_data[0]);
                    num_transmit++;
                    total_transmit += num_transmit;
                }
            }
        }

        // 分散
        // if((Node[k].n_X == (double)Node[k].n_insec_X) && (Node[k].n_Y == (double)Node[k].n_insec_Y)){
        if ((sqrt2(x_a_iNode - x_iNode, y_a_iNode - y_iNode) * Td <= r) && (Node[k].n_X == Node[i].n_X || Node[k].n_Y == Node[i].n_Y))
        { // rの範囲内

            if (Node[i].p_on == 3 && Node[k].p_on == 3 && Node[i].n_xD == Node[k].n_xD && Node[i].n_yD == Node[k].n_yD)
            { /// ノードが情報に従う同士で目的地が一致する場合 同じ　分散

                if (i > k)
                {
                    Node[k].p_on = 0;
                    Node[k].p_num = -1;
                    // printf("(%d,%d)に向かう%dを変更！\n",Node[k].n_xD,Node[k].n_yD,k);
                    Node[k].n_xD = -2;
                    Node[k].n_yD = -2;
                }
                else if (i < k)
                {
                    Node[i].p_on = 0;
                    Node[i].p_num = -1;
                    // printf("%d,%dに向かう%dを変更！\n",Node[k].n_xD,Node[k].n_yD,i);
                    Node[i].n_xD = -2;
                    Node[i].n_yD = -2;
                }
            }
        }
    }
    // 送った回数を返す
    return num_transmit;
}

/* ------------------------------------------------------------------------------------- *
 *	関数名 : detect_trans																 *
 *  機能 : ノードのいる交差点に待ち行列があるか判定し, ある場合はノードのスタックにpush	 *
 *         情報を取得する関数															 *
 *  仮引数 : なし					 													 *
 * ------------------------------------------------------------------------------------- */
void detect_trans()
{

    int data = 0;
    for (int i = 0; i < N_ALL_NUM; i++)
    {

        // ノードが交差点にいるとき---------------------------------------------------------------------------------
        if ((double)Node[i].n_insec_X == Node[i].n_X && (double)Node[i].n_insec_Y == Node[i].n_Y)
        {

            for (int j = 0; j < P_ALL_NUM; j++)
            {
                if (transmit__[i][j] == 0)
                {
                    // y座標が等しく, x座標が通信可能距離内であるとき
                    if (Node[i].n_Y == Pass[j].p_Y && (fabs(Node[i].n_X - Pass[j].p_X) * Td <= r))
                    {
                        if (Pass[j].p_Exist == 1)
                        {
                            Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info = 1;
                            Node[i].Map[Pass[j].p_X][Pass[j].p_Y].p_num = j;
                            Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info_time = Twait;
                            data = Ax * Ax * Twait + Ax * (Pass[j].p_X) + Pass[j].p_Y;
                            push(i, data);
                            get_info += 1; // 情報を取得したカウント
                            transmit__[i][j] = Twait;
                        }
                    }
                    // x座標が等しく, y座標が通信可能距離内であるとき
                    else if (Node[i].n_X == Pass[j].p_X && (fabs(Node[i].n_Y - Pass[j].p_Y) * Td <= r))
                    {
                        if (Pass[j].p_Exist == 1)
                        {
                            Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info = 1;
                            Node[i].Map[Pass[j].p_X][Pass[j].p_Y].p_num = j;
                            Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info_time = Twait;
                            data = Ax * Ax * Twait + Ax * (Pass[j].p_X) + Pass[j].p_Y;
                            push(i, data);
                            get_info += 1; // 情報を取得したカウント
                            transmit__[i][j] = Twait;
                        }
                    }
                }
            }
            // info==1 の座標について待ち客がいるか確認し、いなければ info=0 に更新
            for(int k=0;k<Ax;k++)
            {
                for(int l=0;l<Ay;l++)
                {
                    if(Node[i].Map[k][l].info == 1)
                    {
                        // y座標が等しく, x座標が通信可能距離内であるとき
                        if (Node[i].n_Y == l && (fabs(Node[i].n_X - k) * Td <= r))
                        {
                            // Trans で待ち客がいるか直接確認（p_numは使わない）
                            if(Trans[k][l].wp_Exist == 0){
                                Node[i].Map[k][l].info = 0;
                                Node[i].Map[k][l].p_num = -2;
                                Node[i].Map[k][l].info_time = Twait + 1; // info=0 を優先させるため +1
                            }
                        }
                        // x座標が等しく, y座標が通信可能距離内であるとき
                        else if(Node[i].n_X == k && (fabs(Node[i].n_Y - l) * Td <= r))
                        {
                            // Trans で待ち客がいるか直接確認（p_numは使わない）
                            if(Trans[k][l].wp_Exist == 0){
                                Node[i].Map[k][l].info = 0;
                                Node[i].Map[k][l].p_num = -2;
                                Node[i].Map[k][l].info_time = Twait + 1; // info=0 を優先させるため +1
                            }
                        }
                    }
                }
            }
        }
        else
        { // ノードが交差点でなく, 道路上にいるとき

            // x軸方向に移動を行っているとき
            if (ceil(Node[i].n_X) != floor(Node[i].n_X) || Node[i].n_Y == Node[i].n_insec_Y)
            {
                for (int j = 0; j < P_ALL_NUM; j++)
                {
                    if (transmit__[i][j] == 0)
                    {

                        // y座標が等しく, x座標が通信可能距離内であるとき
                        if (Node[i].n_Y == Pass[j].p_Y && (fabs(Node[i].n_X - Pass[j].p_X) * Td <= r))
                        {
                            if (Pass[j].p_Exist == 1)
                            {
                                Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info = 1;
                                Node[i].Map[Pass[j].p_X][Pass[j].p_Y].p_num = j;
                                Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info_time = Twait;
                                data = Ax * Ax * Twait + Ax * (Pass[j].p_X) + Pass[j].p_Y;
                                push(i, data);
                                get_info += 1; // 情報を取得したカウント
                                transmit__[i][j] = Twait;
                            }
                        }
                    }
                }
                
                // 道路上でも info==1 の更新チェック（x軸方向移動時）
                for(int k=0;k<Ax;k++)
                {
                    for(int l=0;l<Ay;l++)
                    {
                        if(Node[i].Map[k][l].info == 1)
                        {
                            // 同じY座標で通信可能距離内の場合
                            if ((int)Node[i].n_Y == l && (fabs(Node[i].n_X - k) * Td <= r))
                            {
                                // Trans で待ち客がいるか直接確認（p_numは使わない）
                                if (Trans[k][l].wp_Exist == 0)
                                {
                                    Node[i].Map[k][l].info = 0;
                                    Node[i].Map[k][l].p_num = -2;
                                    Node[i].Map[k][l].info_time = Twait + 1; // info=0 を優先させるため +1
                                }
                            }
                        }
                    }
                }
            }
                // y軸方向に移動を行っているとき
            
            else if (ceil(Node[i].n_Y) != floor(Node[i].n_Y) || Node[i].n_X == Node[i].n_insec_X)
            {
                for (int j = 0; j < P_ALL_NUM; j++)
                {
                    if (transmit__[i][j] == 0)
                    {

                        // x座標が等しく, y座標が通信可能距離内であるとき
                        if (Node[i].n_X == Pass[j].p_X && (fabs(Node[i].n_Y - Pass[j].p_Y) * Td <= r))
                        {
                            if (Pass[j].p_Exist == 1)
                            {
                                Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info = 1;
                                Node[i].Map[Pass[j].p_X][Pass[j].p_Y].p_num = j;
                                Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info_time = Twait;
                                data = Ax * Ax * Twait + Ax * (Pass[j].p_X) + Pass[j].p_Y;
                                push(i, data);
                                get_info += 1; // 情報を取得したカウント
                                transmit__[i][j] = Twait;
                            }
                        }
                    }
                }
                
                // 道路上でも info==1 の更新チェック（y軸方向移動時）
                for(int k=0;k<Ax;k++)
                {
                    for(int l=0;l<Ay;l++)
                    {
                        if(Node[i].Map[k][l].info == 1)
                        {
                            // 同じX座標で通信可能距離内の場合
                            if ((int)Node[i].n_X == k && (fabs(Node[i].n_Y - l) * Td <= r))
                            {
                                // Trans で待ち客がいるか直接確認（p_numは使わない）
                                if (Trans[k][l].wp_Exist == 0)
                                {
                                    Node[i].Map[k][l].info = 0;
                                    Node[i].Map[k][l].p_num = -2;
                                    Node[i].Map[k][l].info_time = Twait + 1; // info=0 を優先させるため +1
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// void detect_trans()
// {

//     int data = 0;
//     for (int i = 0; i < N_ALL_NUM; i++)
//     {

//         // ノードが交差点にいるとき---------------------------------------------------------------------------------
//         if ((double)Node[i].n_insec_X == Node[i].n_X && (double)Node[i].n_insec_Y == Node[i].n_Y)
//         {

//             for (int j = 0; j < P_ALL_NUM; j++)
//             {
//                 if (transmit__[i][j] == 0)
//                 {
//                     // y座標が等しく, x座標が通信可能距離内であるとき
//                     if (Node[i].n_Y == Pass[j].p_Y && (fabs(Node[i].n_X - Pass[j].p_X) * Td <= r))
//                     {
//                         if (Pass[j].p_Exist == 1)
//                         {
//                             Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info = 1;
//                             Node[i].Map[Pass[j].p_X][Pass[j].p_Y].p_num = j;
//                             Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info_time = Twait;
//                             data = Ax * Ax * Twait + Ax * (Pass[j].p_X) + Pass[j].p_Y;
//                             push(i, data);
//                             get_info += 1; // 情報を取得したカウント
//                             transmit__[i][j] = Twait;
//                         }
//                     }
//                     // x座標が等しく, y座標が通信可能距離内であるとき
//                     else if (Node[i].n_X == Pass[j].p_X && (fabs(Node[i].n_Y - Pass[j].p_Y) * Td <= r))
//                     {
//                         if (Pass[j].p_Exist == 1)
//                         {
//                             Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info = 1;
//                             Node[i].Map[Pass[j].p_X][Pass[j].p_Y].p_num = j;
//                             Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info_time = Twait;
//                             data = Ax * Ax * Twait + Ax * (Pass[j].p_X) + Pass[j].p_Y;
//                             push(i, data);
//                             get_info += 1; // 情報を取得したカウント
//                             transmit__[i][j] = Twait;
//                         }
//                     }
//                 }
//             }
//             for(int k=0;k<Ax;k++)
//             {
//                 for(int l=0;l<Ay;l++)
//                 {
//                     if(Node[i].Map[k][l].info == 1)
//                     {
//                         if (Node[i].n_Y == Node[i].Map[k][l].m_Y && (fabs(Node[i].n_X - Node[i].Map[k][l].m_X) * Td <= r))
//                         {
//                             // int map_flag = 0;
//                             // for(int m = 0; m < P_ALL_NUM; m++){
                                
//                             //     if(Node[i].Map[k][l].m_X == Pass[m].p_X && Node[i].Map[k][l].m_Y == Pass[m].p_Y){
//                             //         map_flag = 1;
//                             //     }
//                             // }
//                             // if(map_flag == 0){
//                             //     Node[i].Map[k][l].info = 0;
//                             //     Node[i].Map[k][l].p_num = -2;
//                             //     Node[i].Map[k][l].info_time = Twait;
//                             // }
//                             if(Trans[Node[i].Map[k][l].m_X][Node[i].Map[k][l].m_Y].wp_Exist == 0){
//                                 Node[i].Map[k][l].info = 0;
//                                 Node[i].Map[k][l].p_num = -2;
//                                 Node[i].Map[k][l].info_time = Twait;

//                             }
//                         }
//                         else if(Node[i].n_X == Node[i].Map[k][l].m_X && (fabs(Node[i].n_Y - Node[i].Map[k][l].m_Y) * Td <= r))
//                         {
//                             // int map_flag = 0;
//                             // for(int m = 0; m < P_ALL_NUM; m++){
                                
//                             //     if(Node[i].Map[k][l].m_X == Pass[m].p_X && Node[i].Map[k][l].m_Y == Pass[m].p_Y){
//                             //         map_flag = 1;
//                             //     }
//                             // }
//                             // if(map_flag == 0){
//                             //     Node[i].Map[k][l].info = 0;
//                             //     Node[i].Map[k][l].p_num = -2;
//                             //     Node[i].Map[k][l].info_time = Twait;
//                             // }
//                             if(Trans[Node[i].Map[k][l].m_X][Node[i].Map[k][l].m_Y].wp_Exist == 0){
//                                 Node[i].Map[k][l].info = 0;
//                                 Node[i].Map[k][l].p_num = -2;
//                                 Node[i].Map[k][l].info_time = Twait;

//                             }
//                         }
//                     }
//                     //直前でinfoは1になっているはずだから、infoが0の場合はいらない
//                         // else if(Node[i].Map[k][l].info == 0)
//                         // {
//                         //     Node[i].Map[k][l].no_D = 0;
//                         // }
//                 }
//             }
//         }
//         else
//         { // ノードが交差点でなく, 道路上にいるとき
//             for (int j = 0; j < P_ALL_NUM; j++){
//                 // x軸方向に移動を行っているとき
//                 if (ceil(Node[i].n_X) != floor(Node[i].n_X) || Node[i].n_Y == Node[i].n_insec_Y)
//                 {
                    
//                     if (transmit__[i][j] == 0)
//                     {

//                         // y座標が等しく, x座標が通信可能距離内であるとき
//                         if (Node[i].n_Y == Pass[j].p_Y && (fabs(Node[i].n_X - Pass[j].p_X) * Td <= r))
//                         {
//                             if (Pass[j].p_Exist == 1)
//                             {
//                                 Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info = 1;
//                                 Node[i].Map[Pass[j].p_X][Pass[j].p_Y].p_num = j;
//                                 Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info_time = Twait;
//                                 data = Ax * Ax * Twait + Ax * (Pass[j].p_X) + Pass[j].p_Y;
//                                 push(i, data);
//                                 get_info += 1; // 情報を取得したカウント
//                                 transmit__[i][j] = Twait;
//                             }
//                         }
//                     }
//                 }
//                     // y軸方向に移動を行っているとき
                
//                 else if (ceil(Node[i].n_Y) != floor(Node[i].n_Y) || Node[i].n_X == Node[i].n_insec_X)
//                 {
                   
                    
//                     if (transmit__[i][j] == 0)
//                     {

//                         // x座標が等しく, y座標が通信可能距離内であるとき
//                         if (Node[i].n_X == Pass[j].p_X && (fabs(Node[i].n_Y - Pass[j].p_Y) * Td <= r))
//                         {
//                             if (Pass[j].p_Exist == 1)
//                             {
//                                 Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info = 1;
//                                 Node[i].Map[Pass[j].p_X][Pass[j].p_Y].p_num = j;
//                                 Node[i].Map[Pass[j].p_X][Pass[j].p_Y].info_time = Twait;
//                                 data = Ax * Ax * Twait + Ax * (Pass[j].p_X) + Pass[j].p_Y;
//                                 push(i, data);
//                                 get_info += 1; // 情報を取得したカウント
//                                 transmit__[i][j] = Twait;
//                             }
//                         }
//                     }
                    
//                 }
//                 for(int k=0;k<Ax;k++)
//                 {
//                     for(int l=0;l<Ay;l++)
//                     {
//                         if(Node[i].Map[k][l].info == 1)
//                         {
//                             // x軸方向に移動を行っているとき
//                             if (ceil(Node[i].n_X) != floor(Node[i].n_X) || Node[i].n_Y == Node[i].n_insec_Y)
//                             {
//                                 // y座標が等しく, x座標が通信可能距離内であるとき  

//                                 if (Node[i].n_Y == Node[i].Map[k][l].m_Y && (fabs(Node[i].n_X - Node[i].Map[k][l].m_X) * Td <= r))
//                                 {
//                                     // int map_flag = 0;
//                                     // for(int m = 0; m < P_ALL_NUM; m++){
                                        
//                                     //     if(Node[i].Map[k][l].m_X == Pass[m].p_X && Node[i].Map[k][l].m_Y == Pass[m].p_Y){
//                                     //         map_flag = 1;
//                                     //     }
//                                     // }
//                                     // if(map_flag == 0){
//                                     //     Node[i].Map[k][l].info = 0;
//                                     //     Node[i].Map[k][l].p_num = -2;
//                                     //     Node[i].Map[k][l].info_time = Twait;
//                                     // }
//                                     if(Trans[Node[i].Map[k][l].m_X][Node[i].Map[k][l].m_Y].wp_Exist == 0){
//                                         Node[i].Map[k][l].info = 0;
//                                         Node[i].Map[k][l].p_num = -2;
//                                         Node[i].Map[k][l].info_time = Twait;

//                                     }
//                                 }
//                             }
//                             else if(ceil(Node[i].n_Y) != floor(Node[i].n_Y) || Node[i].n_X == Node[i].n_insec_X){

//                                 if(Node[i].n_X == Node[i].Map[k][l].m_X && (fabs(Node[i].n_Y - Node[i].Map[k][l].m_Y) * Td <= r))
//                                 {
//                                     // int map_flag = 0;
//                                     // for(int m = 0; m < P_ALL_NUM; m++){
                                        
//                                     //     if(Node[i].Map[k][l].m_X == Pass[m].p_X && Node[i].Map[k][l].m_Y == Pass[m].p_Y){
//                                     //         map_flag = 1;
//                                     //     }
//                                     // }
//                                     // if(map_flag == 0){
//                                     //     Node[i].Map[k][l].info = 0;
//                                     //     Node[i].Map[k][l].p_num = -2;
//                                     //     Node[i].Map[k][l].info_time = Twait;
//                                     // }
//                                     if(Trans[Node[i].Map[k][l].m_X][Node[i].Map[k][l].m_Y].wp_Exist == 0){
//                                         Node[i].Map[k][l].info = 0;
//                                         Node[i].Map[k][l].p_num = -2;
//                                         Node[i].Map[k][l].info_time = Twait;

//                                     }
//                                 }
//                             }
//                         }
//                         //直前でinfoは1になっているはずだから、infoが0の場合はいらない
//                             // else if(Node[i].Map[k][l].info == 0)
//                             // {
//                             //     Node[i].Map[k][l].no_D = 0;
//                             // }
//                     }
//                 }
//             }
//         }
//     }
// }


/* --------------------------------------------------------- *
 *	関数名 : syokika										 *
 *  機能 : ノードの保持データを初期化する					 *
 *  仮引数 : なし					 						 *
 * --------------------------------------------------------- */
int syokika()
{
    for (int i = 0; i < ssize; i++)
    {
        stack_pop[i] = 0;
    }
    for(int i = 0; i<10000; i++){
        Node0_score[i] = 0;
        Node0_dens[i] = 0;
        Node0_dist[i] = 0;
        Node0_W_grid[i] = 0;
    }
    for (int count = 0; count < N_ALL_NUM; count++)
    {
        for (int i = 0; i < ssize; i++)
        {
            Node[count].stack_data[i] = 0;
            Node[count].stack_nopos[i] = 0;
            for(int j = 0; j < 2; j++){
                Node[count].stack_info[i][j] = 0;
            }
        }
        for (int j = 0; j < P_ALL_NUM; j++)
        {
            transmit__[count][j] = 0;
        }
        for(int i = 0; i < grid_size; i++){
            for(int j = 0; j < grid_size; j++){
                Node[count].W_grid[i][j] = 0.0;
                Node[count].Map_grid[i][j].W_map = 0;
            }
        }
        Node[count].prog_d_surp = 0;
        Node[count].stack_num = 0;
        Node[count].stack_num2 = 0;
        Node[count].stack_num3 = 0;
        Node[count].all_stack_num = 0;
        Node[count].move_flag = 0;
        Node[count].area_number = 0;
	    Node[count].move_flag =  0;
        Node[count].d_length = 0;  // 乗客１の宛先地点までの距離　乗客１の目的地がノード自体の目的地となる
	    Node[count].d_length2 = 0;
        Node[count].move_pattern = 0;
        Node[count].target_grid_x = 0; // 目標グリッドのX座標
	    Node[count].target_grid_y = 0;
        distination_flag[count] = 0;
        for (int i = 0; i < Ax; i++)
        {
            for (int j = 0; j < Ay; j++)
            {
                Node[count].Map[i][j].info = 0;
                Node[count].Map[i][j].p_num = -1;
                Node[count].Map[i][j].no_D = 0;
                Node[count].Map[i][j].info_time = 0;
                Node[count].Map[i][j].m_X = i;
                Node[count].Map[i][j].m_Y = j;
            }
        }
    }
    for (int i = 0; i < Ax; i++)
    {
        for (int j = 0; j < Ay; j++)
        {
            queue_num[i][j] = 0;
            queue_head[i][j] = 0;
            Trans[i][j].wp_Exist = 0;
        }
    }
    o = 0;
    pop_num = 0;
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 20; j++){
            smoothed_count[i][j] = 0;
        }
    }
    return 0;
}

/* --------------------------------------------------------- *
 *	関数名 : check_ride										 *
 *  機能 : 乗客の車待ち状況を調べる							 *
 *  仮引数 : なし					 						 *
 * --------------------------------------------------------- */
int check_ride()
{

    for (int y = 0; y < P_ALL_NUM + p_add_count; y++)
    {
        if (Pass[y].p_Exist == 1 && Pass[y].p_ride == 1)
        {
            ridecount++;
        }
        else if (Pass[y].p_Exist == 1 && Pass[y].p_ride == 0)
        {
            waitcount++;
        }
    }
    return 0;
}

/* --------------------------------------------------------- *
 *	関数名 : P_map											 *
 *  機能 : メッシュ法（グリッド法）で密集地点を検出する	 *
 *  仮引数 : node_index - 対象となるノードの番号			 *
 *          threshold - 密集と判定する閾値（人数）			 *
 * --------------------------------------------------------- */
void P_map(int node_index)
{
    // グリッドサイズとセルサイズはグローバル変数として定義済み
    
    // グリッドのカウント配列を初期化
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            grid_count[i][j] = 0;
        }
    }
    
    // Node[node_index]のMapデータを集計
    for (int l = 0; l < Ax; l++) {
        for (int m = 0; m < Ay; m++) {
            if (Node[node_index].Map[l][m].info == 1) {
                // 座標(l, m)がどのグリッドセルに属するか計算
                int grid_x = l / cell_width;
                int grid_y = m / cell_height;
                
                // 範囲外チェック（念のため）
                if (grid_x >= grid_size) grid_x = grid_size - 1;
                if (grid_y >= grid_size) grid_y = grid_size - 1;
                
                // カウントを増やす
                grid_count[grid_x][grid_y]++;
            }
        }
    }
    
    // 平滑化処理：隣接セルも考慮して密集地点を判定
    // smoothed_count配列はグローバル変数として定義済み
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            int total = grid_count[i][j];
            
            // 8方向の隣接セルを加算
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    if (di == 0 && dj == 0) continue; // 自分自身は既に加算済み
                    
                    int ni = i + di;
                    int nj = j + dj;
                    
                    // 範囲内チェック
                    if (ni >= 0 && ni < grid_size && nj >= 0 && nj < grid_size) {
                        total += grid_count[ni][nj];
                    }
                }
            }
            
            smoothed_count[i][j] = total;
        }
    }
    
    // 密集地点の判定と出力（デバッグ用）
    // printf("Node[%d] 密集地点検出結果（閾値: %d人）:\n", node_index, threshold);
    // for (int i = 0; i < grid_size; i++) {
    //     for (int j = 0; j < grid_size; j++) {
    //         if (smoothed_count[i][j] >= threshold) {
    //             printf("  グリッド[%d][%d]: %d人 → 密集地点！\n", 
    //                    i, j, smoothed_count[i][j]);
                
    //             // 密集地点の中心座標を計算
    //             int center_x_local = (i * cell_width) + (cell_width / 2);
    //             int center_y_local = (j * cell_height) + (cell_height / 2);
    //             printf("    中心座標: (%d, %d)\n", center_x_local, center_y_local);
    //         }
    //     }
    // }
}

/* -------------------------------------------------------------------------------------------- *
 *	関数名 : calculate_direction_score														    *
 *  機能 : 指定された方向への移動スコアを計算する												*
 *  仮引数 : node_index - ノードの番号															*
 *          target_x - 移動先のX座標															    *
 *          target_y - 移動先のY座標															    *
 *  戻り値 : スコア値（高いほど優先度が高い）													*
 * -------------------------------------------------------------------------------------------- */
double calculate_direction_score(int node_index, int target_x, int target_y)
{
    double score = 0.0;
    
    // グリッドサイズとセルサイズはグローバル変数として定義済み
    
    // 目的地座標がどのグリッドセルに属するか計算
    int grid_x = target_x / cell_width;
    int grid_y = target_y / cell_height;
    
    // 範囲外チェック
    if (grid_x < 0) grid_x = 0;
    if (grid_x >= grid_size) grid_x = grid_size - 1;
    if (grid_y < 0) grid_y = 0;
    if (grid_y >= grid_size) grid_y = grid_size - 1;
    
    // その場所の混雑度を取得
    double density = (double)smoothed_count[grid_x][grid_y];
    
    // Node[node_index]からの距離を計算
    double distance = sqrt2(target_x - Node[node_index].n_X, target_y - Node[node_index].n_Y);
    
    // --- 正規化処理 ---
    // density: 0〜50の範囲を想定（平滑化後の現実的な最大値）
    // distance: 0〜マップ対角線の長さ（sqrt(Ax^2 + Ay^2) ≈ 84.85）
    double density_max = 50.0;  // 密度の推定最大値
    //double distance_max = sqrt(Ax * Ax + Ay * Ay);  // マップの対角線距離（約84.85）
    double distance_max = 30.0;
    
    // Min-Max正規化（0〜1の範囲にスケーリング）
    double density_normalized = density / density_max;
    double distance_normalized = distance / distance_max;
    
    // // 正規化値が1を超えないようにクリップ
    // if (density_normalized > 1.0) density_normalized = 1.0;
    // if (distance_normalized > 1.0) distance_normalized = 1.0;
    
    // スコア = (W_dens × 正規化密度) + W_map - (W_dist × 正規化距離) - W_grid
    //score = (W_dens * density_normalized) + (Node[node_index].Map_grid[grid_x][grid_y].W_map) - (W_dist * distance_normalized) - (Node[node_index].W_grid[grid_x][grid_y]);

    // // --- 割り算ベースのスコア計算式 ---
    // // スコア = (W_dens × 混雑度) / ((W_dist × 距離) + 訪問回数 + 1.0)
    // score = (W_dens * density_normalized) / ((W_dist * distance_normalized) + Node[node_index].W_grid[grid_x][grid_y] + 1.0);
    // // W_mapを加算して避難エリア内を優先
    // score += Node[node_index].Map_grid[grid_x][grid_y].W_map;

    // --- exp関数を使った割り算ベースのスコア計算式 ---
    // スコア = 正規化混雑度 / (exp(W_dist × 正規化距離) + 訪問回数)
    score = (W_dens * density_normalized) / (exp(W_dist * distance_normalized) + Node[node_index].W_grid[grid_x][grid_y] + 1.0);
    // W_mapを加算して避難エリア内を優先
    score += Node[node_index].Map_grid[grid_x][grid_y].W_map;


    // // --- 対数変換正規化 ---
    // double log_dens = log(density + 1.0);
    // double log_dist = log(distance + 1.0);

    // double log_dens_max = log(density_max + 1.0);
    // double log_dist_max = log(distance_max + 1.0);

    // double log_density_normalized = log_dens / log_dens_max;
    // double log_distance_normalized = log_dist / log_dist_max;

    // 正規化値が1を超えないようにクリップ
    //if (log_density_normalized > 1.0) log_density_normalized = 1.0;
    //if (log_distance_normalized > 1.0) log_distance_normalized = 1.0;
    
    // // スコア = (W_dens × 正規化密度) + W_map - (W_dist × 正規化距離) - W_grid
    // score = (W_dens * log_density_normalized) + (Node[node_index].Map_grid[grid_x][grid_y].W_map) - (W_dist * log_distance_normalized) - (Node[node_index].W_grid[grid_x][grid_y]);

    // // --- 割り算ベースのスコア計算式 ---
    // // スコア = (W_dens × 混雑度) / ((W_dist × 距離) + 訪問回数 + 1.0)
    // score = (W_dens * log_density_normalized) / ((W_dist * log_distance_normalized) + Node[node_index].W_grid[grid_x][grid_y] + 1.0);
    // // W_mapを加算して避難エリア内を優先
    // score += Node[node_index].Map_grid[grid_x][grid_y].W_map;
        
    return score;
}

/* -------------------------------------------------------------------------------------------- *
 *	関数名 : find_best_grid_in_all_map																		    *
 *  機能 : 全マップで最もスコアが高いグリッドを見つける												*
 *  仮引数 : node_index - ノードの番号																*
 *  戻り値 : なし（Node[node_index].target_grid_x/yに結果を格納）								*
 * -------------------------------------------------------------------------------------------- */
void find_best_grid_in_all_map(int node_index)
{
    double best_score = -99999999.0;
    int best_grid_x = -1;
    int best_grid_y = -1;
    
    // 全グリッドをスキャンしてスコアを計算
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            // グリッドの中心座標を計算
            int center_x_local = (i * cell_width) + (1);
            int center_y_local = (j * cell_height) + (1);
            
            // このグリッドのスコアを計算
            double score = calculate_direction_score(node_index, center_x_local, center_y_local);
            
            // 最高スコアを更新
            if (score > best_score) {
                best_score = score;
                best_grid_x = center_x_local;
                best_grid_y = center_y_local;
            }
        }
    }
    
    if(node_index == 0){    
        // その場所の混雑度を取得
        double density = (double)smoothed_count[best_grid_x/cell_width][best_grid_y/cell_height];
        
        // Node[node_index]からの距離を計算
        double distance = sqrt2(best_grid_x - Node[node_index].n_X, best_grid_y - Node[node_index].n_Y);
        
        // --- 正規化処理 ---
        // density: 0〜50の範囲を想定（平滑化後の現実的な最大値）
        // distance: 0〜マップ対角線の長さ（sqrt(Ax^2 + Ay^2) ≈ 84.85）
        double density_max = 50.0;  // 密度の推定最大値
        double distance_max = sqrt(Ax * Ax + Ay * Ay);  // マップの対角線距離（約84.85）
        
        // Min-Max正規化（0〜1の範囲にスケーリング）
        double density_normalized = density / density_max;
        double distance_normalized = distance / distance_max;
        
        // 正規化値が1を超えないようにクリップ
        if (density_normalized > 1.0) density_normalized = 1.0;
        if (distance_normalized > 1.0) distance_normalized = 1.0;

        Node0_score[Twait] = best_score;
        Node0_dens[Twait] = density;
        Node0_dist[Twait] = distance;
        Node0_W_grid[Twait] = Node[node_index].W_grid[best_grid_x][best_grid_y];



        // // --- 対数変換正規化 ---
        // double log_dens = log(density + 1.0);
        // double log_dist = log(distance + 1.0);

        // double log_dens_max = log(density_max + 1.0);
        // double log_dist_max = log(distance_max + 1.0);

        // double log_density_normalized = log_dens / log_dens_max;
        // double log_distance_normalized = log_dist / log_dist_max;

        // // 正規化値が1を超えないようにクリップ
        // if (log_density_normalized > 1.0) log_density_normalized = 1.0;
        // if (log_distance_normalized > 1.0) log_distance_normalized = 1.0;
    
        // Node0_score[Twait] = best_score;
        // Node0_dens[Twait] = log_density_normalized;
        // Node0_dist[Twait] = log_distance_normalized;
        // Node0_W_grid[Twait] = Node[node_index].W_grid[best_grid_x][best_grid_y];
    }
    
    // 結果をノード構造体に格納
    Node[node_index].target_grid_x = best_grid_x;
    Node[node_index].target_grid_y = best_grid_y;
    
    
    // デバッグ出力（オプション）
    // printf("Node[%d] 最高スコアグリッド: (%d, %d) スコア: %.2f\n", 
    //        node_index, best_grid_x, best_grid_y, best_score);
}

/* -------------------------------------------------------------------------------------------- *
 *	関数名 : re_find_best_grid_in_all_map																		    *
 *  機能 : 全マップで最もスコアが高いグリッドを見つける												*
 *  仮引数 : node_index - ノードの番号																*
 *  戻り値 : なし（Node[node_index].target_grid_x/yに結果を格納）								*
 * -------------------------------------------------------------------------------------------- */
void re_find_best_grid_in_all_map(int node_index)
{
    double best_score = -99999999.0;
    int best_grid_x = -1;
    int best_grid_y = -1;
    
    int now_grid_x = Node[node_index].n_X / cell_width;
    int now_grid_y = Node[node_index].n_Y / cell_height;
    // 全グリッドをスキャンしてスコアを計算
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            // グリッドの中心座標を計算
            int center_x_local = (i * cell_width) + (1);
            int center_y_local = (j * cell_height) + (1);
            
            // このグリッドのスコアを計算
            double score = calculate_direction_score(node_index, center_x_local, center_y_local);
            if(now_grid_x == i && now_grid_y == j){
                score -= 10000.0; // 今いるグリッドは避ける
            }
            
            // 最高スコアを更新
            if (score > best_score) {
                best_score = score;
                best_grid_x = center_x_local;
                best_grid_y = center_y_local;
            }
        }
    }
    if(Node[node_index].W_grid[best_grid_x][best_grid_y] > 0){
        sum_W_grid += 1.0;
    }
    
    if(node_index == 0){    
        // その場所の混雑度を取得
        double density = (double)smoothed_count[best_grid_x][best_grid_y];
        
        // Node[node_index]からの距離を計算
        double distance = sqrt2(best_grid_x - Node[node_index].n_X, best_grid_y - Node[node_index].n_Y);
        
        // --- 正規化処理 ---
        // density: 0〜50の範囲を想定（平滑化後の現実的な最大値）
        // distance: 0〜マップ対角線の長さ（sqrt(Ax^2 + Ay^2) ≈ 84.85）
        double density_max = 50.0;  // 密度の推定最大値
        double distance_max = sqrt(Ax * Ax + Ay * Ay);  // マップの対角線距離（約84.85）
        
        // Min-Max正規化（0〜1の範囲にスケーリング）
        double density_normalized = density / density_max;
        double distance_normalized = distance / distance_max;
        
        // 正規化値が1を超えないようにクリップ
        if (density_normalized > 1.0) density_normalized = 1.0;
        if (distance_normalized > 1.0) distance_normalized = 1.0;
    
        Node0_score[Twait] = best_score;
        Node0_dens[Twait] = density_normalized;
        Node0_dist[Twait] = distance_normalized;
        Node0_W_grid[Twait] = Node[node_index].W_grid[best_grid_x][best_grid_y];
    }
    
    // 結果をノード構造体に格納
    Node[node_index].target_grid_x = best_grid_x;
    Node[node_index].target_grid_y = best_grid_y;
    
    
    // デバッグ出力（オプション）
    // printf("Node[%d] 最高スコアグリッド: (%d, %d) スコア: %.2f\n", 
    //        node_index, best_grid_x, best_grid_y, best_score);


    
}

