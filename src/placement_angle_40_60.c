/*
    placement_angle_40_60.c : 角度ベース配置に関する関数 (実装)
    40%が中心方向(move_flag=10), 60%が外側エリア（角度ベース等分割配置）
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "placement_angle_40_60.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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
 *  機能 : ノードの初期配置を行う.（角度ベース等分割）       *
 *         円周を等分割した角度ごとに最も近い格子点を選択.   *
 *  仮引数 : なし                                            *
 * --------------------------------------------------------- */
void Dist_Init_n(void)
{
    /* 外側ノード数 (60%) と中心方向ノード数 (40%) */
    int num_outer = N_ALL_NUM * 6 / 10;

    /* 外側ノード: 角度ベース等分割配置 */
    for (int i = 0; i < num_outer; i++)
    {
        double angle = 2.0 * M_PI * i / num_outer;
        double target_r = area_disaster + 1.0; /* リングの中心半径 */
        double ideal_x = center_x + target_r * cos(angle);
        double ideal_y = center_y + target_r * sin(angle);

        /* 理想位置に最も近い有効格子点を探索 */
        int best_x = (int)round(ideal_x);
        int best_y = (int)round(ideal_y);
        double best_dist_sq = (best_x - ideal_x) * (best_x - ideal_x) + (best_y - ideal_y) * (best_y - ideal_y);
        int found = 0;

        /* 理想位置周辺の格子点を探索 */
        for (int dx = -2; dx <= 2; dx++)
        {
            for (int dy = -2; dy <= 2; dy++)
            {
                int tx = (int)round(ideal_x) + dx;
                int ty = (int)round(ideal_y) + dy;
                double dist_from_center = sqrt2(tx - center_x, ty - center_y);
                if (dist_from_center >= area_disaster && dist_from_center <= area_disaster + 2)
                {
                    double dist_sq = (tx - ideal_x) * (tx - ideal_x) + (ty - ideal_y) * (ty - ideal_y);
                    if (!found || dist_sq < best_dist_sq)
                    {
                        best_x = tx;
                        best_y = ty;
                        best_dist_sq = dist_sq;
                        found = 1;
                    }
                }
            }
        }

        if (found)
        {
            Node[i].n_X = best_x;
            Node[i].n_Y = best_y;
        }
        else
        {
            /* フォールバック: ランダム配置 */
            do
            {
                Node[i].n_X = randAxAy();
                Node[i].n_Y = randAxAy();
            } while ((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2));
        }
    }

    /* 中心方向へ向かう車両 (move_flag = 10) */
    for (int i = num_outer; i < N_ALL_NUM; i++)
    {
        do
        {
            Node[i].n_X = randAxAy();
            Node[i].n_Y = randAxAy();
            Node[i].move_flag = 10;
        } while ((sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) < area_disaster) || (sqrt2(Node[i].n_X - center_x, Node[i].n_Y - center_y) > area_disaster + 2));
    }

    /* 全ノードの共通初期化 */
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
 *  関数名 : Init_W_map                                      *
 *  機能 : Map_grid構造体のW_mapの初期値を設定.              *
 *         中心12×12グリッドは高い重み、外は低い重み.       *
 *  仮引数 : なし                                            *
 * --------------------------------------------------------- */
void Init_W_map(void)
{
    // 全ノードのMap_gridのW_mapを初期化
    // 中心12×12グリッド（インデックス4〜15）で避難エリアをカバー
    int grid_min = (grid_size / 2) - 6;  // 4
    int grid_max = (grid_size / 2) + 5;  // 15
    
    for (int node = 0; node < N_ALL_NUM; node++) {
        for (int i = 0; i < grid_size; i++) {
            for (int j = 0; j < grid_size; j++) {
                // 中心12×12グリッド内かどうかで重みを設定
                if (i >= grid_min && i <= grid_max && j >= grid_min && j <= grid_max) {
                    // 中心エリア内: 高い重み
                    Node[node].Map_grid[i][j].W_map = 50;
                } else {
                    // 中心エリア外: 低い重み
                    Node[node].Map_grid[i][j].W_map = 0;
                }
            }
        }
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
