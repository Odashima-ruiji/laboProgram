#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
score_10_20.csvファイルからscore, dens, dist, W_gridの値を読み込んで折れ線グラフを作成
"""

import matplotlib.pyplot as plt
import numpy as np

def read_score_20_20_csv(filename):
    """
    score.csvファイルを読み込み、score, dens, dist, W_gridの配列を返す
    """
    score_data = []
    dens_data = []
    dist_data = []
    wgrid_data = []
    
    current_section = None
    
    with open(filename, 'r') as f:
        for line in f:
            line = line.strip()
            
            # セクションヘッダーの判定
            if line == 'score':
                current_section = 'score'
                continue
            elif line == 'dens':
                current_section = 'dens'
                continue
            elif line == 'dist':
                current_section = 'dist'
                continue
            elif line == 'W_grid':
                current_section = 'W_grid'
                continue
            
            # 数値データの読み込み
            try:
                value = float(line)
                if current_section == 'score':
                    score_data.append(value)
                elif current_section == 'dens':
                    dens_data.append(value)
                elif current_section == 'dist':
                    dist_data.append(value)
                elif current_section == 'W_grid':
                    wgrid_data.append(value)
            except ValueError:
                # 数値でない行はスキップ
                continue
    
    return np.array(score_data), np.array(dens_data), np.array(dist_data), np.array(wgrid_data)


def plot_data(score_data, dens_data, dist_data, wgrid_data, save_filename='score_plot.png'):
    """
    4つのデータを折れ線グラフとして描画
    """
    # 0でない値のみを抽出（実際のシミュレーション時間のみ）
    # Twaitまでのデータのみを取得
    non_zero_indices = np.where(score_data != 0)[0]
    if len(non_zero_indices) > 0:
        max_index = non_zero_indices[-1] + 1
    else:
        max_index = len(score_data)
    
    # 時間軸（ステップ数） - 500からスタート
    time_steps = np.arange(500, 500 + max_index)
    
    # 4つのサブプロットを作成
    fig, axes = plt.subplots(4, 1, figsize=(12, 12))
    fig.suptitle('Node0 Score Components Over Time', fontsize=16, fontweight='bold')
    
    # Score のプロット
    axes[0].plot(time_steps, score_data[:max_index], color='blue', linewidth=1.5)
    axes[0].set_ylabel('Score', fontsize=12)
    axes[0].set_title('Total Score', fontsize=12)
    axes[0].grid(True, alpha=0.3)
    axes[0].axhline(y=0, color='red', linestyle='--', linewidth=0.8, alpha=0.5)
    
    # Density (normalized) のプロット
    dens_nonzero_count = np.count_nonzero(dens_data[:max_index])
    if dens_nonzero_count > 0:
        axes[1].plot(time_steps, dens_data[:max_index], color='green', linewidth=1.5)
    else:
        axes[1].text(0.5, 0.5, 'No Data (All Zero)', ha='center', va='center', 
                    transform=axes[1].transAxes, fontsize=14, color='red')
    axes[1].set_ylabel('Normalized Density', fontsize=12)
    axes[1].set_title('Density Component (Normalized)', fontsize=12)
    axes[1].grid(True, alpha=0.3)
    
    # Distance (normalized) のプロット
    dist_nonzero_count = np.count_nonzero(dist_data[:max_index])
    if dist_nonzero_count > 0:
        axes[2].plot(time_steps, dist_data[:max_index], color='orange', linewidth=1.5)
    else:
        axes[2].text(0.5, 0.5, 'No Data (All Zero)', ha='center', va='center', 
                    transform=axes[2].transAxes, fontsize=14, color='red')
    axes[2].set_ylabel('Normalized Distance', fontsize=12)
    axes[2].set_title('Distance Component (Normalized)', fontsize=12)
    axes[2].grid(True, alpha=0.3)
    
    # W_grid のプロット
    wgrid_nonzero_count = np.count_nonzero(wgrid_data[:max_index])
    if wgrid_nonzero_count > 0:
        axes[3].plot(time_steps, wgrid_data[:max_index], color='purple', linewidth=1.5)
    else:
        axes[3].text(0.5, 0.5, 'No Data (All Zero)', ha='center', va='center', 
                    transform=axes[3].transAxes, fontsize=14, color='red')
    axes[3].set_xlabel('Time Step (Twait)', fontsize=12)
    axes[3].set_ylabel('W_grid (Penalty)', fontsize=12)
    axes[3].set_title('Visit Penalty (W_grid)', fontsize=12)
    axes[3].grid(True, alpha=0.3)
    
    # 全てのサブプロットにx軸範囲を500-3000に固定
    for ax in axes:
        ax.set_xlim(500, 3000)
        ax.set_ylim(0, 20)
    # scoreのy軸は0-120に設定
    axes[0].set_ylim(0, 120)
    
    plt.tight_layout()
    plt.savefig(save_filename, dpi=300, bbox_inches='tight')
    print(f'グラフを {save_filename} に保存しました')
    plt.show()


def plot_combined(score_data, dens_data, dist_data, wgrid_data, save_filename='score_combined.png'):
    """
    4つのデータを1つのグラフに重ねて表示（正規化済み）
    """
    # 0でない値のみを抽出
    non_zero_indices = np.where(score_data != 0)[0]
    if len(non_zero_indices) > 0:
        max_index = non_zero_indices[-1] + 1
    else:
        max_index = len(score_data)
    
    # 時間軸（500からスタート）
    time_steps = np.arange(500, 500 + max_index)
    
    plt.figure(figsize=(14, 6))
    
    # Scoreは必ずプロット
    plt.plot(time_steps, score_data[:max_index], label='Score', color='blue', linewidth=1.5, alpha=0.7)
    
    # Densityは0でない場合のみプロット
    dens_nonzero_count = np.count_nonzero(dens_data[:max_index])
    if dens_nonzero_count > 0:
        plt.plot(time_steps, dens_data[:max_index], label='Density (normalized)', color='green', linewidth=1.5, alpha=0.7)
    
    # Distanceは0でない場合のみプロット
    dist_nonzero_count = np.count_nonzero(dist_data[:max_index])
    if dist_nonzero_count > 0:
        plt.plot(time_steps, dist_data[:max_index], label='Distance (normalized)', color='orange', linewidth=1.5, alpha=0.7)
    
    # W_gridは0でない場合のみプロット
    wgrid_nonzero_count = np.count_nonzero(wgrid_data[:max_index])
    if wgrid_nonzero_count > 0:
        plt.plot(time_steps, wgrid_data[:max_index], label='W_grid (penalty)', color='purple', linewidth=1.5, alpha=0.7)
    
    plt.xlabel('Time Step (Twait)', fontsize=12)
    plt.ylabel('Value', fontsize=12)
    plt.title('Node0 Score Components Over Time (Combined)', fontsize=14, fontweight='bold')
    plt.legend(loc='best', fontsize=11)
    plt.grid(True, alpha=0.3)
    plt.axhline(y=0, color='red', linestyle='--', linewidth=0.8, alpha=0.5)
    
    # データなしの警告を表示
    missing_data = []
    if dens_nonzero_count == 0:
        missing_data.append("Density")
    if dist_nonzero_count == 0:
        missing_data.append("Distance")
    if wgrid_nonzero_count == 0:
        missing_data.append("W_grid")
    
    if missing_data:
        warning_text = "Note: " + ", ".join(missing_data) + " data is all zero"
        plt.text(0.5, 0.95, warning_text, ha='center', va='top', 
                transform=plt.gca().transAxes, fontsize=10, color='red', 
                bbox=dict(boxstyle='round', facecolor='yellow', alpha=0.5))
    
    # x軸範囲を500-3000に固定
    plt.xlim(500, 3000)
    # y軸範囲を0-20に固定
    plt.ylim(0, 20)
    
    plt.tight_layout()
    plt.savefig(save_filename, dpi=300, bbox_inches='tight')
    print(f'グラフを {save_filename} に保存しました')
    plt.show()


def print_statistics(score_data, dens_data, dist_data, wgrid_data):
    """
    データの統計情報を表示
    """
    # 0でないデータのみを対象
    score_nonzero = score_data[score_data != 0]
    dens_nonzero = dens_data[dens_data != 0]
    dist_nonzero = dist_data[dist_data != 0]
    wgrid_nonzero = wgrid_data[wgrid_data != 0]
    
    print("\n===== 統計情報 =====")
    print(f"Scoreデータ数: {len(score_nonzero)}")
    print(f"Densデータ数: {len(dens_nonzero)}")
    print(f"Distデータ数: {len(dist_nonzero)}")
    print(f"W_gridデータ数: {len(wgrid_nonzero)}")
    
    print("\n[Score]")
    if len(score_nonzero) > 0:
        print(f"  平均: {np.mean(score_nonzero):.4f}")
        print(f"  標準偏差: {np.std(score_nonzero):.4f}")
        print(f"  最小値: {np.min(score_nonzero):.4f}")
        print(f"  最大値: {np.max(score_nonzero):.4f}")
    else:
        print("  データなし")
    
    print("\n[Density (normalized)]")
    if len(dens_nonzero) > 0:
        print(f"  平均: {np.mean(dens_nonzero):.4f}")
        print(f"  標準偏差: {np.std(dens_nonzero):.4f}")
        print(f"  最小値: {np.min(dens_nonzero):.4f}")
        print(f"  最大値: {np.max(dens_nonzero):.4f}")
    else:
        print("  データなし（全て0）")
    
    print("\n[Distance (normalized)]")
    if len(dist_nonzero) > 0:
        print(f"  平均: {np.mean(dist_nonzero):.4f}")
        print(f"  標準偏差: {np.std(dist_nonzero):.4f}")
        print(f"  最小値: {np.min(dist_nonzero):.4f}")
        print(f"  最大値: {np.max(dist_nonzero):.4f}")
    else:
        print("  データなし（全て0）")
    
    print("\n[W_grid (penalty)]")
    if len(wgrid_nonzero) > 0:
        print(f"  平均: {np.mean(wgrid_nonzero):.4f}")
        print(f"  標準偏差: {np.std(wgrid_nonzero):.4f}")
        print(f"  最小値: {np.min(wgrid_nonzero):.4f}")
        print(f"  最大値: {np.max(wgrid_nonzero):.4f}")
    else:
        print("  データなし（全て0）")
    print("==================\n")


if __name__ == '__main__':
    # CSVファイルの読み込み
    csv_filename = 'build/app/score_20_20.csv'
    
    try:
        score_data, dens_data, dist_data, wgrid_data = read_score_20_20_csv(csv_filename)
        print(f'{csv_filename} を読み込みました')
        print(f'Score データ数: {len(score_data)}')
        print(f'Dens データ数: {len(dens_data)}')
        print(f'Dist データ数: {len(dist_data)}')
        print(f'W_grid データ数: {len(wgrid_data)}')
        
        # 統計情報の表示
        print_statistics(score_data, dens_data, dist_data, wgrid_data)
        
        # グラフの作成
        plot_data(score_data, dens_data, dist_data, wgrid_data, 'score_plot_20_20.png')
        #plot_combined(score_data, dens_data, dist_data, wgrid_data, 'score_combined.png')
        
    except FileNotFoundError:
        print(f'エラー: {csv_filename} が見つかりません')
    except Exception as e:
        print(f'エラーが発生しました: {e}')
