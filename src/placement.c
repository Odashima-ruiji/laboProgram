/*
    placement.c : 配置に関する関数 (実装)
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "placement.h"

// メインの配置関数 -------------------------------------------------------------------------------------------------------------------------
/* --------------------------------------------------------- *
 *  関数名 : Dist_Init_p                                     *
 *  機能 : 乗客の初期配置を行う.                             *
 *  仮引数 : なし                                            *
 * --------------------------------------------------------- */
void Dist_Init_p(void)
{
    Pass_Spawn();
    position_p_get();
    Set_p_Goal();
    pass_input_inter_init();
}

/* --------------------------------------------------------- *
 *  関数名 : Dist_Init_n                                     *
 *  機能 : ノードの初期配置を行う.                           *
 *  仮引数 : なし                                            *
 * --------------------------------------------------------- */
void Dist_Init_n(void)
{
    if(N_ALL_NUM == 100){
        for (int i = 0; i < 8; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = center_y - 2 * i;
			int s=0;
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X > center_x || Node[i].n_Y > center_y));
        }
        for (int i = 8; i < 15; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = randAxAy();
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X > center_x || Node[i].n_Y > center_y));
        }
        for (int i = 15; i < 23; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = center_y + 2 * (i - 15) + 1;
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X > center_x || Node[i].n_Y < center_y));
        }
        for (int i = 23; i < 30; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = randAxAy();
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X > center_x || Node[i].n_Y < center_y));
        }
        for (int i = 30; i < 38; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = center_y + 2 * (i - 30);
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X < center_x || Node[i].n_Y < center_y));
        }
        for (int i = 38; i < 45; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = randAxAy();
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X < center_x || Node[i].n_Y < center_y));
        }
        for (int i = 45; i < 53; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = center_y - 2 * (i - 45) - 1;
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X < center_x || Node[i].n_Y > center_y));
        }
        for (int i = 53; i < 60; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = randAxAy();
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X < center_x || Node[i].n_Y > center_y));
        }
        // for(int i = 0; i < 10; i++)
        // {
        //     do
        //     {
        //         Node[i].n_X = randAxAy();
        //         Node[i].n_Y = randAxAy();
        //     } while ((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2));
        // }
        for (int i = 60; i < N_ALL_NUM; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = randAxAy();
                Node[i].move_flag = 10;
            } while ((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2));
        }
        for (int i = 0; i < N_ALL_NUM; i++)
        {
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

            int rand4 = rand() % 4;
            Node[i].move_pattern = rand4;
        }
    }else if(N_ALL_NUM == 75){
        /* Q1: 左上 (X < center, Y < center) - Y固定 8台 */
        for (int i = 0; i < 8; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = center_y - 2 * i;
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X > center_x || Node[i].n_Y > center_y));
        }
        /* Q2: 左下 (X < center, Y > center) - Y固定 7台 */
        for (int i = 8; i < 15; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = center_y + 2 * (i - 8) + 1;
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X > center_x || Node[i].n_Y < center_y));
        }
        /* Q3: 右下 (X > center, Y > center) - Y固定 8台 */
        for (int i = 15; i < 23; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = center_y + 2 * (i - 15);
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X < center_x || Node[i].n_Y < center_y));
        }
        /* Q4: 右上 (X > center, Y < center) - Y固定 7台 */
        for (int i = 23; i < 30; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = center_y - 2 * (i - 23) - 1;
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X < center_x || Node[i].n_Y > center_y));
        }
        /* 外側へ向かう車両 45台 (move_flag = 10) */
        for (int i = 30; i < N_ALL_NUM; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = randAxAy();
                Node[i].move_flag = 10;
            } while ((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2));
        }
        for (int i = 0; i < N_ALL_NUM; i++)
        {
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

            int rand4 = rand() % 4;
            Node[i].move_pattern = rand4;
        }
    }else if(N_ALL_NUM == 125){
        /* Q1: 左上 (X < center, Y < center) - Y固定 8台 */
        for (int i = 0; i < 8; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = center_y - 2 * i;
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X > center_x || Node[i].n_Y > center_y));
        }
        /* Q1: 左上 - ランダム 5台 */
        for (int i = 8; i < 13; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = randAxAy();
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X > center_x || Node[i].n_Y > center_y));
        }
        /* Q2: 左下 (X < center, Y > center) - Y固定 8台 */
        for (int i = 13; i < 21; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = center_y + 2 * (i - 13) + 1;
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X > center_x || Node[i].n_Y < center_y));
        }
        /* Q2: 左下 - ランダム 4台 */
        for (int i = 21; i < 25; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = randAxAy();
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X > center_x || Node[i].n_Y < center_y));
        }
        /* Q3: 右下 (X > center, Y > center) - Y固定 8台 */
        for (int i = 25; i < 33; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = center_y + 2 * (i - 25);
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X < center_x || Node[i].n_Y < center_y));
        }
        /* Q3: 右下 - ランダム 5台 */
        for (int i = 33; i < 38; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = randAxAy();
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X < center_x || Node[i].n_Y < center_y));
        }
        /* Q4: 右上 (X > center, Y < center) - Y固定 8台 */
        for (int i = 38; i < 46; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = center_y - 2 * (i - 38) - 1;
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X < center_x || Node[i].n_Y > center_y));
        }
        /* Q4: 右上 - ランダム 4台 */
        for (int i = 46; i < 50; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = randAxAy();
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X < center_x || Node[i].n_Y > center_y));
        }
        /* 外側へ向かう車両 75台 (move_flag = 10) */
        for (int i = 50; i < N_ALL_NUM; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = randAxAy();
                Node[i].move_flag = 10;
            } while ((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2));
        }
        for (int i = 0; i < N_ALL_NUM; i++)
        {
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

            int rand4 = rand() % 4;
            Node[i].move_pattern = rand4;
        }
    }else if(N_ALL_NUM == 150){
        /* Q1: 左上 (X < center, Y < center) - Y固定 8台 */
        for (int i = 0; i < 8; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = center_y - 2 * i;
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X > center_x || Node[i].n_Y > center_y));
        }
        /* Q1: 左上 - ランダム 7台 */
        for (int i = 8; i < 15; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = randAxAy();
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X > center_x || Node[i].n_Y > center_y));
        }
        /* Q2: 左下 (X < center, Y > center) - Y固定 8台 */
        for (int i = 15; i < 23; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = center_y + 2 * (i - 15) + 1;
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X > center_x || Node[i].n_Y < center_y));
        }
        /* Q2: 左下 - ランダム 7台 */
        for (int i = 23; i < 30; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = randAxAy();
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X > center_x || Node[i].n_Y < center_y));
        }
        /* Q3: 右下 (X > center, Y > center) - Y固定 8台 */
        for (int i = 30; i < 38; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = center_y + 2 * (i - 30);
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X < center_x || Node[i].n_Y < center_y));
        }
        /* Q3: 右下 - ランダム 7台 */
        for (int i = 38; i < 45; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = randAxAy();
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X < center_x || Node[i].n_Y < center_y));
        }
        /* Q4: 右上 (X > center, Y < center) - Y固定 8台 */
        for (int i = 45; i < 53; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = center_y - 2 * (i - 45) - 1;
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X < center_x || Node[i].n_Y > center_y));
        }
        /* Q4: 右上 - ランダム 7台 */
        for (int i = 53; i < 60; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = randAxAy();
            } while (((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2)) || (Node[i].n_X < center_x || Node[i].n_Y > center_y));
        }
        /* 外側へ向かう車両 90台 (move_flag = 10) */
        for (int i = 60; i < N_ALL_NUM; i++)
        {
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = randAxAy();
                Node[i].move_flag = 10;
            } while ((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2));
        }
        for (int i = 0; i < N_ALL_NUM; i++)
        {
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

            int rand4 = rand() % 4;
            Node[i].move_pattern = rand4;
        }
    }
}

