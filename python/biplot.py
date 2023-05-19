import numpy as np
import matplotlib.pyplot as plt

plt.rcParams["mathtext.fontset"] = 'cm'
plt.rcParams['font.family'] = 'Times New Roman'
plt.rcParams['font.size'] = 6
plt.rcParams['figure.autolayout'] = True
plt.rcParams['figure.dpi'] = 400
plt.rcParams['figure.figsize'] = [5, 3]
plt.rcParams['grid.linewidth'] = 0.6

colors = ['lightsteelblue', 'lightcoral', 'burlywood', 'thistle']
edgecolors = ['cornflowerblue', 'tomato', 'peru', 'violet']

data = np.loadtxt('../files/convergence.txt')
# Largeur des barres
bar_width = 0.22
x = np.arange(1, 10)

# Création du graphique
b2 = plt.bar(x - bar_width * 0.5, data[:9, 0], width=bar_width, align='center', color = colors[0], edgecolor = edgecolors[0], linewidth=.6, label='Bissection')
b3 = plt.bar(x + bar_width * 0.5, data[9:, 0], width=bar_width, align='center', color = colors[1], edgecolor = edgecolors[1], linewidth=.6, label='Nelder-Mead')

# Configuration des axes et de la légende
plt.xlabel('Facteur de maillage')
plt.ylabel("Nombre d'itérations")
plt.xticks(x, [str(M) for M in data[:9, 1]])
plt.legend()

fs = 5
h = lambda y : y + .6

for i in range(9):
    yval = b2[i].get_height()
    plt.text(b2[i].get_x()+bar_width/2, h(yval), str(int(data[i][0])), fontsize = fs, horizontalalignment='center')
    yval = b3[i].get_height()
    plt.text(b3[i].get_x()+bar_width/2, h(yval), str(int(data[9 + i][0])), fontsize = fs, horizontalalignment='center')

plt.show()