/*
    main.c //メインファイル
    サービスエリア20x20 交差点間距離50m
    車両は自由探索
    エリア内の位置の違いによる到着時間の変化を表すヒートマップを表示
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

////////////////////////////////////////////////
// 設定・構造体・グローバル変数・関数のヘッダ読み込み //
////////////////////////////////////////////////
#include "config.h"
#include "globals.h"
#include "base_struct.h"
#include "base_func.h"
#include "placement.h"
#include "Common_func.h"
#include "move.h"
#include "my_func.h"

////////////////
// メイン関数   //
////////////////

int main()
{

    // csv出力のための設定----------------------------------------------------------------------------------------------
    // csv出力するための数値
    FILE *fp;
    char *fname = "test_dens3_dist8.csv";

    char *node = "node";
    char *node0 = "node0";
    char *node1 = "node1";
    char *node2 = "node2";
    char *node3 = "node3";
    char *node4 = "node4";

    fp = fopen(fname, "w");
    if (fp == NULL)
    {
        printf("%sファイルが開けません¥n", fname);
    }

    FILE *fp2;
    fp2 = fopen("speed_counter.csv", "w");

    // csv出力のための説明
    // fprintf(fp,"500人必要ステップ,平均ステップ,一人あたりの乗っているステップ数,一人あたりの待っているステップ数,探索モードで乗せた回数,探索モードで目的地に人がいない場合,情報交換数,情報獲得数\n");

    // 乱数の種を与える
    // srand( ( unsigned int )time( NULL ) );
    //srand((int)RANDOM_SEED + n);
    //srand(2);
    syokika();

    // for(int l =0;l<100;l++){
    /* gnuplotで出力-------------------------------------------------------------------------------------------------- */
    // gnuplotで描画する
    //シミュレーションgif
    FILE *gp;
    gp = popen("gnuplot", "w"); // パイプを開きgnuplotを立ち上げ（batch モード）
    // パイプを通してgnuplotにコマンドを送る
    fprintf(gp, "set term gif animate delay 2\n"); // 出力ターミナルをgif, animateオプションを付ける
#ifdef EPIDEMIC
    fprintf(gp, "set output 'model_animation_EPIDEMIC.gif'\n"); // 出力ファイル名。
#else
    fprintf(gp, "set output 'model_animation_random.gif'\n"); // 出力ファイル名。
#endif
    fprintf(gp, "set size square\n");      // グラフを正方形にする
    fprintf(gp, "set xrange[0:%d]\n", Ax); // x軸の範囲を決定する
    fprintf(gp, "set yrange[0:%d]\n", Ay); // y軸の範囲を決定する
    fprintf(gp, "set xlabel \"x\"\n");     // x軸のラベルを指定する
    fprintf(gp, "set ylabel \"y\"\n");     // y軸のラベルを指定する
    // fprintf(gp, "set grid lt -1 lw 0.5, lt -1 lw 0.5\n"); //グリッド線を入れる
    fprintf(gp, "set mxtics\n");
    fprintf(gp, "set mytics\n");
    fprintf(gp, "set grid xtics mxtics \n");
    fprintf(gp, "set grid ytics mytics \n");
    fprintf(gp, "unset key\n");

#ifdef circle
    fprintf(gp, "set object 1 circle at %f,%f size %f  border rgb \t0xFF0000\" lw 2\n", center_x, center_y, area_disaster);
// fprintf(gp, "set object 2 circle at %f,%f size %f  border rgb \"red\" lw 2\n",center_x, center_y, area_evacuation);
#else
    // エリアの配置(四角)
    fprintf(gp, "set arrow 1 from %d,%d to %d,%d nohead lc rgb \"black\" lw 2\n", Ax_d, Ay_d, Ax - Ax_d, Ay_d);
    fprintf(gp, "set arrow 2 from %d,%d to %d,%d nohead lc rgb \"black\" lw 2\n", Ax - Ax_d, Ay_d, Ax - Ax_d, Ay - Ay_d);
    fprintf(gp, "set arrow 3 from %d,%d to %d,%d nohead lc rgb \"black\" lw 2\n", Ax - Ax_d, Ay - Ay_d, Ax_d, Ay - Ay_d);
    fprintf(gp, "set arrow 4 from %d,%d to %d,%d nohead lc rgb \"black\" lw 2\n", Ax_d, Ay - Ay_d, Ax_d, Ay_d);
