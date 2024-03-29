import os
import matplotlib.pyplot as plt


# Устанавливаем размер графика
plt.figure(figsize=(12, 8))

# Цвета для каждого графика
colors = ['b', 'g', 'r', 'c', 'm', 'y', 'k']


data = """
1 5.000000
2 5.000000
3 5.000000
4 5.000000
5 21.000000
6 21.000000
7 21.000000
8 21.000000
9 32.000000
10 32.000000
11 32.000000
12 32.000000
13 32.000000
14 32.000000
15 32.000000
16 33.000000
17 35.000000
18 36.000000
19 38.000000
20 39.000000
21 39.000000
22 39.000000
23 39.000000
24 39.000000
25 39.000000
26 39.000000
27 39.000000
28 39.000000
29 39.000000
30 39.000000
31 39.000000
32 39.000000
"""

lines = data.strip().split('\n')
x_values, y_values = zip(*[map(float, line.split()) for line in lines])

plt.plot(x_values, y_values, marker='o', linestyle='-', label=f'График для task6')

plt.legend()

plt.title('График времени доступа в зависимости от числа фрагментов')
plt.xlabel('Число фрагментов')
plt.ylabel('Время доступа, в тактах')
plt.grid(True)

# Устанавливаем метки на оси X с нужным шагом
plt.xticks(range(1, 33))

plt.savefig("task6.png")
