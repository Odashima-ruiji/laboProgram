/*
    base_func.c : プログラム簡略化のための基礎関数 (実装)
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "base_func.h"

/* --------------------------------------------------------- *
 *  関数名 : sqrt2                                           *
 *  機能 : 二乗和の平方根を返す                              *
 *  仮引数 : a_x : x座標（目的地-現在地）                    *
 *         : a_y : y座標（目的地-現在地）                    *
 * --------------------------------------------------------- */
double sqrt2(double a_x, double a_y)
{
    return sqrt(a_x * a_x + a_y * a_y);
}

/* --------------------------------------------------------- *
 *  関数名 : randAxAy                                        *
 *  機能 : 0~Axの整数をランダムで返す                        *
 *  仮引数 : なし                                            *
 * --------------------------------------------------------- */
double randAxAy(void)
{
    double ran;
    ran = rand() % Ax;
    return ran;
}

/* --------------------------------------------------------- *
 *  関数名 : randAxAy_d                                      *
 *  機能 : 0~Axの整数をランダムで返す                        *
 *  仮引数 : なし                                            *
 * --------------------------------------------------------- */
double randAxAy_d(void)
{
    double ran_d;
    ran_d = rand() % (Ax - 2 * Ax_d) + Ax_d;
    return ran_d;
}

/* --------------------------------------------------------- *
 *  関数名 : enqueue                                         *
 *  機能 : 交差点配列の構造体に, 待っている乗客の番号を入力  *
 *  仮引数 : p_number : 乗客番号を取得                       *
 *  メモ : 途中で乗客を降ろす場合改良が必要                  *
 * --------------------------------------------------------- */
void enqueue(int p_number)
{
    Trans[Pass[p_number].p_X][Pass[p_number].p_Y].wp_number[(queue_head[Pass[p_number].p_X][Pass[p_number].p_Y] + queue_num[Pass[p_number].p_X][Pass[p_number].p_Y]) % qsize] = p_number;
    queue_num[Pass[p_number].p_X][Pass[p_number].p_Y] += 1;
    Trans[Pass[p_number].p_X][Pass[p_number].p_Y].wp_Exist = 1;
}

/* --------------------------------------------------------- *
 *  関数名 : dequeue                                         *
 *  機能 : 仮引数で交差点の座標を受け取り, その構造体の      *
 *         キューから乗客番号を取り出す.                     *
 *  仮引数 : x : x座標                                       *
 *         : y : y座標                                       *
 * --------------------------------------------------------- */
int dequeue(int x, int y)
{
    int answer = -1;
    if (queue_num[x][y] > 0)
    {
        answer = Trans[x][y].wp_number[queue_head[x][y]];
        queue_head[x][y] += 1;
        queue_num[x][y] += -1;
        if (queue_num[x][y] == 0)
        {
            Trans[x][y].wp_Exist = 0;
        }
    }
    return answer;
}

/* --------------------------------------------------------- *
 *  関数名 : peek                                            *
 *  機能 : 仮引数で交差点の座標を受け取り, その構造体の      *
 *         キューの先頭の乗客番号を返す.                     *
 *  仮引数 : i : ノード番号                                  *
 * --------------------------------------------------------- */
int peek(int i)
{
    if (Node[i].stack_num > 0)
    {
        return Node[i].stack_data[0];
    }
    else
    {
        printf("エラー: キューは空です。\n");
        return -1;
    }
}

/* --------------------------------------------------------- *
 *  関数名 : stack_sort                                      *
 *  機能 : すべてのノードに格納されているデータスタックを    *
 *         降順（大きい順）に並べ替える.                     *
 *  仮引数 : なし                                            *
 * --------------------------------------------------------- */