// 配置関数のための補助関数 ------------------------------------------------------------------------------------------------------------------
/* --------------------------------------------------------- *
 *  関数名 : Pass_Spawn                                      *
 *  機能 : 乗客を存在させる.                                 *
 *  仮引数 : なし                                            *
 * --------------------------------------------------------- */
void Pass_Spawn(void)
{
    for (int i = 0; i < P_ALL_NUM; i++)
    {
        Pass[i].p_Exist = 1;
        Pass[i].p_ride = 0;
    }
}

/* --------------------------------------------------------- *
 *  関数名 : position_p_get                                  *
 *  機能 : 乗客の現在地の決定.                               *
 *  仮引数 : なし                                            *
 * --------------------------------------------------------- */
void position_p_get(void)
{
#ifdef circle
    for (int i = 0; i < P_ALL_NUM; i++)
    {
        do
        {
            Pass[i].p_X = randAxAy();
            Pass[i].p_Y = randAxAy();
            Pass[i].p_xS = Pass[i].p_X;
            Pass[i].p_yS = Pass[i].p_Y;
        } while ((sqrt2(Pass[i].p_X - center_x, Pass[i].p_Y - center_y) > area_disaster));
    }
#else
    for (int i = 0; i < P_ALL_NUM; i++)
    {
        Pass[i].p_X = randAxAy_d();
        Pass[i].p_Y = randAxAy_d();
    }
#endif
}

/* --------------------------------------------------------- *
 *  関数名 : Set_p_Goal                                      *
 *  機能 : 乗客の目的地の決定.                               *
 *  仮引数 : なし                                            *
 * --------------------------------------------------------- */
void Set_p_Goal(void)
{
    int i = 0;
    int flag = 0;
    while (i < P_ALL_NUM)
    {
        int p_D = rand() % 4;

        double min = sqrt2(d[0][0] - Pass[i].p_X, d[0][1] - Pass[i].p_Y);
        Pass[i].p_xD = d[0][0];
        Pass[i].p_yD = d[0][1];
        for (int k = 1; k < 4; k++)
        {
            double tmp = sqrt2(d[k][0] - Pass[i].p_X, d[k][1] - Pass[i].p_Y);
            if (min > tmp)
            {
                Pass[i].p_xD = d[k][0];
                Pass[i].p_yD = d[k][1];
                min = tmp;
            }
        }
        i += 1;
    }
}

/* --------------------------------------------------------- *
 *  関数名 : pass_input_inter_init                           *
 *  機能 : 交差点配列に待っている乗客情報を初期入力.         *
 *  仮引数 : なし                                            *
 * --------------------------------------------------------- */
void pass_input_inter_init(void)
{
    for (int i = 0; i < P_ALL_NUM; i++)
    {
        enqueue(i);
    }
}

/* --------------------------------------------------------- *
 *  関数名 : Dist_add_p                                      *
 *  機能 : 乗客の追加配置.                                   *
 *  仮引数 : なし                                            *
 * --------------------------------------------------------- */
void Dist_add_p(void)
{
    // 追加配置のロジック（必要に応じて実装）
}
