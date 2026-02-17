import numpy as np
import matplotlib.pyplot as plt

# 条件設定
density_normalized = 1.0
distance_normalized = 1.0
W_grid = 0.0

# W_dens の値（config.hで定義されている値を使用）
# 実際の値が不明な場合は適当な値を設定
W_dens = 10.0  # 必要に応じて調整してください

# W_dist の範囲を設定（0から10まで）
W_dist_values = np.linspace(0, 10, 100)

# スコアを計算
# score = (W_dens * density_normalized) / (exp(W_dist * distance_normalized) + W_grid + 1.0)
scores = (W_dens * density_normalized) / (np.exp(W_dist_values * distance_normalized) + W_grid + 1.0)

# グラフを描画
plt.figure(figsize=(10, 6))
plt.plot(W_dist_values, scores, linewidth=2, color='blue')
plt.xlabel('W_dis', fontsize=20)
plt.ylabel('Score', fontsize=20)
plt.title(f'Score vs W_dist (density_normalized=1, distance_normalized=1, W_grid=0, W_dens={W_dens})', fontsize=12)
plt.tick_params(axis='both', which='major', labelsize=28)
plt.grid(True, alpha=0.3)
plt.tight_layout()

# グラフを保存
plt.savefig('score_w_dist_graph.png', dpi=300)
print("グラフを保存しました: score_w_dist_graph.png")

# グラフを表示
plt.show()

# いくつかの代表的な値を表示
print("\n代表的なW_dist値でのスコア:")
for w in [0, 1, 2, 5, 10]:
    score = (W_dens * density_normalized) / (np.exp(w * distance_normalized) + W_grid + 1.0)
    print(f"W_dist = {w:2.0f}: score = {score:.6f}")
