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
    

    // 利用要求を受ける
    // #ifdef EPIDEMIC
    detect_trans();
    // #endif
#ifdef EPIDEMIC
    flooding();
#endif
    D_check(); // 乗客の目的地判定
    allrideon(); // 乗客を乗せる

    // 交差点にいるとき, 次に向かう交差点を指定
    move_new_direction();
// ノードが交差点上にいる状態でフラッディング


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
            { // ノードが乗客を乗せておらず、目的地設定がない場合　スコアベースで移動

                P_map(count);
                // 全マップで最もスコアが高いグリッドを目的地として設定
                //find_best_grid_in_all_map(count);
                
                // 全グリッドのsmoothed_countが0かチェック（マップ情報がない場合）
                int all_density_zero = 1; // すべて0と仮定
                for (int i = 0; i < grid_size; i++) {
                    for (int j = 0; j < grid_size; j++) {
                        if (smoothed_count[i][j] > 0) {
                            all_density_zero = 0; // 1つでも0より大きければフラグを下ろす
                            break;
                        }
                    }
                    if (all_density_zero == 0) break; // 内側のループで見つかったら外側も抜ける
                }
                
                if (all_density_zero == 1) {
                    // マップ情報がない場合は自由走行（move.hのランダム移動ロジック）
                    #ifdef circle
                    if (sqrt2(Node[count].n_X - center_x, Node[count].n_Y - center_y) <= area_disaster){
                    #else
                    if (Node[count].n_X >= Ax_d && Node[count].n_X <= Ax-Ax_d && Node[count].n_Y >= Ay_d && Node[count].n_Y <= Ay-Ay_d){
                    #endif
                        retry_movement:
                        if(Node[count].move_pattern == 0){ // パターン0  右0上1
                            if(direction2 == 0){ // 右
                                Node[count].n_insec_X += 1;
                                Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);
                                #ifdef circle
                                if(sqrt2(Node[count].n_insec_X - center_x, Node[count].n_Y - center_y) > area_disaster)
                                #else
                                if(Node[count].n_insec_X > Ax-Ax_d-1)
                                #endif
                                {
                                    Node[count].n_insec_X -= 1;
                                    direction2 = rand() % 2;
                                    if(direction2==0){
                                        Node[count].move_pattern = 2;
                                    }else{
                                        Node[count].move_pattern = 3;
                                    }
                                    goto retry_movement;
                                }
                            }else if(direction2 == 1){ // 上
                                Node[count].n_insec_Y -= 1;
                                Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);
                                #ifdef circle
                                if(sqrt2(Node[count].n_X - center_x, Node[count].n_insec_Y - center_y) > area_disaster)
                                #else
                                if(Node[count].n_insec_Y < Ay_d)
                                #endif
                                {
                                    Node[count].n_insec_Y += 1;
                                    direction2 = rand() % 2;
                                    if(direction2==0){
                                        Node[count].move_pattern = 1;
                                    }else{
                                        Node[count].move_pattern = 3;
                                    }
                                    goto retry_movement;
                                }
                            }
                        }else if(Node[count].move_pattern == 1){ // パターン1  右0下1
                            if(direction2 == 1){ // 右
                                Node[count].n_insec_X += 1;
                                Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);
                                #ifdef circle
                                if(sqrt2(Node[count].n_insec_X - center_x, Node[count].n_Y - center_y) > area_disaster)
                                #else
                                if(Node[count].n_insec_X > Ax-Ax_d-1)
                                #endif
                                {
                                    Node[count].n_insec_X -= 1;
                                    direction2 = rand() % 2;
                                    if(direction2==0){
                                        Node[count].move_pattern = 2;
                                    }else{
                                        Node[count].move_pattern = 3;
                                    }
                                    goto retry_movement;
                                }
                            }else if(direction2 == 1){ // 下
                                Node[count].n_insec_Y += 1;
                                Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);
                                #ifdef circle
                                if(sqrt2(Node[count].n_X - center_x, Node[count].n_insec_Y - center_y) > area_disaster)
                                #else
                                if(Node[count].n_insec_Y > Ay-Ay_d-1)
                                #endif
                                {
                                    Node[count].n_insec_Y -= 1;
                                    direction2 = rand() % 2;
                                    if(direction2==0){
                                        Node[count].move_pattern = 0;
                                    }else{
                                        Node[count].move_pattern = 2;
                                    }
                                    goto retry_movement;
                                }
                            }
                        }else if(Node[count].move_pattern == 2){ // パターン2  左0上1
                            if(direction2 == 0){ // 左
                                Node[count].n_insec_X -= 1;
                                Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);
                                #ifdef circle
                                if(sqrt2(Node[count].n_insec_X - center_x, Node[count].n_Y - center_y) > area_disaster)
                                #else
                                if(Node[count].n_insec_X < Ax_d)
                                #endif
                                {
                                    Node[count].n_insec_X += 1;
                                    direction2 = rand() % 2;
                                    if(direction2==0){
                                        Node[count].move_pattern = 0;
                                    }else{
                                        Node[count].move_pattern = 1;
                                    }
                                    goto retry_movement;
                                }
                            }else if(direction2 == 1){ // 上
                                Node[count].n_insec_Y -= 1;
                                Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);
                                #ifdef circle
                                if(sqrt2(Node[count].n_X - center_x, Node[count].n_insec_Y - center_y) > area_disaster)
                                #else
                                if(Node[count].n_insec_Y < Ay_d)
                                #endif
                                {
                                    Node[count].n_insec_Y += 1;
                                    direction2 = rand() % 2;
                                    if(direction2==0){
                                        Node[count].move_pattern = 1;
                                    }else{
                                        Node[count].move_pattern = 3;
                                    }
                                    goto retry_movement;
                                }
                            }
                        }else if(Node[count].move_pattern == 3){ // パターン3  左0下1
                            if(direction2 == 0){ // 左
                                Node[count].n_insec_X -= 1;
                                Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);
                                #ifdef circle
                                if(sqrt2(Node[count].n_insec_X - center_x, Node[count].n_Y - center_y) > area_disaster)
                                #else
                                if(Node[count].n_insec_X < Ax_d)
                                #endif
                                {
                                    Node[count].n_insec_X += 1;
                                    direction2 = rand() % 2;
                                    if(direction2==0){
                                        Node[count].move_pattern = 0;
                                    }else{
                                        Node[count].move_pattern = 1;
                                    }
                                    goto retry_movement;
                                }
                            }else if(direction2 == 1){ // 下
                                Node[count].n_insec_Y += 1;
                                Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);
                                #ifdef circle
                                if(sqrt2(Node[count].n_X - center_x, Node[count].n_insec_Y - center_y) > area_disaster)
                                #else
                                if(Node[count].n_insec_Y > Ay-Ay_d-1)
                                #endif
                                {
                                    Node[count].n_insec_Y -= 1;
                                    direction2 = rand() % 2;
                                    if(direction2==0){
                                        Node[count].move_pattern = 0;
                                    }else{
                                        Node[count].move_pattern = 2;
                                    }
                                    goto retry_movement;
                                }
                            }
                        }
                        count += 1;
                    }else{ // 中心へ向かって移動
                        Node[count].d_length = sqrt2(center_x - Node[count].n_X, center_y - Node[count].n_Y);
                        d_now_length = Node[count].d_length;
                        if(direction == 0 && d_now_length != 0){
                            Node[count].n_insec_Y -= 1;
                            Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);
                            if(Node[count].d_length - d_now_length > 0.0){
                                Node[count].n_insec_Y += 1;
                                Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);
                                goto retry_movement;
                            }
                            if(Node[count].n_insec_Y < 0){
                                Node[count].n_insec_Y += 1;
                                Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);
                                goto retry_movement;
                            }
                        }else if(direction == 1 && d_now_length != 0){
                            Node[count].n_insec_Y += 1;
                            Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);
                            if(Node[count].d_length - d_now_length > 0.0){
                                Node[count].n_insec_Y -= 1;
                                Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);
                                goto retry_movement;
                            }
                            if(Node[count].n_insec_Y > Ay-1){
                                Node[count].n_insec_Y -= 1;
                                Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);
                                goto retry_movement;
                            }
                        }else if(direction == 2 && d_now_length != 0){
                            Node[count].n_insec_X -= 1;
                            Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);
                            if(Node[count].d_length - d_now_length > 0.0){
                                Node[count].n_insec_X += 1;
                                Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);
                                goto retry_movement;
                            }
                            if(Node[count].n_insec_X < 0){
                                Node[count].n_insec_X += 1;
                                Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);
                                goto retry_movement;
                            }
                        }else if(direction == 3 && d_now_length != 0){
                            Node[count].n_insec_X += 1;
                            Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);
                            if(Node[count].d_length - d_now_length > 0.0){
                                Node[count].n_insec_X -= 1;
                                Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);
                                goto retry_movement;
                            }
                            if(Node[count].n_insec_X > Ax-1){
                                Node[count].n_insec_X -= 1;
                                Node[count].d_length = sqrt2(center_x - Node[count].n_insec_X, center_y - Node[count].n_insec_Y);
                                goto retry_movement;
                            }
                        }
                        count += 1;
                    }
                } else {
                    score_based_movement: // スコアベース移動の開始ラベル
                    int current_grid_x = (int)(Node[count].n_X / cell_width);
                    int current_grid_y = (int)(Node[count].n_Y / cell_height);
                    
                    if(grid_count[current_grid_x][current_grid_y] > 0 && distination_flag[count] == 0){
                        // 現在のグリッド内にinfo==1の交差点が存在する場合
                        // グリッドの境界を計算
                        int grid_min_x = current_grid_x * cell_width;
                        int grid_max_x = (current_grid_x + 1) * cell_width - 1;
                        int grid_min_y = current_grid_y * cell_height;
                        int grid_max_y = (current_grid_y + 1) * cell_height - 1;
                        
                        // グリッド内で最も近いinfo==1の座標を探す
                        int nearest_x = -1;
                        int nearest_y = -1;
                        double min_distance = 99999999.0;
                        
                        for (int i = grid_min_x; i <= grid_max_x && i < Ax; i++) {
                            for (int j = grid_min_y; j <= grid_max_y && j < Ay; j++) {
                                if (Node[count].Map[i][j].info == 1) {
                                    double dist = sqrt2(i - Node[count].n_X, j - Node[count].n_Y);
                                    if (dist < min_distance) {
                                        min_distance = dist;
                                        nearest_x = i;
                                        nearest_y = j;
                                    }
                                }
                            }
                        }
                        
                        if (nearest_x != -1 && nearest_y != -1) {
                            // 最も近いinfo==1の座標に向かって移動
                            int dx = nearest_x - Node[count].n_insec_X;
                            int dy = nearest_y - Node[count].n_insec_Y;
                            
                            if (dx != 0 || dy != 0) {
                                // X方向とY方向のどちらを優先するか決定
                                if (abs(dx) >= abs(dy)) {
                                    // X方向を優先
                                    if (dx > 0) {
                                        Node[count].n_insec_X += 1;
                                    } else {
                                        Node[count].n_insec_X -= 1;
                                    }
                                } else {
                                    // Y方向を優先
                                    if (dy > 0) {
                                        Node[count].n_insec_Y += 1;
                                    } else {
                                        Node[count].n_insec_Y -= 1;
                                    }
                                }
                            } else {
                                // 目的地（info==1の座標）に到達した場合
                                // info=0にして次のinfo==1を探すか、グリッド外へ
                                Node[count].Map[nearest_x][nearest_y].info = 0;
                                
                                // グリッド内にまだinfo==1の座標があるか再チェック
                                // （次のステップでgrid_countが更新されるので、ここでは移動のみ）
                                // 一歩だけランダムに移動してグリッド内に留まる
                                int random_dir = rand() % 4;
                                if (random_dir == 0 && Node[count].n_insec_X < grid_max_x) {
                                    Node[count].n_insec_X += 1;
                                } else if (random_dir == 1 && Node[count].n_insec_Y > grid_min_y) {
                                    Node[count].n_insec_Y -= 1;
                                } else if (random_dir == 2 && Node[count].n_insec_X > grid_min_x) {
                                    Node[count].n_insec_X -= 1;
                                } else if (random_dir == 3 && Node[count].n_insec_Y < grid_max_y) {
                                    Node[count].n_insec_Y += 1;
                                }
                            }
                        }
                    }
                    else if(distination_flag[count] == 0){
                        // 密度マップを更新してから全マップで最もスコアが高いグリッドを目的地として設定
                        P_map(count);
                        find_best_grid_in_all_map(count);
                        count_map += 1;
                        // マップ情報がある場合はスコアベースで移動
                        // 目的地グリッドに向かって移動する方向を選択
                        int target_x = Node[count].target_grid_x;
                        int target_y = Node[count].target_grid_y;
                        
                        // target_x, target_yからグリッド位置を計算してW_gridに加算
                        int grid_x = target_x / cell_width;
                        int grid_y = target_y / cell_height;

                        distination_flag[count] = 1; // 目的地設定済みフラグを立てる
                        
                        // 範囲チェックとW_gridへの加算
                        if (grid_x >= 0 && grid_x < grid_size && grid_y >= 0 && grid_y < grid_size) {
                            Node[count].W_grid[grid_x][grid_y] += 10.0;
                        }
                        
                        // 現在位置から目的地への方向を計算
                        int dx = target_x - Node[count].n_insec_X;
                        int dy = target_y - Node[count].n_insec_Y;
                        
                        // まだ目的地に到達していない場合
                        if (dx != 0 || dy != 0) {
                            
                            // X方向とY方向のどちらを優先するか決定
                            if (abs(dx) > abs(dy)) {
                                // X方向を優先
                                if (dx > 0) {
                                    // 右に移動
                                    Node[count].n_insec_X += 1;
                                } else {
                                    // 左に移動
                                    Node[count].n_insec_X -= 1;
                                }
                            } else {
                                // Y方向を優先
                                if (dy > 0) {
                                    // 下に移動
                                    Node[count].n_insec_Y += 1;
                                } else {
                                    // 上に移動
                                    Node[count].n_insec_Y -= 1;
                                }
                            }
                        } else {
                            // 目的地に到達した場合、distination_flagを0にリセットして再度スコア計算へ
                            distination_flag[count] = 0;
                            goto score_based_movement;
                        }
                    }else if(distination_flag[count] == 1){
                        // 目的地設定済みの場合は目的地に向かって移動
                        int target_x = Node[count].target_grid_x;
                        int target_y = Node[count].target_grid_y;
                        
                        int dx = target_x - Node[count].n_insec_X;
                        int dy = target_y - Node[count].n_insec_Y;
                        
                        if (dx != 0 || dy != 0) {
                            // X方向とY方向のどちらを優先するか決定
                            if (abs(dx) >= abs(dy)) {
                                // X方向を優先
                                if (dx > 0) {
                                    Node[count].n_insec_X += 1;
                                } else {
                                    Node[count].n_insec_X -= 1;
                                }
                            } else {
                                // Y方向を優先
                                if (dy > 0) {
                                    Node[count].n_insec_Y += 1;
                                } else {
                                    Node[count].n_insec_Y -= 1;
                                }
                            }
                        } else {
                            // 目的地に到達した場合、distination_flagを0にリセットして再度スコア計算へ
                            distination_flag[count] = 0;
                            goto score_based_movement;
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
