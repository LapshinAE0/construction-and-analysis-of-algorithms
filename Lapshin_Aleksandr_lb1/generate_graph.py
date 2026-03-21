import matplotlib.pyplot as plt

x = []
y = []

with open('results.csv', 'r') as f:
    for line in f:
        parts = line.strip().split(',')
        if len(parts) >= 2:
            x.append(int(parts[0]))
            y.append(int(parts[1]))




plt.figure(figsize=(16, 10))
plt.semilogy(x, y, 'bo-', linewidth=2, markersize=8, markerfacecolor='red', markeredgecolor='darkred')
plt.xticks(x)
plt.xlabel('Размер изначального квадрата')
plt.ylabel('Количество операций для заполнения квадрата')
plt.title('График зависимости размера квадрата от количества операций')
plt.grid(True, linestyle='--', alpha=0.7, which='both')
plt.savefig("graph_result.png")
plt.show()

x_2 = [x[i] for i in range(len(x)) if i % 2 != 0]
y_2 = [y[i] for i in range(len(y)) if i % 2 != 0]

plt.figure(figsize=(16, 10))
plt.semilogy(x_2, y_2, 'bo-', linewidth=2, markersize=8, markerfacecolor='red', markeredgecolor='darkred')
plt.xticks(x)
plt.xlabel('Размер изначального квадрата')
plt.ylabel('Количество операций для заполнения квадрата')
plt.title('График зависимости размера квадрата от количества операций, без четных сторон квадрата')
plt.grid(True, linestyle='--', alpha=0.7, which='both')
plt.savefig("graph_result_no_half.png")
plt.show()


plt.figure(figsize=(16, 10))
plt.plot(x_2, y_2, 'bo-', linewidth=2, markersize=8, markerfacecolor='red', markeredgecolor='darkred')
plt.xticks(x)
plt.xlabel('Размер изначального квадрата')
plt.ylabel('Количество операций для заполнения квадрата')
plt.title('График зависимости размера квадрата от количества операций, без четных сторон квадрата и без логарифмической шкалы')
plt.grid(True, linestyle='--', alpha=0.7, which='both')
plt.savefig("graph_result_no_log.png")
plt.show()