#endif
    fprintf(gp, "set title \"model animation  count:0 time:0\"\n");
    //Mapgif
    FILE *gp_map;
    gp_map = popen("gnuplot", "w");
    fprintf(gp_map, "set term gif animate delay 2\n");
    fprintf(gp_map, "set output 'node_map_animation.gif'\n");
    fprintf(gp_map, "set size square\n");
    fprintf(gp_map, "set xrange[0:%d]\n", Ax);
    fprintf(gp_map, "set yrange[0:%d]\n", Ay);
    fprintf(gp_map, "set xlabel \"x\"\n");
    fprintf(gp_map, "set ylabel \"y\"\n");
    fprintf(gp_map, "set cblabel \"Info (0=no passenger, 1=passenger waiting)\"\n");
    fprintf(gp_map, "set palette defined (0 'blue', 1 'red')\n");
    fprintf(gp_map, "set cbrange[0:1]\n");
    fprintf(gp_map, "unset key\n");
    // 円を表示
    fprintf(gp_map, "set object 1 circle at %f,%f size %f border rgb 'black' lw 2 fillstyle empty\n", center_x, center_y, area_disaster);

    // 待ち客とノード位置を表示する別のGIF
    FILE *gp_waiting;
    gp_waiting = popen("gnuplot", "w");
    fprintf(gp_waiting, "set term gif animate delay 2\n");
    fprintf(gp_waiting, "set output 'waiting_passengers_animation.gif'\n");
    fprintf(gp_waiting, "set size square\n");
    fprintf(gp_waiting, "set xrange[0:%d]\n", Ax);
    fprintf(gp_waiting, "set yrange[0:%d]\n", Ay);
    fprintf(gp_waiting, "set xlabel \"x\"\n");
    fprintf(gp_waiting, "set ylabel \"y\"\n");
    fprintf(gp_waiting, "unset key\n");
    // 円を表示
    fprintf(gp_waiting, "set object 1 circle at %f,%f size %f border rgb 'black' lw 2 fillstyle empty\n", center_x, center_y, area_disaster);

    // --------------------------------------------------------------------------------------------------------------------
    for (int jc = 0; jc < 5; jc++)
    {
        // 各ループで乱数シードを設定
        srand((int)RANDOM_SEED + jc + 1);
        
        Twait = 0;
        allstep = 0;
        count_ifgo = 0;  // 情報に従うノードがデータをもとに目的地を設定した回数
        count_ride1 = 0; // 乗客が一人目を乗せた回数
        count_ride2 = 0; // 乗客が二人目を乗せた回数
        count_ride3 = 0; // 情報フローティングで得た情報をもとに乗客を拾った回数
        ride_transmit = 0;
        count_p_no = 0; // 情報に従うノードが目的地に到着したが乗客がいなかった回数
        count_p_on_3 = 0;
        ridecount = 0;  // 乗客が乗っていたステップ
        waitcount = 0;  // 乗客が待っていたステップ
        distancemass = 0;
        sum_P_Twait = 0;
        for (int i = 0; i < 10000; i++)
        {
            all_count[i] = P_ALL_NUM;
        }

        total_transmit = 0;
        get_info = 0;        // 情報を取得した回数をカウント
        traffic_counter = 0; // 交通量をカウント
        count_same = 0;
        count_off = 0;
        syokika(); // 初期化

        // 領域の描画
        Dist_Init_p(); // 乗客の初期配置
        Dist_Init_n(); // ノードの初期配置
        Dist_Init_n_D();
        Init_W_map(); // ノードのW_map初期化

        fprintf(gp, "plot '-' with points pointtype 5\n");
        for (int i = 0; i < N_ALL_NUM; i++)
        {
            fprintf(gp, "%f\t%f\n", Node[i].n_X, Node[i].n_Y);
        }
        fprintf(gp, "e\n");

        // //スピードカウンタの初期化
        // for(int i=0; i<10000; i++){
        //     speed_conter2[i] = 15;
        // }
        // for(int i=0; i<Ax; i++){
        //     for(int j=0; j<Ay; j++){
        //         tra_count_x_plus[i][j] = 0;
        //         tra_count_x_minus[i][j] = 0;
        //         tra_count_y_plus[i][j] = 0;
        //         tra_count_y_minus[i][j] = 0;
        //     }
        // }

        // 利用要求がなくなるまで（利用者がいなくなるまで）輸送を行う-----------------------------------------------------------------------------
        while (P_check() < P_goal)
        { // 乗客が残っているかの判定 0なら乗客が残っている 1なら残っていない

            // Dist_add_p(); //乗客の追加配置
            //  allrideon(); // 乗客を乗せる
            Move(); // ノードの移動
/*for(int d=0; d<N_ALL_NUM; d++){
    //if(d=1){
        fprintf(fp2,"%d,Node[%d] = [%f,%f] \n",Twait,d,Node[d].n_X,Node[d].n_Y);
        fprintf(fp2,"%d,Node[%d].目的地 = [%d,%d] \n",Twait,d,Node[d].n_insec_X,Node[d].n_insec_Y);
    //}
}*/
#ifdef EPIDEMIC
            flooding();
#endif
            // stack_sort();
            // D_check();  // 乗客の目的地判定
            check_ride(); // 乗客の乗待状況カウント

            fprintf(gp, "plot '-' with points pointtype 5 ps variable lc rgb var\n");
            // hubを示す固定点 -------------------------------------------------------------------
            //  fprintf(gp, "1\t1\t2\t0x000000\n");
            //  fprintf(gp, "1\t49\t2\t0x000000\n");
            //  fprintf(gp, "49\t49\t2\t0x000000\n");
            //  fprintf(gp, "49\t1\t2\t0x000000\n");
            // hubを示す可変点 -------------------------------------------------------------------
            fprintf(gp, "%d\t%d\t2\t0x000000\n", d[0][0], d[0][1]);
            fprintf(gp, "%d\t%d\t2\t0x000000\n", d[1][0], d[1][1]);
            fprintf(gp, "%d\t%d\t2\t0x000000\n", d[2][0], d[2][1]);
            fprintf(gp, "%d\t%d\t2\t0x000000\n", d[3][0], d[3][1]);

            for (int i = 0; i < N_ALL_NUM; i++)
            {
                // printf("pre_Node[%d].p_on=%d\n",i,pre_Node[i].p_on);
                if (Node[i].p_on == 0)
                {
                    // 無人探索時：黒
                    fprintf(gp, "%f\t%f\t1\t0x000000\n", Node[i].n_X, Node[i].n_Y);
                }
                else if (Node[i].p_on == 1)
                {
                    // 乗客を乗せて目的地へ向かっているとき：赤
                    fprintf(gp, "%f\t%f\t1\t0xFF0000\n", Node[i].n_X, Node[i].n_Y);
                }
                else if (Node[i].p_on == 2)
                {
                    // 乗客を２人乗せているとき：水色
                    fprintf(gp, "%f\t%f\t1\t0x43A1FF\n", Node[i].n_X, Node[i].n_Y);
                }
                else if (Node[i].p_on == 3)
                {
                    // 情報を得て待ち客がいる目的地へ移動しているとき：黄緑
                    fprintf(gp, "%f\t%f\t1\t0x3BCE00\n", Node[i].n_X, Node[i].n_Y);
                }
                else if (Node[i].p_on == 5)
                {
                    fprintf(gp, "%f\t%f\t1\t0x43A1FF\n", Node[i].n_X, Node[i].n_Y);
                }
            }
            fprintf(gp, "e\n");

            //車両と乗客の情報マップ表示GIF
            fprintf(gp_map, "set title \"Node[0] Map Info - Time:%d Position:(%.1f,%.1f)\"\n", Twait, Node[0].n_X, Node[0].n_Y);
            fprintf(gp_map, "plot '-' using 1:2:3 with image, '-' using 1:2:($3) with points pt 7 ps 2 lc rgb variable\n");
            for (int i = 0; i < Ax; i++) {
                for (int j = 0; j < Ay; j++) {
                    fprintf(gp_map, "%d %d %d\n", i, j, Node[0].Map[i][j].info);
                }
                fprintf(gp_map, "\n"); // 行の区切り
            }
            fprintf(gp_map, "e\n");
            // Node[0]の位置のみをプロット（p_onに応じて色分け）
            if (Node[0].p_on == 0) {
                // 無人探索時：黒
                fprintf(gp_map, "%f %f 0x000000\n", Node[0].n_X, Node[0].n_Y);
            }
            else if (Node[0].p_on == 1) {
                // 乗客を乗せて目的地へ向かっているとき：赤
                fprintf(gp_map, "%f %f 0xFF0000\n", Node[0].n_X, Node[0].n_Y);
            }
            else if (Node[0].p_on == 2) {
                // 乗客を２人乗せているとき：水色
                fprintf(gp_map, "%f %f 0x43A1FF\n", Node[0].n_X, Node[0].n_Y);
            }
            else if (Node[0].p_on == 3) {
                // 情報を得て待ち客がいる目的地へ移動しているとき：黄緑
                fprintf(gp_map, "%f %f 0x3BCE00\n", Node[0].n_X, Node[0].n_Y);
            }
            else if (Node[0].p_on == 5) {
                // p_on==5のとき：水色
                fprintf(gp_map, "%f %f 0x43A1FF\n", Node[0].n_X, Node[0].n_Y);
            }
            fprintf(gp_map, "e\n");

            // 待ち客とノードの位置を表示（別のGIF）
            fprintf(gp_waiting, "set title \"Waiting Passengers - Time:%d\"\n", Twait);
            fprintf(gp_waiting, "plot '-' using 1:2 with points pt 5 ps 1.5 lc rgb 'black' title 'Waiting', '-' using 1:2 with points pt 7 ps 2 lc rgb 'green' title 'Node[50]'\n");
            // 待ち客の位置をプロット（黒色）
            for (int i = 0; i < Ax; i++) {
                for (int j = 0; j < Ay; j++) {
                    if (Trans[i][j].wp_Exist == 1) {
                        fprintf(gp_waiting, "%d %d\n", i, j);
                    }
                }
            }
            fprintf(gp_waiting, "e\n");
            // ノードの位置をプロット（緑色）
            fprintf(gp_waiting, "%f %f\n", Node[50].n_X, Node[50].n_Y);
            fprintf(gp_waiting, "e\n");

            

            // fprintf( fp2, "%d,%f,%f,\n", Twait,Node[0].n_X,Node[0].n_Y); //ノードの座標 csvファイル出力

            /*printf("\n現在位置(%d,%d) 目的地(%d,%d) 客1番号 %d 客2番号 %d 乗車人数 %d\n",Node[1].n_X,Node[1].n_Y,Node[1].n_xD,Node[1].n_yD,Node[1].p_num,Node[1].p_num2,Node[1].p_on);
            printf("客１目的地(%d,%d) 客２目的地(%d,%d) 現在地に待ち客がいるか %d\n",Pass[Node[1].p_num].p_xD,Pass[Node[1].p_num].p_yD,Pass[Node[1].p_num2].p_xD,Pass[Node[1].p_num2].p_yD,Trans[Node[1].n_X][Node[1].n_Y].wp_Exist);
            printf("持っているデータ%d , %d, %d, %d, %d, %d, %d\n",Node[1].stack_data[0],Node[1].stack_data[1],Node[1].stack_data[2],Node[1].stack_data[3],Node[1].stack_data[4],Node[1].stack_data[5],Node[1].stack_data[6]);
            printf("持っているnoデータ %d , %d , %d, %d\n",Node[1].stack_nopos[Node[1].stack_num2-1],Node[1].stack_nopos[Node[1].stack_num2-2],Node[1].stack_nopos[Node[1].stack_num2-3],Node[1].stack_nopos[Node[1].stack_num2-4]);
            printf("到着した乗客数　%d \n",check);
            printf("総客数(到着済 + 未到着) %d\n\n",P_ALL_NUM  + p_add_count);*/

            Twait += 1;
            // エラー判定
            if (Twait > 15000)
            {
                for (int l = 0; l < P_ALL_NUM; l++)
                {
                    if (Pass[l].p_X != Pass[l].p_xD || Pass[l].p_Y != Pass[l].p_yD)
                    {
                        fprintf(fp2, "Pass[%d].p_X=%d, Pass[%d].p_Y=%d\n", l, Pass[l].p_X, l, Pass[l].p_Y);
                        fprintf(fp2, "Trans[%d][%d].wp_Exist=%d\n", Pass[l].p_X, Pass[l].p_Y, Trans[Pass[l].p_X][Pass[l].p_Y].wp_Exist);
                    }
                }
                exit(1);
            }

            // ノード対利用客の情報交換記録を一定時間ごとに初期化
            for (int i = 0; i < N_ALL_NUM; i++)
            {
                for (int j; j < P_ALL_NUM; j++)
                {
                    if (Twait - transmit__[i][j] > reftime)
                    {
                        transmit__[i][j] = 0;
                    }
                }
            }

            printf("経過時間 %d\n", Twait);
            printf("情報取得回数 : %d\n", get_info);
            printf("\n");
            printf("%d\n", jc);

            fprintf(gp, "set title \"model animation  count:%d time:%d\"\n", o, Twait);
            fprintf(gp, "set label 1 \"count:%d\"at %d, %d\n", check_1, d[0][0] + 1, d[0][1] + 3);
            fprintf(gp, "set label 2 \"count:%d\"at %d, %d\n", check_2, d[1][0] + 1, d[1][1] + 3);
            fprintf(gp, "set label 3 \"count:%d\"at %d, %d\n", check_3, d[2][0] + 1, d[2][1] + 3);
            fprintf(gp, "set label 4 \"count:%d\"at %d, %d\n", check_4, d[3][0] + 1, d[3][1] + 3);
            // all10count[Twait] =  P_check();
            all_count[Twait] = P_check();
            // traffic_counter2[Twait] = traffic_counter;
        }
        // printf("%d,%d\n",Node[1].stack_num,Node[1].stack_num2);

        // csvファイルに記述----------------------------------------------------------------------------------------------------------------
        // 500人必要ステップ、平均ステップ、一人あたりの乗っているステップ数、一人あたりの待っているステップ数,探索モードで乗せた回数、探索モードで目的地に人がいない場合,情報交換数、情報獲得数
        // fprintf(fp,"%d,%f,%f,%f,%d,%d,%d,%d,",Twait,allstep/(double)P_goal,ridecount/(double)P_goal,waitcount/(double)P_goal,count_ride3,count_p_no,total_transmit,get_info);
        // fprintf(fp,"得た情報数：");
        // for(int o=1; o<(Twait/10)+1;o++){
        // fprintf(fp,",%d",goalstep[o]);
        // fprintf(fp,",%d",Node[o].all_stack_num);
        // fprintf(fp, "%d,",traffic_counter2[o] /*- traffic_counter2[o-1]*/);
        //  fprintf(fp2, "%.1f,",speed_conter2[o]);
        //}
        // fprintf(fp, "%d,",total_transmit);
        // fprintf(fp, "traffic_counter : ");
        // fprintf(fp, "%d, ",traffic_counter);

        // pushスタックの中身を確認
        /*fprintf(fp,"push \n");
        for(int i=0;i<push_num;i++){
            fprintf(fp,"%d, \n",stack_push[i]);
        }*/

        // 情報取得回数と完了時間を確認
        // n, get_info, total_transmit, Twait
        fprintf(fp, "実行回数 %d, get_info %d, total_transmit %d, Twait %d, 1 x=%d y=%d, 2 x=%d y=%d, 3 x=%d y=%d, 4 x=%d y=%d\n", jc, get_info, total_transmit, Twait, d[0][0], d[0][1], d[1][0], d[1][1], d[2][0], d[2][1], d[3][0], d[3][1]);
        // fprintf(fp,"\n");
        fprintf(fp, "一人目を乗せた数 %d, 二人目を乗せた数 %d, 情報フローティングによる乗車数 %d, 通信による乗車数 %d, p_on == 3になる回数 %d, マップによる移動 %d\n", count_ride1, count_ride2, count_ride3, ride_transmit, count_p_on_3, count_map);
        fprintf(fp, "%d\n", count_same);

        // for(int k = 1 ; k <= Twait/10 ; k++){
        //     sum_all10count[k] += all10count[k*10] - all10count[(k-1)*10];
        //     //printf("%d\n",traffic_counter2[k] - traffic_counter2[k-1]);
        // }

        for(int i = 0; i < P_ALL_NUM; i++){
            if (Pass[i].p_wait > 0)
            {
                sum_P_Twait += Pass[i].p_wait;
            }
        }
        fprintf(fp, "乗客一人あたりの平均待ち時間 %f\n", sum_P_Twait / (double)P_ALL_NUM);

        for (int k = 1; k <= 300; k++)
        {
            fprintf(fp, "%d, %d\n", k * 10, all_count[k * 10] - all_count[(k - 1) * 10]);
        }

        fprintf(fp, "100mごと\n");

        for (int i = 0; i < P_ALL_NUM; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                if (sqrt2(center_x - Pass[i].p_xS, center_y - Pass[i].p_yS) <= j + 1 && sqrt2(center_x - Pass[i].p_xS, center_y - Pass[i].p_yS) > j)
                {
                    range[j] += Pass[i].p_wait;
                    range_count[j] += 1;
                }
            }
        }

        // 平均を先に計算
        double range_avg[15] = {0};
        for (int i = 0; i < 15; i++)
        {
            if (range_count[i] > 0) {
                range_avg[i] = range[i] / range_count[i];
            }
        }

        // 分散を計算
        double range_variance[15] = {0};
        for (int i = 0; i < P_ALL_NUM; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                if (sqrt2(center_x - Pass[i].p_xS, center_y - Pass[i].p_yS) <= j + 1 && sqrt2(center_x - Pass[i].p_xS, center_y - Pass[i].p_yS) > j)
                {
                    double diff = Pass[i].p_wait - range_avg[j];
                    range_variance[j] += diff * diff;
                }
            }
        }

        for (int i = 0; i < 15; i++)
        {
            double std_dev = 0;
            if (range_count[i] > 0) {
                range_variance[i] /= range_count[i];
                std_dev = sqrt(range_variance[i]);
            }
            fprintf(fp, "中心からの距離 %d-%d ,平均, %f ,標準偏差, %f\n", i, i + 1, range_avg[i], std_dev);
        }

        fprintf(fp, "500mごと(探索エリアごと)\n");

        // 500mごとの距離範囲で平均と標準偏差を計算（0-5, 5-10, 10-15）
        double range_500m[3] = {0};       // 合計
        int range_500m_count[3] = {0};    // カウント
        double range_500m_avg[3] = {0};   // 平均
        
        for (int i = 0; i < P_ALL_NUM; i++)
        {
            double distance = sqrt2(center_x - Pass[i].p_xS, center_y - Pass[i].p_yS);
            for (int j = 0; j < 3; j++)
            {
                if (distance <= (j + 1) * 5 && distance > j * 5)
                {
                    range_500m[j] += Pass[i].p_wait;
                    range_500m_count[j] += 1;
                }
            }
        }
        
        // 平均を計算
        for (int i = 0; i < 3; i++)
        {
            if (range_500m_count[i] > 0) {
                range_500m_avg[i] = range_500m[i] / range_500m_count[i];
            }
        }
        
        // 分散を計算
        double range_500m_variance[3] = {0};
        for (int i = 0; i < P_ALL_NUM; i++)
        {
            double distance = sqrt2(center_x - Pass[i].p_xS, center_y - Pass[i].p_yS);
            for (int j = 0; j < 3; j++)
            {
                if (distance <= (j + 1) * 5 && distance > j * 5)
                {
                    double diff = Pass[i].p_wait - range_500m_avg[j];
                    range_500m_variance[j] += diff * diff;
                }
            }
        }
        
        // 標準偏差を出力
        for (int i = 0; i < 3; i++)
        {
            double std_dev = 0;
            if (range_500m_count[i] > 0) {
                range_500m_variance[i] /= range_500m_count[i];
                std_dev = sqrt(range_500m_variance[i]);
            }
            fprintf(fp, "中心からの距離 %d00m-%d00m ,平均, %f ,標準偏差, %f\n", i * 5, (i + 1) * 5, range_500m_avg[i], std_dev);
        }

        // 指定区間内の交通量
        sum_Twait += Twait;

        // printf("%d,%d\n",Node[1].stack_num,Node[1].stack_num2);

        // --- ヒートマップを生成 ---
        // FILE *data_fp = fopen("points_data.dat", "w");
        // if (data_fp == NULL) { return 1; }
        // for (int i = 0; i < P_ALL_NUM; i++) {
        //     // しきい値10以上のデータのみ出力する
        //     if (Pass[i].p_wait >= 0) {
        //         fprintf(data_fp, "%d\t%d\t%d\n", Pass[i].p_xS, Pass[i].p_yS, Pass[i].p_wait);

        //     }
        // }
        // fclose(data_fp);

        // // --- 3. gnuplotスクリプトを生成 ---
        // FILE *gp_pipe = fopen("plot_script.plt", "w");
        // if (gp_pipe == NULL) { return 1; }

        // // 出力形式は通常のPNGでOK
        // fprintf(gp_pipe, "set terminal pngcairo size 600,600\n");
        // fprintf(gp_pipe, "set output 'map_points_1.png'\n");
        // fprintf(gp_pipe, "set size square\n");
        // fprintf(gp_pipe, "set xrange[0:%d]\n", Ax);
        // fprintf(gp_pipe, "set yrange[0:%d]\n", Ay);
        // fprintf(gp_pipe, "set grid xtics mxtics ytics mytics lt -1 lc rgb \"gray50\"\n");
        // fprintf(gp_pipe, "set palette defined (0 'blue', 1 'red')\n");
        // fprintf(gp_pipe, "set cbrange [500:2500]\n");

        // // 円オブジェクトの設定
        // // double center_x = (double)Ax / 2.0;
        // // double center_y = (double)Ay / 2.0;
        // // fprintf(gp_pipe, "set object 1 circle at %f,%f size %f linewidth 2 border rgb \"#000000\"\n", center_x, center_y, 10.0);
        // // fprintf(gp_pipe, "set object 2 circle at %f,%f size %f linewidth 2 border rgb \"#000000\"\n", center_x, center_y, 20.0);

        // // ★★★ プロット方法を変更 ★★★
        // // with points: 点でプロット
        // // pt 5: 点の形を●に
        // // ps 1.5: 点のサイズを1.5倍に
        // // lc palette: パレットの色を適用
        // fprintf(gp_pipe, "plot 'points_data.dat' using 1:2:3 with points pt 5 ps 1.5 lc palette notitle\n");

        // fclose(gp_pipe);

        // // --- 4. gnuplotを実行 ---
        // system("gnuplot plot_script.plt");
        // printf("map_points.png が生成されました。\n");

        // 待ち客の初期位置分布
        FILE *gp_1 = popen("gnuplot", "w");
        if (gp_1 == NULL)
        {
            printf("Gnuplot cannot be opened.\n");
            return 1;
        }
        fprintf(gp_1, "set terminal pngcairo size 800,800 enhanced font 'Arial,12'\n");

        // 保存するファイル名を指定
        fprintf(gp_1, "set output 'simulation_map.png'\n");

        // --------------------------------------------------
        // 3. ご指定のマップ設定 (ここから)
        // --------------------------------------------------
        fprintf(gp_1, "set size square\n");      // グラフを正方形にする
        fprintf(gp_1, "set xrange[0:%d]\n", Ax); // x軸の範囲
        fprintf(gp_1, "set yrange[0:%d]\n", Ay); // y軸の範囲
        fprintf(gp_1, "set xlabel \"x [m]\"\n"); // x軸ラベル
        fprintf(gp_1, "set ylabel \"y [m]\"\n"); // y軸ラベル

        // グリッド設定
        fprintf(gp_1, "set mxtics\n");
        fprintf(gp_1, "set mytics\n");
        fprintf(gp_1, "set grid xtics mxtics\n");
        fprintf(gp_1, "set grid ytics mytics\n");
        fprintf(gp_1, "unset key\n"); // 凡例を消す

