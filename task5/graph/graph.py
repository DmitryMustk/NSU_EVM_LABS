import os
import matplotlib.pyplot as plt

# Путь к директории с данными
data_directory = "../resourses"

# Получаем список файлов в директории
files = [f for f in os.listdir(data_directory) if os.path.isfile(os.path.join(data_directory, f))]

# Устанавливаем размер графика
plt.figure(figsize=(19.20, 10.8))

# Цвета для каждого графика
colors = ['b', 'g', 'r', 'c', 'm', 'y', 'k']

# Проходим по каждому файлу и добавляем данные на график
for i, file_name in enumerate(files):
    file_path = os.path.join(data_directory, file_name)

    # Чтение данных из файла
    with open(file_path, 'r') as file:
        data = file.read()

    # Разбиваем строки на отдельные числа
    lines = data.strip().split('\n')
    x_values, y_values = zip(*[map(int, line.split()) for line in lines])
    xx_val = list(range(len(x_values)))
    # Строим график
    plt.plot(xx_val, y_values, marker='o', linestyle='-', color=colors[i % len(colors)], label=f'График из файла {file_name}')
    if("random" in file_path):
        for x, y, sz in zip(xx_val, y_values, x_values):
            plt.text(x, y, str(sz), ha='right', va='bottom', rotation=45, fontsize=6)
# Добавляем легенду
plt.legend()

# Настройки графика
plt.title('Графики времени доступа в зависимости от размера массива')
plt.xlabel('Размер массива')
plt.ylabel('Время доступа')
plt.grid(True)

# Сохраняем график в файл
plt.savefig("combined_plot.png")

