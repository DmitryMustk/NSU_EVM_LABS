import matplotlib.pyplot as plt

data = """
256 2
307 2
368 2
441 2
529 2
634 2
760 2
912 2
1094 2
1312 2
1574 2
1888 2
2265 2
2718 2
3261 2
3913 2
4695 2
5634 2
6760 2
8112 2
9734 4
11680 4
14016 5
16819 6
20182 5
24218 6
29061 6
34873 6
41847 7
50216 6
60259 7
72310 8
86772 8
104126 9
124951 11
149941 13
179929 16
"""

# Разбиваем строки на отдельные числа
lines = data.strip().split('\n')
x_values, y_values = zip(*[map(int, line.split()) for line in lines])
plt.figure(figsize=(19.20, 10.80))
# Строим график
xx_val = list(range(len(y_values)))
plt.plot(xx_val, y_values, marker='o', linestyle='-')
plt.title('График времени доступа в зависимости от размера массива')
plt.xlabel('Размер массива')
plt.ylabel('Время доступа')
plt.grid(True)

# Создаем равномерный интервал между точками
# plt.xticks(x_values)

# Добавляем подписи к точкам
for x, y, sz in zip(xx_val, y_values, x_values):
    plt.text(x, y, str(sz), ha='right', va='bottom', rotation=45, fontsize=8)


plt.savefig("my_plot.png")
# plt.show()
