/*
    move.c : ノード移動関数の実装
*/

#include "config.h"
#include "base_struct.h"
#include "globals.h"
#include "base_func.h"
#include "Common_func.h"
#include "move.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* move.c内部で使用する変数 */
static int last_direction[2][4];
static int traffic_flag = 0;
static double d_now_length;
static double d_now_length2;
static double n_error; // 座標エラー判定用

/* -------------------------------------------------------------------------------------------- *
 *	関数名 : Move																				*
 *  機能 : ノードを動かす関数																	*
 *  仮引数 : なし													 						    *
 * -------------------------------------------------------------------------------------------- */
void Move()
{
    // printf("move!!!\n");

    // すべてのノードの座標を更新
    move_update_coord();
    D_check(); // 乗客の目的地判定

    // 利用要求を受ける
    // #ifdef EPIDEMIC
    detect_trans();
    // #endif

    allrideon(); // 乗客を乗せる

    // 交差点にいるとき, 次に向かう交差点を指定
    move_new_direction();
// ノードが交差点上にいる状態でフラッディング
#ifdef EPIDEMIC
    flooding();
#endif
    // 余剰進行距離を座標に加算
    move_update_coord_surplus();
    // 利用要求を受ける
    // #ifdef EPIDEMIC
    detect_trans();
}

/* -------------------------------------------------------------------------------------------- *
 *	関数名 : move_update_coord																	*
 *  機能 : 座標を更新する関数																	*
 *  仮引数 : なし													 						    *
 * -------------------------------------------------------------------------------------------- */