// 円（被災範囲）の描画設定
#ifdef circle
        // 修正: rgbの指定を \t ではなく \"#FF0000\" (赤) に修正しました
        fprintf(gp_1, "set object 1 circle at %f,%f size %f  border rgb \t0xFF0000\" lw 2\n", center_x, center_y, area_disaster);
#endif
        // --------------------------------------------------
        // マップ設定 (ここまで)
        // --------------------------------------------------

        // 4. 配列データをプロットするコマンド
        // '-' を指定すると、ファイルではなく「この後の入力データ」を使ってプロットします
        fprintf(gp_1, "plot '-' using 1:2 with points pt 7 ps 1.5 lc rgb \"blue\"\n");

        // 5. for文で配列の中身を1つずつ Gnuplot に送信
        for (int i = 0; i < P_ALL_NUM; i++)
        {
            fprintf(gp_1, "%d %d\n", Pass[i].p_xS, Pass[i].p_yS);
        }

        // 6. データ送信終了の合図
        fprintf(gp_1, "e\n");

        pclose(gp_1);

        n += 1;
    }

    // GIFパイプを閉じる
    pclose(gp_map);
    pclose(gp_waiting);

    double average = sum_Twait / n;
    fprintf(fp, "平均到着時間 %lf\n", average);

    // for(int k = 1; k<=250; k++){
    //         ave_all10count[k] = sum_all10count[k]/n;
    //         fprintf(fp, "%d, %lf\n", k*10, ave_all10count[k]);
    // }

    // printf("\nすべての乗客が目的地に到着しました\n");
    // printf("%d\n",Twait); /*総ステップ数*/
    // printf("%d\n",count_ifgo); /*情報に従うノードがデータをもとに目的地設定した回数*/
    // printf("%d\n",count_goalif);/*得た情報をもとに乗客を載せた回数*/
    printf("二人目の乗客を降ろした回数 %d\n", count_off); /*二人目の乗客をおろした回数 */
    // printf("%d\n\n",total_transmit); /*すべての通信回数*/
    fclose(fp);
    fclose(fp2);

    // 10秒ごとの到着人数の表示
    //  for(int k = 1 ; k <= Twait/10 ; k++){
    //      printf("%d\n",all10count[k*10] - all10count[(k-1)*10]);
    //      //printf("%d\n",traffic_counter2[k] - traffic_counter2[k-1]);
    //  }

    return 0;
}