void stack_sort(void)
{
    int i, j, tmp;
    for (int k = 0; k < N_ALL_NUM; k++)
    {
        for (i = 0; i < Node[k].stack_num; ++i)
        {
            for (j = i + 1; j < Node[k].stack_num; ++j)
            {
                if (Node[k].stack_data[i] < Node[k].stack_data[j])
                {
                    tmp = Node[k].stack_data[i];
                    Node[k].stack_data[i] = Node[k].stack_data[j];
                    Node[k].stack_data[j] = tmp;
                }
            }
        }
    }
}

/* --------------------------------------------------------- *
 *  関数名 : push                                            *
 *  機能 : ノードの情報スタックにデータをpushする関数        *
 *  仮引数 : i : ノード番号                                  *
 *         : push_data : pushするデータ                      *
 * --------------------------------------------------------- */
void push(int i, int push_data)
{
    if (Node[i].stack_num < ssize)
    {
        Node[i].stack_data[Node[i].stack_num] = push_data;
        stack_sort();
        Node[i].stack_num++;
        Node[i].all_stack_num++;
    }
    else
    {
        printf("スタックがいっぱいです\n");
    }
}

/* --------------------------------------------------------- *
 *  関数名 : push2                                           *
 *  機能 : ノードの情報スタックにデータをpushする関数        *
 *  仮引数 : i : ノード番号                                  *
 *         : push_data_1, push_data_2 : pushするデータ       *
 * --------------------------------------------------------- */
void push2(int i, int push_data_1, int push_data_2)
{
    if (Node[i].stack_num3 < ssize)
    {
        Node[i].stack_info[Node[i].stack_num3][0] = push_data_1;
        Node[i].stack_info[Node[i].stack_num3][1] = push_data_2;
        Node[i].stack_num3++;
    }
    else
    {
        printf("スタックがいっぱいです\n");
    }
}

/* --------------------------------------------------------- *
 *  関数名 : pushnopos                                       *
 *  機能 : 乗客が存在していない位置をpushする関数            *
 *  仮引数 : i : ノード番号                                  *
 *         : push_data2 : pushするデータ                     *
 * --------------------------------------------------------- */
void pushnopos(int i, int push_data2)
{
    if (Node[i].stack_num2 < ssize)
    {
        Node[i].stack_nopos[Node[i].stack_num2] = push_data2;
        Node[i].stack_num2++;
    }
    else
    {
        printf("スタックがいっぱいです\n");
    }
}

/* --------------------------------------------------------- *
 *  関数名 : search                                          *
 *  機能 : 一度訪れた地点を記憶し, 再び行かないように避ける. *
 *  仮引数 : count : ノード番号                              *
 *         : number :                                        *
 * --------------------------------------------------------- */
int search(int count, int number)
{
    for (int i = 0; i < Node[count].stack_num2 + 1; i++)
    {
        if (Node[count].stack_nopos[i] == number)
        {
            return 1;
        }
    }
    return 0;
}

/* --------------------------------------------------------- *
 *  関数名 : search2                                         *
 *  機能 : 一度訪れた地点を記憶し, 再び行かないように避ける. *
 *  仮引数 : count : ノード番号                              *
 *         : number_1, number_2 :                            *
 * --------------------------------------------------------- */
int search2(int count, int number_1, int number_2)
{
    for (int i = 0; i < Node[count].stack_num3 + 1; i++)
    {
        if (Node[count].stack_info[i][0] == number_1)
        {
            if (number_2 - Node[count].stack_info[i][1] < 60)
            {
                return 1;
            }
        }
    }
    return 0;
}

/* --------------------------------------------------------- *
 *  関数名 : pop                                             *
 *  機能 : ノードの情報スタックからデータをpopする.          *
 *  仮引数 : i : ノード番号                                  *
 * --------------------------------------------------------- */
int pop(int i)
{
    int answer;
    if (Node[i].stack_num > 0)
    {
        Node[i].stack_num--;
        answer = Node[i].stack_data[0];
        Node[i].stack_data[0] = 0;
        if (i == 0)
        {
            stack_pop[pop_num] = answer;
            pop_num++;
        }
        stack_sort();
        return answer;
    }
    else
    {
        return -1;
    }
}