void move_update_coord()
{
    int count = 0;
    int free_counter = 0;
    while (count < N_ALL_NUM)
    {
        Node[count].prog_d_surp = 0;

        // x軸方向に移動を行っているとき
        if (ceil(Node[count].n_X) != floor(Node[count].n_X) || Node[count].n_Y == Node[count].n_insec_Y)
        {

            /*----------------------------------*/
            // 右方向の移動
            if ((double)Node[count].n_insec_X - Node[count].n_X > 0.0000)
            {
                // 座標を更新
                n_error = Node[count].n_X;
                if (Node[count].p_on == 2)
                {
                    if (Pass[Node[count].p_num].p_xD != Pass[Node[count].p_num2].p_xD || Pass[Node[count].p_num].p_yD != Pass[Node[count].p_num2].p_yD)
                    {
                        if (sqrt2(Node[count].n_X - center_x, Node[count].n_Y - center_y) <= area_disaster)
                        {
                            if (sqrt2((Node[count].n_X + ((Node[count].v / 3600) * 1000) / Td) - center_x, Node[count].n_Y - center_y) > area_disaster)
                            {

                                Pass[Node[count].p_num2].p_X = Node[count].n_X; // 乗客２の現在地を更新
                                Pass[Node[count].p_num2].p_Y = Node[count].n_Y;
                                Pass[Node[count].p_num2].p_ride = 0; // 乗客の乗車情報の変更

                                enqueue(Node[count].p_num2); // 更新した現在位置のキューに乗客２の情報を入力

                                push(count, Ax * Ax * Twait + Ax * Node[count].n_insec_X + Node[count].n_insec_Y); // ノードに待ち客情報をpush
                                get_info += 1;

                                Node[count].p_num2 = -1;
                                Node[count].n_xD2 = -1;
                                Node[count].n_yD2 = -1;

                                Node[count].p_on += -1;

                                count_off += 1;
                            }
                        }
                    }
                }

                Node[count].n_X += ((Node[count].v / 3600) * 1000) / Td;
                if (n_error == Node[count].n_X)
                {
                    printf("座標エラー : Node[%d]\n", count);
                    exit(1);
                }

                // 目的の座標を超えるときは交差点座標を参照
                if ((double)Node[count].n_insec_X - Node[count].n_X < 0.0000)
                {
                    Node[count].prog_d_surp = fabs((double)Node[count].n_insec_X - Node[count].n_X);
                    Node[count].n_X = (double)Node[count].n_insec_X;
                }
                // 左方向の移動
            }
            else
            {
                // 座標を更新
                n_error = Node[count].n_X;
                if (Node[count].p_on == 2)
                {
                    if (Pass[Node[count].p_num].p_xD != Pass[Node[count].p_num2].p_xD || Pass[Node[count].p_num].p_yD != Pass[Node[count].p_num2].p_yD)
                    {
                        if (sqrt2(Node[count].n_X - center_x, Node[count].n_Y - center_y) <= area_disaster)
                        {
                            if (sqrt2((Node[count].n_X - ((Node[count].v / 3600) * 1000) / Td) - center_x, Node[count].n_Y - center_y) > area_disaster)
                            {

                                Pass[Node[count].p_num2].p_X = Node[count].n_X; // 乗客２の現在地を更新
                                Pass[Node[count].p_num2].p_Y = Node[count].n_Y;
                                Pass[Node[count].p_num2].p_ride = 0; // 乗客の乗車情報の変更

                                enqueue(Node[count].p_num2); // 更新した現在位置のキューに乗客２の情報を入力

                                push(count, Ax * Ax * Twait + Ax * Node[count].n_insec_X + Node[count].n_insec_Y); // ノードに待ち客情報をpush
                                get_info += 1;

                                Node[count].p_num2 = -1;
                                Node[count].n_xD2 = -1;
                                Node[count].n_yD2 = -1;

                                Node[count].p_on += -1;

                                count_off += 1;
                            }
                        }
                    }
                }
                Node[count].n_X -= ((Node[count].v / 3600) * 1000) / Td;
                if (n_error == Node[count].n_X)
                {
                    printf("座標エラー : Node[%d]\n", count);
                    exit(1);
                }

                // 目的の座標を超えるときは交差点座標を参照
                if ((double)Node[count].n_insec_X - Node[count].n_X > 0.0000)
                {
                    Node[count].prog_d_surp = fabs((double)Node[count].n_insec_X - Node[count].n_X);
                    Node[count].n_X = (double)Node[count].n_insec_X;
                }
            }

            // y軸方向に移動を行っているとき
        }
        else if (ceil(Node[count].n_Y) != floor(Node[count].n_Y) || Node[count].n_X == Node[count].n_insec_X)
        {
            /*----------------------------------*/
            // 上方向の移動
            if ((double)Node[count].n_insec_Y - Node[count].n_Y > 0.0000)
            {
                n_error = Node[count].n_Y;
                if (Node[count].p_on == 2)
                {
                    if (Pass[Node[count].p_num].p_xD != Pass[Node[count].p_num2].p_xD || Pass[Node[count].p_num].p_yD != Pass[Node[count].p_num2].p_yD)
                    {
                        if (sqrt2(Node[count].n_X - center_x, Node[count].n_Y - center_y) <= area_disaster)
                        {
                            if (sqrt2(Node[count].n_X - center_x, (Node[count].n_Y + ((Node[count].v / 3600) * 1000) / Td) - center_y) > area_disaster)
                            {

                                Pass[Node[count].p_num2].p_X = Node[count].n_X; // 乗客２の現在地を更新
                                Pass[Node[count].p_num2].p_Y = Node[count].n_Y;
                                Pass[Node[count].p_num2].p_ride = 0; // 乗客の乗車情報の変更

                                enqueue(Node[count].p_num2); // 更新した現在位置のキューに乗客２の情報を入力

                                push(count, Ax * Ax * Twait + Ax * Node[count].n_insec_X + Node[count].n_insec_Y); // ノードに待ち客情報をpush
                                get_info += 1;

                                Node[count].p_num2 = -1;
                                Node[count].n_xD2 = -1;
                                Node[count].n_yD2 = -1;

                                Node[count].p_on += -1;

                                count_off += 1;
                            }
                        }
                    }
                }
                Node[count].n_Y += ((Node[count].v / 3600) * 1000) / Td;
                if (n_error == Node[count].n_Y)
                {
                    printf("座標エラー : Node[%d]\n", count);
                    exit(1);
                }

                // 目的の座標を超えるときは交差点座標を参照
                if ((double)Node[count].n_insec_Y - Node[count].n_Y < 0.0000)
                {
                    Node[count].prog_d_surp = fabs((double)Node[count].n_insec_Y - Node[count].n_Y);
                    Node[count].n_Y = (double)Node[count].n_insec_Y;
                }
                // 下方向の移動
            }
            else
            {
                // 座標を更新
                n_error = Node[count].n_Y;
                if (Node[count].p_on == 2)
                {
                    if (Pass[Node[count].p_num].p_xD != Pass[Node[count].p_num2].p_xD || Pass[Node[count].p_num].p_yD != Pass[Node[count].p_num2].p_yD)
                    {
                        if (sqrt2(Node[count].n_X - center_x, Node[count].n_Y - center_y) <= area_disaster)
                        {
                            if (sqrt2(Node[count].n_X - center_x, (Node[count].n_Y - ((Node[count].v / 3600) * 1000) / Td) - center_y) > area_disaster)
                            {

                                Pass[Node[count].p_num2].p_X = Node[count].n_X; // 乗客２の現在地を更新
                                Pass[Node[count].p_num2].p_Y = Node[count].n_Y;
                                Pass[Node[count].p_num2].p_ride = 0; // 乗客の乗車情報の変更

                                enqueue(Node[count].p_num2); // 更新した現在位置のキューに乗客２の情報を入力

                                push(count, Ax * Ax * Twait + Ax * Node[count].n_insec_X + Node[count].n_insec_Y); // ノードに待ち客情報をpush
                                get_info += 1;

                                Node[count].p_num2 = -1;
                                Node[count].n_xD2 = -1;
                                Node[count].n_yD2 = -1;

                                Node[count].p_on += -1;

                                count_off += 1;
                            }
                        }
                    }
                }
                Node[count].n_Y -= ((Node[count].v / 3600) * 1000) / Td;
                if (n_error == Node[count].n_Y)
                {
                    printf("座標エラー : Node[%d]\n", count);
                    exit(1);
                }

                // 目的の座標を超えるときは交差点座標を参照
                if ((double)Node[count].n_insec_Y - Node[count].n_Y > 0.0000)
                {
                    Node[count].prog_d_surp = fabs((double)Node[count].n_insec_Y - Node[count].n_Y);
                    Node[count].n_Y = (double)Node[count].n_insec_Y;
                }
            }
        }
        else
        {
            printf("座標更新エラー : Node[%d]\n", count);
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
void move_update_coord_surplus()
{
    for (int count = 0; count < N_ALL_NUM; count++)
    {
        if (Node[count].prog_d_surp != 0)
        {
            if ((double)Node[count].n_insec_X - Node[count].n_X > 0.0000)
            {
                // 座標を更新
                Node[count].n_X += Node[count].prog_d_surp;
            }
            else if ((double)Node[count].n_insec_X - Node[count].n_X < 0.0000)
            {
                // 座標を更新
                Node[count].n_X -= Node[count].prog_d_surp;
            }
            else if ((double)Node[count].n_insec_Y - Node[count].n_Y > 0.0000)
            {
                // 座標を更新
                Node[count].n_Y += Node[count].prog_d_surp;
            }
            else if ((double)Node[count].n_insec_Y - Node[count].n_Y < 0.0000)
            {
                // 座標を更新
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
void move_new_direction()
{
    int count = 0;

    while (count < N_ALL_NUM)
    {

        // 交差点にいるとき, 次に向かう交差点を指定
        if ((Node[count].n_X == (double)Node[count].n_insec_X) && (Node[count].n_Y == (double)Node[count].n_insec_Y))
        {
            d_now_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);

            int direction = rand() % 4;  // 0~3までの4つの整数をランダムに発生
            int direction2 = rand() % 2; // 0~1までの2つの整数をランダムに発生

            if (Node[count].p_on == 0)
            { // ノードが乗客を乗せておらず、目的地設定がない場合　ランダム移動

#ifdef circle
                if (sqrt2(Node[count].n_X - center_x, Node[count].n_Y - center_y) <= area_disaster)
                {
#else
                if (Node[count].n_X >= Ax_d && Node[count].n_X <= Ax - Ax_d && Node[count].n_Y >= Ay_d && Node[count].n_Y <= Ay - Ay_d)
                {
#endif
                    if (Node[count].move_pattern == 0)
                    { // パターン0  右0上1

                        if (direction2 == 0)
                        { // 右

                            Node[count].n_insec_X += 1;
                            Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);

#ifdef circle
                            if (sqrt2(Node[count].n_insec_X - center_x, Node[count].n_Y - center_y) > area_disaster)
#else
                            if (Node[count].n_insec_X > Ax - Ax_d - 1)
#endif
                            {
                                Node[count].n_insec_X += -1; // 元に戻す
                                direction2 = rand() % 2;
                                if (direction2 == 0)
                                {
                                    Node[count].move_pattern = 2;
                                }
                                else
                                {
                                    Node[count].move_pattern = 3;
                                }
                                continue;
                            }
                        }
                        else if (direction2 == 1)
                        { // 上

                            Node[count].n_insec_Y += -1;
                            Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);

#ifdef circle
                            if (sqrt2(Node[count].n_X - center_x, Node[count].n_insec_Y - center_y) > area_disaster)
#else
                            if (Node[count].n_insec_Y < Ay_d)
#endif
                            {

                                Node[count].n_insec_Y += 1; // 元に戻す
                                direction2 = rand() % 2;
                                if (direction2 == 0)
                                {
                                    Node[count].move_pattern = 1;
                                }
                                else
                                {
                                    Node[count].move_pattern = 3;
                                }
                                continue;
                            }
                        }
                    }
                    else if (Node[count].move_pattern == 1)
                    { // パターン1  右0下1

                        if (direction2 == 1)
                        { // 右

                            Node[count].n_insec_X += 1;
                            Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);

#ifdef circle
                            if (sqrt2(Node[count].n_insec_X - center_x, Node[count].n_Y - center_y) > area_disaster)
#else
                            if (Node[count].n_insec_X > Ax - Ax_d - 1)
#endif
                            {

                                Node[count].n_insec_X += -1; // 元に戻す
                                direction2 = rand() % 2;
                                if (direction2 == 0)
                                {
                                    Node[count].move_pattern = 2;
                                }
                                else
                                {
                                    Node[count].move_pattern = 3;
                                }
                                continue;
                            }
                        }
                        else if (direction2 == 1)
                        { // 下

                            Node[count].n_insec_Y += 1;
                            Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);

#ifdef circle
                            if (sqrt2(Node[count].n_X - center_x, Node[count].n_insec_Y - center_y) > area_disaster)
#else
                            if (Node[count].n_insec_Y > Ay - Ay_d - 1)
#endif
                            {

                                Node[count].n_insec_Y += -1; // 元に戻す
                                direction2 = rand() % 2;
                                if (direction2 == 0)
                                {
                                    Node[count].move_pattern = 0;
                                }
                                else
                                {
                                    Node[count].move_pattern = 2;
                                }
                                continue;
                            }
                        }
                    }
                    else if (Node[count].move_pattern == 2)
                    { // パターン2  左0上1

                        if (direction2 == 0)
                        { // 左

                            Node[count].n_insec_X += -1;
                            Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);

#ifdef circle
                            if (sqrt2(Node[count].n_insec_X - center_x, Node[count].n_Y - center_y) > area_disaster)
#else
                            if (Node[count].n_insec_X < Ax_d)
#endif
                            {

                                Node[count].n_insec_X += 1; // 元に戻す
                                direction2 = rand() % 2;
                                if (direction2 == 0)
                                {
                                    Node[count].move_pattern = 0;
                                }
                                else
                                {
                                    Node[count].move_pattern = 1;
                                }
                                continue;
                            }
                        }
                        else if (direction2 == 1)
                        { // 上

                            Node[count].n_insec_Y += -1;
                            Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);

#ifdef circle
                            if (sqrt2(Node[count].n_X - center_x, Node[count].n_insec_Y - center_y) > area_disaster)
#else
                            if (Node[count].n_insec_Y < Ay_d)
#endif
                            {

                                Node[count].n_insec_Y += 1; // 元に戻す
                                direction2 = rand() % 2;
                                if (direction2 == 0)
                                {
                                    Node[count].move_pattern = 1;
                                }
                                else
                                {
                                    Node[count].move_pattern = 3;
                                }
                                continue;
                            }
                        }
                    }
                    else if (Node[count].move_pattern == 3)
                    { // パターン3  左0下1

                        if (direction2 == 0)
                        { // 左

                            Node[count].n_insec_X += -1;
                            Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);

#ifdef circle
                            if (sqrt2(Node[count].n_insec_X - center_x, Node[count].n_Y - center_y) > area_disaster)
#else
                            if (Node[count].n_insec_X < Ax_d)
#endif
                            {

                                Node[count].n_insec_X += 1; // 元に戻す
                                direction2 = rand() % 2;
                                if (direction2 == 0)
                                {
                                    Node[count].move_pattern = 0;
                                }
                                else
                                {
                                    Node[count].move_pattern = 1;
                                }
                                continue;
                            }
                        }
                        else if (direction2 == 1)
                        { // 下

                            Node[count].n_insec_Y += 1;
                            Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);

#ifdef circle
                            if (sqrt2(Node[count].n_X - center_x, Node[count].n_insec_Y - center_y) > area_disaster)
#else
                            if (Node[count].n_insec_Y > Ay - Ay_d - 1)
#endif
                            {

                                Node[count].n_insec_Y += -1; // 元に戻す
                                direction2 = rand() % 2;
                                if (direction2 == 0)
                                {
                                    Node[count].move_pattern = 0;
                                }
                                else
                                {
                                    Node[count].move_pattern = 2;
                                }
                                continue;
                            }
                        }
                    }

                    count += 1; // 調べるノード番号を増加
                }
                else
                { // 中心へ向かって移動
                    Node[count].d_length = sqrt2(center_x - Node[count].n_X, center_y - Node[count].n_Y);
                    d_now_length = Node[count].d_length;
                    if (direction == 0 && d_now_length != 0)
                    {

                        Node[count].n_insec_Y += -1;
                        Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);

                        if (Node[count].d_length - d_now_length > 0.0)
                        {

                            Node[count].n_insec_Y += 1;
                            Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);

                            continue;
                        }

                        if (Node[count].n_insec_Y < 0)
                        {

                            Node[count].n_insec_Y += 1;
                            Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);
                            continue;
                        }
                    }

                    else if (direction == 1 && d_now_length != 0)
                    {

                        Node[count].n_insec_Y += 1;
                        Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);

                        if (Node[count].d_length - d_now_length > 0.0)
                        {
                            Node[count].n_insec_Y += -1;
                            Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);

                            continue;
                        }

                        if (Node[count].n_insec_Y > Ay - 1)
                        {

                            Node[count].n_insec_Y += -1;
                            Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);

                            continue;
                        }
                    }

                    else if (direction == 2 && d_now_length != 0)
                    {

                        Node[count].n_insec_X += -1;

                        Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);

                        if (Node[count].d_length - d_now_length > 0.0)
                        {

                            Node[count].n_insec_X += 1;
                            Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);

                            continue;
                        }

                        if (Node[count].n_insec_X < 0)
                        {
                            Node[count].n_insec_X += 1;
                            Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);

                            continue;
                        }
                    }

                    else if (direction == 3 && d_now_length != 0)
                    {

                        Node[count].n_insec_X += 1;

                        Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);

                        if (Node[count].d_length - d_now_length > 0.0)
                        {

                            Node[count].n_insec_X += -1;
                            Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);

                            continue;
                        }

                        if (Node[count].n_insec_X > Ax - 1)
                        {

                            Node[count].n_insec_X += -1;
                            Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);

                            continue;
                        }
                    }

                    count += 1;
                }
            }
            else if (Node[count].p_on == 1 || Node[count].p_on == 3 || Node[count].p_on == 5)
            {

                d_now_length = Node[count].d_length;

                if (direction == 0 && d_now_length != 0)
                {

                    Node[count].n_insec_Y += -1;
                    Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);

                    if (Node[count].d_length - d_now_length > 0.0)
                    {

                        Node[count].n_insec_Y += 1;
                        Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);

                        continue;
                    }

                    if (Node[count].n_insec_Y < 0)
                    {

                        Node[count].n_insec_Y += 1;
                        Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);
                        continue;
                    }
                }

                else if (direction == 1 && d_now_length != 0)
                {

                    Node[count].n_insec_Y += 1;
                    Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);

                    if (Node[count].d_length - d_now_length > 0.0)
                    {
                        Node[count].n_insec_Y += -1;
                        Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);

                        continue;
                    }

                    if (Node[count].n_insec_Y > Ay - 1)
                    {

                        Node[count].n_insec_Y += -1;
                        Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);

                        continue;
                    }
                }

                else if (direction == 2 && d_now_length != 0)
                {

                    Node[count].n_insec_X += -1;

                    Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);

                    if (Node[count].d_length - d_now_length > 0.0)
                    {

                        Node[count].n_insec_X += 1;
                        Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);

                        continue;
                    }

                    if (Node[count].n_insec_X < 0)
                    {
                        Node[count].n_insec_X += 1;
                        Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);

                        continue;
                    }
                }

                else if (direction == 3 && d_now_length != 0)
                {

                    Node[count].n_insec_X += 1;

                    Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);

                    if (Node[count].d_length - d_now_length > 0.0)
                    {

                        Node[count].n_insec_X += -1;
                        Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);

                        continue;
                    }

                    if (Node[count].n_insec_X > Ax - 1)
                    {

                        Node[count].n_insec_X += -1;
                        Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);

                        continue;
                    }
                }

                count += 1;
            }
            else if (Node[count].p_on == 2)
            {

                for (int i = 0; i < 2; i++)
                {
                    for (int k = 0; k < 4; k++)
                    {
                        last_direction[i][k] = 0;
                    }
                }

                for (direction = 0; direction < 4; direction++)
                {

                    d_now_length = Node[count].d_length;
                    d_now_length2 = Node[count].d_length2;

                    if (direction == 0)
                    {
                        Node[count].n_insec_Y += 1;

                        Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);
                        Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X, Node[count].n_yD2 - Node[count].n_insec_Y);

                        if (d_now_length - Node[count].d_length > 0)
                        {
                            last_direction[0][0] = 1;
                        }

                        if (d_now_length2 - Node[count].d_length2 > 0)
                        {
                            last_direction[1][0] = 1;
                        }
                        else if (sqrt2(Node[count].n_X - center_x, Node[count].n_Y - center_y) >= area_disaster)
                        {
                            last_direction[1][0] = 1;
                        }

                        Node[count].n_insec_Y += -1;
                        Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);
                        Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X, Node[count].n_yD2 - Node[count].n_insec_Y);
                    }
                    else if (direction == 1)
                    {
                        Node[count].n_insec_Y += -1;

                        Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);
                        Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X, Node[count].n_yD2 - Node[count].n_insec_Y);

                        if (d_now_length - Node[count].d_length > 0)
                        {
                            last_direction[0][1] = 1;
                        }

                        if (d_now_length2 - Node[count].d_length2 > 0)
                        {
                            last_direction[1][1] = 1;
                        }
                        else if (sqrt2(Node[count].n_X - center_x, Node[count].n_Y - center_y) >= area_disaster)
                        {
                            last_direction[1][1] = 1;
                        }

                        Node[count].n_insec_Y += 1;
                        Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);
                        Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X, Node[count].n_yD2 - Node[count].n_insec_Y);
                    }
                    else if (direction == 2)
                    {
                        Node[count].n_insec_X += -1;

                        Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);
                        Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X, Node[count].n_yD2 - Node[count].n_insec_Y);

                        if (d_now_length - Node[count].d_length > 0)
                        {
                            last_direction[0][2] = 1;
                        }

                        if (d_now_length2 - Node[count].d_length2 > 0)
                        {
                            last_direction[1][2] = 1;
                        }
                        else if (sqrt2(Node[count].n_X - center_x, Node[count].n_Y - center_y) >= area_disaster)
                        {
                            last_direction[1][2] = 1;
                        }

                        Node[count].n_insec_X += 1;
                        Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);
                        Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X, Node[count].n_yD2 - Node[count].n_insec_Y);
                    }
                    else if (direction == 3)
                    {
                        Node[count].n_insec_X += 1;

                        Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);
                        Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X, Node[count].n_yD2 - Node[count].n_insec_Y);

                        if (d_now_length - Node[count].d_length > 0)
                        {
                            last_direction[0][3] = 1;
                        }

                        if (d_now_length2 - Node[count].d_length2 > 0)
                        {
                            last_direction[1][3] = 1;
                        }
                        else if (sqrt2(Node[count].n_X - center_x, Node[count].n_Y - center_y) >= area_disaster)
                        {
                            last_direction[1][3] = 1;
                        }

                        Node[count].n_insec_X += -1;
                        Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);
                        Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X, Node[count].n_yD2 - Node[count].n_insec_Y);
                    }
                }

                if (last_direction[0][0] == 1 && last_direction[1][0] == 1)
                {
                    Node[count].n_insec_Y += 1;

                    Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);
                    Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X, Node[count].n_yD2 - Node[count].n_insec_Y);
                }
                else if (last_direction[0][1] == 1 && last_direction[1][1] == 1)
                {
                    Node[count].n_insec_Y += -1;

                    Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);
                    Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X, Node[count].n_yD2 - Node[count].n_insec_Y);
                }
                else if (last_direction[0][2] == 1 && last_direction[1][2] == 1)
                {
                    Node[count].n_insec_X += -1;
                    Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);
                    Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X, Node[count].n_yD2 - Node[count].n_insec_Y);
                }
                else if (last_direction[0][3] == 1 && last_direction[1][3] == 1)
                {
                    Node[count].n_insec_X += 1;

                    Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);
                    Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X, Node[count].n_yD2 - Node[count].n_insec_Y);
                }

                else if (last_direction[0][0] == 1 && last_direction[1][0] == 0)
                {

                    Pass[Node[count].p_num2].p_X = Node[count].n_insec_X;
                    Pass[Node[count].p_num2].p_Y = Node[count].n_insec_Y;
                    Pass[Node[count].p_num2].p_ride = 0;

                    enqueue(Node[count].p_num2);
                    push(count, Ax * Ax * Twait + Ax * Node[count].n_insec_X + Node[count].n_insec_Y);
                    Node[count].Map[Node[count].n_insec_X][Node[count].n_insec_Y].info = 1;
                    Node[count].Map[Node[count].n_insec_X][Node[count].n_insec_Y].info_time = Twait;
                    get_info += 1;

                    Node[count].n_insec_Y += 1;

                    Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);
                    Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X, Node[count].n_yD2 - Node[count].n_insec_Y);

                    Node[count].p_num2 = -1;
                    Node[count].n_xD2 = -1;
                    Node[count].n_yD2 = -1;

                    Node[count].p_on += -1;

                    count_getoff += 1;
                }
                else if (last_direction[0][1] == 1 && last_direction[1][1] == 0)
                {

                    Pass[Node[count].p_num2].p_X = Node[count].n_insec_X;
                    Pass[Node[count].p_num2].p_Y = Node[count].n_insec_Y;
                    Pass[Node[count].p_num2].p_ride = 0;

                    enqueue(Node[count].p_num2);

                    push(count, Ax * Ax * Twait + Ax * Node[count].n_insec_X + Node[count].n_insec_Y);
                    Node[count].Map[Node[count].n_insec_X][Node[count].n_insec_Y].info = 1;
                    Node[count].Map[Node[count].n_insec_X][Node[count].n_insec_Y].info_time = Twait;
                    get_info += 1;

                    Node[count].n_insec_Y += -1;

                    Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);
                    Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X, Node[count].n_yD2 - Node[count].n_insec_Y);

                    Node[count].p_num2 = -1;
                    Node[count].n_xD2 = -1;
                    Node[count].n_yD2 = -1;

                    Node[count].p_on += -1;

                    count_getoff += 1;
                }
                else if (last_direction[0][2] == 1 && last_direction[1][2] == 0)
                {

                    Pass[Node[count].p_num2].p_X = Node[count].n_insec_X;
                    Pass[Node[count].p_num2].p_Y = Node[count].n_insec_Y;
                    Pass[Node[count].p_num2].p_ride = 0;

                    enqueue(Node[count].p_num2);

                    push(count, Ax * Ax * Twait + Ax * Node[count].n_insec_X + Node[count].n_insec_Y);
                    Node[count].Map[Node[count].n_insec_X][Node[count].n_insec_Y].info = 1;
                    Node[count].Map[Node[count].n_insec_X][Node[count].n_insec_Y].info_time = Twait;
                    get_info += 1;

                    Node[count].n_insec_X += -1;

                    Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);
                    Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X, Node[count].n_yD2 - Node[count].n_insec_Y);

                    Node[count].p_num2 = -1;
                    Node[count].n_xD2 = -1;
                    Node[count].n_yD2 = -1;

                    Node[count].p_on += -1;

                    count_getoff += 1;
                }
                else if (last_direction[0][3] == 1 && last_direction[1][3] == 0)
                {

                    Pass[Node[count].p_num2].p_X = Node[count].n_insec_X;
                    Pass[Node[count].p_num2].p_Y = Node[count].n_insec_Y;
                    Pass[Node[count].p_num2].p_ride = 0;

                    enqueue(Node[count].p_num2);

                    push(count, Ax * Ax * Twait + Ax * Node[count].n_insec_X + Node[count].n_insec_Y);
                    Node[count].Map[Node[count].n_insec_X][Node[count].n_insec_Y].info = 1;
                    Node[count].Map[Node[count].n_insec_X][Node[count].n_insec_Y].info_time = Twait;
                    get_info += 1;

                    Node[count].n_insec_X += 1;

                    Node[count].d_length = sqrt2(Node[count].n_xD - Node[count].n_insec_X, Node[count].n_yD - Node[count].n_insec_Y);
                    Node[count].d_length2 = sqrt2(Node[count].n_xD2 - Node[count].n_insec_X, Node[count].n_yD2 - Node[count].n_insec_Y);

                    Node[count].p_num2 = -1;
                    Node[count].n_xD2 = -1;
                    Node[count].n_yD2 = -1;

                    Node[count].p_on += -1;

                    count_getoff += 1;
                }
                count += 1;
            }
        }
        else
        {
            count += 1;
        }
    }
}

/* -------------------------------------------------------------------------------------------- *
 *	関数名 : speed_error       																	*
 *  機能 : 速度エラーを表示する関数							            						*
 *  仮引数 : なし													 						    *
 * -------------------------------------------------------------------------------------------- */
void speed_error(int count)
{
    if (Node[count].v < 10)
    {
        printf("speed_error = Node[%d]\n", count);
        exit(1);
    }
}
