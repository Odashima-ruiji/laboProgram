/*
    my_func.c : カスタム関数の実装
*/

#include "config.h"
#include "base_struct.h"
#include "globals.h"
#include "base_func.h"
#include "Common_func.h"
#include "move.h"
#include "my_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void Dist_Init_n_D()
{
    for (int i = 0; i < N_ALL_NUM; i++)
    {

        if (Node[i].move_flag == 0)
        {
            int dif = 0;
            if (Node[i].n_X < center_x && Node[i].n_Y <= center_y)
            {
                Node[i].n_xD = center_x - dif;
                Node[i].n_yD = center_y - dif;
                Node[i].p_on = 5;
                Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
            }
            else if (Node[i].n_X <= center_x && Node[i].n_Y > center_y)
            {
                Node[i].n_xD = center_x - dif;
                Node[i].n_yD = center_y + dif;
                Node[i].p_on = 5;
                Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
            }
            else if (Node[i].n_X > center_x && Node[i].n_Y >= center_y)
            {
                Node[i].n_xD = center_x + dif;
                Node[i].n_yD = center_y + dif;
                Node[i].p_on = 5;
                Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
            }
            else if (Node[i].n_X >= center_x && Node[i].n_Y < center_y)
            {
                Node[i].n_xD = center_x + dif;
                Node[i].n_yD = center_y - dif;
                Node[i].p_on = 5;
                Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
            }
        }
    }
}

/* ------------------------------------------------------------------------------------ *
 *	関数名 : initial_move										 						*
 *  機能 :　シミュレーション開始時、乗客を乗せずにマップの中心に向かう。			　　*
 *  仮引数 : なし											 							*
 * ------------------------------------------------------------------------------------ */
void initial_move()
{
    for (int i = 0; i < N_ALL_NUM; i++)
    {
        if (Node[i].move_flag == 0)
        {
            Node[i].n_xD = center_x;
            Node[i].n_yD = center_y;
            Node[i].p_on = 5;
            Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y);
        }
    }
    for (int i = 0; i < N_ALL_NUM; i++)
    {
        if (Node[i].move_flag == 0)
        {
            // すべてのノードの座標を更新
            move_update_coord();

            initial_D_check(); // 乗客の目的地判定

            // 利用要求を受ける
            initial_detect_trans();

            // 交差点にいるとき, 次に向かう交差点を指定
            move_new_direction();

            flooding();

            // 余剰進行距離を座標に加算
            move_update_coord_surplus();
        }
    }
}

void initial_D_check()
{
    for (int i = 0; i < N_ALL_NUM; i++)
    {
        if (Node[i].p_on == 5)
        {
            // 移動後、中心に到着したかの確認と、目的とする乗客の設定
            if (Node[i].n_X == Node[i].n_xD && Node[i].n_Y == Node[i].n_yD)
            {
                Node[i].move_flag = 1;
                Node[i].p_on = 0; // ノードの乗車情報を変更

                if (Node[i].stack_num > 0 && Node[i].p_on == 0 && i < po)
                {
                    int count = 0;
                    do
                    {
                        count += 1;
                        int numdata = pop(i);
                        stack_sort();
                        if (search(i, numdata) == 0)
                        { // 同じデータがなければ

                            Node[i].p_on = 3;
                            Node[i].n_xD = (numdata % (Ax * Ax)) / Ax;
                            Node[i].n_yD = numdata % Ax;
                            count_p_on_3 += 1;
                            if ((Twait * Ax * Ax - numdata > reftime * Ax * Ax) || (Node[i].n_xD - Node[i].n_X > refdistance || Node[i].n_X - Node[i].n_xD > refdistance) || (Node[i].n_yD - Node[i].n_Y > refdistance || Node[i].n_Y - Node[i].n_yD > refdistance))
                            { // 長い時間と距離経過しているとき参照しない

                                Node[i].p_on = 0;
                                Node[i].n_xD = -1;
                                Node[i].n_yD = -1;
                                count_p_on_3 -= 1;
                                pushnopos(i, numdata);
                                continue;
                            }
                            pushnopos(i, numdata);

                            count = 0;
                            break;
                        }

                    } while (count < Node[i].stack_num + 1);

                    if (Node[i].n_yD < 0)
                    {
                        Node[i].p_on = 0;
                    }

                    Node[i].p_num = -1;
                    Node[i].p_num2 = -1;
                    Node[i].d_length = sqrt2(Node[i].n_xD - Node[i].n_X, Node[i].n_yD - Node[i].n_Y); // 目的地との距離を入力
                }
            }
        }
    }
}

void initial_detect_trans()
{
    int data = 0;
    for (int i = 0; i < N_ALL_NUM; i++)
    {

        // ノードが交差点にいるとき
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
                            data = Ax * Ax * Twait + Ax * (Pass[j].p_X) + Pass[j].p_Y;
                            push(i, data);
                            get_info += 1; // 情報を取得したカウント
                            transmit__[i][j] = Twait;
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
                                data = Ax * Ax * Twait + Ax * (Pass[j].p_X) + Pass[j].p_Y;
                                push(i, data);
                                get_info += 1; // 情報を取得したカウント
                                transmit__[i][j] = Twait;
                            }
                        }
                    }
                }

                // y軸方向に移動を行っているとき
            }
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
                                data = Ax * Ax * Twait + Ax * (Pass[j].p_X) + Pass[j].p_Y;
                                push(i, data);
                                get_info += 1; // 情報を取得したカウント
                                transmit__[i][j] = Twait;
                            }
                        }
                    }
                }
            }
        }
    }
}
